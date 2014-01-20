/* Coinjoin Client
 * Written in 2014 by
 *   Andrew Poelstra <apoelstra@wpsoftware.net>
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to
 * the public domain worldwide. This software is distributed without
 * any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication
 * along with this software.
 * If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

/*! \brief Maintains a list of addresses so we don't burn through the keypool so fast */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <jansson.h>

#include "jsonrpc.h"
#include "global.h"
#include "output.h"

/*!\brief Internal address linked list */
struct output
{
  char *address;
  int b_from_bitcoind;
  output_t *prev;
  output_t *next;
  output_list_t *list;
};

struct output_list
{
  output_t *head;
  output_t *tail;
  int b_use_bitcoind;
  int len;
  int len_no_bitcoind;
};

/*!\brief Output contructor */
static output_t *output_new (const char *addr, int b_from_bitcoind)
{
  output_t *rv = malloc (sizeof *rv);
  if (rv != NULL)
  {
    rv->address = malloc (1 + strlen (addr));
    if (rv->address)
    {
      strcpy (rv->address, addr);
      rv->b_from_bitcoind = b_from_bitcoind;
      rv->prev = rv->next = NULL;
    }
    else
    {
      free (rv);
      rv = NULL;
    }
  }
  return rv;
}

/*!\brief Output destructor */
void output_destroy (output_t *out)
{
  if (out)
  {
    if (out->prev)
      out->prev->next = out->next;
    if (out->next)
      out->next->prev = out->prev;
    if (out->list)
    {
      if (out->list->head == out)
        out->list->head = out->next;
      if (out->list->tail == out)
        out->list->tail = out->prev;
      if (out->b_from_bitcoind)
        --out->list->len_no_bitcoind;
      --out->list->len;
    }
    free (out->address);
    free (out);
  }
}

/*!\brief List constructor */
output_list_t *output_list_new ()
{
  output_list_t *rv = malloc (sizeof *rv);
  if (rv == NULL)
    return NULL;
  rv->head = rv->tail = NULL;
  rv->b_use_bitcoind = 1;  /* use bitcoind by default */
  rv->len_no_bitcoind = 0;
  rv->len = 0;
  return rv;
}

/*!\brief List destructor */
void output_list_destroy (output_list_t *list)
{
  output_t *scan = list->head;
  while (scan)
  {
    output_t *next = scan->next;
    output_destroy (scan);
    scan = next;
  }
  free (list);
}

/*!\brief Output list push (prepend) */
static void output_list_push (output_list_t *list, const char *addr, int b_from_bitcoind)
{
  output_t *newout = output_new (addr, b_from_bitcoind);
  if (newout != NULL)
  {
    newout->list = list;
    if (list->head != NULL)
      list->head->prev = newout;
    newout->next = list->head;
    list->head = newout;
    if (list->tail == NULL)
      list->tail = list->head;
    ++list->len;
    if (!b_from_bitcoind) ++list->len_no_bitcoind;
  }
}

/*!\brief Should we query bitcoind when we are out of addresses? */
void output_list_set_use_bitcoind (output_list_t *list, int s)
{
  assert (list != NULL);
  list->b_use_bitcoind = s;
}

int output_list_get_use_bitcoind (const output_list_t *list)
{
  assert (list != NULL);
  return list->b_use_bitcoind;
}

/*!\brief Call getnewaddress and push the obtained address onto the list */
static int push_bitcoind_address (output_list_t *list, const jsonrpc_t *bitcoind)
{
  assert (list);
  assert (list->b_use_bitcoind);

  json_t *response = jsonrpc_request (bitcoind, "getnewaddress", NULL);
  json_t *new_addr = json_object_get (response, "result");
  if (json_is_string (new_addr))
  {
    output_list_push (list, json_string_value (new_addr), 1);
    json_decref (response);
    return 1;
  }
  else
  {
    fprintf (stderr, "Received unexpected result %s\n", json_dumps (response, 0));
    json_decref (response);
    return 0;
  }
}

/*!\brief Add a new address without the aid of bitcoind */
void output_list_add_new_address (output_list_t *list, const char *address)
{
  output_list_push (list, address, 0);
}

/*!\brief Returns an output list in JSON form suitable for bitcoind */
json_t *output_list_json (output_list_t *list, const jsonrpc_t *bitcoind, u64_t total, u64_t target_size, u64_t fee, const char *donation_address)
{
  int n_outputs;
  int n_needed_outputs;
  json_t *rv = json_object ();

  assert (list);

  /* Check that we have enough outputs */
  n_outputs = list->b_use_bitcoind ? list->len : list->len_no_bitcoind;
  n_needed_outputs = 1 + (total - fee) / target_size + !!((total - fee) % target_size);
  if (n_outputs < n_needed_outputs)
  {
    /* Try asking bitcoind ... */
    if (list->b_use_bitcoind)
    {
      while (list->len < n_needed_outputs &&
             push_bitcoind_address (list, bitcoind))
        ++n_outputs;
    }
    /* failing that.. */
    if (n_outputs < n_needed_outputs)
    {
      json_object_set (rv, "error", json_string ("Not enough outputs available."));
      json_object_set (rv, "use_bitcoind", json_boolean (list->b_use_bitcoind));
      json_object_set (rv, "n_outputs", json_integer (list->len_no_bitcoind));
      json_object_set (rv, "n_needed_outputs", json_integer (n_needed_outputs));
      return rv;
    }
  }

  /* Run through the list */
  const output_t *scan = list->head;
  while (scan && total > target_size + fee)
  {
    if (list->b_use_bitcoind || !scan->b_from_bitcoind)
      json_object_set_new (rv, scan->address, json_decimal (target_size, 8));
    scan = scan->next;
    total -= target_size;
  }

  /* Add the ragged output and the fee */
  json_object_set_new (rv, scan->address, json_decimal (total - fee, 8));
  json_object_set_new (rv, donation_address, json_decimal (fee, 8));

  return rv;
}


/*!\brief Returns the first output in the list */
const output_t *output_list_get_first_output (const output_list_t *list)
{
  assert (list != NULL);
  return list->head;
}

/*!\brief Did an output come from bitcoind? */
int output_get_from_bitcoind (const output_t *out)
{
  assert (out != NULL);
  return out->b_from_bitcoind;
}

/*!\brief Returns the address corresponding to some output */
const char *output_get_address (const output_t *out)
{
  assert (out != NULL);
  return out->address;
}

/*!\brief Returns the next output in the list */
const output_t *output_get_next (const output_t *out)
{
  assert (out != NULL);
  return out->next;
}


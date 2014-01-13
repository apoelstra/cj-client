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

#include <stdlib.h>
#include <string.h>

#include <jansson.h>

#include "jsonrpc.h"
#include "global.h"
#include "output.h"

struct _address
{
  char *address;
  struct _address *next;
};

static struct _address *add_new_address (const jsonrpc_t *bitcoind, struct _address *end)
{
  struct _address *rv = malloc (sizeof *rv);
  if (rv != NULL)
  {
    json_t *response = jsonrpc_request (bitcoind, "getnewaddress", NULL);
    json_t *new_addr = json_object_get (response, "result");
    if (json_is_string (new_addr))
    {
      rv->address = malloc (1 + strlen (json_string_value (new_addr)));
      strcpy (rv->address, json_string_value (new_addr));
    }
    else fprintf (stderr, "Received unexpected result %s\n", json_dumps (new_addr, 0));
    json_decref (response);
    rv->next = NULL;
  }

  if (end != NULL)
    end->next = rv;
  return rv;
}

static struct _address *address_bank;
json_t *output_list (const jsonrpc_t *bitcoind, u64_t total, u64_t target_size, u64_t fee, const char *donation_address)
{
  struct _address *scan;
  json_t *rv = json_object ();

  if (address_bank == NULL)
    address_bank = add_new_address (bitcoind, NULL);

  scan = address_bank;
  while (total > target_size + fee)
  {
    json_object_set_new (rv, scan->address, json_decimal (target_size, 8));

    if (scan->next == NULL)
      scan = add_new_address (bitcoind, scan);
    else
      scan = scan->next;
    total -= target_size;
  }
  /* Add the ragged output and the fee */
  json_object_set_new (rv, scan->address, json_decimal (total - fee, 8));
  json_object_set_new (rv, donation_address, json_decimal (fee, 8));

  return rv;
}

void output_clear_cache ()
{
  struct _address *scan = address_bank;
  while (scan)
  {
    struct _address *next = scan->next;
    free (scan->address);
    free (scan);
    scan = next;
  }
  address_bank = NULL;
}



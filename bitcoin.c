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

#include <stdlib.h>
#include <string.h>

#include "bitcoin.h"
#include "jsonrpc.h"
#include "global.h"

struct utxo {
  char *txid;
  unsigned vout;
  unsigned nconfirms;
  u64_t value;
  utxo_list_t *next;
};

/*! \brief strdup is not ANSI C */
static char *x_strdup (const char *s)
{
  if (s == NULL)
    return NULL;
  else
  {
    char *rv = malloc (1 + strlen (s));
    strcpy (rv, s);
    return rv;
  }
}

/*! \brief Returns a linked list of utxos */
utxo_list_t *bitcoin_get_utxos (jsonrpc_t *js)
{
  utxo_list_t *rv = NULL;
  json_t *response = jsonrpc_request (js, "listunspent", NULL);
  json_t *unspent = json_object_get (response, "result");

  /* If it's not an array, treat it as "no coins" */
  if (json_is_array (unspent))
  {
    unsigned i;
    unsigned N = json_array_size (unspent);
    for (i = 0; i < N; ++i)
    {
      json_t *json_coin = json_array_get (unspent, i);
      if (json_is_object (json_coin))
      {
        utxo_list_t *coin = malloc (sizeof *coin);
        if (coin != NULL)
        {
          coin->txid = x_strdup (json_string_value (json_object_get (json_coin, "txid")));
          coin->vout = json_integer_value (json_object_get (json_coin, "vout"));
          coin->value = TO_SATOSHI (json_real_value (json_object_get (json_coin, "amount")));
          coin->nconfirms = json_integer_value (json_object_get (json_coin, "confirmations"));
          coin->next = rv;
          rv = coin;
        }
      } else fputs ("Strange things are afoot at the Circle-K.", stderr);
    }
  }

  json_decref (response);
  return rv;
}

/*! \brief Destroys a linked list of utxos */
void bitcoin_utxos_destroy (utxo_list_t *txlist)
{
  if (txlist == NULL)
    return;
  if (txlist->next)
    bitcoin_utxos_destroy (txlist->next);
  free (txlist->txid);
  free (txlist);
}


/*! \brief Getter for utxo txid */
const char *bitcoin_utxo_txid (const utxo_list_t *txlist)
{
  return txlist == NULL ? NULL : txlist->txid;
}

/*! \brief Getter for utxo vout */
unsigned bitcoin_utxo_vout (const utxo_list_t *txlist)
{
  return txlist == NULL ? 0 : txlist->vout;
}

/*! \brief Getter for utxo value */
u64_t bitcoin_utxo_value (const utxo_list_t *txlist)
{
  return txlist == NULL ? 0 : txlist->value;
}

/*! \brief Getter for utxo # of confirmations */
unsigned bitcoin_utxo_nconfirms (const utxo_list_t *txlist)
{
  return txlist == NULL ? 0 : txlist->nconfirms;
}

/*! \brief Getter for utxo-list next pointer */
const utxo_list_t *bitcoin_utxo_next (const utxo_list_t *txlist)
{
  return txlist == NULL ? NULL : txlist->next;
}


/* TRANSACTION MANAGAMENT */
static json_t *my_tx;
static json_t *my_keys;

/*! \brief Add a raw tx to the "transaction I submitted" variable */
void bitcoin_add_my_transaction_raw (const jsonrpc_t *js, const char *tx)
{
  if (my_tx == NULL)
  {
    my_tx = json_array();
    json_array_append_new (my_tx, json_array());
    json_array_append_new (my_tx, json_object());
  }

  json_t *params = json_array();
  json_array_append_new (params, json_string (tx));

  json_t *response = jsonrpc_request (js, "decoderawtransaction", params);
  json_t *decodetx = json_object_get (response, "result");
  if (json_is_object (decodetx))
  {
    json_t *vin = json_object_get (decodetx, "vin");
    json_t *vout = json_object_get (decodetx, "vout");

    /* Blindly add all the inputs to the pile -- if there are dupes the
     * transaction is not getting off the ground, so that's a safe
     * failure mode. But that shouldn't ever happen. */
    fprintf (stderr, "Adding a tx with %zu inputs.\n", json_array_size (vin));
    for (unsigned i = 0; i < json_array_size (vin); ++i)
      json_array_append (json_array_get (my_tx, 0), json_array_get (vin, i));
    /* For outputs, we check for dupes and add them together */
    for (unsigned i = 0; i < json_array_size (vout); ++i)
    {
      json_t *old_outs = json_array_get (my_tx, 1);
      json_t *scriptvalue  = json_object_get (json_array_get (vout, i), "value");
      json_t *scriptpubkey = json_object_get (json_array_get (vout, i), "scriptPubKey");
      /* We will assume single-address transactions so that we can use
       * createrawtransaction as the inverse encoder. A TODO is to do
       * raw transaction manipulation directly in this codebase. */
      json_t *first_address = json_array_get (json_object_get (scriptpubkey, "addresses"), 0);
      const char *ch_address = json_string_value (first_address);

      /* if it's a dupe do addition: */
      if (json_is_number (json_object_get (old_outs, ch_address)))
      {
        double old_val = json_number_value (json_object_get (old_outs, ch_address));
        json_object_set_new (old_outs, ch_address, json_real (old_val + json_number_value (scriptvalue)));
      }
      /* if it's not a dupe it's easy: */
      json_object_set (old_outs, ch_address, scriptvalue);
    }
  }

  json_decref (params);
  json_decref (response);
}

/*! \brief Retrieves a raw transaction which is a join of all */
char *bitcoin_get_my_transactions_raw (const jsonrpc_t *js)
{
  char *rv = NULL;
  if (my_tx == NULL)
    return "";
  json_t *response = jsonrpc_request (js, "createrawtransaction", my_tx);
  json_t *encodetx = json_object_get (response, "result");
  if (json_is_string (encodetx))
  {
    const char *raw = json_string_value (encodetx);
    rv = malloc (1 + strlen (raw));
    strcpy (rv, raw);
  }
  json_decref (response);
  return rv;
}

/*! \brief Checks if a given transaction is a superset of all
 *  previously-submitted transactions.
 *
 *  \return 1 on success, 0 on failure
 */
int bitcoin_has_my_transactions_raw (const jsonrpc_t *js, const char *tx, const char *donation_address)
{
  json_t *params = json_array();
  json_array_append_new (params, json_string (tx));

  json_t *response = jsonrpc_request (js, "decoderawtransaction", params);
  json_t *decodetx = json_object_get (response, "result");
  if (json_is_object (decodetx))
  {
    json_t *elem;
    const char *address;
    json_t *old_outs = json_array_get (my_tx, 1);
    json_t *vout = json_object_get (decodetx, "vout");

    /* For now we won't bother verifying that all inputs are present.
     * There is no benefit for an attacker to drop them.
     * We do check for every output. */
    json_object_foreach (old_outs, address, elem)
    {
      int found = 0;
      if (!strcmp (address, donation_address))
        found = 1;
      else for (unsigned i = 0; i < json_array_size (vout); ++i)
      {
        json_t *scriptvalue  = json_object_get (json_array_get (vout, i), "value");
        json_t *scriptpubkey = json_object_get (json_array_get (vout, i), "scriptPubKey");
        json_t *scriptaddress = json_array_get (json_object_get (scriptpubkey, "addresses"), 0);

        if (json_is_string (scriptaddress) &&
            !strcmp (json_string_value (scriptaddress), address) &&
            json_number_value (scriptvalue) == json_number_value (elem))
          found = 1;
      }
      if (!found)
        return 0;
    }
  }

  return 1;
}

/*! \brief Getter for 'have I submitted any transactions' */
int bitcoin_my_transactions_p ()
{
  return my_tx != NULL;
}

/*! \brief Remove all transactions from the 'my transactions' API */
void bitcoin_my_transactions_reset ()
{
  json_decref (my_tx);
  my_tx = NULL;
}

/*! \brief Fetch signing keys from bitcoind */
void bitcoin_my_transactions_fetch_signing_keys (const jsonrpc_t *js)
{
  json_t *vin;
  if (my_tx == NULL)
    return;
  if (my_keys == NULL)
    my_keys = json_array ();

  vin = json_array_get (my_tx, 0);
  for (unsigned i = 0; i < json_array_size (vin); ++i)
  {
    json_t *input = json_array_get (vin, i);
    int has_key = 0;
    /* we keep track of our key posession with a flag my_tx */
    if (json_is_integer (json_object_get (input, "have_key")))
      has_key = json_integer_value (json_object_get (input, "have_key"));
    if (!has_key)
    {
      int success = 1;
      json_t *response, *addresses;
      json_t *params = json_array ();
      json_array_append (params, json_object_get (input, "txid"));
      json_array_append (params, json_object_get (input, "vout"));

      response = jsonrpc_request (js, "gettxout", params);
      addresses = json_object_get (json_object_get (json_object_get (response, "result"), "scriptPubKey"), "addresses");
      if (json_is_array (addresses))
      {
        for (unsigned i = 0; i < json_array_size (addresses); ++i)
        {
          json_t *address = json_array_get (addresses, i);

          json_t *response;
          json_t *params = json_array ();
          json_array_append (params, address);
          response = jsonrpc_request (js, "dumpprivkey", params);
          /* add the privkey to our store */
          if (json_is_string (json_object_get (response, "result")))
          {
            fprintf (stderr, "Loaded privkey for address %s into memory.\n",
                     json_string_value (address));
            json_array_append (my_keys, json_object_get (response, "result"));
          }
        }
      } else success = 0;

      if (success)
        json_object_set_new (input, "have_key", json_integer (1));
      json_decref (response);
      json_decref (params);
    }
  }
}

/*! \brief Getter for 'are the necessary signing keys in memory'
 * \return 1 on success, 0 on failure */
int bitcoin_my_transactions_signing_keys_p ()
{
  json_t *vin;
  if (my_tx == NULL)
    return 1;
  if (my_keys == NULL)
    return 0;

  vin = json_array_get (my_tx, 0);
  for (unsigned i = 0; i < json_array_size (vin); ++i)
  {
    json_t *input = json_array_get (vin, i);
    if (!json_is_integer (json_object_get (input, "have_key")))
      return 0;
    if (!json_integer_value (json_object_get (input, "have_key")))
      return 0;
  }
  return 1;
}

/*! \brief Sign a raw transaction with the keys in memory
 * \return The signed raw transaction in hex, or NULL on failure */
char *bitcoin_my_transactions_sign_raw (const jsonrpc_t *js, const char *tx)
{
  char *rv = NULL;
  json_t *params, *response;
  params = json_array ();
  json_array_append_new (params, json_string (tx));
  json_array_append_new (params, json_array ());
  json_array_append_new (params, my_keys);
  response = jsonrpc_request (js, "signrawtransaction", params);

  if (json_is_string (json_object_get (json_object_get (response, "result"), "hex")))
  {
    json_t *res = json_object_get (json_object_get (response, "result"), "hex");
    rv = malloc (1 + strlen (json_string_value (res)));
    strcpy (rv, json_string_value (res));
  }

  json_decref (response);
  json_decref (params);
  return rv;
}



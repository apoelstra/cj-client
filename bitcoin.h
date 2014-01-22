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

#ifndef BITCOIN_H
#define BITCOIN_H

#include <jansson.h>

#include "jsonrpc.h"
#include "global.h"

typedef struct utxo utxo_list_t;

/*! \brief Returns a linked list of utxos */
utxo_list_t *bitcoin_get_utxos (jsonrpc_t *);

/*! \brief Destroys a linked list of utxos */
void bitcoin_utxos_destroy (utxo_list_t *);

/*! \brief Getter for utxo address */
const char *bitcoin_utxo_address (const utxo_list_t *);

/*! \brief Getter for utxo txid */
const char *bitcoin_utxo_txid (const utxo_list_t *);

/*! \brief Getter for utxo vout */
unsigned bitcoin_utxo_vout (const utxo_list_t *);

/*! \brief Getter for utxo value */
u64_t bitcoin_utxo_value (const utxo_list_t *txlist);

/*! \brief Getter for utxo # of confirmations */
unsigned bitcoin_utxo_nconfirms (const utxo_list_t *txlist);

/*! \brief Getter for utxo-list next pointer */
const utxo_list_t *bitcoin_utxo_next (const utxo_list_t *);


/*! \brief Add a raw tx to the "transaction I submitted" variable
 *
 * Because it's easy to MITM my server, since curl does not check
 * SSL certs, we need to keep track of every submission we make
 * to the server. Then in the signing phase, we ensure that whatever
 * we are signing is a superset of the transaction we submitted.
 *
 * Since a single person can submit multiple transactions, we have
 * these helper functions to keep track of all of them.
 */
void bitcoin_add_my_transaction_raw (const jsonrpc_t *, const char *tx);

/*! \brief Retrieves a raw transaction which is a join of all
 *  previously-submitted transactions.
 *
 *  We want to store this in the configuration file.
 */
char *bitcoin_get_my_transactions_raw (const jsonrpc_t *);

/*! \brief Checks if a given transaction's outputs is a superset
 *  of all previously-submitted transactions' outputs.
 *
 *  The one exception is: whatever is passed as the donation
 *  address, we do not check that that output is still present,
 *  since it won't be and that's fine.
 */
int bitcoin_has_my_transactions_raw (const jsonrpc_t *js, const char *tx, const char *donation_address);

/*! \brief Checks that a signed transaction's signed inputs are only
 *  ones that were actually submitted.
 *
 *  If we sign outputs that we didn't submit, presumably some player
 *  has added evil inputs to trick us into spending money we didn't
 *  mean to.
 */
int bitcoin_has_only_my_utxos_raw (const jsonrpc_t *js, const char *tx);

/*! \brief Getter for 'have I submitted any transactions' */
int bitcoin_my_transactions_p ();

/*! \brief Fetch signing keys from bitcoind 
 *
 * To avoid having the user wait around for the session to end,
 * instead we ask him for passphrase as soon as we know what
 * keys are needed, then load them into memory with 'dumpprivkey'.
 * (These keys are never stored to disk, so when the program
 * is restarted the passphrase needs to be reentered.)
 *
 * Then when the time comes to sign, we give the list of keys to
 * 'signrawtransaction' and have no need to access the wallet.
 */
void bitcoin_my_transactions_fetch_signing_keys (const jsonrpc_t *);

/*! \brief Getter for 'are the necessary signing keys in memory' */
int bitcoin_my_transactions_signing_keys_p ();

/*! \brief Remove all transactions from the 'my transactions' API */
void bitcoin_my_transactions_reset ();

/*! \brief Sign a raw transaction with the keys in memory */
char *bitcoin_my_transactions_sign_raw (const jsonrpc_t *js, const char *tx);

/*! \brief Check if the bitcoind wallet is unlocked */
int bitcoin_is_unlocked (const jsonrpc_t *);

#endif


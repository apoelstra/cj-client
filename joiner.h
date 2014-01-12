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

#ifndef JOINER_H
#define JOINER_H

#include "global.h"

typedef struct joiner joiner_t;

enum joiner_status {
  JS_NOTFOUND = -1,
  JS_NOTYETOPEN = 5,
  JS_STILLOPEN = 0,
  JS_NEEDSIGS = 1,
  JS_COMPLETE = 2,
  JS_INVALID = 4,
  JS_UNKNOWN = 99
};

/*! \brief Create a new joiner object */
joiner_t *joiner_new (const char *update_url, const char *session_id);

/*! \brief Destroys a joiner object */
void joiner_destroy (joiner_t *);

/*! \brief Resync joiner object with the server */
void joiner_update (joiner_t *jn);

/*! \brief Submits an unsigned transaction to the server
 *  \return The joiner session ID
 */
const char *joiner_submit_unsigned (joiner_t *, const char *raw_tx);

/*! \brief Submits an signed transaction to the server */
void joiner_submit_signed (joiner_t *, const char *raw_tx);

/*! \brief Getter for joiner status */
enum joiner_status joiner_status (const joiner_t *);

/*! \brief Getter for joiner status */
const char *joiner_status_text (const joiner_t *);

/*! \brief Getter for donation address */
const char *joiner_donation_address (const joiner_t *);

/*! \brief Getter for per-input fee */
u64_t joiner_per_input_fee (const joiner_t *);

/*! \brief Getter for most popular output */
u64_t joiner_most_popular_output (const joiner_t *);

/*! \brief Getter for # of transactions */
unsigned joiner_n_transactions (const joiner_t *);

/*! \brief Getter for time remaining */
int joiner_minutes_remaining (const joiner_t *);

/*! \brief Getter for tx to sign (if the joiner is waiting on sigs) */
const char *joiner_tx_to_sign (const joiner_t *);

/*! \brief Getter for txid (if the joiner status is complete) */
const char *joiner_txid (const joiner_t *);

#endif


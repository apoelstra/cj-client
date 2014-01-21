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

#ifndef OUTPUT_H
#define OUTPUT_H

#include <jansson.h>

#include "global.h"


typedef struct output_list output_list_t;
typedef struct output output_t;

/*!\brief List constructor */
output_list_t *output_list_new ();

/*!\brief List destructor */
void output_list_destroy (output_list_t *list);

/*!\brief Should we query bitcoind when we are out of addresses? */
int output_list_get_use_bitcoind (const output_list_t *list);
void output_list_set_use_bitcoind (output_list_t *list, int s);

/*!\brief Add a new address without the aid of bitcoind */
const output_t *output_list_add_new_address (output_list_t *list, const char *address);

/*!\brief Returns an output list in JSON form suitable for bitcoind */
json_t *output_list_json (output_list_t *list, const jsonrpc_t *bitcoind, u64_t total, u64_t target_size, u64_t fee, const char *donation_address);

/*!\brief Returns the first output in the list */
const output_t *output_list_get_first_output (const output_list_t *);

/*!\brief Did an output come from bitcoind? */
int output_get_from_bitcoind (const output_t *);

/*!\brief Returns the address corresponding to some output */
const char *output_get_address (const output_t *);

/*!\brief Returns the next output in the list */
const output_t *output_get_next (const output_t *);

/*!\brief Destroys an output */
void output_destroy (output_t *out);

#endif


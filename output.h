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

/*! \brief Returns a json_t of {"addr": value} pairs suitable for sending to bitcoind */
json_t *output_list (const jsonrpc_t *bitcoind, u64_t total, u64_t target_size, u64_t fee, const char *donation_address);

/*! \brief Throws away any cached addresses */
void output_clear_cache ();

#endif


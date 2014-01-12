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

#ifndef JSONRPC_H
#define JSONRPC_H

#include <jansson.h>

typedef struct jsonrpc jsonrpc_t;

/*! \brief Create a new JSONRPC object */
jsonrpc_t *jsonrpc_new (const char *server, unsigned port,
                        const char *user, const char *pass);

/*! \brief Destroys a JSONRPC object */
void jsonrpc_destroy (jsonrpc_t *js);

/*! \brief Execute a JSONRPC request */
json_t *jsonrpc_request (const jsonrpc_t *, const char *, json_t *);


#endif


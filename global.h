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

/*! \file global.h
 *  \brief Definitions needed by all parts of the codebase.
 */
#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef __WIN32__
typedef int uid_t;
#endif

/*! \brief The version that is displayed in the window title. */
#define VERSION "v0.1"

/*! \brief The default URL which cj-client uses to get the joiner status */
#define JOINER_URL "https://wpsoftware.net/coinjoin/cj-client.php"

/*! \brief The number of seconds to unlock the wallet in order to load privkeys */
#define WALLET_UNLOCK_TIME 5

typedef unsigned long long u64_t;

#define TO_SATOSHI(n) ((n) * ((u64_t) 100000000))
#define FROM_SATOSHI(n) ((n) / 100000000.0)

#endif

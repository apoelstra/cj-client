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

#ifndef SETTINGS_H
#define SETTINGS_H

/*! \brief Attempt to read the server configuration from disk */
int settings_read_config ();
/*! \brief Attempt to save the server configuration to disk */
void settings_save_config ();

/*! \brief Getter for rpc-server */
const char *settings_get_rpc_server ();
/*! \brief Getter for rpc-port */
int settings_get_rpc_port ();
/*! \brief Getter for rpc-user */
const char *settings_get_rpc_user ();
/*! \brief Getter for rpc-pass */
const char *settings_get_rpc_pass ();
/*! \brief Getter for joiner URL */
const char *settings_get_server_url ();
/*! \brief Getter for session ID */
const char *settings_get_session_id ();
/*! \brief Getter for submitted rawtx */
const char *settings_get_submission ();

/*! \brief Setter for rpc-server */
void settings_set_rpc_server (const char *);
/*! \brief Setter for rpc-port */
void settings_set_rpc_port (int);
/*! \brief Setter for rpc-user */
void settings_set_rpc_user (const char *);
/*! \brief Setter for rpc-pass */
void settings_set_rpc_pass (const char *);

/*! \brief Setter for session ID */
void settings_set_session_id (const char *);
/*! \brief Setter for submitted rawtx */
void settings_set_submission (const char *);


/*! \brief Memory cleanup function */
void settings_cleanup ();

#endif


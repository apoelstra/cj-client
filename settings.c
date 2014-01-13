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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>
#include <glib/gstdio.h>

#include "buffer.h"
#include "global.h"

static struct {
  char *server;
  unsigned int port;
  char *user;
  char *pass;
  char *server_url;
  char *session_id;
  char *submission;
} config;

/* TODO fix these functions for win/mac */
/*! \brief Returns the directory where we store our data */
static const char *get_configuration_dir ()
{
  static char *rv;
#ifdef __WINDOWS__
  if (rv == NULL)
    rv = g_strconcat (g_get_user_config_dir (), "/cjclient", NULL);
#else
  if (rv == NULL)
    rv = g_strconcat (g_get_home_dir(), "/.cjclient", NULL);
#endif
  return rv;
}

/*! \brief Returns the filename where we store our configuration */
static const char *get_configuration_filename ()
{
  static char *rv;
#ifdef __WINDOWS__
  if (rv == NULL)
    rv = g_strconcat (g_get_user_config_dir (), "/cjclient/cjclient.conf", NULL);
#else
  if (rv == NULL)
    rv = g_strconcat (g_get_home_dir(), "/.cjclient/cjclient.conf", NULL);
#endif
  return rv;
}

/*! \brief Returns the filename where Bitcoin's configuration is */
static const char *get_bitcoin_configuration_filename ()
{
  static char *rv;
#ifdef __WINDOWS__
  if (rv == NULL)
    rv = g_strconcat (g_get_user_config_dir (), "/Bitcoin/bitcoin.conf", NULL);
  puts (rv);
  puts ("on windows");
#else
  if (rv == NULL)
    rv = g_strconcat (g_get_home_dir(), "/.bitcoin/bitcoin.conf", NULL);
#endif
  return rv;
}


/*! \brief Helper which reads "var = val" */
typedef struct { int success; char *var; char *val; } cfg_pair;
static cfg_pair read_config_line (const char *ln)
{
  cfg_pair rv;
  rv.val = NULL;
  rv.var = malloc (500);
  int val_offset = 0;
  rv.success = 1;

  if (rv.var != NULL &&
      sscanf (ln, " %499[^ \t=]%*[ =\t]%n", rv.var, &val_offset) > 0)
  {
    int len = strlen (ln + val_offset) - 1;  /* -1 to cut off newline */
    rv.val = g_strndup (ln + val_offset, len);
  }
  else
  {
    rv.success = 0;
    free (rv.var); rv.var = NULL;
  }
  return rv;
}


/*! \brief Attempt to read the server configuration from disk */
int settings_read_config ()
{
  FILE *fh;
  /* Set defaults */
  config.server_url = g_strdup (JOINER_URL);
  config.user = config.pass = config.session_id = config.submission = NULL;
  config.server = g_strdup ("localhost");
  config.port = 8332;

#define CHECK_STR(conf_var, c_var)	\
        else if (!g_ascii_strcasecmp (ln.var, conf_var))	\
        {	\
          g_free (config.c_var);	\
          config.c_var = g_strdup (ln.val);	\
          free (ln.val);	\
          free (ln.var);	\
        }
  
  /* Try overriding them from bitcoin's config */
  fh = fopen (get_bitcoin_configuration_filename(), "r");
  if (fh != NULL)
  {
    fprintf (stderr, "Reading from bitcoin configuration at %s\n",
             get_bitcoin_configuration_filename());
    buffer_t *buf = buffer_new ();
    while (buffer_fgets (buf, fh))
    {
      cfg_pair ln = read_config_line (buffer_get_data (buf));
      if (ln.success)
      {
        if (!g_ascii_strcasecmp (ln.var, "rpcport"))
          config.port = g_ascii_strtoll (ln.val, NULL, 0);
        CHECK_STR ("rpcuser", user)
        CHECK_STR ("rpcpassword", pass)
        CHECK_STR ("rpcconnect", server)
        CHECK_STR ("joinerserver", server_url)
      }
      buffer_empty (buf);
    }
    buffer_destroy (buf);
    fclose (fh);
  }

  /* Try overriding them from our config */
  fh = fopen (get_configuration_filename(), "r");
  if (fh != NULL)
  {
    buffer_t *buf = buffer_new ();
    while (buffer_fgets (buf, fh))
    {
      cfg_pair ln = read_config_line (buffer_get_data (buf));
      if (ln.success)
      {
        if (!g_ascii_strcasecmp (ln.var, "rpcport"))
          config.port = g_ascii_strtoll (ln.val, NULL, 0);
        CHECK_STR ("rpcuser", user)
        CHECK_STR ("rpcpassword", pass)
        CHECK_STR ("rpcconnect", server)
        CHECK_STR ("joinerserver", server_url)
        CHECK_STR ("sessionid", session_id)
        CHECK_STR ("submission", submission)
      }
      buffer_empty (buf);
    }
    buffer_destroy (buf);
    fclose (fh);
  }

  /* We need a user and password to return success */
  return (config.user != NULL && config.pass != NULL);
}


void settings_save_config ()
{
  FILE *fh;
  fh = fopen (get_configuration_filename(), "w");
  if (fh == NULL)
  {
    int err = g_mkdir (get_configuration_dir(), 0755);
    fprintf (stderr, "g_mkdir(%s) error %d\n",
             get_configuration_dir(), err);
    fh = fopen (get_configuration_filename(), "w");
  }
  if (fh == NULL)
  {
    fprintf (stderr, "Failed to write configuration to %s\n",
             get_configuration_filename());
    return;
  }

  fprintf (fh, "joinerserver = %s\n", config.server_url);
  if (config.session_id != NULL)
    fprintf (fh, "sessionid = %s\n", config.session_id);
  if (config.submission != NULL)
    fprintf (fh, "submission = %s\n", config.submission);
  fputs ("\n\n", fh);
  fclose (fh);
}


/* GETTERS */
const char *settings_get_rpc_server () { return config.server; }
int settings_get_rpc_port ()           { return config.port; }
const char *settings_get_rpc_user ()   { return config.user; }
const char *settings_get_rpc_pass ()   { return config.pass; }
const char *settings_get_server_url () { return config.server_url; }
const char *settings_get_session_id () { return config.session_id; }
const char *settings_get_submission () { return config.submission; }

/* SETTERS */
void settings_set_session_id (const char *sess_id)
{
  g_free (config.session_id);
  config.session_id = sess_id ? g_strdup (sess_id) : NULL;
}

void settings_set_submission (const char *submission)
{
  g_free (config.submission);
  config.submission = submission ? g_strdup (submission) : NULL;
}


/*! \brief Memory cleanup function */
void settings_cleanup ()
{
  free (config.server);
  free (config.user);
  free (config.pass);
  free (config.server_url);
  free (config.session_id);
}


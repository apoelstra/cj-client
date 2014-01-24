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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>
#include <glib/gstdio.h>
#ifdef __WINDOWS__
#include <shlobj.h>
#endif

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
  int save_rpc_server;
  int save_rpc_auth;
  int save_rpc_port;
} config;

/* TODO fix these functions for win/mac */
#ifdef __WINDOWS__
const char *get_special_folder (int id)
{
  static char pszPath[MAX_PATH] = {0};

  if(SHGetSpecialFolderPathA(NULL, pszPath, id, 0))
    return pszPath;

  return "";
}
#endif


/*! \brief Returns the directory where we store our data */
static const char *get_configuration_dir ()
{
  static char *rv;
#ifdef __WINDOWS__
  if (rv == NULL)
    rv = g_strconcat (get_special_folder (CSIDL_APPDATA), "/cjclient", NULL);
  puts (rv);
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
  if (rv == NULL)
    rv = g_strconcat (get_configuration_dir (), "/cjclient.conf", NULL);
  return rv;
}

/*! \brief Returns the filename where Bitcoin's configuration is */
static const char *get_bitcoin_configuration_filename ()
{
  static char *rv;
#ifdef __WINDOWS__
  if (rv == NULL)
    rv = g_strconcat (get_special_folder (CSIDL_APPDATA), "/Bitcoin/bitcoin.conf", NULL);
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
    int len;
    while (isspace (ln[val_offset]))
      ++val_offset;
    len = strlen (ln + val_offset);
    while (isspace (ln[val_offset + len - 1]))
      --len;
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
  config.save_rpc_server =
  config.save_rpc_auth =
  config.save_rpc_port = 0;

#define CHECK_STR(conf_var, c_var)	\
        else if (!g_ascii_strcasecmp (ln.var, conf_var))	\
        {	\
          g_free (config.c_var);	\
          config.c_var = g_strdup (ln.val);	\
          free (ln.val);	\
          free (ln.var);	\
        }
#define CHECK_STR_SAVE(conf_var, c_var, save_var)	\
        else if (!g_ascii_strcasecmp (ln.var, conf_var))	\
        {	\
          g_free (config.c_var);	\
          config.c_var = g_strdup (ln.val);	\
          config.save_var = 1;	\
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
        {
          config.port = g_ascii_strtoll (ln.val, NULL, 0);
          config.save_rpc_port = 1;
        }
        CHECK_STR_SAVE ("rpcuser", user, save_rpc_auth)
        CHECK_STR_SAVE ("rpcpassword", pass, save_rpc_auth)
        CHECK_STR_SAVE ("rpcconnect", server, save_rpc_server)
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
  if (config.save_rpc_server)
    fprintf (fh, "rpcconnect = %s\n", config.server);
  if (config.save_rpc_port)
    fprintf (fh, "rpcport = %d\n", config.port);
  if (config.save_rpc_auth)
  {
    fprintf (fh, "rpcuser = %s\n", config.user);
    fprintf (fh, "rpcpass = %s\n", config.pass);
  }
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
void settings_set_rpc_server (const char *server)
{
  g_free (config.server);
  config.save_rpc_server = 1;
  config.server = server ? g_strdup (server) : NULL;
}

void settings_set_rpc_port (int port)
{
  config.save_rpc_port = 1;
  config.port = port;
}

void settings_set_rpc_user (const char *user)
{
  g_free (config.user);
  config.save_rpc_auth = 1;
  config.user = user ? g_strdup (user) : NULL;
}

void settings_set_rpc_pass (const char *pass)
{
  g_free (config.pass);
  config.save_rpc_auth = 1;
  config.pass = pass ? g_strdup (pass) : NULL;
}

void settings_set_server_url (const char *server)
{
  g_free (config.server_url);
  config.server_url = server ? g_strdup (server) : NULL;
}

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


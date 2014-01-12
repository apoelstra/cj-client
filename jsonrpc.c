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
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <jansson.h>

#include "buffer.h"
#include "jsonrpc.h"

struct jsonrpc {
  char *server;
  char *user;
  char *pass;
  int port;
};

/*! \brief Create a new JSONRPC object */
jsonrpc_t *jsonrpc_new (const char *server, unsigned port,
                        const char *user, const char *pass)
{
  jsonrpc_t *new_js = malloc (sizeof *new_js);

  assert (server != NULL);
  assert (user != NULL);
  assert (pass != NULL);

  if (new_js != NULL)
  {
    new_js->server = malloc (1 + strlen (server));
    if (new_js->server)  strcpy (new_js->server, server);
    new_js->user = malloc (1 + strlen (user));
    if (new_js->user)  strcpy (new_js->user, user);
    new_js->pass = malloc (1 + strlen (pass));
    if (new_js->pass)  strcpy (new_js->pass, pass);
    new_js->port = port;

    if (new_js->server == NULL || new_js->user == NULL || new_js->pass == NULL)
    {
      jsonrpc_destroy (new_js);
      new_js = NULL;
    }
  }
  return new_js;
}

/*! \brief Destroys a JSONRPC object */
void jsonrpc_destroy (jsonrpc_t *js)
{
  free (js->server);
  free (js->user);
  free (js->pass);
  free (js);
}

/*! \brief Execute a JSONRPC request */
json_t *jsonrpc_request (const jsonrpc_t *js, const char *req_name, json_t *data)
{
  static int request_ctr = 1;
  CURL *curl;
  char curl_error_buf[CURL_ERROR_SIZE];
  json_t *request = json_object ();
  json_error_t json_error;
  char *request_str = NULL;
  buffer_t *response = buffer_new ();

  /* Build request */
  if (request == NULL || response == NULL)
  {
    fputs ("OOM.\n", stderr);
    goto failure;
  }
  if (json_object_set_new (request, "method", json_string (req_name)))
  {
    fputs ("Failed to create request (unable to set method).\n", stderr);
    goto failure;
  }
  if (data != NULL && json_object_set (request, "params", data))
  {
    fputs ("Failed to create request (unable to set params).\n", stderr);
    goto failure;
  }
  if (json_object_set_new (request, "id", json_integer (request_ctr++)))
  {
    fputs ("Failed to create request (unable to set id).\n", stderr);
    goto failure;
  }

  request_str = json_dumps (request, 0);
  char url[500];
  snprintf (url, sizeof url, "%s:%s@%s:%u", js->user, js->pass, js->server, js->port);

  /* Submit request */
  curl = curl_easy_init ();
  curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, buffer_write_data);
  curl_easy_setopt (curl, CURLOPT_ERRORBUFFER, curl_error_buf);
  curl_easy_setopt (curl, CURLOPT_WRITEDATA, response);
  curl_easy_setopt (curl, CURLOPT_POSTFIELDS, request_str);
  curl_easy_setopt (curl, CURLOPT_URL, url);

  if (curl_easy_perform (curl) != 0)
  {
    fprintf (stderr, "RPC Failure (%s): %s\n", req_name, curl_error_buf);
    curl_easy_cleanup (curl);
    goto failure;
  }
  /* NULL-terminate the curl data */
  buffer_null_terminate (response);

  /* Parse response */
  json_t *rv = json_loads (buffer_get_data (response), 0, &json_error);
  /* A parse error probably means bitcoind didn't like us */
  if (rv == NULL)
    fprintf (stderr, "Connection error: %s\nJSON parse error: %s\n", buffer_get_data (response), json_error.text);

  json_decref (request);
  free (request_str);
  buffer_destroy (response);
  curl_easy_cleanup (curl);
  return rv;

failure:
  json_decref (request);
  free (request_str);
  buffer_destroy (response);
  return NULL;
}


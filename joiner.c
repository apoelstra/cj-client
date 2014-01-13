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
#include <string.h>
#include <stdlib.h>

#include <curl/curl.h>

#include "buffer.h"
#include "joiner.h"
#include "global.h"

struct joiner
{
  enum joiner_status status;
  char *session_id;
  char *tx_to_sign;
  char *update_url;
  char donation_address[41];
  int time_to_switch;
  u64_t per_input_fee;
  u64_t mpo;
  unsigned n_transactions;
};

/*! \brief Create a new joiner object */
joiner_t *joiner_new (const char *update_url, const char *session_id)
{
  joiner_t *rv = malloc (sizeof *rv);
  if (rv)
  {
    rv->status = JS_UNKNOWN;
    rv->update_url = malloc (1 + strlen (update_url));
    strcpy (rv->update_url, update_url);
    rv->tx_to_sign = NULL;
    rv->session_id = NULL;
    rv->per_input_fee = 0;
    rv->mpo = 0;
    if (session_id != NULL)
    {
      rv->session_id = malloc (1 + strlen (session_id));
      strcpy (rv->session_id, session_id);
    }
  }
  return rv;
}

/*! \brief Destroys a joiner object */
void joiner_destroy (joiner_t *jn)
{
  if (jn) free (jn->update_url);
  if (jn) free (jn->session_id);
  free (jn);
}

static buffer_t *read_to_buffer (CURL *curl)
{
  char curl_error_buf[CURL_ERROR_SIZE];
  buffer_t *response = buffer_new ();

  if (response == NULL)
  {
    fputs ("OOM creating buffer", stderr);
    return NULL;
  }

  curl_easy_setopt (curl, CURLOPT_ERRORBUFFER, curl_error_buf);
  curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, buffer_write_data);
  curl_easy_setopt (curl, CURLOPT_WRITEDATA, response);

  if (curl_easy_perform (curl) != 0)
  {
    fprintf (stderr, "Join server: %s\n", curl_error_buf);
    buffer_destroy (response);
    return NULL;
  }
  /* NULL-terminate the curl data */
  buffer_null_terminate (response);
  return response;
}

/*! \brief Resync joiner object with the server */
void joiner_update (joiner_t *jn)
{ 
  CURL *curl = curl_easy_init ();
  assert (jn != NULL);

  if (jn->session_id != NULL)
  {
    char *url = malloc (30 + strlen (jn->update_url) + strlen (jn->session_id));
    if (url != NULL)
    {
      sprintf (url, "%s?session=%s", jn->update_url, jn->session_id);
      curl_easy_setopt (curl, CURLOPT_URL, url);
    } else fputs ("OOM setting URL, Bad Things will happen.\n", stderr);
    free (url);

    fprintf (stderr, "Syncing with joiner, session ID %s\n", jn->session_id);
  }
  else
  {
    curl_easy_setopt (curl, CURLOPT_URL, jn->update_url);
    fprintf (stderr, "Syncing with joiner, session ID unknown\n");
  }

  buffer_t *response = read_to_buffer (curl);
  if (response == NULL)
    return;
  /* Parse response */
  free (jn->tx_to_sign);
  jn->tx_to_sign = malloc (buffer_get_size (response));
  /* session_status:time_to_switch:donation_addr:required_fee:most_popular_output:n_transactions:session_id:session_spookwords:tx_to_sign */
#ifdef __WINDOWS__
  __mingw_sscanf
#else
  sscanf
#endif
         (buffer_get_data (response), "%d:%d:%40[1a-zA-Z]:%llu:%llu:%u:%*[^:]:%*[^:]:%s",
          &jn->status, &jn->time_to_switch, jn->donation_address,
          &jn->per_input_fee, &jn->mpo, &jn->n_transactions,
          jn->tx_to_sign);
  buffer_destroy (response);
  curl_easy_cleanup (curl);
}

static char *real_joiner_submit (joiner_t *jn, const char *raw_tx, const char *post_var)
{
  char *rv;
  char *post_fields = malloc (30 + strlen (raw_tx));
  CURL *curl = curl_easy_init ();

  assert (jn != NULL);
  if (post_fields == NULL)
    return NULL;

  if (jn->session_id != NULL)
  {
    char *url = malloc (30 + strlen (jn->update_url) + strlen (jn->session_id));
    if (url != NULL)
    {
      sprintf (url, "%s?session=%s", jn->update_url, jn->session_id);
      curl_easy_setopt (curl, CURLOPT_URL, url);
    } else fputs ("OOM setting URL, Bad Things will happen.\n", stderr);
    free (url);

    fprintf (stderr, "Submitting %s to joiner, session ID %s\n", post_var, jn->session_id);
  }
  else
  {
    curl_easy_setopt (curl, CURLOPT_URL, jn->update_url);
    fprintf (stderr, "Submitting %s to joiner, session ID unknown\n", post_var);
  }

  sprintf (post_fields, "%s=%s", post_var, raw_tx);
  curl_easy_setopt (curl, CURLOPT_POSTFIELDS, post_fields);

  buffer_t *response = read_to_buffer (curl);
  if (response == NULL)
    return NULL;

  rv = malloc (1 + buffer_get_size (response));
  strcpy (rv, buffer_get_data (response));
  /* trim trailing whitespace/crap from sess id */
  int i;
  for (i = 0; isalnum (rv[i]); ++i);
  rv[i] = 0;

  buffer_destroy (response);
  curl_easy_cleanup (curl);
  free (post_fields);

  return rv;
}

/*! \brief Submits an unsigned transaction to the server
 *  \return The joiner session ID
 */
const char *joiner_submit_unsigned (joiner_t *jn, const char *raw_tx)
{
  char *sess_id = real_joiner_submit (jn, raw_tx, "unsigned_tx");
  if (sess_id == NULL)
    return NULL;
  jn->session_id = sess_id;
  return sess_id;
}

/*! \brief Submits an signed transaction to the server */
void joiner_submit_signed (joiner_t *jn, const char *raw_tx)
{
  free (real_joiner_submit (jn, raw_tx, "signed_tx"));
}

/*! \brief Getter for joiner status */
enum joiner_status joiner_status (const joiner_t *jn)
{
  assert (jn != NULL);
  return jn->status;
}

/*! \brief Getter for joiner status */
const char *joiner_status_text (const joiner_t *jn)
{
  assert (jn != NULL);
  switch (jn->status)
  {
  case JS_NOTFOUND:   return "session not found";
  case JS_NOTYETOPEN: return "no open session (submit a transaction to start one)";
  case JS_STILLOPEN:  return "session active";
  case JS_NEEDSIGS:   return "session waiting on signatures";
  case JS_COMPLETE:   return "session complete :)";
  case JS_INVALID:    return "invalid session";
  default:            return "unknown";
  }
}

/*! \brief Getter for donation address */
const char *joiner_donation_address (const joiner_t *jn)
{
  assert (jn != NULL);
  return jn->donation_address;
}

/*! \brief Getter for per-input fee */
u64_t joiner_per_input_fee (const joiner_t *jn)
{
  assert (jn != NULL);
  return jn->per_input_fee;
}

/*! \brief Getter for most popular output */
u64_t joiner_most_popular_output (const joiner_t *jn)
{
  assert (jn != NULL);
  return jn->mpo;
}

/*! \brief Getter for # of transactions */
unsigned joiner_n_transactions (const joiner_t *jn)
{
  assert (jn != NULL);
  return jn->n_transactions;
}

/*! \brief Getter for time remaining */
int joiner_minutes_remaining (const joiner_t *jn)
{
  assert (jn != NULL);
  return jn->time_to_switch;
}

/*! \brief Getter for tx to sign (if the joiner is waiting on sigs) */
const char *joiner_tx_to_sign (const joiner_t *jn)
{
  assert (jn != NULL);
  return jn->tx_to_sign;
}

/*! \brief Getter for txid (if the joiner status is complete) */
const char *joiner_txid (const joiner_t *jn)
{
  assert (jn != NULL);
  return jn->tx_to_sign;
}


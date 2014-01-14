/* fetchnews
 * Written in 2013 by
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

/*! \file buffer.c
 *  \brief Stores downloaded data from curl (implementation)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

#define DEFAULT_BUFFER_SIZE	4000

struct buffer
{
  unsigned char *data;
  size_t used;
  size_t max_size;
};


buffer_t *buffer_new ()
{
  buffer_t *rv = malloc (sizeof *rv);
  if (rv == NULL)
    return NULL;

  rv->max_size = DEFAULT_BUFFER_SIZE;
  rv->data = malloc (rv->max_size);
  rv->used = 0;

  return rv;
}

/*! \brief Buffer destructor */
void buffer_destroy (buffer_t *buf)
{
  if (buf)
    free (buf->data);
  free (buf);
}

int buffer_get_size (const buffer_t *buf)
{
  return (int) buf->used; 
}

const char *buffer_get_data (const buffer_t *buf)
{
  return (const char *) buf->data;
}

/*!\brief NULL-terminate a buffer
 * \return 0 on failure, 1 on success
 */
int buffer_null_terminate (buffer_t *buf)
{
  char zero = 0;
  return buffer_write_data (&zero, 1, 1, buf);
}

/*!\brief Function to give curl
 *
 * Set CURLOPT_WRITEFUNCTION to this function, set CURLOPT_WRITEDATA
 * to your buffer structure.
 */
size_t buffer_write_data (void *buffer, size_t size, size_t nmemb, void *userp)
{
  buffer_t *buf = userp;

  while (size * nmemb + buf->used > buf->max_size)
  {
    void *newbuf = realloc (buf->data, 2 * buf->max_size);
    if (newbuf == NULL)
      return 0;

    buf->max_size *= 2;
    buf->data = newbuf;
  }

  memcpy (buf->data + buf->used, buffer, size * nmemb);
  buf->used += size * nmemb;
  return size * nmemb;
}

/*! \brief Clear the contents of a buffer */
void buffer_empty (buffer_t *buf)
{
  buf->used = 0;
}

/*!\brief Reads a line from a file */
#define RAWBUFLEN 80
size_t buffer_fgets (buffer_t *buf, FILE *fh)
{
  size_t old_len = buffer_get_size (buf);
  char smallbuf[RAWBUFLEN] = {0};

  smallbuf[RAWBUFLEN - 2] = 0;
  while (fgets (smallbuf, sizeof smallbuf, fh) &&
         smallbuf[RAWBUFLEN - 2] != 0 && smallbuf[RAWBUFLEN - 2] != '\n')
  {
    buffer_write_data (smallbuf, 1, (sizeof smallbuf) - 1, buf);
    smallbuf[RAWBUFLEN - 2] = 0;
  }
  buffer_write_data (smallbuf, 1, strlen (smallbuf), buf);
  buffer_null_terminate (buf);

  return buffer_get_size (buf) - old_len - 1;
}



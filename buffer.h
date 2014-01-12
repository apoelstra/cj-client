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

/*! \file buffer.h
 *  \brief Stores downloaded data from curl
 */
#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stddef.h>

typedef struct buffer buffer_t;

/*! \brief Buffer constructor */
buffer_t *buffer_new ();
/*! \brief Buffer destructor */
void buffer_destroy (buffer_t *);
/*! \brief Clear the contents of a buffer */
void buffer_empty (buffer_t *);

/*! \brief Retrieve the buffer contents */
const char *buffer_get_data (const buffer_t *);
/*! \brief Retrieve the used space in the buffer */
int buffer_get_size (const buffer_t *);

/*!\brief Function to give curl
 *
 * Set CURLOPT_WRITEFUNCTION to this function, set CURLOPT_WRITEDATA
 * to your buffer structure.
 */
size_t buffer_write_data (void *buffer, size_t size, size_t nmemb, void *userp); 

/*!\brief NULL-terminate a buffer */
int buffer_null_terminate (buffer_t *buf);

/*!\brief Reads a line from a file */
size_t buffer_fgets (buffer_t *, FILE *);

#endif

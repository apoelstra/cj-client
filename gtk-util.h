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

#ifndef GTK_UTIL_H
#define GTK_UTIL_H

typedef struct ui_input_list ui_input_list_t;

/*! \brief Add a text entry input to the input list */
void ui_input_list_push_text (ui_input_list_t **, const char *label, const char *default_text);

/*! \brief Add a spinbox input to the input list */
void ui_input_list_push_spinbox (ui_input_list_t **, const char *label, double default_val, double min, double max, double step);

/*! \brief Remove the most recently-added widget from the list and return it */
const char *ui_input_list_pop_text (ui_input_list_t **);
/*! \brief Remove the most recently-added widget from the list and return it */
double ui_input_list_pop_spinbox (ui_input_list_t **);

/*! \brief Return a GtkGrid with all the inputs in it
 *
 * Note that you NEED to run this function in order that the
 * label/entry widgets get reffed, otherwise they'll just be
 * floating forever.
 */
GtkWidget *ui_input_list_grid (const ui_input_list_t *);

#endif

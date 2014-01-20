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

#include "global.h"

#include <assert.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#include "gtk-util.h"

struct ui_input_list {
  GtkWidget *label;
  GtkWidget *input;
  ui_input_list_t *next;
};



/*! \brief Add a text entry input to the input list */
void ui_input_list_push_text (ui_input_list_t **uil, const char *label, const char *default_text)
{
  ui_input_list_t *new_uil = malloc (sizeof *new_uil);
  if (new_uil == NULL)
    return;

  /* Create the input/label pair */
  new_uil->label = gtk_label_new (label);
  gtk_misc_set_alignment (GTK_MISC (new_uil->label), 1, 0.5);
  new_uil->input = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY (new_uil->input), default_text);

  /* Append the new node */
  new_uil->next = *uil;
  *uil = new_uil;
}

/*! \brief Add a spinbox input to the input list */
void ui_input_list_push_spinbox (ui_input_list_t **uil, const char *label, double default_value, double min, double max, double step)
{
  ui_input_list_t *new_uil = malloc (sizeof *new_uil);
  if (new_uil == NULL)
    return;

  /* Create the input/label pair */
  new_uil->label = gtk_label_new (label);
  gtk_misc_set_alignment (GTK_MISC (new_uil->label), 1, 0.5);
  new_uil->input = gtk_spin_button_new_with_range (min, max, step);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (new_uil->input), default_value);

  /* Append the new node */
  new_uil->next = *uil;
  *uil = new_uil;
}

/*! \brief Remove the most recently-added widget from the list and return it */
const char *ui_input_list_pop_text (ui_input_list_t **uil)
{
  assert (uil != NULL);
  GtkWidget *rv = (*uil)->input;
  *uil = (*uil)->next;
  return gtk_entry_get_text (GTK_ENTRY (rv));
}

/*! \brief Remove the most recently-added widget from the list and return it */
double ui_input_list_pop_spinbox (ui_input_list_t **uil)
{
  assert (uil != NULL);
  GtkWidget *rv = (*uil)->input;
  *uil = (*uil)->next;
  return gtk_spin_button_get_value (GTK_SPIN_BUTTON (rv));
}

/*! \brief Return a GtkGrid with all the inputs in it */
GtkWidget *ui_input_list_grid (const ui_input_list_t *uil)
{
  GtkWidget *grid = gtk_grid_new ();

  gtk_grid_set_row_spacing (GTK_GRID (grid), 5);
  gtk_grid_set_column_spacing (GTK_GRID (grid), 5);

  int i = 0;
  while (uil)
  {
    gtk_grid_attach (GTK_GRID (grid), uil->label, 0, i, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), uil->input, 1, i, 1, 1);
    uil = uil->next;
    ++i;
  }
  return grid;
}






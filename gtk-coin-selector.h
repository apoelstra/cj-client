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

#ifndef GTK_COIN_SELECTOR_H
#define GTK_COIN_SELECTOR_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <jansson.h>

#include "bitcoin.h"

G_BEGIN_DECLS  /* keep c++ happy */

#define GTK_COIN_SELECTOR_TYPE            (gtk_coin_selector_get_type ())
#define GTK_COIN_SELECTOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_COIN_SELECTOR_TYPE, GtkCoinSelector))
#define GTK_COIN_SELECTOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_COIN_SELECTOR_TYPE, GtkCoinSelectorClass))
#define IS_GTK_COIN_SELECTOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_COIN_SELECTOR_TYPE))
#define IS_GTK_COIN_SELECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_COIN_SELECTOR_TYPE))

typedef struct _GtkCoinSelector       GtkCoinSelector;
typedef struct _GtkCoinSelectorClass  GtkCoinSelectorClass;

GType gtk_coin_selector_get_type (void);
GtkWidget* gtk_coin_selector_new (void);

void gtk_coin_selector_add_coins (GtkCoinSelector *, const utxo_list_t *);
unsigned gtk_coin_selector_count (GtkCoinSelector *cs);
u64_t gtk_coin_selector_total (GtkCoinSelector *);
json_t *gtk_coin_selector_list (GtkCoinSelector *);

/*! \brief Mark all coins as stale (to be deleted on next update) */
void gtk_coin_selector_mark_all_stale (GtkCoinSelector *cs);

/*! \brief Remove all coins marked stale */
void gtk_coin_selector_clear_stale (GtkCoinSelector *cs);

/*! \brief Remove all coins from the GtkCoinSelector */
void gtk_coin_selector_clear (GtkCoinSelector *cs);

G_END_DECLS  /* keep c++ happy */
#endif

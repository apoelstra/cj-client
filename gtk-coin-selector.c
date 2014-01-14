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

#include <gtk/gtk.h>
#include <jansson.h>

#include "gtk-coin-selector.h"

/*! \brief Signals exposed by the widget */
enum {
  TOGGLE_COIN_SIGNAL,
  LAST_SIGNAL
};

/*! \brief Columns used for internal data store */
enum {
  SELECTED_COL,
  ADDRESS_COL,
  TXID_COL,
  FULL_TXID_COL,
  VOUT_COL,
  VALUE_COL,
  DISP_VALUE_COL,
  NCONFIRMS_COL,
  N_COLS
};

static GtkTreeView *gtk_coin_selector_parent_class;
static guint gtk_coin_selector_signals[LAST_SIGNAL] = { 0 };

/*! \brief private coin selector structure */
struct _GtkCoinSelector
{
  GtkTreeView parent;

  GtkListStore *list_store;
};

/*! \brief private coin selector class structure */
struct _GtkCoinSelectorClass
{
  GtkTreeViewClass parent_class;

  void (* toggle_coin) (GtkCoinSelector *, gint);
};


/*! \brief Clean up object before garbage collection */
static void gtk_coin_selector_finalize (GObject *object)
{
  GtkCoinSelector *cs = (GtkCoinSelector *) object;
  g_object_unref (cs->list_store);
  G_OBJECT_CLASS (gtk_coin_selector_parent_class)->finalize (object);
}

/*! \brief Initialize the GtkCoinSelector class */
static void gtk_coin_selector_class_init (GtkCoinSelectorClass *klass)
{
  GObjectClass *object_class = (GObjectClass *) klass;

  gtk_coin_selector_signals[TOGGLE_COIN_SIGNAL] =
    g_signal_new ("toggle-coin",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
                  G_STRUCT_OFFSET (GtkCoinSelectorClass, toggle_coin),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

  object_class->finalize = gtk_coin_selector_finalize;
}

static void coin_toggled_cb (GtkCellRendererToggle *cell, gchar *path, gpointer misc)
{
  gboolean state;
  GtkTreeIter iter;
  GtkCoinSelector *cs = misc;
 
  if (!gtk_tree_model_get_iter_from_string (GTK_TREE_MODEL (cs->list_store),
                                            &iter, path))
    return;

  /* Actually toggle the state */
  gtk_tree_model_get (GTK_TREE_MODEL (cs->list_store),
                      &iter,
                      SELECTED_COL, &state,
                      -1);
  gtk_list_store_set (cs->list_store,
                      &iter,
                      SELECTED_COL, !state,
                      -1);
  g_signal_emit (cs, gtk_coin_selector_signals[TOGGLE_COIN_SIGNAL], 0);
  (void) cell;
}

/*!\brief Actually create the GtkCoinSelector */
static void gtk_coin_selector_init (GtkCoinSelector *cs)
{
  GtkTreeViewColumn *col_selected;
  GtkTreeViewColumn *col_addr;
  GtkTreeViewColumn *col_txid;
  GtkTreeViewColumn *col_value;
  GtkTreeViewColumn *col_conf;

  GtkCellRenderer *toggle_renderer = gtk_cell_renderer_toggle_new();
  g_signal_connect (toggle_renderer, "toggled",
                    G_CALLBACK (coin_toggled_cb), cs);
  GtkCellRenderer *text_renderer = gtk_cell_renderer_text_new();
  gtk_cell_renderer_set_alignment (text_renderer, 0.5, 0.5);
  GtkCellRenderer *left_text_renderer = gtk_cell_renderer_text_new();


  #define gtk_tv_column_new gtk_tree_view_column_new_with_attributes
  col_selected = gtk_tv_column_new ("Included",
                                    toggle_renderer,
                                    "active", SELECTED_COL,
                                    NULL);
  col_value = gtk_tv_column_new ("Value",
                                 text_renderer,
                                 "text", DISP_VALUE_COL,
                                 NULL);
  col_addr = gtk_tv_column_new ("Address",
                                left_text_renderer,
                                "text", ADDRESS_COL,
                                NULL);
  col_txid = gtk_tv_column_new ("Previous Transaction",
                                left_text_renderer,
                                "text", TXID_COL,
                                NULL);
  col_conf = gtk_tv_column_new ("Confirmations",
                                text_renderer,
                                "text", NCONFIRMS_COL,
                                NULL);
  #undef gtk_tv_column_new

  cs->list_store = gtk_list_store_new (N_COLS,
                 /* SELECTED_COL    */ G_TYPE_BOOLEAN,
                 /* ADDRESS_COL     */ G_TYPE_STRING,
                 /* TXID_COL        */ G_TYPE_STRING,
                 /* FULL_TXID_COL   */ G_TYPE_STRING,
                 /* VOUT_COL        */ G_TYPE_UINT,
                 /* VALUE_COL       */ G_TYPE_UINT64,
                 /* DISP_VALUE_COL  */ G_TYPE_DOUBLE,
                 /* NCONFIRMS_COL   */ G_TYPE_UINT);

  gtk_tree_view_insert_column (GTK_TREE_VIEW (cs), col_selected, -1);
  gtk_tree_view_insert_column (GTK_TREE_VIEW (cs), col_value, -1);
  gtk_tree_view_insert_column (GTK_TREE_VIEW (cs), col_addr, -1);
  gtk_tree_view_insert_column (GTK_TREE_VIEW (cs), col_conf, -1);
  gtk_tree_view_insert_column (GTK_TREE_VIEW (cs), col_txid, -1);
  gtk_tree_view_set_model (GTK_TREE_VIEW (cs), GTK_TREE_MODEL (cs->list_store));
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (cs), TRUE);
}

/*! \brief Create a new GtkCoinSelector
 *
 *  \return a freshly-allocated GtkCoinSelector.
 */
GtkWidget *gtk_coin_selector_new (void)
{
  return GTK_WIDGET (g_object_new (GTK_COIN_SELECTOR_TYPE, NULL));
}

/*! \brief Add a list of coins to the GtkCoinSelector */
void gtk_coin_selector_add_coins (GtkCoinSelector *cs, const utxo_list_t *utxo_list)
{
  GtkTreeIter iter;

  while (utxo_list)
  {
    char *truncated_txid = g_strdup_printf ("%.12s..., output %u",
                                            bitcoin_utxo_txid (utxo_list),
                                            bitcoin_utxo_vout (utxo_list));
    gtk_list_store_append (cs->list_store, &iter);
    gtk_list_store_set (cs->list_store, &iter,
                        SELECTED_COL, FALSE,
                        TXID_COL,     truncated_txid,
                        ADDRESS_COL,  bitcoin_utxo_address (utxo_list),
                        FULL_TXID_COL, bitcoin_utxo_txid (utxo_list),
                        VOUT_COL,     bitcoin_utxo_vout (utxo_list),
                        VALUE_COL,    bitcoin_utxo_value (utxo_list),
                        DISP_VALUE_COL, FROM_SATOSHI (bitcoin_utxo_value (utxo_list)),
                        NCONFIRMS_COL, bitcoin_utxo_nconfirms (utxo_list),
                        -1);
    g_free (truncated_txid);
    utxo_list = bitcoin_utxo_next (utxo_list);
  }
}

/*! \brief Getter for fees */
unsigned gtk_coin_selector_count (GtkCoinSelector *cs)
{
  unsigned rv = 0;
  GtkTreeIter iter;

  if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (cs->list_store), &iter))
  {
    do
      {
        gboolean sel;
        gtk_tree_model_get (GTK_TREE_MODEL (cs->list_store),
                            &iter, SELECTED_COL, &sel, -1);
        if (sel) ++rv;
      }
    while (gtk_tree_model_iter_next (GTK_TREE_MODEL (cs->list_store), &iter));
  }
  return rv;
}

/*! \brief Getter for total input value */
u64_t gtk_coin_selector_total (GtkCoinSelector *cs)
{
  u64_t rv = 0;
  GtkTreeIter iter;

  if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (cs->list_store), &iter))
  {
    do
      {
        gboolean sel;
        u64_t val;
        gtk_tree_model_get (GTK_TREE_MODEL (cs->list_store),
                            &iter, SELECTED_COL, &sel, VALUE_COL, &val, -1);
        if (sel) rv += val;
      }
    while (gtk_tree_model_iter_next (GTK_TREE_MODEL (cs->list_store), &iter));
  }
  return rv;
}

/*! \brief Getter for JSON-encoded list of selected transactions */
json_t *gtk_coin_selector_list (GtkCoinSelector *cs)
{
  GtkTreeIter iter;
  json_t *rv = json_array ();

  if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (cs->list_store), &iter))
  {
    do
      {
        gboolean sel;
        char *txid;
        unsigned vout;
        gtk_tree_model_get (GTK_TREE_MODEL (cs->list_store), &iter,
                            SELECTED_COL, &sel,
                            FULL_TXID_COL, &txid,
                            VOUT_COL, &vout, -1);
        if (sel)
        {
          json_t *append = json_object();
          json_object_set_new (append, "txid", json_string (txid));
          json_object_set_new (append, "vout", json_integer (vout));
          json_array_append_new (rv, append);
        }
        free (txid);
      }
    while (gtk_tree_model_iter_next (GTK_TREE_MODEL (cs->list_store), &iter));
  }
  return rv;
}


/*! \brief Returns the GType of the coin selector
 *  This is just Gtk+ boilerplate.
 */
GType gtk_coin_selector_get_type (void)
{
  static GType cs_type = 0;

  if (!cs_type)
    {
      const GTypeInfo cs_info =
      {
        sizeof (GtkCoinSelectorClass),
        NULL, /* base_init */
        NULL, /* base_finalize */
        (GClassInitFunc) gtk_coin_selector_class_init,
        NULL, /* class_finalize */
        NULL, /* class_data */
        sizeof (GtkCoinSelector),
        0,    /* n_preallocs */
        (GInstanceInitFunc) gtk_coin_selector_init,
        NULL
      };

      cs_type = g_type_register_static (GTK_TYPE_TREE_VIEW,
                                        "GtkCoinSelector",
                                        &cs_info,
                                        0);
    }

  return cs_type;
}





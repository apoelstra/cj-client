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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <jansson.h>

#include "bitcoin.h"
#include "gtk-coin-selector.h"
#include "gtk-util.h"
#include "joiner.h"
#include "jsonrpc.h"
#include "output.h"
#include "settings.h"

static void popup_message (const char *title, const char *text);
static void view_button_clicked_cb (GtkButton *bt, gpointer misc);
static void submit_button_clicked_cb (GtkButton *bt, gpointer misc);
static void delete_output_button_clicked_cb (GtkButton *bt, gpointer misc);
static void delete_privkey_button_clicked_cb (GtkButton *bt, gpointer misc);
static void add_output_button_clicked_cb (GtkButton *bt, gpointer misc);
static void add_privkey_button_clicked_cb (GtkButton *bt, gpointer misc);
static gboolean server_status_update (gpointer misc);

struct {
  GtkApplication *app;
  GtkWidget *window, *view_window;
  GtkTextBuffer *txview_buffer;
  GtkWidget *status_bar;
  GtkWidget *coin_selector, *output_entry;
  GtkWidget *noutput_disp, *total_disp, *fee_disp;
  GMenu *menu;
  output_list_t *output_list;
  jsonrpc_t *bitcoind;
  joiner_t *joiner;
  u64_t per_input_fee;
  gboolean use_bitcoind_for_privkeys;
} gui_data;

/* MENU CALLBACKS */
static void menu_quit (GSimpleAction *action, GVariant *parameter, gpointer misc)
{
  GApplication *app = misc;
  g_application_quit (app);
  (void) action;
  (void) parameter;
}

static void menu_viewtx (GSimpleAction *action, GVariant *parameter, gpointer misc)
{
  view_button_clicked_cb (NULL, misc);
  (void) action;
  (void) parameter;
}

static void menu_submittx (GSimpleAction *action, GVariant *parameter, gpointer misc)
{
  submit_button_clicked_cb (NULL, misc);
  (void) action;
  (void) parameter;
}

static void menu_forgetsession (GSimpleAction *action, GVariant *parameter, gpointer misc)
{
  /* Reset the joiner connection */
  joiner_destroy (gui_data.joiner);
  gui_data.joiner = joiner_new (settings_get_server_url (), NULL);
  /* Clear saved session data */
  bitcoin_my_transactions_reset ();
  settings_set_session_id (NULL);
  settings_set_submission (NULL);
  settings_save_config ();
  /* Update display */
  server_status_update (NULL);
  (void) action;
  (void) parameter;
  (void) misc;
}

static void menu_settings (GSimpleAction *action, GVariant *parameter, gpointer misc)
{
  ui_input_list_t *uil = NULL;
  GtkWidget *dialog, *content, *grid;
  dialog = gtk_dialog_new_with_buttons ("Settings",
                                        GTK_WINDOW (gui_data.window),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        "_OK", GTK_RESPONSE_ACCEPT,
                                        "_Cancel", GTK_RESPONSE_REJECT,
                                        NULL);

  ui_input_list_push_text (&uil, "RPC Server:", settings_get_rpc_server());
  ui_input_list_push_spinbox (&uil, "Port:", settings_get_rpc_port(), 0, 65535, 1);
  ui_input_list_push_text (&uil, "Username:", settings_get_rpc_user());
  ui_input_list_push_text (&uil, "Password:", settings_get_rpc_pass());
  ui_input_list_push_text (&uil, "Joiner Server:", settings_get_server_url());

  grid = ui_input_list_grid (uil);

  content = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  gtk_container_set_border_width (GTK_CONTAINER (content), 15);
  gtk_box_pack_start (GTK_BOX (content), grid, TRUE, TRUE, 5);
  gtk_widget_show_all (dialog);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
    const char *server = ui_input_list_pop_text (&uil);
    const char *rpcpass = ui_input_list_pop_text (&uil);
    const char *rpcuser = ui_input_list_pop_text (&uil);
    int rpcport = ui_input_list_pop_spinbox (&uil); 
    const char *rpcserver = ui_input_list_pop_text (&uil);

    /* Warn user about server changes */
    if (strcmp (server, settings_get_server_url()))
      popup_message ("Settings", "Note that if you changed the joiner URL, "
                                 "you may need to click Sesson->Forget Sesson.");

    settings_set_rpc_server (rpcserver);
    settings_set_rpc_port (rpcport);
    settings_set_rpc_user (rpcuser);
    settings_set_rpc_pass (rpcpass);
    settings_set_server_url (server);
    settings_save_config ();

    /* Reset everything */
    jsonrpc_destroy (gui_data.bitcoind);
    gui_data.bitcoind = jsonrpc_new (settings_get_rpc_server (),
                                     settings_get_rpc_port (),
                                     settings_get_rpc_user (),
                                     settings_get_rpc_pass ());
    joiner_destroy (gui_data.joiner);
    gui_data.joiner = joiner_new (settings_get_server_url (),
                                  settings_get_session_id ());
    server_status_update (NULL);
  }

  gtk_widget_destroy (dialog);

  (void) action;
  (void) parameter;
  (void) misc;
}

static void menu_addinput (GSimpleAction *action, GVariant *parameter, gpointer misc)
{
  ui_input_list_t *uil = NULL;
  GtkWidget *dialog, *content, *grid;
  dialog = gtk_dialog_new_with_buttons ("Add Inputs",
                                        GTK_WINDOW (gui_data.window),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        "_OK", GTK_RESPONSE_ACCEPT,
                                        NULL);

  ui_input_list_push_spinbox (&uil, "Value (sat):", 0, 0, ((u64_t) 1 << 40), 1);
  ui_input_list_push_spinbox (&uil, "Index:", 0, 0, 10000, 1);
  ui_input_list_push_text (&uil, "Tx ID:", "");
  grid = ui_input_list_grid (uil);

  content = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  gtk_container_set_border_width (GTK_CONTAINER (content), 15);
  gtk_box_pack_start (GTK_BOX (content), grid, TRUE, TRUE, 5);
  gtk_widget_show_all (dialog);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
    const char *txid = ui_input_list_pop_text (&uil);
    unsigned vout = ui_input_list_pop_spinbox (&uil);
    u64_t value = ui_input_list_pop_spinbox (&uil);

    gtk_coin_selector_add_coin (GTK_COIN_SELECTOR (gui_data.coin_selector),
                                txid, vout, value);
  }

  gtk_widget_destroy (dialog);
  
  (void) action;
  (void) parameter;
  (void) misc;
}

static void menu_addoutputs (GSimpleAction *action, GVariant *parameter, gpointer misc)
{
  GtkWidget *checkbutton;
  GtkWidget *dialog, *content, *grid;
  dialog = gtk_dialog_new_with_buttons ("Add Outputs",
                                        GTK_WINDOW (gui_data.window),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        "_OK", GTK_RESPONSE_ACCEPT,
                                        NULL);

  grid = gtk_grid_new ();
  checkbutton = gtk_check_button_new_with_mnemonic ("O_btain extra addresses from bitcoind");
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton),
                                output_list_get_use_bitcoind (gui_data.output_list));

  const output_t *out = output_list_get_first_output (gui_data.output_list);
  int i = 0;
  while (out)
  {
    if (!output_get_from_bitcoind (out))
    {
      GtkWidget *delete_button = gtk_button_new_with_label ("Delete");
      gtk_grid_attach (GTK_GRID (grid), gtk_label_new (output_get_address (out)), 0, i, 1, 1);
      gtk_grid_attach (GTK_GRID (grid), delete_button, 1, i, 1, 1);
      /* Warning: we pass `out' to the signal handler as non-const so
       * that it can be deleted. */
      g_signal_connect (G_OBJECT (delete_button), "clicked",
                        G_CALLBACK (delete_output_button_clicked_cb), (void *) out);
      ++i;
    }
    out = output_get_next (out);
  }

  GtkWidget *add_field = gtk_entry_new ();
  GtkWidget *add_button = gtk_button_new_with_mnemonic ("_Add");
  g_signal_connect (G_OBJECT (add_button), "clicked",
                    G_CALLBACK (add_output_button_clicked_cb), add_field);

  gtk_grid_attach (GTK_GRID (grid), add_field, 0, 10000, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), add_button, 1, 10000, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), checkbutton, 0, 10001, 1, 1);
  
  /* Display dialog */
  gtk_grid_set_row_spacing (GTK_GRID (grid), 5);
  gtk_grid_set_column_spacing (GTK_GRID (grid), 5);

  content = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  gtk_container_set_border_width (GTK_CONTAINER (content), 15);
  gtk_box_pack_start (GTK_BOX (content), grid, TRUE, TRUE, 5);
  gtk_widget_show_all (dialog);
  gtk_dialog_run (GTK_DIALOG (dialog));

  /* Read out results */
  output_list_set_use_bitcoind (gui_data.output_list,
                                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbutton)));
  gtk_widget_destroy (dialog);
  
  (void) action;
  (void) parameter;
  (void) misc;
}

/* end MENU CALLBACKS */
/* SIGNAL CALLBACKS */
static void update_display ()
{
  u64_t cs_total = gtk_coin_selector_total (GTK_COIN_SELECTOR (gui_data.coin_selector));
  u64_t target_val = TO_SATOSHI (gtk_spin_button_get_value (GTK_SPIN_BUTTON (gui_data.output_entry)));
  gchar *new_total_text = g_strdup_printf ("%.08f", FROM_SATOSHI (cs_total));
  gchar *new_fee_text = g_strdup_printf ("%.08f", FROM_SATOSHI (gui_data.per_input_fee * gtk_coin_selector_count (GTK_COIN_SELECTOR (gui_data.coin_selector))));
  gchar *new_noutput_text = g_strdup_printf ("%u", (unsigned) ceil ((double) cs_total / target_val));
  gtk_label_set_text (GTK_LABEL (gui_data.total_disp), new_total_text);
  gtk_label_set_text (GTK_LABEL (gui_data.fee_disp), new_fee_text);
  gtk_label_set_text (GTK_LABEL (gui_data.noutput_disp), new_noutput_text);
  g_free (new_total_text);
  g_free (new_fee_text);
  g_free (new_noutput_text);
}

static void menu_addprivkeys (GSimpleAction *action, GVariant *parameter, gpointer misc)
{
  GtkWidget *checkbutton;
  GtkWidget *dialog, *content, *grid;
  dialog = gtk_dialog_new_with_buttons ("Add Private Keys",
                                        GTK_WINDOW (gui_data.window),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        "_OK", GTK_RESPONSE_ACCEPT,
                                        NULL);

  grid = gtk_grid_new ();
  checkbutton = gtk_check_button_new_with_mnemonic ("O_btain missing keys from bitcoind");
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton),
                                gui_data.use_bitcoind_for_privkeys);

  const json_t *privkeys = bitcoin_my_transaction_privkeys ();
  for (unsigned i = 0; i < json_array_size (privkeys); ++i)
  {
    const char *key = json_string_value (json_array_get (privkeys, i));
    GtkWidget *delete_button = gtk_button_new_with_label ("Delete");
    gtk_grid_attach (GTK_GRID (grid), gtk_label_new (key), 0, i, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), delete_button, 1, i, 1, 1);
    g_signal_connect (G_OBJECT (delete_button), "clicked",
                      G_CALLBACK (delete_privkey_button_clicked_cb), (gpointer) key);
  }

  GtkWidget *add_field = gtk_entry_new ();
  GtkWidget *add_button = gtk_button_new_with_mnemonic ("_Add");
  g_signal_connect (G_OBJECT (add_button), "clicked",
                    G_CALLBACK (add_privkey_button_clicked_cb), add_field);

  gtk_grid_attach (GTK_GRID (grid), add_field, 0, 10000, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), add_button, 1, 10000, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), checkbutton, 0, 10001, 1, 1);
  
  /* Display dialog */
  gtk_grid_set_row_spacing (GTK_GRID (grid), 5);
  gtk_grid_set_column_spacing (GTK_GRID (grid), 5);

  content = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  gtk_container_set_border_width (GTK_CONTAINER (content), 15);
  gtk_box_pack_start (GTK_BOX (content), grid, TRUE, TRUE, 5);
  gtk_widget_show_all (dialog);
  gtk_dialog_run (GTK_DIALOG (dialog));

  /* Read out results */
  gui_data.use_bitcoind_for_privkeys =
    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbutton));
  gtk_widget_destroy (dialog);

  (void) action;
  (void) parameter;
  (void) misc;
}

static void delete_output_button_clicked_cb (GtkButton *bt, gpointer misc)
{
  output_destroy (misc);
  gtk_button_set_label (bt, "Deleted");
  gtk_widget_set_sensitive (GTK_WIDGET (bt), FALSE);
}

static void delete_privkey_button_clicked_cb (GtkButton *bt, gpointer misc)
{
  bitcoin_my_transactions_delete_privkey (misc);
  gtk_button_set_label (bt, "Deleted");
  gtk_widget_set_sensitive (GTK_WIDGET (bt), FALSE);
}

static void add_output_button_clicked_cb (GtkButton *bt, gpointer misc)
{
  GtkWidget *entry = misc;
  GtkWidget *grid = gtk_widget_get_parent (entry);
  const char *addr = gtk_entry_get_text (GTK_ENTRY (entry));

  /* Insert the new address */
  const output_t *newout = output_list_add_new_address (gui_data.output_list, addr);

  gtk_grid_insert_row (GTK_GRID (grid), 0);
  GtkWidget *delete_button = gtk_button_new_with_label ("Delete");
  gtk_grid_attach (GTK_GRID (grid), gtk_label_new (addr), 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), delete_button, 1, 0, 1, 1);
  /* Warning: we pass `out' to the signal handler as non-const so
   * that it can be deleted. */
  g_signal_connect (G_OBJECT (delete_button), "clicked",
                    G_CALLBACK (delete_output_button_clicked_cb), (void *) newout);
  gtk_entry_set_text (GTK_ENTRY (entry), "");
  gtk_widget_show_all (grid);
  (void) bt;
}

static void add_privkey_button_clicked_cb (GtkButton *bt, gpointer misc)
{
  GtkWidget *entry = misc;
  GtkWidget *grid = gtk_widget_get_parent (entry);
  const char *key = gtk_entry_get_text (GTK_ENTRY (entry));

  /* Insert the new address */
  const json_t *newkey = bitcoin_my_transactions_add_privkey (key);

  gtk_grid_insert_row (GTK_GRID (grid), 0);
  GtkWidget *delete_button = gtk_button_new_with_label ("Delete");
  gtk_grid_attach (GTK_GRID (grid), gtk_label_new (key), 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), delete_button, 1, 0, 1, 1);
  g_signal_connect (G_OBJECT (delete_button), "clicked",
                    G_CALLBACK (delete_privkey_button_clicked_cb),
                    (void *) json_string_value (newkey));
  gtk_entry_set_text (GTK_ENTRY (entry), "");
  gtk_widget_show_all (grid);
  (void) bt;
}

static void coin_selector_toggle_cb (GtkCoinSelector *cs, gpointer misc)
{
  (void) cs;
  (void) misc;
  update_display ();
}

static void output_entry_update_cb (GtkSpinButton *sb, gpointer misc)
{
  (void) sb;
  (void) misc;
  update_display ();
}


static void view_window_close_button_cb (GtkButton *bt, gpointer misc)
{
  gtk_widget_hide (gui_data.view_window);
  (void) bt;
  (void) misc;
}

static void view_window_destroyed_cb (gpointer misc)
{
  gtk_widget_destroy (gui_data.view_window);
  gui_data.view_window = NULL;
  (void) misc;
}

static json_t *get_tx_json (u64_t total, u64_t target, u64_t fee, unsigned n_inputs)
{
  /* Sanity checks */
  if (n_inputs == 0) return NULL;
  if (total < fee)   return NULL;

  /* Create the JSON request */
  json_t *combined = json_array ();
  json_array_append (combined, gtk_coin_selector_list (GTK_COIN_SELECTOR (gui_data.coin_selector)));
  json_array_append (combined, output_list_json (gui_data.output_list, gui_data.bitcoind, total, target,
                                                 fee, joiner_donation_address (gui_data.joiner)));

  return combined;
}

static void view_button_clicked_cb (GtkButton *bt, gpointer misc)
{
  /* Sanity check the data and get the transaction */
  u64_t cs_total = gtk_coin_selector_total (GTK_COIN_SELECTOR (gui_data.coin_selector));
  u64_t target_val = TO_SATOSHI (gtk_spin_button_get_value (GTK_SPIN_BUTTON (gui_data.output_entry)));
  unsigned count = gtk_coin_selector_count (GTK_COIN_SELECTOR (gui_data.coin_selector));
  u64_t fee = gui_data.per_input_fee * count;

  json_t *tx_json = get_tx_json (cs_total, target_val, fee, count);
  if (tx_json == NULL)
  {
    popup_message ("View Transaction", "Couldn't create transaction (not enough inputs).");
    return;
  }
  char *tx_text = json_dumps (tx_json, JSON_INDENT (4));

  /* Display the window */
  /* If the window is already there, just surface it */
  if (gui_data.view_window != NULL)
    gtk_window_present (GTK_WINDOW (gui_data.view_window));
  else {
    GtkWidget *view_grid;
    GtkWidget *close_button;
    GtkWidget *text_view, *scroll_window;

    gui_data.view_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (gui_data.view_window), "View Transaction");
    g_signal_connect (G_OBJECT (gui_data.view_window), "delete_event",
                      G_CALLBACK (view_window_destroyed_cb), NULL);

    view_grid = gtk_grid_new ();
    close_button = gtk_button_new_with_mnemonic ("_Close");
    g_signal_connect (G_OBJECT (close_button), "clicked",
                      G_CALLBACK (view_window_close_button_cb), NULL);
    gtk_grid_attach (GTK_GRID (view_grid), close_button, 0, 1, 1, 1);

    scroll_window = gtk_scrolled_window_new (NULL, NULL);
    text_view = gtk_text_view_new ();
    gtk_text_view_set_editable (GTK_TEXT_VIEW (text_view), FALSE);
    gtk_widget_set_hexpand (text_view, TRUE);
    gtk_widget_set_vexpand (text_view, TRUE);
    gtk_container_add (GTK_CONTAINER (scroll_window), text_view);
    gtk_grid_attach (GTK_GRID (view_grid), scroll_window, 0, 0, 1, 1);

    gui_data.txview_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));

    gtk_container_add (GTK_CONTAINER (gui_data.view_window), view_grid);
    gtk_window_set_default_size (GTK_WINDOW (gui_data.view_window), 400, 600);
    gtk_widget_show_all (gui_data.view_window);
  }

  gtk_text_buffer_set_text (gui_data.txview_buffer, tx_text, -1);
  free (tx_text);
  (void) bt;
  (void) misc;
}

static void popup_message (const char *title, const char *text)
{
  GtkWidget *message = gtk_label_new (text);
  GtkWidget *dialog, *content;
  dialog = gtk_dialog_new_with_buttons (title,
                                        GTK_WINDOW (gui_data.window),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        "_OK", GTK_RESPONSE_ACCEPT, NULL);
  content = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  gtk_container_set_border_width (GTK_CONTAINER (content), 15);
  gtk_box_pack_start (GTK_BOX (content), message, TRUE, TRUE, 5);
  gtk_widget_show_all (dialog);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

static void submit_button_clicked_cb (GtkButton *bt, gpointer misc)
{
  /* Sanity check the data and get the transaction */
  u64_t cs_total = gtk_coin_selector_total (GTK_COIN_SELECTOR (gui_data.coin_selector));
  u64_t target_val = TO_SATOSHI (gtk_spin_button_get_value (GTK_SPIN_BUTTON (gui_data.output_entry)));
  unsigned count = gtk_coin_selector_count (GTK_COIN_SELECTOR (gui_data.coin_selector));
  u64_t fee = gui_data.per_input_fee * count;

  json_t *response, *result;
  json_t *tx_json = get_tx_json (cs_total, target_val, fee, count);
  if (tx_json == NULL)
  {
    popup_message ("Transaction Submission", "Couldn't submit transaction (not enough inputs).");
    return;
  }

  response = jsonrpc_request (gui_data.bitcoind, "createrawtransaction", tx_json);
  result = json_object_get (response, "result");
  if (json_is_string (result))
  {
    const char *sess_id = joiner_submit_unsigned (gui_data.joiner, json_string_value (result));
    if (sess_id == NULL || sess_id[0] == 0)
      popup_message ("Transaction Submission", "The joiner rejected the transaction.");
    else
    {
      popup_message ("Transaction Submission",
                     "Thanks! The status will update to reflect your transaction in about a minute.");
      char *raw;
      output_list_destroy (gui_data.output_list);
      gui_data.output_list = output_list_new ();
      bitcoin_add_my_transaction_raw (gui_data.bitcoind, json_string_value (result));
      raw = bitcoin_get_my_transactions_raw (gui_data.bitcoind);
      settings_set_submission (raw);
      free (raw);
      settings_set_session_id (sess_id);
      settings_save_config ();
    }
  }
  else if (response) puts (json_dumps (response, JSON_INDENT(4)));
  else puts ("Failed to createrawtransaction (bitcoind is down?)");
  json_decref (response);

  (void) bt;
  (void) misc;
}
/* end SIGNAL CALLBACKS */

/* COINJOIN SERVER */
static void pp_entry_activate_cb (GtkEntry *en, gpointer dialog)
{
  g_signal_emit_by_name (dialog, "response", GTK_RESPONSE_ACCEPT, NULL);
  (void) en;
}

static gboolean server_status_update (gpointer misc)
{
  int have_tx_in_pot = bitcoin_my_transactions_p();
  int have_all_keys  = bitcoin_my_transactions_signing_keys_p (gui_data.bitcoind);
  int use_bitcoind   = gui_data.use_bitcoind_for_privkeys;
  int bitcoin_locked = !bitcoin_is_unlocked (gui_data.bitcoind);
  /* If we have transactions in the pot, get the required privkeys 
   * from the user, even if it's not yet time to sign. */
  fprintf (stderr, "Updating keylist (havetx %d  havekeys %d  usebitcoin %d  bitcoin_locked %d)\n",
           have_tx_in_pot, have_all_keys, use_bitcoind, bitcoin_locked);
  if (have_tx_in_pot)
  {
    if (!have_all_keys) 
    {
      if (!use_bitcoind)
        fputs ("Warn: still missing privkeys but not allowed to ask bitcoind.\n", stderr);
      else if (!bitcoin_locked)
        bitcoin_my_transactions_fetch_signing_keys (gui_data.bitcoind);
      else
      {
        GtkWidget *dialog = gtk_dialog_new_with_buttons ("Wallet Passphrase",
                                                         GTK_WINDOW (gui_data.window),
                                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                                         "_OK", GTK_RESPONSE_ACCEPT,
                                                         "_Cancel", GTK_RESPONSE_REJECT,
                                                         NULL);
        GtkWidget *content = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
        GtkWidget *message = gtk_label_new ("You have a transaction in the joiner. "
                                            "To ensure that it can be signed in a "
                                            "timely manner, please enter your wallet "
                                            "passphrase so that the Coinjoin Client "
                                            "can access your signing keys.");
        GtkWidget *pp_label = gtk_label_new ("Passphrase: ");
        GtkWidget *pp_entry = gtk_entry_new ();

        GtkWidget *grid = gtk_grid_new ();

        g_signal_connect (G_OBJECT (pp_entry), "activate",
                          G_CALLBACK (pp_entry_activate_cb), dialog);

        gtk_label_set_line_wrap (GTK_LABEL (message), TRUE);
        gtk_misc_set_alignment (GTK_MISC (pp_label), 1, 0.5);
        gtk_entry_set_visibility (GTK_ENTRY (pp_entry), FALSE);
        gtk_entry_set_width_chars (GTK_ENTRY (pp_entry), 15);
        gtk_grid_attach (GTK_GRID (grid), message, 0, 0, 4, 1);
        gtk_grid_attach (GTK_GRID (grid), pp_label, 1, 1, 1, 1);
        gtk_grid_attach (GTK_GRID (grid), pp_entry, 2, 1, 1, 1);

        gtk_grid_set_row_spacing (GTK_GRID (grid), 5);
        gtk_grid_set_column_spacing (GTK_GRID (grid), 5);
        gtk_box_pack_start (GTK_BOX (content), grid, TRUE, TRUE, 5);
        gtk_container_set_border_width (GTK_CONTAINER (content), 3);
        gtk_widget_show_all (dialog);
        while (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
        {
          json_t *params, *response;
          params = json_array ();
          json_array_append_new (params, json_string (gtk_entry_get_text (GTK_ENTRY (pp_entry))));
          json_array_append_new (params, json_integer (WALLET_UNLOCK_TIME));
          response = jsonrpc_request (gui_data.bitcoind, "walletpassphrase", params);
          /* Did the passphrase succeed? Then get the keys and let the user go. */
          if (json_is_null (json_object_get (response, "error")))
          {
            bitcoin_my_transactions_fetch_signing_keys (gui_data.bitcoind);
            break;
          } else popup_message ("Passphrase Error", "The passphrase you entered was incorrect.");
          json_decref (response);
          json_decref (params);
        }
        gtk_widget_destroy (dialog);
      }
    }
  } /* end if(have_tx_in_pot) */

  /* Refresh the coins list */
  utxo_list_t *coins = bitcoin_get_utxos (gui_data.bitcoind);
  if (coins != NULL)
  {
    gtk_coin_selector_mark_all_stale (GTK_COIN_SELECTOR (gui_data.coin_selector));
    gtk_coin_selector_add_coins (GTK_COIN_SELECTOR (gui_data.coin_selector), coins);
    gtk_coin_selector_clear_stale (GTK_COIN_SELECTOR (gui_data.coin_selector));
  }
  bitcoin_utxos_destroy (coins);

  /* Query the joiner */
  joiner_update (gui_data.joiner);
  gui_data.per_input_fee = joiner_per_input_fee (gui_data.joiner);

  gchar *status_text;
  switch (joiner_status (gui_data.joiner))
  {
  case JS_STILLOPEN:
    if (joiner_minutes_remaining (gui_data.joiner) >= 0)
      status_text = g_strdup_printf ("Joiner status: %d minutes until signing, about %u"
                                     " transactions in the pot, recommended output"
                                     " size %.8g",
                                     joiner_minutes_remaining (gui_data.joiner),
                                     joiner_n_transactions (gui_data.joiner),
                                     FROM_SATOSHI (joiner_most_popular_output (gui_data.joiner)));
    else
      status_text = g_strdup_printf ("Joiner status: still waiting for others to join, but "
                                     "the session has expired. Please click Session->Forget "
                                     "Session and try again.");
    break;
  case JS_COMPLETE:
    status_text = g_strdup_printf ("Joiner status: transaction complete, txid %s. When"
                                   " you are done with this join, please click"
                                   " Session->Forget Session.",
                                   joiner_txid (gui_data.joiner));
    break;
  case JS_NEEDSIGS:
    if (joiner_tx_to_sign (gui_data.joiner) == NULL)
      fputs ("Joiner waiting on signatures, but hasn't told us what to sign.\n", stderr);
    else
    {
      const char *to_sign = joiner_tx_to_sign (gui_data.joiner);

      /* Don't do anything unless we are given a superset of txes that we submitted */
      if (!bitcoin_has_my_transactions_raw (gui_data.bitcoind, to_sign, joiner_donation_address (gui_data.joiner)))
        fprintf (stderr, "Received suspicious looking tx, not signing it: %s\n",
                 to_sign);
      else if (settings_get_submission() == NULL)
        fputs ("Not signing transaction since we've nothing pending to sign.\n", stderr);
      else if (!bitcoin_my_transactions_signing_keys_p (gui_data.bitcoind))
        fputs ("Not signing transaction due to lack of keys.\n", stderr);
      else
      {
        char *signed_tx = bitcoin_my_transactions_sign_raw (gui_data.bitcoind, to_sign, NULL);
        if (signed_tx != NULL)
        {
          if (bitcoin_has_only_my_utxos_raw (gui_data.bitcoind, signed_tx))
          {
            fprintf (stderr, "Submitted raw to joiner: %s\n", signed_tx);
            joiner_submit_signed (gui_data.joiner, signed_tx);
            bitcoin_my_transactions_reset ();
            settings_set_submission (NULL);
            settings_save_config ();
          }
          else fprintf (stderr, "Signed inputs that weren't submitted, refusing to submit: %s\n", to_sign);
          free (signed_tx);
        }
        else fprintf (stderr, "Failed to sign raw transaction: %s\n", to_sign);
      }
    }
    /* fallthrough */
  default:
    status_text = g_strdup_printf ("Joiner status: %s.", joiner_status_text (gui_data.joiner));
    break;
  }
  gtk_statusbar_push (GTK_STATUSBAR (gui_data.status_bar), 1, status_text);
  g_free (status_text);
  (void) misc;
  return TRUE;
}


/* MAIN PROGRAM */
static GActionEntry app_entries[] = {
  { "quit", menu_quit, NULL, NULL, NULL, {0} },
  { "viewtx", menu_viewtx, NULL, NULL, NULL, {0} },
  { "submittx", menu_submittx, NULL, NULL, NULL, {0} },
  { "forgetsession", menu_forgetsession, NULL, NULL, NULL, {0} },
  { "settings", menu_settings, NULL, NULL, NULL, {0} },
  { "addins", menu_addinput, NULL, NULL, NULL, {0} },
  { "addouts", menu_addoutputs, NULL, NULL, NULL, {0} },
  { "addprivs", menu_addprivkeys, NULL, NULL, NULL, {0} },
};


void gui_activate (GtkApplication *app, gpointer misc)
{
  GtkWidget *main_grid;
  GtkWidget *label[5];
  GtkWidget *button[2];
  GtkBuilder *builder;

  /* set any gui_data vars that have locally-determinable values */
  gui_data.use_bitcoind_for_privkeys = TRUE;
  
  /* Create menu actions */
  g_action_map_add_action_entries (G_ACTION_MAP (app), app_entries, 
  		G_N_ELEMENTS (app_entries), app);
  
  /* Create main window and menu */
  gui_data.window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (gui_data.window),
  		"Coinjoiner Client " VERSION);
  
  builder = gtk_builder_new ();
  gtk_builder_add_from_string (builder,
#include "menus.xml"
                               , -1, NULL);
  gtk_application_set_menubar (GTK_APPLICATION (app), G_MENU_MODEL (gtk_builder_get_object (builder, "menubar")));
  g_object_unref (builder);

  /* Create main interface */
  /* labels */
  label[0] = gtk_label_new (NULL);
  gtk_label_set_markup (GTK_LABEL (label[0]), "<b>Select Input Coins</b>");
  gtk_misc_set_alignment (GTK_MISC (label[0]), 0, 0.5);
  label[1] = gtk_label_new ("Output target size:");
  label[2] = gtk_label_new ("Number of outputs:");
  label[3] = gtk_label_new ("Total join amount:");
  label[4] = gtk_label_new ("Joiner/network fee:");
  gui_data.output_entry = gtk_spin_button_new_with_range (0.05, 10000, 0.05);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (gui_data.output_entry), 0.5);
  gtk_spin_button_set_digits (GTK_SPIN_BUTTON (gui_data.output_entry), 8);
  gui_data.noutput_disp = gtk_label_new ("");
  gui_data.total_disp = gtk_label_new ("");
  gui_data.fee_disp = gtk_label_new ("");
  gtk_misc_set_alignment (GTK_MISC (label[1]), 1, 0.5);
  gtk_misc_set_alignment (GTK_MISC (label[2]), 1, 0.5);
  gtk_misc_set_alignment (GTK_MISC (label[3]), 1, 0.5);
  gtk_misc_set_alignment (GTK_MISC (label[4]), 1, 0.5);
  gtk_misc_set_alignment (GTK_MISC (gui_data.noutput_disp), 0, 0.5);
  gtk_misc_set_alignment (GTK_MISC (gui_data.total_disp), 0, 0.5);
  gtk_misc_set_alignment (GTK_MISC (gui_data.fee_disp), 0, 0.5);
  g_signal_connect (G_OBJECT (gui_data.output_entry), "value-changed",
                    G_CALLBACK (output_entry_update_cb), NULL);
  /* buttons */
  button[0] = gtk_button_new_with_mnemonic ("_View Transaction");
  button[1] = gtk_button_new_with_mnemonic ("S_ubmit Transaction to Joiner");
  gui_data.view_window = NULL;
  g_signal_connect (G_OBJECT (button[0]), "clicked",
                    G_CALLBACK (view_button_clicked_cb), NULL);
  g_signal_connect (G_OBJECT (button[1]), "clicked",
                    G_CALLBACK (submit_button_clicked_cb), NULL);
  /* coin selector */
  gui_data.coin_selector = gtk_coin_selector_new ();
  GtkWidget *cs_scrollbars = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_vexpand (gui_data.coin_selector, TRUE);
  gtk_container_add (GTK_CONTAINER (cs_scrollbars), gui_data.coin_selector);
  gtk_widget_set_vexpand (cs_scrollbars, TRUE);
  g_signal_connect (G_OBJECT (gui_data.coin_selector), "toggle-coin",
                    G_CALLBACK (coin_selector_toggle_cb),
                    NULL);
  /* status bar */
  gui_data.status_bar = gtk_statusbar_new ();
  /* put it all together */
  main_grid = gtk_grid_new ();
  gtk_grid_set_column_homogeneous (GTK_GRID (main_grid), TRUE);
  gtk_grid_attach (GTK_GRID (main_grid), label[0], 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (main_grid), cs_scrollbars, 0, 1, 4, 1);
  gtk_grid_attach (GTK_GRID (main_grid), label[1], 1, 2, 1, 1);
  gtk_grid_attach (GTK_GRID (main_grid), gui_data.output_entry, 2, 2, 1, 1);
  gtk_grid_attach (GTK_GRID (main_grid), label[2], 1, 3, 1, 1);
  gtk_grid_attach (GTK_GRID (main_grid), gui_data.noutput_disp, 2, 3, 1, 1);
  gtk_grid_attach (GTK_GRID (main_grid), label[3], 1, 4, 1, 1);
  gtk_grid_attach (GTK_GRID (main_grid), gui_data.total_disp, 2, 4, 1, 1);
  gtk_grid_attach (GTK_GRID (main_grid), label[4], 1, 5, 1, 1);
  gtk_grid_attach (GTK_GRID (main_grid), gui_data.fee_disp, 2, 5, 1, 1);
  gtk_grid_attach (GTK_GRID (main_grid), button[0], 0, 99, 2, 1);
  gtk_grid_attach (GTK_GRID (main_grid), button[1], 2, 99, 2, 1);
  gtk_grid_attach (GTK_GRID (main_grid), gui_data.status_bar, 0, 100, 4, 1);

  /* Connect to bitcoind, populate the coins list, decode the current submission */
  gui_data.bitcoind = jsonrpc_new (settings_get_rpc_server (),
                                   settings_get_rpc_port (),
                                   settings_get_rpc_user (),
                                   settings_get_rpc_pass ());
  if (settings_get_submission() != NULL)
  {
    fputs ("Found transaction already in the joiner.\n", stderr);
    bitcoin_add_my_transaction_raw (gui_data.bitcoind, settings_get_submission ());
  }
  gui_data.output_list = output_list_new ();
  
  /* Connect to andytoshi's server to get current status */
  gui_data.joiner = joiner_new (settings_get_server_url (),
                                settings_get_session_id ());
  server_status_update (NULL);
  g_timeout_add_seconds (60, (GSourceFunc) server_status_update, NULL);

  /* set display labels to their proper values */
  coin_selector_toggle_cb (GTK_COIN_SELECTOR (gui_data.coin_selector), NULL);

  /* Go! */
  gtk_container_set_border_width (GTK_CONTAINER (gui_data.window), 3);
  gtk_grid_set_row_spacing (GTK_GRID (main_grid), 5);
  gtk_grid_set_column_spacing (GTK_GRID (main_grid), 5);

  gtk_container_add (GTK_CONTAINER (gui_data.window), main_grid);
  gtk_window_set_default_size (GTK_WINDOW (gui_data.window), -1, 450);
  gtk_widget_show_all (gui_data.window);

  (void) misc;
}


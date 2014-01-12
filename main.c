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


#include <curl/curl.h>
#include <gtk/gtk.h>

#include "global.h"
#include "gtk-ui.h"
#include "settings.h"

int main (int argc, char *argv[])
{
  int status;
  GtkApplication *app = gtk_application_new ("net.wpsoftware.cj-client",
                                             G_APPLICATION_FLAGS_NONE);

  curl_global_init (CURL_GLOBAL_ALL);
  settings_read_config ();

  g_signal_connect (app, "activate", G_CALLBACK (gui_activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);

  g_object_unref (app);
  curl_global_cleanup ();
  return status;
}


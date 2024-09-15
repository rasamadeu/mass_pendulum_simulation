#include "../lib/Gtk3_CssAux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_style_from_file(gchar *cssFile) {
  gsize bytes_written, bytes_read;
  GError *error = 0;
  gchar *fileUTF8;

  GtkCssProvider *provider = gtk_css_provider_new();
  GdkDisplay *display = gdk_display_get_default();
  GdkScreen *screen = gdk_display_get_default_screen(display);

  gtk_style_context_add_provider_for_screen(
      screen, GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  fileUTF8 = g_filename_to_utf8(cssFile, strlen(cssFile), &bytes_read,
                                &bytes_written, &error);

  gtk_css_provider_load_from_path(provider, fileUTF8, NULL);

  free(fileUTF8);

  g_object_unref(provider);
}

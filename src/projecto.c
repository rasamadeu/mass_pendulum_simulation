#include "../lib/Gtk3_CssAux.h"
#include "../lib/window.h"

int main(int argc, char **argv) {

  gtk_init(&argc, &argv);

  create_style_from_file("./src/projecto.css");

  Main_struct *main_struct;

  main_struct = (Main_struct *)malloc(sizeof(Main_struct));

  main_struct = cria_window();

  g_signal_connect(G_OBJECT(main_struct->widgets->window), "destroy",
                   G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(main_struct->widgets->window);
  gtk_main();

  return 0;
}

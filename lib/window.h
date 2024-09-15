#ifndef __WINDOW_H__
#define __WINDOW_H__ 1

#include "callback.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Main_struct *cria_window();

GtkWidget *cria_texto_combo_box(int initial);

GtkWidget *cria_start_stop_button(GtkWidget *image, GtkWidget *image2);

GtkWidget *cria_spin_button(gdouble current, gdouble maximum, gdouble minimum,
                            gdouble step, gdouble page, guint digits);

Graphics_struct *cria_linked_list_node();

Graphics_struct *cria_linked_list_circular();

#endif /* __WINDOW_H__ */

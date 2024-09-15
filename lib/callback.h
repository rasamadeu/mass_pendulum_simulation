#ifndef __CALLBACK_H__
#define __CALLBACK_H__ 1

#include "variables.h"
#include <gtk-3.0/gtk/gtk.h>

#define NUMBER_COMBO_BOX_ITEMS 4
#define INITIAL_MASS_M 1.
#define INITIAL_MASS_m 1.
#define INITIAL_K_LEFT 25.
#define INITIAL_K_RIGHT 25.
#define INITIAL_LENGHT 2.
#define INITIAL_x 1.
#define INITIAL_v 0.5
#define INITIAL_theta -M_PI_4
#define INITIAL_omega 0.03
#define INITIAL_dt 1e-2
#define MAXIMUM_MASS_M 10.
#define MAXIMUM_MASS_m 10.
#define MAXIMUM_K_LEFT 100.
#define MAXIMUM_K_RIGHT 100.
#define MAXIMUM_LENGHT 6.
#define MAXIMUM_x 3.
#define MAXIMUM_v 5.
#define MAXIMUM_theta M_PI
#define MAXIMUM_omega 2 * M_PI
#define MINIMUM_MASS_M 0.100
#define MINIMUM_MASS_m 0.100
#define MINIMUM_K_LEFT 0.1
#define MINIMUM_K_RIGHT 0.1
#define MINIMUM_LENGHT 0.5
#define MINIMUM_x -3.
#define MINIMUM_v -5.
#define MINIMUM_theta -M_PI
#define MINIMUM_omega -2 * M_PI
#define STEP_MASS_M 0.05
#define STEP_MASS_m 0.05
#define STEP_K_LEFT 0.5
#define STEP_K_RIGHT 0.5
#define STEP_LENGHT 0.1
#define STEP_x 0.05
#define STEP_v 0.05
#define STEP_theta 0.15
#define STEP_omega 0.10
#define PAGE_MASS_M 0.5
#define PAGE_MASS_m 0.5
#define PAGE_K_LEFT 5.
#define PAGE_K_RIGHT 5.
#define PAGE_LENGHT 0.5
#define PAGE_x 1.
#define PAGE_v 1.
#define PAGE_theta M_PI_4 / 2
#define PAGE_omega 1.
#define MASS_DIGITS 3.
#define K_DIGITS 1.
#define LENGHT_DIGITS 2.
#define x_DIGITS 2.
#define theta_DIGITS 2.

typedef struct {

  GtkWidget *window;
  GtkWidget *window_main_box_h;
  GtkWidget *left_box_v;
  GtkWidget *middle_box_v;
  GtkWidget *right_box_v;

  GtkWidget *combo_box_top;
  GtkWidget *combo_box_bottom;
  GtkWidget *spin_button_top;
  GtkWidget *spin_button_bottom;
  GtkWidget *set_button_top;
  GtkWidget *set_button_bottom;
  GtkWidget *graphic_drawing_area_top;
  GtkWidget *graphic_drawing_area_bottom;

  GtkWidget *drawing_area_frame;
  GtkWidget *drawing_area;

  GtkWidget *start_stop_box_h;
  GtkWidget *start_stop_label;
  GtkWidget *start_stop_button;
  GtkWidget *start_image;
  GtkWidget *stop_image;
  GtkWidget *reset_button;
  GtkWidget *apply_button;

  GtkWidget *spin_button_mass_M;
  GtkWidget *spin_button_mass_m;
  GtkWidget *spin_button_spring_K_left;
  GtkWidget *spin_button_spring_K_right;
  GtkWidget *spin_button_pendulum_length;
  GtkWidget *spin_button_x;
  GtkWidget *spin_button_v;
  GtkWidget *spin_button_theta;
  GtkWidget *spin_button_omega;

  GtkWidget *time_info;
  GtkWidget *time_button_real;
  GtkWidget *time_button_plus;
  GtkWidget *time_button_minus;
  GtkWidget *time_buttons_state;
  GtkWidget *collision_button;

} Widget_pointer_struct;

enum option {
  x_rec,
  x_cir,
  y_cir,
  x,
  v,
  theta,
  omega,
  x_rec_theta,
  x_cir_theta,
  y_cir_theta
};

typedef struct Node_graphics {

  double *variables;
  int position;
  double time;

  struct Node_graphics *next;

} Graphics_struct;

typedef struct {

  Widget_pointer_struct *widgets;
  Variables_struct *variables;
  Graphics_struct *linked_list_head;

  double top_scale;
  double bottom_scale;
  double contador_graph; // controla o display dos graficos
  double controla_escala_tempo;
  int just_pause; // Esta variável vai ter 3 valores: 1,0 e -1. Carregar start =
                  // 1; Ao parar, carrega variáveis se possível = -1; Se o
                  // utilizador alterar algum valor, a função de callback
                  // reconhece -1 e põe just_pause a 0;

} Main_struct;

void carrega_info_para_spin_buttons_se_possivel(Main_struct *main_str);

void limpar_informacao_grafico(Graphics_struct *head);

void atualiza_grafico(Main_struct *main_str);

void set_time_config(Main_struct *main_str);

void altera_adjustment_spin_button(GtkWidget *combo_box, GtkWidget *button);

gboolean cb_start_stop_button(GtkWidget *start_stop_bt, Main_struct *main_str);

gboolean cb_reset_button(GtkWidget *reset_bt, Main_struct *main_str);

gboolean cb_apply_button(GtkWidget *apply_bt, Main_struct *main_struct);

gboolean cb_spin_button(GtkWidget *button, gdouble *value);

gboolean cb_motion_spin_button(GtkWidget *button, Main_struct *main_str);

gboolean cb_set_button_top(GtkWidget *button, Main_struct *main_struct);

gboolean cb_set_button_bottom(GtkWidget *button, Main_struct *main_struct);

gboolean cb_combo_box_top(GtkWidget *combo_box, Main_struct *main_str);

gboolean cb_combo_box_bottom(GtkWidget *combo_box, Main_struct *main_str);

void draw_graph_function(gint option, double scale, GtkWidget *darea,
                         cairo_t *cr, Main_struct *main_str, gint choice);

gboolean cb_draw_graphic_top(GtkWidget *darea, cairo_t *cr,
                             Main_struct *main_str);

gboolean cb_draw_graphic_bottom(GtkWidget *darea, cairo_t *cr,
                                Main_struct *main_str);

gboolean cb_draw_event(GtkWidget *darea, cairo_t *cr, Main_struct *main_str);

gboolean time_handler(Widget_pointer_struct *widget_struct);

gboolean cb_time_button_minus(GtkWidget *time_button_minus,
                              Main_struct *main_str);

gboolean cb_time_button_plus(GtkWidget *time_button_plus,
                             Main_struct *main_str);

gboolean cb_time_buttons_state(GtkWidget *button, Main_struct *main_str);

#endif /* __CALLBACK_H__ */

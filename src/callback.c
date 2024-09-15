#include "../lib/callback.h"

void carrega_info_para_spin_buttons_se_possivel(Main_struct *main_str) {

  if (main_str->variables->motion_conditions->x <= MAXIMUM_x &&
      main_str->variables->motion_conditions->x >= MINIMUM_x)
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(main_str->widgets->spin_button_x),
                              main_str->variables->motion_conditions->x);

  if (main_str->variables->motion_conditions->v <= MAXIMUM_v &&
      main_str->variables->motion_conditions->v >= MINIMUM_v)
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(main_str->widgets->spin_button_v),
                              main_str->variables->motion_conditions->v);

  if (main_str->variables->motion_conditions->omega <= MAXIMUM_omega &&
      main_str->variables->motion_conditions->omega >= MINIMUM_omega)
    gtk_spin_button_set_value(
        GTK_SPIN_BUTTON(main_str->widgets->spin_button_omega),
        main_str->variables->motion_conditions->omega);
}

void limpar_informacao_grafico(Graphics_struct *head) {

  Graphics_struct *current_node = head;
  int i;

  for (i = 0; i < 100; i++) {
    current_node->variables[x_rec] = 0;
    current_node->variables[x_cir] = 0;
    current_node->variables[y_cir] = 0;
    current_node->variables[x] = 0;
    current_node->variables[v] = 0;
    current_node->variables[theta] = 0;
    current_node->variables[omega] = 0;
    current_node->time = 0;
  }
}

void atualiza_grafico(Main_struct *main_str) {

  double paragem, time_calc;
  static double time;
  Graphics_struct *current_node;

  current_node = (Graphics_struct *)malloc(sizeof(Graphics_struct));
  current_node = main_str->linked_list_head;

  paragem = fmod(main_str->contador_graph,
                 100); // O resto de main_str -> contador_graph/100 é a posicao
                       // do node de 0 a 99 a ser atualizada
  while (current_node->position != paragem) {
    current_node = current_node->next;
  }

  current_node->variables[x_rec] = main_str->variables->motion_conditions->x;
  current_node->variables[x_cir] =
      main_str->variables->motion_conditions->x +
      main_str->variables->physical_properties->l *
          sin(main_str->variables->motion_conditions->theta);
  current_node->variables[y_cir] =
      main_str->variables->physical_properties->l *
      cos(main_str->variables->motion_conditions->theta);
  current_node->variables[x] = main_str->variables->motion_conditions->x;
  current_node->variables[v] = main_str->variables->motion_conditions->v;
  current_node->variables[theta] =
      main_str->variables->motion_conditions->theta;
  current_node->variables[omega] =
      main_str->variables->motion_conditions->omega;

  if (((int)main_str->contador_graph / 1) ==
      0) { // Basta verificar a 1ª iteração
    time = 0;
  }

  if (!main_str->controla_escala_tempo) { // Se a flag não está ativa
    if (fmod(main_str->contador_graph, 100) == 99) {
      time_calc = main_str->variables->motion_conditions->dt / 1e-2;
      time = time + time_calc;
      current_node->time = time;
    }
  } else { // Se estiver ativa, força time a 0 e na função de callback de
           // desenho dos gráficos não é desenhada a escala de tempo
    current_node->time = 0;
  }
  (main_str->contador_graph)++;
}

void set_time_config(Main_struct *main_str) {

  char *text;
  double time;

  time = main_str->variables->motion_conditions->dt / 1e-2;
  text = (char *)malloc(30 * sizeof(char));

  if (time == 10) {
    strcpy(text, "Tempo da Simulação: x 10");
    gtk_widget_set_sensitive(main_str->widgets->time_button_plus, FALSE);
  } else if (time == 0.1) {
    strcpy(text, "Tempo da Simulação: x 0.1");
  } else if (time == 0.01) {
    strcpy(text, "Tempo da Simulação: x 0.01");
    gtk_widget_set_sensitive(main_str->widgets->time_button_minus, FALSE);
  } else {
    strcpy(text, "Tempo da Simulação: Tempo Real");
  }
  gtk_label_set_text(GTK_LABEL(main_str->widgets->time_info), text);
  free(text);
}

void altera_adjustment_spin_button(GtkWidget *combo_box, GtkWidget *button) {

  int choice = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
  GtkAdjustment *adjustment;

  adjustment =
      (GtkAdjustment *)gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(button));

  if (choice == x_rec || choice == x_rec_theta || choice == y_cir ||
      choice == y_cir_theta || choice == x) {
    gtk_adjustment_configure(GTK_ADJUSTMENT(adjustment), MAXIMUM_x / 2,
                             MAXIMUM_x / 2, 5 * MAXIMUM_x, MAXIMUM_x / 20,
                             MAXIMUM_x / 10, 0);
  } else if (choice == x_cir || choice == x_cir_theta) {
    gtk_adjustment_configure(GTK_ADJUSTMENT(adjustment), MAXIMUM_x / 2,
                             MAXIMUM_x / 2, 7 * MAXIMUM_x, 3 * MAXIMUM_x / 20,
                             3 * MAXIMUM_x / 10, 0);
  } else if (choice == v) {
    gtk_adjustment_configure(GTK_ADJUSTMENT(adjustment), MAXIMUM_v / 2,
                             MAXIMUM_v / 2, 7 * MAXIMUM_v, MAXIMUM_x / 20,
                             MAXIMUM_x / 10, 0);
  } else if (choice == theta) {
    gtk_adjustment_configure(GTK_ADJUSTMENT(adjustment), M_PI / 2, M_PI / 2,
                             M_PI / 2, 0.1, 0.1,
                             0); // Fixou-se a escala do angulo
  } else {
    gtk_adjustment_configure(adjustment, MAXIMUM_omega / 2, MAXIMUM_omega / 2,
                             5 * MAXIMUM_omega, MAXIMUM_omega / 20,
                             MAXIMUM_omega / 10, 0);
  }
}

/***********************************************************************************************************************************************
 ** FUNÇÕES DE CALLBACK *
 *                                                                                                                                             *
 **********************************************************************************************************************************************/

gboolean time_handler(Widget_pointer_struct *widget_struct) {

  if ((!GTK_IS_WIDGET(widget_struct->drawing_area)) ||
      (!gtk_widget_get_window(widget_struct->drawing_area)))
    return FALSE;

  gtk_widget_queue_draw(widget_struct->drawing_area);
  gtk_widget_queue_draw(widget_struct->graphic_drawing_area_top);
  gtk_widget_queue_draw(widget_struct->graphic_drawing_area_bottom);

  return TRUE;
}

gboolean cb_time_buttons_state(GtkWidget *button, Main_struct *main_str) {

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))) {
    gtk_button_set_label(GTK_BUTTON(button), "Enabled");
    gtk_widget_set_sensitive(main_str->widgets->time_button_plus, TRUE);
    gtk_widget_set_sensitive(main_str->widgets->time_button_minus, TRUE);
    main_str->controla_escala_tempo = 1;
  } else {
    gtk_button_set_label(GTK_BUTTON(button), "Disabled");
    gtk_widget_set_sensitive(main_str->widgets->time_button_plus, FALSE);
    gtk_widget_set_sensitive(main_str->widgets->time_button_minus, FALSE);
  }
  return FALSE;
}

gboolean cb_time_button_minus(GtkWidget *time_button_minus,
                              Main_struct *main_str) {

  main_str->variables->motion_conditions->dt =
      main_str->variables->motion_conditions->dt / 10;
  gtk_widget_set_sensitive(main_str->widgets->time_button_plus, TRUE);
  set_time_config(main_str);

  return FALSE;
}

gboolean cb_time_button_plus(GtkWidget *time_button_plus,
                             Main_struct *main_str) {

  main_str->variables->motion_conditions->dt =
      main_str->variables->motion_conditions->dt * 10;
  gtk_widget_set_sensitive(main_str->widgets->time_button_minus, TRUE);
  set_time_config(main_str);

  return FALSE;
}

gboolean cb_start_stop_button(GtkWidget *start_stop_bt, Main_struct *main_str) {

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(start_stop_bt))) {
    gtk_label_set_text(GTK_LABEL(main_str->widgets->start_stop_label),
                       "Pausar: ");
    g_object_ref(main_str->widgets->start_image);
    gtk_button_set_image(GTK_BUTTON(start_stop_bt),
                         main_str->widgets->stop_image);
    gtk_toggle_button_set_active(
        GTK_TOGGLE_BUTTON(main_str->widgets->time_buttons_state), FALSE);
    // Limpa-se a informação da lista circular e dá-se reset ao contador da
    // main_str, para que ao iniciar seja feito um novo gráfico
    main_str->contador_graph = 0;
    limpar_informacao_grafico(main_str->linked_list_head);
    main_str->controla_escala_tempo = 0;
    main_str->just_pause = 1;
  } else {
    gtk_label_set_text(GTK_LABEL(main_str->widgets->start_stop_label),
                       "Iniciar/Continuar: ");
    g_object_ref(main_str->widgets->stop_image);
    gtk_button_set_image(GTK_BUTTON(start_stop_bt),
                         main_str->widgets->start_image);
    gtk_toggle_button_set_active(
        GTK_TOGGLE_BUTTON(main_str->widgets->time_buttons_state), TRUE);
    carrega_info_para_spin_buttons_se_possivel(main_str);
    // Para theta é sempre possível
    gtk_spin_button_set_value(
        GTK_SPIN_BUTTON(main_str->widgets->spin_button_theta),
        main_str->variables->motion_conditions->theta);
    main_str->just_pause = -1; // Permite que apenas se pare a imagem
  }
  return FALSE;
}

gboolean cb_reset_button(GtkWidget *reset_bt, Main_struct *main_str) {

  gtk_toggle_button_set_active(
      GTK_TOGGLE_BUTTON(main_str->widgets->start_stop_button), FALSE);
  gtk_spin_button_set_value(
      GTK_SPIN_BUTTON(main_str->widgets->spin_button_mass_M), INITIAL_MASS_M);
  gtk_spin_button_set_value(
      GTK_SPIN_BUTTON(main_str->widgets->spin_button_mass_m), INITIAL_MASS_m);
  gtk_spin_button_set_value(
      GTK_SPIN_BUTTON(main_str->widgets->spin_button_spring_K_left),
      INITIAL_K_LEFT);
  gtk_spin_button_set_value(
      GTK_SPIN_BUTTON(main_str->widgets->spin_button_spring_K_right),
      INITIAL_K_RIGHT);
  gtk_spin_button_set_value(
      GTK_SPIN_BUTTON(main_str->widgets->spin_button_pendulum_length),
      INITIAL_LENGHT);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(main_str->widgets->spin_button_x),
                            INITIAL_x);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(main_str->widgets->spin_button_v),
                            INITIAL_v);
  gtk_spin_button_set_value(
      GTK_SPIN_BUTTON(main_str->widgets->spin_button_theta), INITIAL_theta);
  gtk_spin_button_set_value(
      GTK_SPIN_BUTTON(main_str->widgets->spin_button_omega), INITIAL_omega);
  main_str->just_pause = 0;

  gtk_combo_box_set_active(GTK_COMBO_BOX(main_str->widgets->combo_box_top), 0);
  gtk_combo_box_set_active(GTK_COMBO_BOX(main_str->widgets->combo_box_bottom),
                           1);

  gtk_widget_set_sensitive(main_str->widgets->time_button_plus, TRUE);
  gtk_widget_set_sensitive(main_str->widgets->time_button_minus, TRUE);
  main_str->variables->motion_conditions->dt = 1e-2;
  gtk_label_set_text(GTK_LABEL(main_str->widgets->time_info),
                     "Tempo da Simulação: Tempo Real");

  return FALSE;
}

gboolean cb_apply_button(GtkWidget *apply_bt, Main_struct *main_struct) {

  main_struct->variables->motion_conditions->x = gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(main_struct->widgets->spin_button_x));
  main_struct->variables->motion_conditions->v = gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(main_struct->widgets->spin_button_v));
  main_struct->variables->motion_conditions->theta = gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(main_struct->widgets->spin_button_theta));
  main_struct->variables->motion_conditions->omega = gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(main_struct->widgets->spin_button_omega));
  main_struct->contador_graph = 0;
  limpar_informacao_grafico(main_struct->linked_list_head);
  main_struct->controla_escala_tempo = 0;
  return FALSE;
}

gboolean cb_spin_button(GtkWidget *button, gdouble *value) {

  *value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(button));
  return FALSE;
}

gboolean cb_motion_spin_button(GtkWidget *button, Main_struct *main_str) {

  if (main_str->just_pause == -1) {
    main_str->just_pause = 0;
  }

  return FALSE;
}

gboolean cb_set_button_top(GtkWidget *button, Main_struct *main_str) {

  main_str->top_scale = gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(main_str->widgets->spin_button_top));
  return FALSE;
}

gboolean cb_set_button_bottom(GtkWidget *button, Main_struct *main_str) {

  main_str->bottom_scale = gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(main_str->widgets->spin_button_bottom));
  return FALSE;
}

gboolean cb_combo_box_top(GtkWidget *combo_box, Main_struct *main_str) {

  altera_adjustment_spin_button(combo_box, main_str->widgets->spin_button_top);
  main_str->top_scale = gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(main_str->widgets->spin_button_top));
  return FALSE;
}

gboolean cb_combo_box_bottom(GtkWidget *combo_box, Main_struct *main_str) {

  altera_adjustment_spin_button(combo_box,
                                main_str->widgets->spin_button_bottom);
  main_str->bottom_scale = gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(main_str->widgets->spin_button_bottom));
  return FALSE;
}

gboolean cb_draw_graphic_top(GtkWidget *darea, cairo_t *cr,
                             Main_struct *main_str) {

  gint option, draw_func_choose;

  option =
      gtk_combo_box_get_active(GTK_COMBO_BOX(main_str->widgets->combo_box_top));
  draw_func_choose = (gint)(main_str->contador_graph / 100);

  draw_graph_function(option, main_str->top_scale, darea, cr, main_str,
                      draw_func_choose);

  return FALSE;
}

gboolean cb_draw_graphic_bottom(GtkWidget *darea, cairo_t *cr,
                                Main_struct *main_str) {

  gint option, draw_func_choose;

  option = gtk_combo_box_get_active(
      GTK_COMBO_BOX(main_str->widgets->combo_box_bottom));
  draw_func_choose = (gint)(main_str->contador_graph / 100);

  draw_graph_function(option, main_str->bottom_scale, darea, cr, main_str,
                      draw_func_choose);

  return FALSE;
}

void draw_graph_function(gint option, double scale, GtkWidget *darea,
                         cairo_t *cr, Main_struct *main_str, gint choice) {

  double i, paragem, max, theta_graph, theta_graph_next, origin, x_axis_pos,
      scale_value_position;
  char str_scale[5];
  Graphics_struct *current, *before;
  GtkAllocation alloc, alloc_calculo_origem;
  gdouble darea_width, darea_height, div_width, div_height;

  gtk_widget_get_allocation(darea, &alloc);
  darea_width = alloc.width;
  darea_height = alloc.height;

  div_height = darea_height / 10;
  div_width = darea_width / 10;
  x_axis_pos = div_width + 10;
  scale_value_position = x_axis_pos / 5;

  current = (Graphics_struct *)malloc(sizeof(Graphics_struct));
  before = (Graphics_struct *)malloc(sizeof(Graphics_struct));
  current = main_str->linked_list_head;

  if (choice) {
    // Vai-se percorrer a lista até se alcançar o nodo atualizado
    // Assim, o último nodo utilizado é o nodo atualizado e o 1º nodo a ser
    // utilizado é o nodo atualizado -> next
    paragem = fmod(main_str->contador_graph, 100);
    while (current->position != paragem) {
      current = current->next;
    }
    current = current->next;
    max = 99;
  } else {
    max = main_str->contador_graph;
  }

  if (option == x_rec || option == x_rec_theta || option == x_cir ||
      option == x_cir_theta) {
    gtk_widget_get_allocation(main_str->widgets->drawing_area,
                              &alloc_calculo_origem);
    origin = (double)(alloc_calculo_origem.width / 2 -
                      alloc_calculo_origem.width / 10) /
             30; // Esta conversão deve a 30 = 1 m
  } else {
    origin = 0;
  }

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 2.0);
  cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
  cairo_set_font_size(cr, 7.5);

  // EIXO Y
  cairo_move_to(cr, x_axis_pos, div_height - 15);
  cairo_line_to(cr, x_axis_pos, 9 * div_height + 10);

  // SETA DO EIXO
  cairo_move_to(cr, x_axis_pos, div_height - 15);
  cairo_line_to(cr, x_axis_pos - 5, div_height - 10);
  cairo_move_to(cr, x_axis_pos, div_height - 15);
  cairo_line_to(cr, x_axis_pos + 5, div_height - 10);

  // Colocação de valores na escala
  cairo_move_to(cr, x_axis_pos - 5, div_height);
  cairo_line_to(cr, x_axis_pos + 5, div_height);
  if (origin + 2 * scale > 10) {
    cairo_move_to(cr, scale_value_position - 3, div_height + 3);
  } else {
    cairo_move_to(cr, scale_value_position, div_height + 3);
  }
  sprintf(str_scale, "%.2f", origin + 2 * scale);
  cairo_show_text(cr, str_scale);

  cairo_move_to(cr, x_axis_pos - 5, 3 * div_height);
  cairo_line_to(cr, x_axis_pos + 5, 3 * div_height);
  cairo_move_to(cr, scale_value_position, 3 * div_height + 3);
  sprintf(str_scale, "%.2f", origin + scale);
  cairo_show_text(cr, str_scale);

  cairo_move_to(cr, x_axis_pos - 5, 5 * div_height);
  cairo_line_to(cr, x_axis_pos, 5 * div_height);
  cairo_move_to(cr, scale_value_position, 5 * div_height + 3);
  sprintf(str_scale, "%.2f", origin);
  cairo_show_text(cr, str_scale);

  cairo_move_to(cr, x_axis_pos - 5, 7 * div_height);
  cairo_line_to(cr, x_axis_pos + 5, 7 * div_height);
  if (origin - scale < 0) {
    cairo_move_to(cr, scale_value_position - 3, 7 * div_height + 3);
  } else {
    cairo_move_to(cr, scale_value_position, 7 * div_height + 3);
  }
  sprintf(str_scale, "%.2f", origin - scale);
  cairo_show_text(cr, str_scale);

  cairo_move_to(cr, x_axis_pos - 5, 9 * div_height);
  cairo_line_to(cr, x_axis_pos + 5, 9 * div_height);
  if (origin - 2 * scale < -10) {
    cairo_move_to(cr, scale_value_position - 6, 9 * div_height + 3);
  } else if (origin - 2 * scale > -10 && origin - 2 * scale < 0) {
    cairo_move_to(cr, scale_value_position - 3, 9 * div_height + 3);
  } else {
    cairo_move_to(cr, scale_value_position, 9 * div_height + 3);
  }
  sprintf(str_scale, "%.2f", origin - 2 * scale);
  cairo_show_text(cr, str_scale);

  // EIXO X

  cairo_move_to(cr, x_axis_pos, darea_height / 2);
  cairo_line_to(cr, x_axis_pos + 8 * div_width, darea_height / 2);

  // SETA DO EIXO
  cairo_move_to(cr, x_axis_pos + 8 * div_width, darea_height / 2);
  cairo_line_to(cr, x_axis_pos + 8 * div_width - 5, darea_height / 2 + 5);
  cairo_move_to(cr, x_axis_pos + 8 * div_width, darea_height / 2);
  cairo_line_to(cr, x_axis_pos + 8 * div_width - 5, darea_height / 2 - 5);

  cairo_stroke(cr);

  cairo_move_to(cr, x_axis_pos + 10, div_height - 5);
  cairo_set_font_size(cr, 10);
  if (option < v || option > omega) {
    cairo_show_text(cr, "(m)");
  } else if (option == theta) {
    cairo_show_text(cr, "(rad)");
  } else if (option == v) {
    cairo_show_text(cr, "(m/s)");
  } else {
    cairo_show_text(cr, "(rad/s)");
  }

  cairo_set_line_width(cr, 1.0);
  cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
  if (option < x_rec_theta) {
    cairo_set_font_size(cr, 10);
    cairo_move_to(cr, x_axis_pos + 8 * div_width - 7, darea_height / 2 + 15);
    cairo_show_text(cr, "(s)");
    // conversão para gráfico: 1 scale equivale a 2*div_height ///// 8/10 = 4/5
    // Como são 100 pontos cada passo = 4/500
    cairo_move_to(cr, x_axis_pos,
                  darea_height / 2 -
                      (current->variables[option]) / scale * div_height * 2);
    for (i = 1; i < max; i++) {
      before = current;
      current = current->next;
      if (current->time != 0) {
        cairo_move_to(cr, x_axis_pos + i * 4 * darea_width / 500,
                      darea_height / 2 + 5);
        cairo_line_to(cr, x_axis_pos + i * 4 * darea_width / 500,
                      darea_height / 2 - 5);
        cairo_move_to(cr, x_axis_pos + i * 4 * darea_width / 500 - 3.1,
                      darea_height / 2 + 15);
        if ((current->time - (int)(current->time)) ==
            0) { // Verifica se é inteiro
          sprintf(str_scale, "%d", (int)current->time);
        } else {
          sprintf(str_scale, "%.2f", current->time);
        }
        cairo_show_text(cr, str_scale);
        cairo_move_to(cr, x_axis_pos + (i - 1) * 4 * darea_width / 500,
                      darea_height / 2 -
                          (before->variables[option]) / scale * div_height * 2);
      }
      cairo_line_to(cr, x_axis_pos + i * 4 * darea_width / 500,
                    darea_height / 2 -
                        (current->variables[option]) / scale * div_height * 2);
    }
  } else {
    theta_graph = current->variables[theta] + M_PI;
    // ESCALA DE THETA
    cairo_set_font_size(cr, 10);
    cairo_move_to(cr, x_axis_pos + 4, darea_height / 2 + 15);
    cairo_show_text(cr, "-π");
    cairo_move_to(cr, x_axis_pos + 4 * div_width, darea_height / 2 + 5);
    cairo_line_to(cr, x_axis_pos + 4 * div_width, darea_height / 2 - 5);
    cairo_move_to(cr, x_axis_pos + 4 * div_width - 3.1, darea_height / 2 + 15);
    cairo_show_text(cr, "0");
    cairo_move_to(cr, x_axis_pos + 8 * div_width - 15, darea_height / 2 + 15);
    cairo_show_text(cr, "π");
    cairo_move_to(cr, x_axis_pos + 8 * div_width - 7, darea_height / 2 + 15);
    cairo_show_text(cr, "(rad)");

    cairo_move_to(cr, x_axis_pos + 8 * div_width * (theta_graph) / (2 * M_PI),
                  darea_height / 2 - (current->variables[option - 7]) / scale *
                                         div_height * 2);
    for (i = 1; i < max; i++) {
      current = current->next;
      theta_graph_next = current->variables[theta] + M_PI;
      if (fabs(theta_graph_next - theta_graph) < M_PI / 2) {
        cairo_line_to(
            cr, x_axis_pos + 8 * div_width * (theta_graph_next) / (2 * M_PI),
            darea_height / 2 -
                (current->variables[option - 7]) / scale * div_height * 2);
      } else {
        cairo_move_to(
            cr, x_axis_pos + 8 * div_width * (theta_graph_next) / (2 * M_PI),
            darea_height / 2 -
                (current->variables[option - 7]) / scale * div_height * 2);
      }
      theta_graph = theta_graph_next;
    }
  }
  cairo_stroke(cr);
}

gboolean cb_draw_event(GtkWidget *darea, cairo_t *cr, Main_struct *main_str) {

  int i, rec_collision;
  double dt;
  gdouble darea_width, darea_height, x, theta, l, K_l, K_r, l_spring, r_spring,
      K_l_height, K_r_height, x_axis_height, x_l_spring, x_r_spring;
  gdouble pos_x_rec, pos_y_rec, pos_x_cir, pos_y_cir, width_rec, length_rec,
      r_cir;

  GtkAllocation alloc;

  gtk_widget_get_allocation(darea, &alloc);
  darea_width = alloc.width;
  darea_height = alloc.height;

  // Posição dos extremos das molas
  x_axis_height = darea_height / 2;
  x_l_spring = darea_width / 10;
  x_r_spring = 9 * darea_width / 10;

  // Conversão para representação gráfica
  l = 30 * main_str->variables->physical_properties->l;
  K_l = 0.1 + 0.1 * main_str->variables->physical_properties->K_left;
  K_r = 0.1 + 0.1 * main_str->variables->physical_properties->K_right;
  x = 30 * main_str->variables->motion_conditions->x;
  theta = main_str->variables->motion_conditions->theta;

  // Cálculo das dimensões e posições das figuras
  width_rec = 20 + 3 * main_str->variables->physical_properties->M;
  length_rec = width_rec / 2;
  r_cir = 4 + 1.5 * main_str->variables->physical_properties->m;
  pos_x_rec = darea_width / 2 - width_rec / 2 + x;
  pos_y_rec = x_axis_height - length_rec / 2;
  pos_x_cir = darea_width / 2 + l * sin(theta) + x;
  pos_y_cir = x_axis_height + l * cos(theta);

  // Comprimento e grossura das molas
  l_spring = (darea_width / 2 - width_rec / 2 + x) - darea_width / 10;
  r_spring = x_r_spring - (darea_width / 2 + width_rec / 2 + x);
  K_l_height = 20 + K_l * 0.2;
  K_r_height = 20 + K_r * 0.2;

  // Escala

  cairo_set_line_width(cr, 3);
  cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
  cairo_move_to(cr, x_l_spring, 9 * darea_height / 10);
  cairo_line_to(cr, x_l_spring + 30, 9 * darea_height / 10);
  cairo_move_to(cr, x_l_spring, 9 * darea_height / 10 - 10);
  cairo_line_to(cr, x_l_spring, 9 * darea_height / 10 + 10);
  cairo_move_to(cr, x_l_spring + 30, 9 * darea_height / 10 - 10);
  cairo_line_to(cr, x_l_spring + 30, 9 * darea_height / 10 + 10);
  cairo_move_to(cr, x_l_spring + 35, 9 * darea_height / 10 + 3);
  cairo_select_font_face(cr, "Comic Sans", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 10);
  cairo_show_text(cr, "1m");
  cairo_stroke(cr);

  // Left Spring

  cairo_set_line_width(cr, K_l);
  cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
  cairo_move_to(cr, x_l_spring, x_axis_height - 20);
  cairo_line_to(cr, x_l_spring, x_axis_height + 20);
  cairo_move_to(cr, x_l_spring, x_axis_height);
  cairo_line_to(cr, x_l_spring + l_spring * 1 / 4, x_axis_height);
  cairo_line_to(cr, x_l_spring + l_spring * 9 / 28, x_axis_height + K_l_height);
  cairo_line_to(cr, x_l_spring + l_spring * 11 / 28,
                x_axis_height - K_l_height);
  cairo_line_to(cr, x_l_spring + l_spring * 13 / 28,
                x_axis_height + K_l_height);
  cairo_line_to(cr, x_l_spring + l_spring * 15 / 28,
                x_axis_height - K_l_height);
  cairo_line_to(cr, x_l_spring + l_spring * 17 / 28,
                x_axis_height + K_l_height);
  cairo_line_to(cr, x_l_spring + l_spring * 19 / 28,
                x_axis_height - K_l_height);
  cairo_line_to(cr, x_l_spring + l_spring * 21 / 28, x_axis_height);
  cairo_line_to(cr, x_l_spring + l_spring, x_axis_height);
  cairo_stroke(cr);

  // Left Spring

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, K_r);
  cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
  cairo_move_to(cr, x_r_spring, x_axis_height - 20);
  cairo_line_to(cr, x_r_spring, x_axis_height + 20);
  cairo_move_to(cr, x_r_spring, x_axis_height);
  cairo_line_to(cr, x_r_spring - r_spring * 1 / 4, x_axis_height);
  cairo_line_to(cr, x_r_spring - r_spring * 9 / 28, x_axis_height + K_r_height);
  cairo_line_to(cr, x_r_spring - r_spring * 11 / 28,
                x_axis_height - K_r_height);
  cairo_line_to(cr, x_r_spring - r_spring * 13 / 28,
                x_axis_height + K_r_height);
  cairo_line_to(cr, x_r_spring - r_spring * 15 / 28,
                x_axis_height - K_r_height);
  cairo_line_to(cr, x_r_spring - r_spring * 17 / 28,
                x_axis_height + K_r_height);
  cairo_line_to(cr, x_r_spring - r_spring * 19 / 28,
                x_axis_height - K_r_height);
  cairo_line_to(cr, x_r_spring - r_spring * 21 / 28, x_axis_height);
  cairo_line_to(cr, x_r_spring - r_spring, x_axis_height);
  cairo_stroke(cr);

  // Pendulum

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 2.5);
  cairo_move_to(cr, darea_width / 2 + x, x_axis_height);
  cairo_line_to(cr, darea_width / 2 + l * sin(theta) + x,
                x_axis_height + l * cos(theta));
  cairo_stroke(cr);

  // Rectangle

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 5.0);
  cairo_rectangle(cr, pos_x_rec, pos_y_rec, width_rec, length_rec);
  cairo_stroke_preserve(cr);
  cairo_set_source_rgb(cr, 0, 1, 1);
  cairo_fill(cr);
  cairo_stroke(cr);

  // Circle

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 5.0);
  cairo_arc(cr, pos_x_cir, pos_y_cir, r_cir, 0, 2. * M_PI);
  cairo_stroke_preserve(cr);
  cairo_set_source_rgb(cr, 0.8, 0.3, 1);
  cairo_fill(cr);

  cairo_stroke(cr);

  rec_collision = 0;

  // Verifica se collision_button está ativo

  if ((pos_x_rec < x_l_spring + 3 && pos_x_rec > x_l_spring) ||
      (pos_x_rec + width_rec > x_r_spring - 3 &&
       pos_x_rec + width_rec < x_r_spring)) { // MECANISMO DE SEGURANÇA
    if (gtk_toggle_button_get_active(
            GTK_TOGGLE_BUTTON(main_str->widgets->collision_button))) {
      rec_collision = 1;
    }
  } else if (pos_x_rec < x_l_spring || pos_x_rec + width_rec > x_r_spring) {
    gtk_toggle_button_set_active(
        GTK_TOGGLE_BUTTON(main_str->widgets->collision_button), FALSE);
    gtk_widget_set_sensitive(main_str->widgets->collision_button, FALSE);
  } else {
    gtk_widget_set_sensitive(main_str->widgets->collision_button, TRUE);
  }

  // Se estiver ativo o start_stop_button, atualiza a informação do gráfico  e
  // calcula as grandezas do movimento

  if (gtk_toggle_button_get_active(
          GTK_TOGGLE_BUTTON(main_str->widgets->start_stop_button))) {
    atualiza_grafico(main_str);
    dt = main_str->variables->motion_conditions->dt / 1e4;
    for (i = 0; i < 1e4; i++) {
      calculo_numerico_Euler_Cromer(main_str->variables, dt, &rec_collision);
    }
  } else {
    // Se as variaveis do movimento se encontrarem dentro dos valores permitidos
    // para os spin_buttons, estes são alterados
    if (main_str->just_pause == 0) {
      main_str->variables->motion_conditions->x = gtk_spin_button_get_value(
          GTK_SPIN_BUTTON(main_str->widgets->spin_button_x));
      main_str->variables->motion_conditions->v = gtk_spin_button_get_value(
          GTK_SPIN_BUTTON(main_str->widgets->spin_button_v));
      main_str->variables->motion_conditions->theta = gtk_spin_button_get_value(
          GTK_SPIN_BUTTON(main_str->widgets->spin_button_theta));
      main_str->variables->motion_conditions->omega = gtk_spin_button_get_value(
          GTK_SPIN_BUTTON(main_str->widgets->spin_button_omega));
    }
  }

  return FALSE;
}

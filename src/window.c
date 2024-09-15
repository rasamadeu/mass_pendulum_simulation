#include "../lib/window.h"

Graphics_struct *cria_linked_list_node(int posicao) {

  Graphics_struct *linked_list_node;
  double *variables;

  linked_list_node = (Graphics_struct *)calloc(1, sizeof(Graphics_struct));
  variables = (double *)calloc(7, sizeof(double));

  linked_list_node->variables = variables;
  linked_list_node->position = posicao;
  linked_list_node->time = 0.;

  return linked_list_node;
}

Graphics_struct *cria_linked_list_circular() {

  int i;
  Graphics_struct *current_node, *linked_list_head;

  linked_list_head = cria_linked_list_node(0);
  current_node = (Graphics_struct *)malloc(sizeof(Graphics_struct));

  current_node = linked_list_head;
  for (i = 1; i < 100; i++) {
    current_node->next = cria_linked_list_node(i);
    current_node = current_node->next;
  }

  current_node->next = linked_list_head;

  return linked_list_head;
}

void inicia_variaveis(Variables_struct *variables) {

  variables->motion_conditions->x = INITIAL_x;
  variables->motion_conditions->v = INITIAL_v;
  variables->motion_conditions->theta = INITIAL_theta;
  variables->motion_conditions->omega = INITIAL_omega;
  variables->motion_conditions->dt = INITIAL_dt;

  variables->physical_properties->M = INITIAL_MASS_M;
  variables->physical_properties->m = INITIAL_MASS_m;
  variables->physical_properties->K_left = INITIAL_K_LEFT;
  variables->physical_properties->K_right = INITIAL_K_RIGHT;
  variables->physical_properties->l = INITIAL_LENGHT;
}

GtkWidget *cria_spin_button(gdouble current, gdouble maximum, gdouble minimum,
                            gdouble step, gdouble page, guint digits) {

  GtkWidget *spin_button;
  GtkAdjustment *adjustment;

  adjustment = (GtkAdjustment *)gtk_adjustment_new(current, minimum, maximum,
                                                   step, page, 0);
  spin_button = gtk_spin_button_new(adjustment, 1, digits);
  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(spin_button), TRUE);

  return spin_button;
}

GtkWidget *cria_start_stop_button(GtkWidget *image, GtkWidget *image2) {

  GtkWidget *t_button;

  t_button = gtk_toggle_button_new();
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(t_button), FALSE);
  gtk_button_set_image(GTK_BUTTON(t_button), image);
  g_object_set_data(G_OBJECT(t_button), "other-image", image2);

  return t_button;
}

GtkWidget *cria_texto_combo_box(int initial) {

  GtkWidget *combo_box;
  int i;
  char texto_combo_box[10][50] = {"Posição x(t) da massa retangular",
                                  "Posição x(t) da massa circular",
                                  "Posição y(t) da massa circular",
                                  "Elongação x(t)",
                                  "Velocidade v(t) da massa retangular",
                                  "Ângulo θ(t)",
                                  "Velocidade angular ω(t)",
                                  "Posição x(θ) da massa retangular",
                                  "Posição x(θ) da massa circular",
                                  "Posição y(θ) da massa circular"};

  combo_box = gtk_combo_box_text_new();

  for (i = 0; i < 10; ++i) {
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box),
                                   texto_combo_box[i]);
  }

  gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), initial);

  return combo_box;
}

Main_struct *cria_window() {

  Widget_pointer_struct *widget_struct;

  motion_conditions_struct *conditions_str;
  physical_properties_struct *physical_str;
  Variables_struct *variables_struct;

  Main_struct *main_struct;

  gint height, width;

  GtkWidget *graphics_box_v, *separator_between_graphics, *graphics_frame,
      *graphics_frame_top, *graphics_frame_bottom, *top_label, *bottom_label,
      *scale_top_box_h, *scale_bottom_box_h, *graphics_title_label;

  GtkWidget *time_box_v, *time_box_h_2, *time_box_h_1, *time_box_center_h,
      *time_label, *collision_label, *collision_image;

  GtkWidget *separator_between_boxes, *separator_between_boxes_1;

  GtkWidget *label_mass_M, *label_mass_m, *label_spring_K_left,
      *label_spring_K_right, *label_pendulum_length, *physical_properties_frame,
      *physical_properties_box_v, *box_mass_M_h, *box_mass_m_h,
      *box_spring_K_left_h, *box_spring_K_right_h, *box_pendulum_length_h,
      *physical_properties_title_label, *initial_conditions_title_label;

  GtkWidget *label_x, *label_v, *label_theta, *label_omega,
      *initial_conditions_frame, *initial_conditions_box_v, *box_x_h, *box_v_h,
      *box_theta_h, *box_omega_h;

  widget_struct =
      (Widget_pointer_struct *)malloc(sizeof(Widget_pointer_struct));
  variables_struct = (Variables_struct *)malloc(sizeof(Variables_struct));

  conditions_str =
      (motion_conditions_struct *)malloc(sizeof(motion_conditions_struct));
  physical_str =
      (physical_properties_struct *)malloc(sizeof(physical_properties_struct));

  main_struct = (Main_struct *)malloc(sizeof(Main_struct));

  variables_struct->physical_properties = physical_str;
  variables_struct->motion_conditions = conditions_str;
  main_struct->widgets = widget_struct;
  main_struct->variables = variables_struct;

  // INICIALIZAÇÃO DOS VALORES DAS VARIÁVEIS E CRIAÇÃO DA LISTA LIGADA CIRCULAR
  // A USAR NOS GRÁFICOS
  main_struct->just_pause = -1;
  main_struct->contador_graph = 0;
  main_struct->top_scale = MAXIMUM_x / 2;
  main_struct->bottom_scale = MAXIMUM_x / 2;
  inicia_variaveis(variables_struct);
  main_struct->linked_list_head = cria_linked_list_circular();

  // CRIAÇÃO DA WINDOW
  widget_struct->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(widget_struct->window));
  width = gdk_screen_get_width(screen);
  height = gdk_screen_get_height(screen);
  gtk_window_set_default_size(GTK_WINDOW(widget_struct->window), 8 * width / 10,
                              3 * height / 4);
  gtk_widget_set_size_request(GTK_WIDGET(widget_struct->window), 8 * width / 10,
                              3 * height / 4);
  gtk_window_set_title(
      GTK_WINDOW(widget_struct->window),
      "Simulação do movimento dum sistema de duas molas e um pêndulo");
  gtk_window_set_position(GTK_WINDOW(widget_struct->window),
                          GTK_WIN_POS_CENTER);

  // INTRODUÇÃO NA WINDOW DA CAIXA HORIZONTAL window_main_box
  widget_struct->window_main_box_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_container_add(GTK_CONTAINER(widget_struct->window),
                    widget_struct->window_main_box_h);

  // CAIXA window_h_main_box CONTÉM 3 CAIXAS VERTICAIS COM DOIS SEPARADOR NO
  // MEIO

  widget_struct->left_box_v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start(GTK_BOX(widget_struct->window_main_box_h),
                     widget_struct->left_box_v, FALSE, FALSE, 5);
  separator_between_boxes = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_start(GTK_BOX(widget_struct->window_main_box_h),
                     separator_between_boxes, FALSE, FALSE, 0);
  widget_struct->middle_box_v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start(GTK_BOX(widget_struct->window_main_box_h),
                     widget_struct->middle_box_v, TRUE, TRUE, 5);
  separator_between_boxes_1 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_start(GTK_BOX(widget_struct->window_main_box_h),
                     separator_between_boxes_1, FALSE, FALSE, 0);
  widget_struct->right_box_v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start(GTK_BOX(widget_struct->window_main_box_h),
                     widget_struct->right_box_v, FALSE, FALSE, 5);

  // PERSONALIZAÇÃO DA CAIXA left_box_v

  // FRAME COM GRÁFICOS

  // Criação da frame
  graphics_title_label = gtk_label_new("Gráficos");
  gtk_widget_set_name(graphics_title_label, "title");
  graphics_frame = gtk_frame_new(NULL);
  gtk_frame_set_label_widget(GTK_FRAME(graphics_frame), graphics_title_label);
  gtk_frame_set_label_align(GTK_FRAME(graphics_frame), 0.5, 0.5);
  gtk_widget_set_name(graphics_frame, "frame_normal");
  graphics_box_v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

  // Criação das widgets
  separator_between_graphics = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  widget_struct->combo_box_top = cria_texto_combo_box(0);
  widget_struct->combo_box_bottom = cria_texto_combo_box(1);
  widget_struct->spin_button_top =
      cria_spin_button(MAXIMUM_x / 2, 5 * MAXIMUM_x, MAXIMUM_x / 2,
                       MAXIMUM_x / 20, MAXIMUM_x / 10, 2);
  widget_struct->spin_button_bottom =
      cria_spin_button(MAXIMUM_x / 2, 5 * MAXIMUM_x, MAXIMUM_x / 2,
                       MAXIMUM_x / 20, MAXIMUM_x / 10, 2);
  widget_struct->set_button_top = gtk_button_new_with_label("Set");
  widget_struct->set_button_bottom = gtk_button_new_with_label("Set");
  widget_struct->graphic_drawing_area_top = gtk_drawing_area_new();
  graphics_frame_top = gtk_frame_new(NULL);
  gtk_widget_set_name(graphics_frame_top, "frame_graphic");
  widget_struct->graphic_drawing_area_bottom = gtk_drawing_area_new();
  graphics_frame_bottom = gtk_frame_new(NULL);
  gtk_widget_set_name(graphics_frame_bottom, "frame_graphic");
  scale_top_box_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  scale_bottom_box_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  top_label = gtk_label_new("Menor Divisão:");
  bottom_label = gtk_label_new("Menor Divisão:");

  // Colocação das widgets
  gtk_box_pack_start(GTK_BOX(widget_struct->left_box_v), graphics_frame, TRUE,
                     TRUE, 2);
  gtk_container_add(GTK_CONTAINER(graphics_frame), graphics_box_v);
  gtk_container_add(GTK_CONTAINER(graphics_frame_top),
                    widget_struct->graphic_drawing_area_top);
  gtk_container_add(GTK_CONTAINER(graphics_frame_bottom),
                    widget_struct->graphic_drawing_area_bottom);

  gtk_box_pack_start(GTK_BOX(graphics_box_v), widget_struct->combo_box_top,
                     FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(graphics_box_v), scale_top_box_h, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(scale_top_box_h), top_label, TRUE, FALSE, 0);
  gtk_box_pack_end(GTK_BOX(scale_top_box_h), widget_struct->set_button_top,
                   TRUE, FALSE, 0);
  gtk_box_pack_end(GTK_BOX(scale_top_box_h), widget_struct->spin_button_top,
                   TRUE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(graphics_box_v), graphics_frame_top, TRUE, TRUE,
                     0);

  gtk_box_pack_start(GTK_BOX(graphics_box_v), separator_between_graphics, FALSE,
                     FALSE, 0);

  gtk_box_pack_start(GTK_BOX(graphics_box_v), widget_struct->combo_box_bottom,
                     FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(graphics_box_v), scale_bottom_box_h, FALSE, FALSE,
                     0);
  gtk_box_pack_start(GTK_BOX(scale_bottom_box_h), bottom_label, TRUE, FALSE, 0);
  gtk_box_pack_end(GTK_BOX(scale_bottom_box_h),
                   widget_struct->set_button_bottom, TRUE, FALSE, 0);
  gtk_box_pack_end(GTK_BOX(scale_bottom_box_h),
                   widget_struct->spin_button_bottom, TRUE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(graphics_box_v), graphics_frame_bottom, TRUE, TRUE,
                     0);

  // Funções de callback das combo box e de desenho dos gráficos

  g_signal_connect(G_OBJECT(widget_struct->set_button_top), "clicked",
                   G_CALLBACK(cb_set_button_top), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->set_button_bottom), "clicked",
                   G_CALLBACK(cb_set_button_bottom), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->combo_box_top), "changed",
                   G_CALLBACK(cb_combo_box_top), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->combo_box_bottom), "changed",
                   G_CALLBACK(cb_combo_box_bottom), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->graphic_drawing_area_top), "draw",
                   G_CALLBACK(cb_draw_graphic_top), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->graphic_drawing_area_bottom), "draw",
                   G_CALLBACK(cb_draw_graphic_bottom), main_struct);

  // PERSONALIZAÇÃO DA CAIXA middle_box_v

  // DRAWING AREA

  widget_struct->drawing_area_frame = gtk_frame_new("Simulação do Movimento");
  gtk_widget_set_name(widget_struct->drawing_area_frame, "frame_simulation");
  gtk_frame_set_label_align(GTK_FRAME(widget_struct->drawing_area_frame), 0.5,
                            1);
  widget_struct->drawing_area = gtk_drawing_area_new();
  gtk_box_pack_start(GTK_BOX(widget_struct->middle_box_v),
                     widget_struct->drawing_area_frame, TRUE, TRUE, 5);
  gtk_container_add(GTK_CONTAINER(widget_struct->drawing_area_frame),
                    widget_struct->drawing_area);

  // Associação a função de callback

  g_signal_connect(G_OBJECT(widget_struct->drawing_area), "draw",
                   G_CALLBACK(cb_draw_event), main_struct);
  g_timeout_add(10, (GSourceFunc)time_handler, widget_struct);

  // OPÇÕES RELATIVAS AO TEMPO DE SIMULAÇÃO

  // Criação da box e inclusão na middle_box_v.

  time_box_center_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  time_box_v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  time_box_h_1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  time_box_h_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  gtk_box_pack_end(GTK_BOX(widget_struct->middle_box_v), time_box_center_h,
                   FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(time_box_center_h), time_box_v, TRUE, FALSE, 5);

  gtk_box_pack_start(GTK_BOX(time_box_v), time_box_h_1, FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(time_box_v), time_box_h_2, FALSE, FALSE, 5);

  // Criação dos buttons
  time_label = gtk_label_new("Velocidade da Simulação:");
  collision_label = gtk_label_new("Colisões: ");
  collision_image =
      gtk_image_new_from_icon_name("system-shutdown", GTK_ICON_SIZE_BUTTON);
  widget_struct->collision_button = gtk_toggle_button_new();
  gtk_button_set_image(GTK_BUTTON(widget_struct->collision_button),
                       collision_image);
  gtk_widget_set_name(widget_struct->collision_button, "time_button");
  widget_struct->time_button_plus =
      gtk_button_new_from_icon_name("media-seek-forward", GTK_ICON_SIZE_BUTTON);
  widget_struct->time_button_minus = gtk_button_new_from_icon_name(
      "media-seek-backward", GTK_ICON_SIZE_BUTTON);
  gtk_widget_set_name(widget_struct->time_button_plus, "time_button");
  gtk_widget_set_name(widget_struct->time_button_minus, "time_button");
  widget_struct->time_buttons_state =
      gtk_toggle_button_new_with_label("Enabled");
  gtk_toggle_button_set_active(
      GTK_TOGGLE_BUTTON(widget_struct->time_buttons_state), TRUE);
  widget_struct->time_info = gtk_label_new("Tempo da Simulação: Tempo Real");

  gtk_box_pack_start(GTK_BOX(time_box_h_1), time_label, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(time_box_h_1), widget_struct->time_button_minus,
                     FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(time_box_h_1), widget_struct->time_button_plus,
                     FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(time_box_h_1), widget_struct->time_buttons_state,
                     FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(time_box_h_2), widget_struct->collision_button,
                   FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(time_box_h_2), collision_label, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(time_box_h_2), widget_struct->time_info, TRUE,
                     FALSE, 5);

  // Associação das funções de callback aos buttons

  g_signal_connect(G_OBJECT(widget_struct->time_buttons_state), "toggled",
                   G_CALLBACK(cb_time_buttons_state), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->time_button_minus), "clicked",
                   G_CALLBACK(cb_time_button_minus), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->time_button_plus), "clicked",
                   G_CALLBACK(cb_time_button_plus), main_struct);

  // PERSONALIZAÇÃO DA CAIXA right_box_v

  // INICIAR/PARAR

  // Criação das Widgets
  widget_struct->start_stop_label = gtk_label_new("Iniciar/Continuar:");
  widget_struct->start_image = gtk_image_new_from_icon_name(
      "media-playback-start", GTK_ICON_SIZE_BUTTON);
  widget_struct->stop_image = gtk_image_new_from_icon_name(
      "media-playback-pause", GTK_ICON_SIZE_BUTTON);
  widget_struct->start_stop_button = cria_start_stop_button(
      widget_struct->start_image, widget_struct->stop_image);
  widget_struct->reset_button = gtk_button_new_with_mnemonic("_Reset");
  widget_struct->apply_button = gtk_button_new_with_mnemonic("A_plicar");

  // Associação das widgets às funções de callback
  g_signal_connect(G_OBJECT(widget_struct->start_stop_button), "clicked",
                   G_CALLBACK(cb_start_stop_button), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->reset_button), "clicked",
                   G_CALLBACK(cb_reset_button), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->apply_button), "clicked",
                   G_CALLBACK(cb_apply_button), main_struct);

  // Colocação das widgets
  widget_struct->start_stop_box_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start(GTK_BOX(widget_struct->right_box_v),
                     widget_struct->start_stop_box_h, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(widget_struct->start_stop_box_h),
                     widget_struct->start_stop_label, FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(widget_struct->start_stop_box_h),
                   widget_struct->reset_button, FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(widget_struct->start_stop_box_h),
                   widget_struct->apply_button, FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(widget_struct->start_stop_box_h),
                   widget_struct->start_stop_button, FALSE, FALSE, 20);

  // PROPRIEDADES FÍSICAS DO SISTEMA
  physical_properties_title_label = gtk_label_new("Grandezas Físicas");
  gtk_widget_set_name(physical_properties_title_label, "title");
  physical_properties_frame = gtk_frame_new(NULL);
  gtk_frame_set_label_widget(GTK_FRAME(physical_properties_frame),
                             physical_properties_title_label);
  gtk_widget_set_name(physical_properties_frame, "frame_normal");
  gtk_box_pack_start(GTK_BOX(widget_struct->right_box_v),
                     physical_properties_frame, TRUE, TRUE, 5);
  physical_properties_box_v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_set_homogeneous(GTK_BOX(physical_properties_box_v), TRUE);
  gtk_container_add(GTK_CONTAINER(physical_properties_frame),
                    physical_properties_box_v);

  // Criação de labels
  label_mass_M = gtk_label_new("Massa retangular (kg):");
  label_mass_m = gtk_label_new("Massa circular (kg):");
  label_spring_K_left = gtk_label_new("Constante da mola esq. (N/m):");
  label_spring_K_right = gtk_label_new("Constante da mola dir. (N/m):");
  label_pendulum_length = gtk_label_new("Comprimento da haste (m):");

  // Criação de spin buttons
  widget_struct->spin_button_mass_M =
      cria_spin_button(INITIAL_MASS_M, MAXIMUM_MASS_M, MINIMUM_MASS_M,
                       STEP_MASS_M, PAGE_MASS_M, MASS_DIGITS);
  widget_struct->spin_button_mass_m =
      cria_spin_button(INITIAL_MASS_m, MAXIMUM_MASS_m, MINIMUM_MASS_m,
                       STEP_MASS_m, PAGE_MASS_m, MASS_DIGITS);
  widget_struct->spin_button_spring_K_left =
      cria_spin_button(INITIAL_K_LEFT, MAXIMUM_K_LEFT, MINIMUM_K_LEFT,
                       STEP_K_LEFT, PAGE_K_LEFT, K_DIGITS);
  widget_struct->spin_button_spring_K_right =
      cria_spin_button(INITIAL_K_RIGHT, MAXIMUM_K_RIGHT, MINIMUM_K_RIGHT,
                       STEP_K_RIGHT, PAGE_K_RIGHT, K_DIGITS);
  widget_struct->spin_button_pendulum_length =
      cria_spin_button(INITIAL_LENGHT, MAXIMUM_LENGHT, MINIMUM_LENGHT,
                       STEP_LENGHT, PAGE_LENGHT, LENGHT_DIGITS);

  // Associação dos botões às funções de callback

  g_signal_connect(G_OBJECT(widget_struct->spin_button_mass_M), "changed",
                   G_CALLBACK(cb_spin_button), &(physical_str->M));
  g_signal_connect(G_OBJECT(widget_struct->spin_button_mass_m), "changed",
                   G_CALLBACK(cb_spin_button), &(physical_str->m));
  g_signal_connect(G_OBJECT(widget_struct->spin_button_spring_K_left),
                   "changed", G_CALLBACK(cb_spin_button),
                   &(physical_str->K_left));
  g_signal_connect(G_OBJECT(widget_struct->spin_button_spring_K_right),
                   "changed", G_CALLBACK(cb_spin_button),
                   &(physical_str->K_right));
  g_signal_connect(G_OBJECT(widget_struct->spin_button_pendulum_length),
                   "changed", G_CALLBACK(cb_spin_button), &(physical_str->l));

  // Criação de boxes para colocar labels e spin buttons e colocação das boxes
  // na frame
  box_mass_M_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  box_mass_m_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  box_spring_K_left_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  box_spring_K_right_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  box_pendulum_length_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  gtk_box_pack_start(GTK_BOX(physical_properties_box_v), box_mass_M_h, FALSE,
                     FALSE, 5);
  gtk_box_pack_start(GTK_BOX(physical_properties_box_v), box_mass_m_h, FALSE,
                     FALSE, 5);
  gtk_box_pack_start(GTK_BOX(physical_properties_box_v), box_spring_K_left_h,
                     FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(physical_properties_box_v), box_spring_K_right_h,
                     FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(physical_properties_box_v), box_pendulum_length_h,
                     FALSE, FALSE, 5);

  gtk_box_pack_start(GTK_BOX(box_mass_M_h), label_mass_M, FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(box_mass_M_h), widget_struct->spin_button_mass_M,
                   FALSE, FALSE, 5);

  gtk_box_pack_start(GTK_BOX(box_mass_m_h), label_mass_m, FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(box_mass_m_h), widget_struct->spin_button_mass_m,
                   FALSE, FALSE, 5);

  gtk_box_pack_start(GTK_BOX(box_spring_K_left_h), label_spring_K_left, FALSE,
                     FALSE, 5);
  gtk_box_pack_end(GTK_BOX(box_spring_K_left_h),
                   widget_struct->spin_button_spring_K_left, FALSE, FALSE, 5);

  gtk_box_pack_start(GTK_BOX(box_spring_K_right_h), label_spring_K_right, FALSE,
                     FALSE, 5);
  gtk_box_pack_end(GTK_BOX(box_spring_K_right_h),
                   widget_struct->spin_button_spring_K_right, FALSE, FALSE, 5);

  gtk_box_pack_start(GTK_BOX(box_pendulum_length_h), label_pendulum_length,
                     FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(box_pendulum_length_h),
                   widget_struct->spin_button_pendulum_length, FALSE, FALSE, 5);

  // CONDIÇÕES INICIAIS DO SISTEMA
  initial_conditions_title_label = gtk_label_new("Condições Iniciais");
  gtk_widget_set_name(initial_conditions_title_label, "title");
  initial_conditions_frame = gtk_frame_new(NULL);
  gtk_frame_set_label_widget(GTK_FRAME(initial_conditions_frame),
                             initial_conditions_title_label);
  gtk_widget_set_name(initial_conditions_frame, "frame_normal");
  gtk_box_pack_start(GTK_BOX(widget_struct->right_box_v),
                     initial_conditions_frame, TRUE, TRUE, 5);
  initial_conditions_box_v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_set_homogeneous(GTK_BOX(initial_conditions_box_v), TRUE);
  gtk_container_add(GTK_CONTAINER(initial_conditions_frame),
                    initial_conditions_box_v);

  // Criação de labels
  label_x = gtk_label_new("Elongação (m):");
  label_v = gtk_label_new("Velocidade da massa M (m/s):");
  label_theta = gtk_label_new("Ângulo do pêndulo (rad):");
  label_omega = gtk_label_new("Velocidade angular (rad/s):");

  // Criação de spin buttons
  widget_struct->spin_button_x = cria_spin_button(
      INITIAL_x, MAXIMUM_x, MINIMUM_x, STEP_x, PAGE_x, x_DIGITS);
  widget_struct->spin_button_v = cria_spin_button(
      INITIAL_v, MAXIMUM_v, MINIMUM_v, STEP_v, PAGE_v, x_DIGITS);
  widget_struct->spin_button_theta =
      cria_spin_button(INITIAL_theta, MAXIMUM_theta, MINIMUM_theta, STEP_theta,
                       PAGE_theta, theta_DIGITS);
  widget_struct->spin_button_omega =
      cria_spin_button(INITIAL_omega, MAXIMUM_omega, MINIMUM_omega, STEP_omega,
                       PAGE_omega, theta_DIGITS);

  // Criação de boxes para colocar labels e spin buttons e colocação das boxes
  // na frame
  box_x_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  box_v_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  box_theta_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  box_omega_h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  gtk_box_pack_start(GTK_BOX(initial_conditions_box_v), box_x_h, FALSE, FALSE,
                     5);
  gtk_box_pack_start(GTK_BOX(initial_conditions_box_v), box_v_h, FALSE, FALSE,
                     5);
  gtk_box_pack_start(GTK_BOX(initial_conditions_box_v), box_theta_h, FALSE,
                     FALSE, 5);
  gtk_box_pack_start(GTK_BOX(initial_conditions_box_v), box_omega_h, FALSE,
                     FALSE, 5);

  gtk_box_pack_start(GTK_BOX(box_x_h), label_x, FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(box_x_h), widget_struct->spin_button_x, FALSE, FALSE,
                   5);

  gtk_box_pack_start(GTK_BOX(box_v_h), label_v, FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(box_v_h), widget_struct->spin_button_v, FALSE, FALSE,
                   5);

  gtk_box_pack_start(GTK_BOX(box_theta_h), label_theta, FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(box_theta_h), widget_struct->spin_button_theta,
                   FALSE, FALSE, 5);

  gtk_box_pack_start(GTK_BOX(box_omega_h), label_omega, FALSE, FALSE, 5);
  gtk_box_pack_end(GTK_BOX(box_omega_h), widget_struct->spin_button_omega,
                   FALSE, FALSE, 5);

  // Associação dos botões às funções de callback

  g_signal_connect(G_OBJECT(widget_struct->spin_button_x), "changed",
                   G_CALLBACK(cb_motion_spin_button), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->spin_button_v), "changed",
                   G_CALLBACK(cb_motion_spin_button), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->spin_button_theta), "changed",
                   G_CALLBACK(cb_motion_spin_button), main_struct);
  g_signal_connect(G_OBJECT(widget_struct->spin_button_omega), "changed",
                   G_CALLBACK(cb_motion_spin_button), main_struct);

  return main_struct;
}

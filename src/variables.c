#include "../lib/variables.h"

void calculo_numerico_Euler_Cromer(Variables_struct *variables, double dt,
                                   int *rec_collision) {

  double x, v, theta, omega, M, m, K_left, K_right, l;
  double c, w, K;

  // Declaram-se as variáveis para a escrita das funções ser mais perceptível

  x = variables->motion_conditions->x;
  v = variables->motion_conditions->v;
  theta = variables->motion_conditions->theta;
  omega = variables->motion_conditions->omega;

  M = variables->physical_properties->M;
  m = variables->physical_properties->m;
  K_left = variables->physical_properties->K_left;
  K_right = variables->physical_properties->K_right;
  l = variables->physical_properties->l;

  K = K_left + K_right;
  c = 1 + m / M * sin(theta) * sin(theta);
  w = m / M * sin(theta) * cos(theta);

  if (*rec_collision == 0) {
    variables->motion_conditions->v =
        v +
        (w * G + m / M * l * sin(theta) * omega * omega - K / M * x) / c * dt;
    variables->motion_conditions->x =
        x + (variables->motion_conditions->v) * dt;
  } else {
    variables->motion_conditions->v = -v;
    variables->motion_conditions->x =
        x + (variables->motion_conditions->v) * dt;
    *rec_collision = 0;
  }

  variables->motion_conditions->omega =
      omega + (-w * l * omega * omega - (1 + m / M) * G * sin(theta) +
               K / M * cos(theta) * x) /
                  c / l * dt;
  theta = theta + (variables->motion_conditions->omega) * dt;

  theta = fmod(theta, 2 * M_PI);
  if (theta < 0) {
    theta = theta + 2 * M_PI;
  }

  if (theta > M_PI) {
    theta = theta - 2 * M_PI;
  }

  variables->motion_conditions->theta = theta;
}

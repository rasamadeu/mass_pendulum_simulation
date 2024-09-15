#ifndef __VARIABLES_H__
#define __VARIABLES_H__ 1

#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define G 9.806

typedef struct {

  double M;
  double m;
  double K_left;
  double K_right;
  double l;

} physical_properties_struct;

typedef struct {

  double x;
  double v;
  double theta;
  double omega;
  double dt;

} motion_conditions_struct;

typedef struct {

  physical_properties_struct *physical_properties;
  motion_conditions_struct *motion_conditions;

} Variables_struct;

void calculo_numerico_Euler_Cromer(Variables_struct *variables, double dt,
                                   int *rec_collision);

#endif /* __VARIABLES_H__ */

#include "mathlib.h"
#include <stdio.h>
static double p = 1;
double Power2(double base, int k) {
  double Base_Value = 1;
  if (k == 1) {
    return base;
  }

  for (int i = 1; i <= k; k--) {
    Base_Value *= base;
  }
  return Base_Value;
}

double pi_euler(void) {
  p = 1;
  double total;
  total = 0;
  double num = (1 / (Power2(p, 2)));
  while (num > EPSILON) {
    // printf("Value: %16.15f\n", total);
    // printf("Power Value: %f\n", Power2(p,2));
    num = (1 / (Power2(p, 2)));
    total += (1 / (Power2(p, 2)));
    p++;
  }
  return sqrt_newton(6 * total);
}

int pi_euler_terms() { return p; }

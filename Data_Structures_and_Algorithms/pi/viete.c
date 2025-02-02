#include "mathlib.h"
static int j = 0;
double pi_viete() {
  double total = 1;
  j = 0;
  double tot = 2;
  double bruh = sqrt_newton((2));
  tot *= (2.00) / (sqrt_newton(2));

  for (int i = 0; i < 29; i++) {
    j++;
    total = (2.00 / (sqrt_newton((bruh + 2.00))));
    bruh = sqrt_newton(2.00 + bruh);
    tot *= total;
  }
  return tot;
}

int pi_viete_factors() { return j; }

#include "mathlib.h"
static int k = 0;
double Power5(double base, int k) {
  double Base_Value = 1;
  if (k == 0) {
    return 1;
  }
  if (k == 1) {
    return base;
  }

  for (int i = 1; i <= k; k--) {
    Base_Value *= base;
    // printf("%f\n", Base_Value);
  }
  // printf("%d", k);
  return Base_Value;
}

double pi_madhava() {
  double num = 0;
  double total = 0;
  k = 0;
  do {
    if (k % 2 == 0) {
      total += (1) / ((2 * k + 1) * (Power5(3, k)));
    } else {
      total -= 1 / ((2 * k + 1) * (Power5(3, k)));
    }
    num = (1) / ((2 * k + 1) * (Power5(3, k)));
    k++;

  } while (num > EPSILON);

  total *= sqrt_newton(12);
  return total;
}

int pi_madhava_terms() { return k; }

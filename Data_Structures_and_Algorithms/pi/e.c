#include "mathlib.h"
static int k = 0;
double e() {
  double Factorial_Val = 1;
  double num = 0;
  double total = 0;

  k = 0;
  do {
    Factorial_Val = 1;
    for (int j = 2; j <= k; j++) {
      Factorial_Val *= j;
    }
    total += (1.00 / Factorial_Val);
    num = (1.00 / Factorial_Val);
    k++;
  } while (num > EPSILON);
  return total;
}

int e_terms() { return k; }

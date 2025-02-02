#include "mathlib.h"
double Power(double base, int k);
static double k = 0;
double pi_bbp() {
  k = 0;
  double sum = 0;
  double num;
  do {
    // printf("Term: %f\n",k );
    num = (Power(0.0625, k)) *
          ((k * ((120 * k) + 151) + 47) /
           (k * (k * (k * ((512 * k) + 1024) + 712) + 194) + 15));
    sum += (Power(0.0625, k)) *
           ((k * ((120 * k) + 151) + 47) /
            (k * (k * (k * ((512 * k) + 1024) + 712) + 194) + 15));
    // printf ("Num: %f \n", num);
    k++;

  } while (EPSILON < num);
  return sum;
}

double Power(double base, int k) {
  double Base_Value = 0.0625;
  if (k == 0) {
    return 1;
  }

  for (int i = 1; i < k; k--) {
    Base_Value *= base;
    // printf("%f\n", Base_Value);
  }
  // printf("%d", k);
  return Base_Value;
}

int pi_bbp_terms() { return (int)(k - 1); }

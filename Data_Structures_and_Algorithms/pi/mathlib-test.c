#include "mathlib.h"
#include <stdio.h>
#include <unistd.h>
#define OPTIONS "hambensvrz:"
#define M_PI 3.14159265358979323846
#define M_E 2.718281828459045

int main(int argc, char *argv[]) {
  int opt = 0;
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'a':
      // printf("e() = %16.15lf", e_Val);
      printf(", M_E = %16.15f , ", M_E);
      // printf("diff = %16.20lf\n", total);
      printf("e_terms() = %d\n", e_terms());
      printf("pi_bbp() = %16.15lf", pi_bbp());
      printf(", M_PI = %16.15lf , ", M_PI);
      printf("diff = %16.15lf\n", M_PI - pi_bbp());
      printf("pi_bbp () terms = %d\n", pi_bbp_terms());
      printf("pi_madhava() = %16.15lf", pi_madhava());
      printf(", M_PI = %16.15lf , ", M_PI);
      printf("diff = %16.20lf\n", (M_PI - pi_madhava()));
      printf("pi_madhava_terms() = %d\n", pi_madhava_terms());
      printf("pi_euler() = %16.15lf", pi_euler());
      printf(", M_PI = %16.15lf , ", M_PI);
      printf("diff = %16.20lf\n", (M_PI - pi_euler()));
      printf("pi_euler_terms() = %d\n", pi_euler_terms());
      printf("pi_viete() = %16.15lf", pi_viete());
      printf(", M_PI = %16.15lf , ", M_PI);
      printf("diff = %16.20lf\n", M_PI - pi_viete());
      printf("pi_viete_factors() = %d\n", pi_viete_factors());
      printf("sqrt_newton_iters(): %d\n", sqrt_newton_iters());
      break;
    case 'b':
      printf("pi_bbp() = %16.15lf", pi_bbp());
      printf(", M_PI = %16.15lf , ", M_PI);
      printf("diff = %16.15lf\n", M_PI - pi_bbp());
      break;
    case 'e':
      printf("e() = %16.15lf", e());
      printf(", M_E = %16.15f , ", M_E);
      printf("diff = %16.20lf\n", (M_E - e()));

      break;
    case 'n':
      printf("Newton value: %f\n", sqrt_newton(64.00));
      break;
    case 'v':
      printf("pi_viete() = %16.15lf", pi_viete());
      printf(", M_PI = %16.15lf , ", M_PI);
      printf("diff = %16.20lf\n", M_PI - pi_viete());

      break;
    case 'r':
   
      printf("pi_euler() = %16.15lf", pi_euler());
      printf(", M_PI = %16.15lf , ", M_PI);
      printf("diff = %16.20lf\n", (M_PI - pi_euler()));

      break;

    case 'm':

      printf("pi_madhava() = %16.15lf", pi_madhava());
      printf(", M_PI = %16.15lf , ", M_PI);
      printf("diff = %16.20lf\n", (M_PI - pi_madhava()));

      break;
    case 's':
      if (pi_bbp_terms() > 0) {
        printf("pi_bbp () terms = %d\n", pi_bbp_terms());
      }

      if (pi_madhava_terms() != 0) {
        printf("madHava(): %d \n", pi_madhava_terms());
      }
      if (pi_euler_terms() > 1) {
        printf("pi_euler_terms() = %d\n", pi_euler_terms());
      }
      if (e_terms() != 0) {
        printf("e_terms() = %d\n", e_terms());
      }
      if (pi_viete_factors() > 0) {
        printf("pi_viete_factors() = %d\n", pi_viete_factors());
      }
      break;
    case 'h':
      printf("-a : Runs all tests.\n-e : Runs e approximation test.\n-b : Runs "
             "Bailey-Borwein-Plouffe π approximation test.\n-m : RunsMadhava π "
             "approximation test.\n-r : Runs Euler sequence π approximation "
             "test.\n-v : Runs Viète π approximation test.\n-n : Runs "
             "Newton-Raphson square root approximation tests.\n-s : Enable "
             "printing of statistics to see computed terms and factors for "
             "each tested function.\n-h : Display a help message detailing "
             "programusage.\n");
      break;
    default:

      printf("Error please enter one of the select outputs");
      break;
    }
  }
  return 0;
}

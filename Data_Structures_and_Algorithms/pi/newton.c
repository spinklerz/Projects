#include "mathlib.h"
#include <stdio.h>
static double i = 0;
double sqrt_newton(double x)
{
    
    double z = 0;
    double y = 1;
    double calc = y - z;
    while ((calc) > EPSILON) {
        z = y;
        y = 0.5 * (z + x / z);
        calc = y - z;
       if ((y-z) < 0){
          calc += z;
      }
     i++;
    }
    return y;
    
    
}

int sqrt_newton_iters(void){
	return i; 
}




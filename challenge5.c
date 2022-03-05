/** Challenge 5 Complex Numbers 
 **
 ** Can you extend the derivative (challenge 2) to the complex domain:
 ** that is, functions thar receive and return double complex values?
 **/

#include <stdlib.h>
#include <stdio.h>
#include <tgmath.h>

// Machine epsilon
static double const eps = 0x1P-52;

double complex F(double complex z) {
  return 1 / (cos(z));
}

double complex f(double complex z) {
  double complex h = z ? sqrt(eps) * z : eps;

  return (F(z+h) - F(z)) / h;
}

int main(int argc, char* argv[argc+1]) {

  double complex z = 0.5 + 0.6*I;
  double complex res = f(z);
  double complex exp = sin(z) / (cos(z)*cos(z));
  printf("z=%f%+fi\nf(z)=%f%+fi\nexpected=%f%+fi\n",
         crealf(z), cimagf(z), crealf(res), cimagf(res), crealf(exp), cimagf(exp));

  return 0;
}

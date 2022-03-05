/** Challenge 2 Numerical derivatives
 **
 ** Something we'll deal with a lot is the concept of numerical algorithms.
 ** To get your hands dirty, see if you can implement the numerical derivative
 ** double f(double x) of a function double F(double x).
 **
 ** Implement this with an example F for the function that you use for this
 ** exercise. A good primary choice for F would be a function for which you
 ** know the derivative, such as sin, cos, or sqrt. This allows you to check
 ** your results for correctness.
 **
 **/

#include <stdlib.h>
#include <stdio.h>
#include <tgmath.h>

// Machine epsilon
static double const eps = 0x1P-52;

double F(double x) {
  return cos(x);
}

double f(double x) {
  double h = x ? sqrt(eps) * x : eps;

  return (F(x+h) - F(x)) / h;
}

int main(int argc, char* argv[argc+1]) {
  for (int i = 1; i < argc; ++i) {
    double const x = strtod(argv[i], 0);
	printf("x=%f\t-sin(x)=%f\tcos`(x)=%f\n", x, -sin(x), f(x));
  }

  return 0;
}

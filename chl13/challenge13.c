/* Challenge 13 General Derivative
 * Can you extend the real and complex derivatives (chapters 2 and 5) such
 * that they recieve the function F and the value x as a parameter?
 *
 * Can you use the generic real derivatives to implement Newton's method
 * for finding roots?
 *
 * Can you find the real zeros of polynomials?
 *
 * Can you find the complex zeros of polynomials?
 */
#include <stdlib.h>
#include <stdio.h>
#include <tgmath.h>
#include <float.h>
#include <stdbool.h>

// Machine epsilon
static double const eps = 0x1P-52;
static double const lim = 1e-9;

typedef double real_function(double);
typedef double complex complex_function(double complex);
typedef double complex polynomial(double complex);

double f_real(real_function* F, double x) {
  double h = cbrt(eps);
  //double h = x ? sqrt(eps) * x : eps;
  
  return (F(x+h) - F(x-h)) / (2*h);
}

double complex f_complex(complex_function* F, double complex z) {
  double complex h = cbrt(eps);
  //double complex h = z ? sqrt(eps) * z : eps;

  return (F(z+h) - F(z-h)) / (2*h);
}

double newt_methodr(real_function* F, double x) {
  double dy = f_real(F, x);
  if (fabs(dy) < lim) return x;
  double new = x - (F(x) / dy);
  if (fabs(new - x) < lim)
    return new;
  else
    return newt_methodr(F, new);
}

double complex newt_methodc(complex_function* F, double complex z) {
  double complex dy = f_complex(F, z);
  if (fabs(dy) < lim) return z;
  double complex new = z - (F(z) / dy);
  if (fabs(new - z) < lim)
    return new;
  else
    return newt_methodc(F, new);
}

/* Polynomial functions */
/* polyr_eval: Evaluate real polynomial with coefficients from poly (from
 * least to most significant) and degree deg at value x.
 */
double polyr_eval(size_t deg, double poly[deg+1], double x) {
  double ret = 0.0;
  for (size_t i = deg+1; i > 0; --i) {
    ret *= x;
    ret += poly[i-1];
  }
  return ret;
}
/* polyc_eval: Evaluate complex polynomial with coefficients from poly (from
 * least to most significant) and degree deg at value x.
 */
double complex polyc_eval(size_t deg, double complex poly[deg+1],
                          double complex x) {
  double complex ret = 0.0;
  for (size_t i = deg+1; i > 0; --i) {
    ret *= x;
    ret += poly[i-1];
  }
  return ret;
}
/* polyr_ldiv: Perform polynomial division with numerator poly and denominator
 * the linear polynomial X - x.
 */
double polyr_ldiv(size_t deg, double poly[deg+1], double res[deg+1], double x) {
  res[deg] = poly[deg];
  for (size_t i = deg; i > 0; --i) {
    double tmp = x * res[i];
    res[i-1] = poly[i-1] + tmp;
  }
  return res[0];
}
/* polyr_bound: Uses Cauchy's bound to determine maximum size of the zeros of
 * polynomial poly
 */
double polyr_bound(size_t deg, double poly[deg+1]) {
  double max = -DBL_MAX; 
  for (size_t i = 0; i < deg; ++i) {
    double tmp = fabs(poly[i] / poly[deg]);
    max = max < tmp ? tmp : max;
  }
  return max + 1.0;
}

double polyc_bound(size_t deg, double complex poly[deg+1]) {
  double max = -DBL_MAX;
  for (size_t i = 0; i < deg; ++i) {
    double tmp = fabs(poly[i] / poly[deg]);
    max = max < tmp ? tmp : max;
  }
  return max + 1.0;
}

/* polyr_zeros: Finds the zeros of a given polynomial poly
 */
void polyr_zeros2(size_t deg, double guess,
                  double poly[deg+1]) {
  if (!deg) return;
  double p(double x) {
    return polyr_eval(deg, poly, x);
  }
  double root = newt_methodr(p, guess);
  printf("%f\n", root);
  double q[deg+1];
  for (size_t j = 0; j < deg+1; ++j) {
    q[j] = 0.0;
  }
  polyr_ldiv(deg, poly, q, root);
  double new[deg];
  for (size_t j = 0; j < deg; ++j) {
    new[j] = q[j+1];
  }
  polyr_zeros2(deg-1, root, new);
}

void polyr_zeros(size_t deg, double poly[deg+1]) {
  double guess = polyr_bound(deg, poly);
  polyr_zeros2(deg, guess, poly);
}

/* init_guesses: Initializes solution vector with guesses along the upper portion
 * of a circle of radius r along with the corresponding complex conjugates. If deg is odd,
 * one guess will be the real number r.
 */
double complex* init_guesses(size_t deg, double r, double complex sol[deg]) {
  if (deg < 2) {
    sol[0] = r;
    return sol;
  }
  // double angle = 2*M_PI / (deg + 1);
  double angle = M_PI / ((deg / 2)+1);
  double complex rot = cos(angle) + I*sin(angle);
  double complex x = r;
  // for (size_t i = 0; i < deg; ++i) {
  //   x *= rot;
  //   sol[i] = x;
  // }
  size_t i = 0;
  while (i < deg-1) {
    x *= rot;
    sol[i] = x;
    ++i;
    sol[i] = conj(x);
    ++i;
  }
  if (i == deg-1) sol[i] = r;
  return sol;
}

void polyc_zeros(size_t deg, double complex poly[deg+1]) {
  if (fabs(poly[deg] - 1.0) >= lim) {
    double complex a = poly[deg];
    for (size_t i = 0; i < deg+1; ++i) {
      poly[i] /= a;
    }
  }
  double complex sol[deg];
  init_guesses(deg, polyc_bound(deg, poly), sol);
  // sol[0] = deg % 2 ? 1.0 : 0.4 + 0.9*I;
  // for (size_t i = 1; i < deg; ++i) {
  //   sol[i] = sol[i-1] * (0.4 + 0.9*I);
  // }

  bool changed = true;
  while (changed) {
    bool updated = false;
    for (size_t i = 0; i < deg; ++i) {
      double complex prev = sol[i];
      double complex a = polyc_eval(deg, poly, prev);
      for (size_t j = 0; j < deg; ++j) {
        if (i == j) continue;
        a /= (prev - sol[j]);
      }
      sol[i] -= a;
      updated = updated || (fabs(sol[i] - prev) >= lim);
    }
    changed = updated;
  }
  for (size_t i = 0; i < deg; ++i) {
    printf("%f%+fi\n", crealf(sol[i]), cimagf(sol[i]));
  }
}

int main(int argc, char* argv[argc+1]) {
  unsigned deg = argc - 2;
  double complex p[deg+1];
  for (unsigned i = 1; i < argc; ++i) {
    p[i-1] = strtod(argv[i], 0);
  }
  polyc_zeros(deg, p);
  return 0;
}

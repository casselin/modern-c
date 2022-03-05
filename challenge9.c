/* Challenge 9 Factorization
 * Now that we've covered functions, see if you can implement a program
 * factor that receives a number N on the command line and prints out
 * N: F0 F1 F2 ...
 * where F0 and so on are all the prime factors of N.
 * The core of your implementation should be a function that, given a value
 * of type size_t, returns its smallest prime factor.
 * Extend this program to receive a list of such numbers and output such a
 * line for each of them.
 */

#include <stdio.h>
#include <stdlib.h>

void factorize(size_t n, size_t p) {
  if (n == 1) {
    printf("\n");
    return;
  }

  if (!(n % p)) {
    printf("%zu ", p);
    factorize(n / p, p);
  } else {
    factorize(n, p+1);
  }
}

void factors(size_t n) {
  printf("%zu: ", n);
  factorize(n, 2);
}

int main(int argc, char* argv[argc+1]) {
  if (argc < 2) {
    printf("Usage: %s n1 ...\n", argv[0]);
    return 0;
  }

  for (unsigned i = 1; i < argc; i++) {
    size_t n = strtoull(argv[i], 0, 10);
    factors(n);
  } 
  return 0;
}

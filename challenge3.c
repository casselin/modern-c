/** Challenge 3 Pi
 **
 ** Compute the N first decimal places of Pi.
 **
 **/

#include <stdlib.h>
#include <stdio.h>
#include <tgmath.h>

/* Implementation of the spigot algorithm by Rabinowitz and Wagon */
int main(int argc, char* argv[argc+1]) {
  long n = strtol(argv[1], 0, 0);
  long len = 10*n / 3;

  long a[len];

  for (int i = 0; i < len; ++i) {
    a[i] = 2;
  }
  int nines = 0;
  long predigit = 0;

  for (int j = 0; j < n; ++j) {
    long q = 0;

    for (int i = len - 1; i >= 0; --i) {
      long x = 10*a[i] + q*(i+1);
	  a[i] = x % (2*i + 1);
	  q = x / (2*i + 1);
	}

	a[0] = q % 10;
	q = q / 10;

	if (q == 9) {
      nines++;
	} else if (q == 10) {
      printf("%ld", predigit + 1);
	  for (int k = 0; k < nines; ++k) {
        printf("0");
	  }
      predigit = 0;
	  nines = 0;
	} else {
      printf("%ld", predigit);
	  predigit = q;
      for (int k = 0; k < nines; ++k) {
        printf("9");
      }
      nines = 0;
	}
  }
  printf("%ld\n", predigit);

  return 0;
}


/* Implementation of Machin's formula for Pi using doubles, approximates up to 10 digits of Pi
int main(int argc, char* argv[argc+1]) {
  double digits = strtod(argv[1], 0);
  double e = pow(10, -(digits+1));

  double term1 = 1.0 / 5;
  double term2 = 1.0 / 239;
  double sum = 16*term1 - 4*term2;

  for (int n = 1; ; ++n) {
    term1 /= -5*5;
	term2 /= -239*239;


	double check = fabs((16*term1 - 4*term2) / (2*n + 1));

	if ((!term1 && !term2) || check < e) {
      break;
	}

	sum += (16*term1 - 4*term2) / (2*n + 1);
  }

  sum = fmod(sum, 1);
  long long result = trunc(sum * pow(10, digits)); 

  printf("3.%lld\n", result);
  return 0;
}
*/

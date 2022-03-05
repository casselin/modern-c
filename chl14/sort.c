/* Challenge 14 Generic sorting
 * Can you extend your sorting algorithms (challenge 1) to other sort keys?
 *
 * Can you condense your functions for different sort keys to functions
 * that have the same signature as qsort: that is, receive generic pointers
 * to data, size information, and a comparison function as parameters?
 *
 * Can you extend the performance comparison of your sorting algorithms
 * (challenge 10) to the C library function qsort?
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

void merge(void* base,
           size_t l, size_t m, size_t h, size_t size,
           int (*comp)(void const*, void const*)) {
  size_t lenl = (m - l)*size;
  size_t lenr = (h - m)*size;

  unsigned char L[lenl];
  unsigned char R[lenr];

  unsigned char* ptr = base;

  for (size_t i = 0; i < lenl; ++i) {
    L[i] = ptr[size*l + i];
  }

  for (size_t i = 0; i < lenr; ++i) {
    R[i] = ptr[size*m + i];
  }

  size_t i = 0;
  size_t j = 0;

  for (size_t k = l*size; k < h*size; k += size) {
    if (i < lenl && (j >= lenr || (comp(L + i, R + j) <= 0))) {
      for (size_t m = 0; m < size; ++m) {
        ptr[k + m] = L[i + m];
      }
      i += size;
    } else {
      for (size_t m = 0; m < size; ++m) {
        ptr[k + m] = R[j + m];
      }
      j += size;
    }
  }
}

void mergesplit(void* base,
                size_t l, size_t h, size_t size,
                int (*comp)(void const*, void const*)) {
  if (h - l <= 1) return;

  size_t mid = (h + l) / 2;

  mergesplit(base, l, mid, size, comp);
  mergesplit(base, mid, h, size, comp);

  merge(base, l, mid, h, size, comp);
}

void mergesort(void* base,
               size_t n, size_t size,
               int (*comp)(void const*, void const*)) {
  mergesplit(base, 0, n, size, comp);
}

void swap(void* a, void* b, size_t size) {
  unsigned char* A = a;
  unsigned char* B = b;

  while (size) {
    unsigned char tmp = *A;
    *A = *B;
    *B = tmp;
    A++;
    B++;
    size--;
  }
}

void quicksort2(void* base,
               size_t l, size_t h, size_t size,
               int (*comp)(void const*, void const*)) {
  if (h - l <= 1) {
    return;
  }

  unsigned char* p = base;

  size_t pivot = size*((h + l) / 2);
  size_t last = size*l;

  swap(p + pivot, p + last, size);

  for (size_t i = l*size; i < h*size; i += size) {
    if (comp(p + i, p + l*size) < 0) {
      last += size;
      swap(p + i, p + last, size);
    }
  }
  swap(p + l*size, p + last, size);

  quicksort2(base, l, last / size, size, comp);
  quicksort2(base, (last / size) + 1, h, size, comp);
}

void quicksort(void* base,
               size_t n, size_t size,
               int (*comp)(void const*, void const*)) {
  quicksort2(base, 0, n, size, comp);
}

struct timespec timespec_diff(struct timespec start,
                              struct timespec stop) {
  if (stop.tv_sec < start.tv_sec || 
      (stop.tv_sec == start.tv_sec && stop.tv_nsec < start.tv_nsec)) {
    return stop = timespec_diff(stop, start);
  } 
  assert(stop.tv_sec > start.tv_sec || stop.tv_nsec >= start.tv_nsec);

  if (stop.tv_nsec < start.tv_nsec) {
    if (stop.tv_sec > start.tv_sec) {
      stop.tv_nsec -= start.tv_nsec;
      stop.tv_nsec += 1000000000;
      stop.tv_sec--;
    }
  } else {
    stop.tv_nsec -= start.tv_nsec;
  }
  stop.tv_sec -= start.tv_sec;

  return stop;
}

int compare_dbl(void const* a, void const* b) {
  double const* A = a;
  double const* B = b;

  if (*A < *B) {
    return -1;
  } else if (*A > *B) {
    return +1;
  } else {
    return 0;
  }
}

int compare_unsigned(void const* a, void const* b) {
  unsigned const* A = a;
  unsigned const* B = b;

  if (*A < *B) {
    return -1;
  } else if (*A > *B) {
    return +1;
  } else {
    return 0;
  }
}

int compare_char(void const* a, void const* b) {
  char const* A = a;
  char const* B = b;
  if (*A < *B) {
    return -1;
  } else if (*A > *B) {
    return +1;
  } else {
    return 0;
  }
}

/*
int main(void) {
  srand(time(0));
  double A[10];
  for (size_t i = 0; i < 10; ++i) {
    A[i] = cos(rand());
  }
  mergesort(A, 10, sizeof A[0], compare_dbl);

  for (size_t i = 0; i < 10; ++i) {
    printf("%.1f\n", A[i]);
  }

  return 0;
}
*/

#define MIN 32768ULL // 2^15
#define MAX 524288ULL // 2^19
// Through trial and error, can create a double array of size 10^6 ~=< 2^20
int main(void) {
  srand(time(0));
  static double A[MAX];
  static struct timespec elapsed[5]; // Verify this if you change MIN/MAX
  static size_t n;
  printf("My merge sort on arrays of double\n");
  n = MIN;
  for (size_t i = 0; i < 5; ++i, n <<= 1) {
    for (size_t j = 0; j < n; ++j) {
      A[j] = cos(rand());
    }
    printf("n = %zu:\n", n);
    struct timespec start = { 0 };
    struct timespec stop = { 0 };
    timespec_get(&start, TIME_UTC);
    mergesort(A, n, sizeof A[0], compare_dbl);
    timespec_get(&stop, TIME_UTC);
    elapsed[i] = timespec_diff(start, stop);
    
    printf("Time elapsed: %lld.%.9lds\t",
           (long long)elapsed[i].tv_sec, elapsed[i].tv_nsec);
    if (i > 0) {
      double num = (elapsed[i].tv_sec + elapsed[i].tv_nsec*1E-9);
      double denom = (elapsed[i-1].tv_sec + elapsed[i-1].tv_nsec*1E-9);
      double ratio = num / denom;
      double expected = (n*log2(n)) / ((n/2)*log2(n/2));
      printf("Actual ratio: %g\tExpected: %g", ratio, expected);
    }
    putchar('\n');
  }
  printf("My quick sort on arrays of double\n");
  n = MIN;
  for (size_t i = 0; i < 5; ++i, n <<= 1) {
    for (size_t j = 0; j < n; ++j) {
      A[j] = cos(rand());
    }
    printf("n = %zu:\n", n);
    struct timespec start = { 0 };
    struct timespec stop = { 0 };
    timespec_get(&start, TIME_UTC);
    quicksort(A, n, sizeof A[0], compare_dbl);
    timespec_get(&stop, TIME_UTC);
    elapsed[i] = timespec_diff(start, stop);
    
    printf("Time elapsed: %lld.%.9lds\t",
           (long long)elapsed[i].tv_sec, elapsed[i].tv_nsec);
    if (i > 0) {
      double num = (elapsed[i].tv_sec + elapsed[i].tv_nsec*1E-9);
      double denom = (elapsed[i-1].tv_sec + elapsed[i-1].tv_nsec*1E-9);
      double ratio = num / denom;
      double expected = (n*log2(n)) / ((n/2)*log2(n/2));
      printf("Actual ratio: %g\tExpected: %g", ratio, expected);
    }
    putchar('\n');
  }
  printf("qsort on arrays of double\n");
  n = MIN;
  for (size_t i = 0; i < 5; ++i, n <<= 1) {
    for (size_t j = 0; j < n; ++j) {
      A[j] = cos(rand());
    }
    printf("n = %zu:\n", n);
    struct timespec start = { 0 };
    struct timespec stop = { 0 };
    timespec_get(&start, TIME_UTC);
    qsort(A, n, sizeof A[0], compare_dbl);
    timespec_get(&stop, TIME_UTC);
    elapsed[i] = timespec_diff(start, stop);
    
    printf("Time elapsed: %lld.%.9lds\t",
           (long long)elapsed[i].tv_sec, elapsed[i].tv_nsec);
    if (i > 0) {
      double num = (elapsed[i].tv_sec + elapsed[i].tv_nsec*1E-9);
      double denom = (elapsed[i-1].tv_sec + elapsed[i-1].tv_nsec*1E-9);
      double ratio = num / denom;
      double expected = (n*log2(n)) / ((n/2)*log2(n/2));
      printf("Actual ratio: %g\tExpected: %g", ratio, expected);
    }
    putchar('\n');
  }
 
  return EXIT_SUCCESS;
}

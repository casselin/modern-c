/* Challenge 10 Performance comparison of sorting algorithms
 *
 * Can you compare the time efficiency of your sorting programs
 * (challenge 1) with data sizes of several orders of magnitude?
 *
 * Be careful to check that you have some randomness in the
 * creation of the data and that the data size does not exceed
 * the available memory of your computer.
 *
 * For both algorithms, you should roughly observe a behaviour
 * that is proportional to NlogN, where N is the number of elements
 * that are sorted.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

void merge(double A[static 1], int l, int m, int h) {
  int lenl = m - l;
  int lenr = h - m;

  double L[lenl];
  double R[lenr];

  for (int i = 0; i < lenl; ++i) {
    L[i] = A[l + i];
  }

  for (int i = 0; i < lenr; ++i) {
    R[i] = A[m + i];
  }

  int i = 0;
  int j = 0;

  for (int k = l; k < h; ++k) {
    if (i < lenl && (j >= lenr || L[i] <= R[j])) {
      A[k] = L[i];
	  ++i;
	} else {
      A[k] = R[j];
	  ++j;
	}
  }

  return;
}

void mergesplit(double A[static 1], int l, int h)  {
  if (h - l <= 1) {
    return;
  }

  int mid = (h + l) / 2;

  mergesplit(A, l, mid);
  mergesplit(A, mid, h);

  merge(A, l, mid, h);

  return;

}

void mergesort(double A[static 1], int len) { 
  mergesplit(A, 0, len);

  return;
}

void swapc(char S[static 1], int n, int m) {
  char temp = S[n];
  S[n] = S[m];
  S[m] = temp;
}

void quicksort(char S[static 1], int l, int h) {
  if (h - l <= 1) {
    return;
  }

  int pivot = (h + l) / 2;
  int last = l;

  swapc(S, pivot, l);

  for (int i = l; i < h; ++i) {
    if (S[i] < S[l]) {
      ++last;
	  swapc(S, i, last);
	}
  }

  swapc(S, l, last);

  quicksort(S, l, last);
  quicksort(S, last+1, h);
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

/* Through trial and error, can create a double array of size 10^6 */
int main(void) {
  srand(time(NULL));
  printf("Merge sort on arrays of double\n");
  size_t n = 1;
  for (int i = 1; i < 6; ++i) {
    n *= 10;
    double a[n];
    for (size_t j = 0; j < n; ++j) {
      a[j] = cos(rand());
    }
    printf("n = %zu:\n", n);
    struct timespec start = { 0 };
    struct timespec stop = { 0 };
    timespec_get(&start, TIME_UTC);
    mergesort(a, n);
    timespec_get(&stop, TIME_UTC);
    struct timespec elapsed = timespec_diff(start, stop);

    printf("Time elapsed: %lld.%.9lds\n",
           (long long)elapsed.tv_sec, elapsed.tv_nsec);
  }
  printf("Quick sort on arrays of char\n");
  n = 1;
  for (int i = 1; i < 6; ++i) {
    n *= 10;
    char s[n];
    for (size_t j = 0; j < n; ++j) {
      s[j] = 'a' + (rand() % 26);
    }
    printf("n = %zu:\n", n);
    struct timespec start = { 0 };
    struct timespec stop = { 0 };
    timespec_get(&start, TIME_UTC);
    quicksort(s, 0, n);
    timespec_get(&stop, TIME_UTC);
    struct timespec elapsed = timespec_diff(start, stop);

    printf("Time elapsed: %lld.%.9lds\n",
           (long long)elapsed.tv_sec, elapsed.tv_nsec);
  }
  return EXIT_SUCCESS;
}

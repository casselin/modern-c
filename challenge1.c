/**
 ** Challenge 1 Sequential sorting algorithms
 **
 ** Can you do
 **
 **   1. A merge sort (with recursion)
 **   2. A quick sort (with recursion)
 **
 ** on arrays with sort keys such as double or strings to your liking?
 ** Nothing is gained if you don't know whether your programs are correct.
 ** Therefore, can you provide a simple test routine that checks if the resulting
 ** array really is sorted?
 **
 ** This test routine should just scan once through the array and should be much,
 ** much faster than your sorting algorithms.
 **/

#include <stdio.h>

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

void testsortd(double A[static 1], int len) {
  for (int i = 0; i < len-1; ++i) {
    if (A[i] > A[i+1]) {
      printf("Not sorted: index %d=%f,\t next %d=%f\n",
	         i, A[i], i+1, A[i+1]);
	  return;
	}
  }
  printf("Array is sorted\n");
}

void testsorts(char S[static 1], int len) {
  for (int i = 0; i < len-1; ++i) {
    if (S[i] > S[i+1]) {
      printf("Not sorted: index %d=%c,\t next %d=%c\n",
	         i, S[i], i+1, S[i+1]);
	  return;
	}
  }
  printf("Array is sorted\n");
}

int main(void) {

  double A[20] = {
    0.80789,
    0.89780,
    0.14918,
    0.87102,
    0.06035,
    0.60252,
    0.43360,
    0.23112,
    0.84854,
    0.45964,
    0.50260,
    0.39855,
    0.63959,
    0.36508,
    0.82687,
    0.23205,
    0.48685,
    0.00650,
    0.88230,
    0.15413,
  };

  mergesort(A, 20);

  testsortd(A, 20);

  char S[] = "aljhfablskjfbbajkjhg";

  quicksort(S, 0, 20);

  testsorts(S, 20);

  return 0;
}

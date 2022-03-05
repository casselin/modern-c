/* Challenge 6 Linear Algebra
 * Some of the most important problems for which arrays are used stem
 * from linear algebra.
 * Can you write functions that do vector-to-vector or matrix-to-
 * vector products at this point?
 * What about GauB elimination or iterative algorithms for matrix
 * inversion?
 */

#include <tgmath.h>
#include <stdio.h>

#include <assert.h>

#define EPS 1E-12

void print_vector(const unsigned n, double V[static n]) {
  for (unsigned i = 0; i < n; i++) {
    printf(i < (n - 1) ? " %g," : " %g\n", V[i]);
  }
}

void print_matrix(const unsigned m, const unsigned n, double M[static m][n]) {
  for (unsigned i = 0; i < m; i++) {
    for (unsigned j = 0; j < n; j++) {
      printf(j < (n - 1) ? " %g," : " %g\n", M[i][j]);
    }
  }
}

double dot_product(const unsigned n, double U[static n], double V[static n]) {
  double sum = 0.0;

  for (unsigned i = 0; i < n; i++) {
    sum += U[i] * V[i];
  }

  return sum;
}

void test_dot_product() {
  double A[] = { 1.5, 2.3, 5.4, 7.2 };
  double B[] = { 1.2, 9.6, 4.5, 8.3 };
  
  printf("dot product:\n %g\n", dot_product((sizeof A)/(sizeof A[0]), A, B));
}

void matrix_vector_product(const unsigned m, const unsigned n,
    double M[static m][n], double U[static n], double V[static m]) {

  for (unsigned i = 0; i < m; i++) {
    double sum = 0.0;
    unsigned j = 0;

    for ( ; j < n; j++) {
      sum += M[i][j] * U[j];
    }

    V[i] = sum;
  }
}

void test_matrix_vector_product() {
  double A[] = { 1.5, 2.3, 5.4, 7.2 };
  double M[3][4] = { 
    { 1.0, 0.0, 0.0, 0.0 },
    { 0.0, 0.0, 0.0, 0.0 },
    { 0.0, 0.0, 0.0, 0.0 } 
  };

  double U[3] = { 0 };
  matrix_vector_product(3, 4, M, A, U);

  printf("M*A = ");
  print_vector((sizeof U) / (sizeof U[0]), U);
}

// cross product vectors must be in R^3
void cross_product(double U[static 3], double V[static 3], double W[static 3]) {
  double M[3][3] = { 0 };
  M[0][1] = -U[2];
  M[0][2] = U[1];
  M[1][0] = U[2];
  M[1][2] = -U[0];
  M[2][0] = -U[1];
  M[2][1] = U[0];

  matrix_vector_product(3, 3, M, V, W);
}

void test_cross_product() {
  double C[] = { 2.0, 3.0, 4.0 };
  double D[] = { 5.0, 6.0, 7.0 };
  double V[3] = { 0 };

  cross_product(C, D, V);
  printf("[2.0, 3.0, 4.0] x [5.0, 6.0, 7.0] =");
  print_vector(3, V);
}

// functions to perform gaussian elimination
// elementary row operations
// swap rows
void swap_rows(const unsigned r1, const unsigned r2, const unsigned n,
   double M[][n]) {
  for (unsigned i = 0; i < n; i++) {
    double tmp = M[r1][i];
    M[r1][i] = M[r2][i];
    M[r2][i] = tmp;
  }
}

void test_swap_rows() {
  double M[3][4] = {
    { 1.0, 0.0, 0.0, 1.0 },
    { 0.0, 1.0, 0.0, 2.0 },
    { 0.0, 0.0, 1.0, 3.0 }
  };

  swap_rows(0, 1, 4, M);
  swap_rows(1, 2, 4, M);
  printf("test swap_rows\n");  
  print_matrix(3, 4, M);
}

// multiply row by scalar
void mult_row(const unsigned r, const unsigned n, const double c,
    double M[static (r+1)][n]) {
  for (unsigned i = 0; i < n; i++) {
    M[r][i] *= c;
  }
}

void test_mult_row() {
  double M[2][2] = {
    { 1.0, 2.0 },
    { 3.0, 4.0 }
  };

  mult_row(0, 2, 2.0, M);
  mult_row(1, 2, 1 / 2.0, M);
  printf("test mult_row:\n");
  print_matrix(2, 2, M);
}
// add multiple of row to another row
void add_row(const unsigned r1, const unsigned r2, const unsigned n,
    const double c, double M[][n]) {
  for (unsigned i = 0; i < n; i++) {
    M[r2][i] += c * M[r1][i];
  }
}

void test_add_row() {
  double M[2][2] = {
    { 1.0, 2.0 },
    { 3.0, 4.0 }
  };

  add_row(0, 1, 2, -3.0, M);
  printf("test add_row:\n");
  print_matrix(2, 2, M);
}

int find_nonzero(unsigned r, const unsigned m,
    const unsigned c, const unsigned n, double M[static m][n]) {
  for (unsigned i = r; i < m; i++) {
    if (fabs(M[i][c]) >= EPS) {
      return i;
    }
  }
  return -1;
}

void test_find_nonzero() {
  double M[4][3] = {
    { 1.0, 0.0, 0.0 },
    { 0.0, 1.0, 0.0 },
    { 0.0, 0.0, 1.0 },
    { 0.0, 5.0, 0.0 }
  }; 

  assert(find_nonzero(0, 4, 0, 3, M) == 0);
  assert(find_nonzero(0, 4, 1, 3, M) == 1);
  assert(find_nonzero(0, 4, 2, 3, M) == 2);
  assert(find_nonzero(1, 4, 0, 3, M) == -1);
}

// reduced row echelon form
void reduced_row_form(const unsigned m, const unsigned n,
  double M[static m][n]) {
  unsigned pivot = 0;

  for (unsigned i = 0; i < m; i++) {
    if (pivot >= n) return;
    int r = find_nonzero(i, m, pivot, n, M);
    while (r < 0) {
      pivot++;
      if (pivot >= n) return;
      r = find_nonzero(m, i, pivot, n, M);
    }
    if (r != i) {
      swap_rows(r, i, n, M);
    }
    mult_row(i, n, 1.0 / M[i][pivot], M);
    for (unsigned j = 0; j < m; j++) {
      if (i != j) {
        add_row(i, j, n, -M[j][pivot], M);
      }
    }
    pivot++;
  }
} 

void test_reduced_row_form() {
  double M[3][4] = {
    { 1.0, 2.0, 3.0, 9.0 },
    { 2.0, -1.0, 1.0, 8.0 },
    { 3.0, 0.0, -1.0, 3.0 }
  };

  printf("M =\n");
  print_matrix(3, 4, M);
  printf("Reduced row echelon form of M:\n");
  reduced_row_form(3, 4, M);
  print_matrix(3, 4, M);
}

// matrix inversion using gauss-jordan elimination
void matrix_inverse(const unsigned n, double M[static n][n]) {
  double A[n][2*n];

  for (unsigned i = 0; i < n; i++) {
    for (unsigned j = 0; j < n; j++) {
      A[i][j] = M[i][j];
    }
  }

  for (unsigned i = 0; i < n; i++) {
    for (unsigned j = n; j < 2*n; j++) {
      if (j - n == i) A[i][j] = 1.0;
      else A[i][j] = 0.0;
    }
  }

  reduced_row_form(n, 2*n, A);

  for (unsigned i = 0; i < n; i++) {
    if (fabs(A[i][i] < EPS)) return;
  }

  for (unsigned i = 0; i < n; i++) {
    for (unsigned j = 0; j < n; j++) {
      M[i][j] = A[i][n + j];
    }
  }
}
  
void test_matrix_inverse() {
  double M[3][3] = {
    { -2.0, -2.0, 1.0 },
    { -4.0, -8.0, 4.0 },
    { -1.0, 5.0, 0.0 },
  };

  matrix_inverse(3, M);
  printf("Inverse Matrix:\n");
  print_matrix(3, 3, M);

}

int main(void) {
  test_dot_product();
  test_matrix_vector_product();
  test_cross_product();
  test_swap_rows();
  test_mult_row();
  test_add_row();
  test_find_nonzero();
  test_reduced_row_form();
  test_matrix_inverse();

  return 0;
}

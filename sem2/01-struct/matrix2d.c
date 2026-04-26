#include <stdlib.h> // malloc, free, rand(), srand(), RAND_MAX
#include <string.h> // memcpy
#include <math.h> // for fabs
#include <time.h> // for time() - seed for srand()
#include "matrix2d.h"

// memory methods
Matrix2D *matrix2d_construct_default() {
  Matrix2D* matrix = (Matrix2D*)malloc(sizeof(Matrix2D));
  matrix->rows = 0; matrix->cols = 0; matrix->data = NULL;
  return matrix;
}

Matrix2D *matrix2d_construct(Matrix2D* matrix, size_t n, size_t m, const float* arr) {
  if (!arr) {
    matrix->rows = n;
    matrix->cols = m;
    matrix->arr = NULL;
    return matrix;
  }
  if (matrix->rows * matrix->cols != n*m) {
    free(matrix->data);
    matrix->data = (float*)malloc(n*m * sizeof(float));
  }
  memcpy(matrix->data, arr, sizeof(float)*n*m);
  matrix->rows = n;
  matrix->cols = m;
  return matrix;
}

Matrix2D *matrix2d_construct_copy(Matrix2D* dst, Matrix2D* src) {
  if (src->data == NULL) dst->data = NULL;
  if (dst->rows * dst->cols != src->rows * src->cols) {
    free(dst->data);
    dst->data = (float*)malloc(src->rows * src->cols * sizeof(float));
  }
  memcpy(dst->data, src->data, sizeof(float) * src->rows * src->cols);
  dst->rows = src->rows;
  dst->cols = src->cols;
  return dst;
}

void matrix2d_destruct(Matrix2D* matrix) {
  matrix->rows = 0;
  matrix->cols = 0;
  free(matrix->data);
  matrix->data=NULL;
  free(matrix);
  matrix = NULL;
}

// matrix2d methods
// grade "Satisfactory"
int matrix2d_cmp(Matrix2D* m1, Matrix2D* m2) {
  if (m1->rows * m1->cols != m2->rows * m2->cols)
    return (m1->rows * m1->cols > m2->rows * m2->cols) ? 1 : -1;

  for (size_t i=0; i < m1->rows * m1->cols; i++) {
    if (fabs(*(m1->data+i) - *(m2->data+i)) > EPS) {
      if (*(m1->data+i) > *(m2->data+i)) return 1;
      else return -1;
    }
  }

  return 0;
}

wint_t matrix2d_wscanf(Matrix2D* matrix) {
  size_t n=0, m=0;
  wint_t code=0;
  
  wprintf(L"Enter the number of rows: ");
  while ((code = wscanf(L"%zu", &n))!=1) {
    if (code == WEOF) {
      wprintf(L"WEOF ERROR! ABORTING.\n");
      return WEOF;
    }
    while (getwchar() != L'\n');
    wprintf(L"Invalid input! Try again: ");
  }

  wprintf(L"Enter the number of cols: ");
  while ((code = wscanf(L"%zu", &m))!=1) {
    if (code == WEOF) {
      wprintf(L"WEOF ERROR! ABORTING.\n");
      return WEOF;
    }
    while (getwchar() != L'\n');
    wprintf(L"Invalid input! Try again: ");
  }

  float* arr = (float*)malloc(n*m * sizeof(float));
  wprintf(L"Enter %zu elements of matrix here:\n", n*m);
  for (size_t i=0; i<n*m; i++) {
    wprintf(L"Element [%zu][%zu]: ", i/m, i%m);
    while ((code = wscanf(L"%f", arr+i))!=1) {
      if (code == WEOF) {
        wprintf(L"WEOF ERROR! ABORTING.\n");
        return WEOF;
      }
      while (getwchar() != L'\n');
      wprintf(L"Invalid input! Try again: ");
    }
  }
  
  matrix2d_construct(matrix, n, m, arr);
  free(arr);
  matrix->rows = n;
  matrix->cols = m;

  return 0;
}

void matrix2d_wprintf(Matrix2D* matrix) {
  wprintf(L"rows: %zu\ncols: %zu\n", matrix->rows, matrix->cols);
  wprintf(L"{\n  ");
  for (size_t i=0; i<matrix->rows; i++) {
    putwchar(L'{');
    for (size_t j=0; j<matrix->cols-1; j++) {
      wprintf(L"%.2f, ", *(matrix->data+(i*matrix->cols)+j));
    }
    wprintf(L"%.2f}", *(matrix->data+(i*matrix->cols)+(matrix->cols-1)));
    if (i < matrix->rows - 1) wprintf(L",\n  ");
  }
  wprintf(L"\n}\n");
}

void matrix2d_increment(Matrix2D* matrix) {
  for (size_t i=0; i < matrix->rows * matrix->cols; i++) (*(matrix->data+i))++;
}

void matrix2d_decrement(Matrix2D* matrix) {
  for (size_t i=0; i < matrix->rows * matrix->cols; i++) (*(matrix->data+i))--;
}

void matrix2d_setter(Matrix2D* matrix, size_t row, size_t col, float value) {
  *(matrix->data + row * matrix->cols + col) = value;
}

void matrix2d_random(Matrix2D* matrix, float min, float max) {
  srand(time(NULL)); // make seed for random number
  for (size_t i=0; i < matrix->rows * matrix->cols; i++) 
       *(matrix->data+i) = min + (float)rand() / RAND_MAX * (max-min);
}

// grade "Good"
Matrix2D* matrix2d_get_row(Matrix2D* matrix, size_t row) {
  float *arr = (float*)malloc(matrix->cols * sizeof(float));
  for (size_t i=0; i < matrix->cols; i++) {
    *(arr+i) = *(matrix->data + matrix->cols * row + i);
  }

  Matrix2D* m = matrix2d_construct_default();
  matrix2d_construct(m, 1, matrix->cols, arr);

  free(arr);
  return m;
}

Matrix2D* matrix2d_get_col(Matrix2D* matrix, size_t col) {
  float *arr = (float*)malloc(matrix->rows * sizeof(float));
  for (size_t i=0; i < matrix->rows; i++) {
    *(arr+i) = *(matrix->data + i * matrix->cols + col);
  }

  Matrix2D* m = matrix2d_construct_default();
  matrix2d_construct(m, matrix->rows, 1, arr);

  free(arr);
  return m;
}

Matrix2D* matrix2d_transpose(Matrix2D* matrix) {
  float *sub = (float*)malloc(matrix->rows * matrix->cols * sizeof(float));

  for (size_t i=0; i<matrix->rows; i++) {
    for (size_t j=0; j<matrix->cols; j++) {
      *(sub + j*matrix->rows + i) = *(matrix->data + i*matrix->cols + j);
    }
  }

  Matrix2D *result = matrix2d_construct_default();
  result = matrix2d_construct(result, matrix->cols, matrix->rows, sub);

  free(sub);
  return result;
}

float matrix2d_determinant(Matrix2D* matrix) {
  if (matrix->rows != matrix->cols) return 0;
  return _determinant(matrix->data, matrix->rows);
}

float _determinant(const float *data, size_t n) {
  if (n == 1) return *(data);
  if (n == 2) return *(data) * *(data+3) - *(data+1) * *(data+2);

  float det = 0, sign=1;
  float* minor = NULL;
  
  for (size_t j=0; j < n; j++) {
    minor = matrix2d_minor(data, n, 0, j);
    if (!minor) return 0;

    sign = (j%2) ? -1 : 1;
    det += sign * *(data+j) * _determinant(minor, n-1);
    free(minor);
    }
  
  return det;
}

float* matrix2d_minor(const float* data, size_t n, size_t row, size_t col) {
  if (n==1) return NULL;
  float *minor = (float*)malloc((n-1)*(n-1) * sizeof(float));
  if (!minor) return NULL;
  size_t idx=0;
  for (size_t i=0; i < n; i++) {
    if (i == row) continue;
    for (size_t j=0; j < n; j++) {
      if (j == col) continue;
      *(minor + idx++) = *(data + i*n + j);
    }
  }
  return minor;
}

// grade "Excellent"
Matrix2D* matrix2d_scale(Matrix2D* matrix, const float num) {
  for (size_t i=0; i<matrix->cols * matrix->rows; i++) *(matrix->data + i) *= num;
  return matrix;
}

Matrix2D* matrix2d_cofactor(Matrix2D* matrix) {
  if (matrix->rows != matrix->cols) return NULL;
  size_t n = matrix->rows;
  
  if (n == 1) {
    float *cof = (float*)malloc(sizeof(float));
    *cof = 1;
    Matrix2D *result = matrix2d_construct_default();
    result = matrix2d_construct(result, 1, 1, cof);
    free(cof);
    return result;
  }

  float *cof = (float*)malloc(n*n * sizeof(float));
  if (!cof) return NULL;

  float det_minor=0, sign=1;
  float* minor = NULL;
  
  for (size_t i=0; i < n; i++) {
    for (size_t j=0; j < n; j++) {
      minor = matrix2d_minor(matrix->data, n, i, j);
      if (!minor) {
        free(cof);
        return NULL;
      }
      sign = ((i+j)%2) ? -1 : 1;
      det_minor = _determinant(minor, n-1);
      free(minor);
      *(cof + i*n+j) = sign * det_minor;
    }
  }

  Matrix2D *result = matrix2d_construct_default();
  result = matrix2d_construct(result, n, n, cof);
  free(cof);
  return result;
}

Matrix2D* matrix2d_inverse(Matrix2D* matrix) {
  if (matrix->rows != matrix->cols) return NULL;
  float det = matrix2d_determinant(matrix);
  if (fabs(det) < EPS) return NULL;
  
  Matrix2D *cof = matrix2d_cofactor(matrix);
  if (!cof) return NULL;

  Matrix2D *adj = matrix2d_transpose(cof);
  matrix2d_destruct(cof);
  if (!adj) return NULL;

  for (size_t i=0; i<adj->rows * adj->cols; i++) *(adj->data + i) /= det;
  
  return adj;
}

// some cmp funcs
_Bool matrix2d_eql(Matrix2D* m1, Matrix2D* m2) {
  return matrix2d_cmp(m1, m2) == 0;
}

_Bool matrix2d_not_eql(Matrix2D* m1, Matrix2D* m2) {
  return matrix2d_cmp(m1, m2) != 0;
}

_Bool matrix2d_grtr(Matrix2D* m1, Matrix2D* m2) {
  return matrix2d_cmp(m1, m2) > 0;
}

_Bool matrix2d_grtr_or_eql(Matrix2D* m1, Matrix2D* m2) {
  return matrix2d_cmp(m1, m2) >= 0;
}

_Bool matrix2d_less(Matrix2D* m1, Matrix2D* m2) {
  return matrix2d_cmp(m1, m2) < 0;
}

_Bool matrix2d_less_or_eql(Matrix2D* m1, Matrix2D* m2) {
  return matrix2d_cmp(m1, m2) <= 0;
}

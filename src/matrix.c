#include "matlib/matrix.h"
#include "matlib/random.h"
#include <stdlib.h>
#include <string.h>

#define ALIGNMENT_VAL 32

Matrix *matlib_matrix_create(size_t rows, size_t cols) {
  if (rows == 0 || cols == 0) {
    return NULL;
  }

  Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
  if (mat == NULL) {
    return NULL;
  }

  mat->rows = rows;
  mat->cols = cols;

  size_t raw_size = rows * cols * sizeof(float);
  size_t rem = raw_size % ALIGNMENT_VAL;
  size_t aligned_size = rem == 0 ? raw_size : raw_size + (ALIGNMENT_VAL - rem);

  mat->data = (float *)aligned_alloc(ALIGNMENT_VAL, aligned_size);
  if (mat->data == NULL) {
    free(mat);
    return NULL;
  }

  return mat;
}

void matlib_matrix_free(Matrix *mat) {
  if (mat != NULL) {
    free(mat->data);
    free(mat);
  }
}

Matrix *matlib_matrix_zeros(size_t rows, size_t cols) {
  Matrix *mat = matlib_matrix_create(rows, cols);
  if (mat == NULL) {
    return NULL;
  }
  memset(mat->data, 0, rows * cols * sizeof(float));

  return mat;
}

Matrix *matlib_matrix_ones(size_t rows, size_t cols) {
  Matrix *mat = matlib_matrix_create(rows, cols);
  if (mat == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < rows * cols; i++) {
    mat->data[i] = 1.0f;
  }

  return mat;
}

Matrix *matlib_matrix_rand(size_t rows, size_t cols, float min, float max) {
  Matrix *mat = matlib_matrix_create(rows, cols);
  if (mat == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < rows * cols; i++) {
    mat->data[i] = matlib_random_uniform_float(min, max);
  }
  return mat;
}

float matlib_matrix_get(const Matrix *mat, size_t row, size_t col) {
  size_t i = (mat->cols * row) + col;
  return mat->data[i];
}

void matlib_matrix_set(Matrix *mat, size_t row, size_t col, float val) {
  size_t i = (mat->cols * row) + col;
  mat->data[i] = val;
}

Matrix *matlib_matrix_transpose(const Matrix *mat) {
  Matrix *t_mat = matlib_matrix_create(mat->cols, mat->rows);
  if (t_mat == NULL) {
    return NULL;
  }

  for (size_t r = 0; r < mat->rows; r++) {
    for (size_t c = 0; c < mat->cols; c++) {
      t_mat->data[(mat->rows * c) + r] = mat->data[(mat->cols * r) + c];
    }
  }

  return t_mat;
}

Matrix *matlib_matrix_add(const Matrix *a, const Matrix *b) {
  Matrix *c = matlib_matrix_create(a->rows, a->cols);
  if (c == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < a->rows * a->cols; i++) {
    c->data[i] = a->data[i] + b->data[i];
  }
  return c;
}

Matrix *matlib_matrix_sub(const Matrix *a, const Matrix *b) {
  Matrix *c = matlib_matrix_create(a->rows, a->cols);
  if (c == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < a->rows * a->cols; i++) {
    c->data[i] = a->data[i] - b->data[i];
  }
  return c;
}

Matrix *matlib_matrix_mul_elementwise(const Matrix *a, const Matrix *b) {
  Matrix *c = matlib_matrix_create(a->rows, a->cols);
  if (c == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < a->rows * a->cols; i++) {
    c->data[i] = a->data[i] * b->data[i];
  }
  return c;
}

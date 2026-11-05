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

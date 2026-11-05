#ifndef MATLIB_MATRIX_H
#define MATLIB_MATRIX_H

#include <stddef.h>

typedef struct {
  size_t rows;
  size_t cols;
  float *data;
} Matrix;

Matrix *matlib_matrix_create(size_t rows, size_t cols);
void matlib_matrix_free(Matrix *mat);
Matrix *matlib_matrix_zeros(size_t rows, size_t cols);
Matrix *matlib_matrix_ones(size_t rows, size_t cols);
Matrix *matlib_matrix_rand(size_t rows, size_t cols, float min, float max);

#endif
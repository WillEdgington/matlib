#include "matlib/matrix.h"
#include "matlib/random.h"
#include <stdlib.h>
#include <string.h>

#define ALIGNMENT_VAL 32
#define TILE_SIZE 8

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

  const size_t rows = mat->rows;
  const size_t cols = mat->cols;
  const float *restrict src = mat->data;
  float *restrict dst = t_mat->data;

  for (size_t r_start = 0; r_start < rows; r_start += TILE_SIZE) {
    const size_t r_end =
        (r_start + TILE_SIZE < rows) ? r_start + TILE_SIZE : rows;

    for (size_t c_start = 0; c_start < cols; c_start += TILE_SIZE) {
      const size_t c_end =
          (c_start + TILE_SIZE < cols) ? c_start + TILE_SIZE : cols;

      for (size_t r = r_start; r < r_end; r++) {
        const float *src_row = src + (r * cols);
        for (size_t c = c_start; c < c_end; c++) {
          dst[(c * rows) + r] = src_row[c];
        }
      }
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

Matrix *matlib_matrix_add_scalar(const Matrix *mat, float scalar) {
  Matrix *res = matlib_matrix_create(mat->rows, mat->cols);
  if (res == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < mat->rows * mat->cols; i++) {
    res->data[i] = mat->data[i] + scalar;
  }
  return res;
}

Matrix *matlib_matrix_sub_scalar(const Matrix *mat, float scalar) {
  Matrix *res = matlib_matrix_create(mat->rows, mat->cols);
  if (res == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < mat->rows * mat->cols; i++) {
    res->data[i] = mat->data[i] - scalar;
  }
  return res;
}

Matrix *matlib_matrix_scale(const Matrix *mat, float scalar) {
  Matrix *res = matlib_matrix_create(mat->rows, mat->cols);
  if (res == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < mat->rows * mat->cols; i++) {
    res->data[i] = mat->data[i] * scalar;
  }
  return res;
}

Matrix *matlib_matrix_mul(const Matrix *a, const Matrix *b) {
  Matrix *c = matlib_matrix_zeros(a->rows, b->cols);
  if (c == NULL) {
    return NULL;
  }

  size_t m = a->rows;
  size_t k = a->cols;
  size_t n = b->cols;

  for (size_t i = 0; i < m; i++) {
    for (size_t p = 0; p < k; p++) {
      float a_val = a->data[(i * k) + p];
      for (size_t j = 0; j < n; j++) {
        c->data[(i * n) + j] += a_val * b->data[(p * n) + j];
      }
    }
  }

  return c;
}

float matlib_matrix_sum(const Matrix *mat) {
  const size_t n = mat->rows * mat->cols;
  const float *restrict data = mat->data;

  // unroll to eight
  float acc0 = 0.0f;
  float acc1 = 0.0f;
  float acc2 = 0.0f;
  float acc3 = 0.0f;
  float acc4 = 0.0f;
  float acc5 = 0.0f;
  float acc6 = 0.0f;
  float acc7 = 0.0f;

  size_t i = 0;
  for (; i + 8 <= n; i += 8) {
    acc0 += data[i];
    acc1 += data[i + 1];
    acc2 += data[i + 2];
    acc3 += data[i + 3];
    acc4 += data[i + 4];
    acc5 += data[i + 5];
    acc6 += data[i + 6];
    acc7 += data[i + 7];
  }

  for (; i < n; i++) {
    acc0 += data[i];
  }

  return (acc0 + acc1) + (acc2 + acc3) + (acc4 + acc5) + (acc6 + acc7);
}

float matlib_matrix_mean(const Matrix *mat) {
  return matlib_matrix_sum(mat) / (float)(mat->cols * mat->rows);
}

float matlib_matrix_max(const Matrix *mat) {
  const size_t n = mat->rows * mat->cols;
  const float *restrict data = mat->data;

  // unroll to eight
  float m0 = data[0];
  float m1 = data[0];
  float m2 = data[0];
  float m3 = data[0];
  float m4 = data[0];
  float m5 = data[0];
  float m6 = data[0];
  float m7 = data[0];

  size_t i = 0;
  for (; i + 8 <= n; i += 8) {
    const float v0 = data[i];
    const float v1 = data[i + 1];
    const float v2 = data[i + 2];
    const float v3 = data[i + 3];
    const float v4 = data[i + 4];
    const float v5 = data[i + 5];
    const float v6 = data[i + 6];
    const float v7 = data[i + 7];

    if (v0 > m0) {
      m0 = v0;
    }
    if (v1 > m1) {
      m1 = v1;
    }
    if (v2 > m2) {
      m2 = v2;
    }
    if (v3 > m3) {
      m3 = v3;
    }
    if (v4 > m4) {
      m4 = v4;
    }
    if (v5 > m5) {
      m5 = v5;
    }
    if (v6 > m6) {
      m6 = v6;
    }
    if (v7 > m7) {
      m7 = v7;
    }
  }

  float res = m0;
  if (m1 > res) {
    res = m1;
  }
  if (m2 > res) {
    res = m2;
  }
  if (m3 > res) {
    res = m3;
  }
  if (m4 > res) {
    res = m4;
  }
  if (m5 > res) {
    res = m5;
  }
  if (m6 > res) {
    res = m6;
  }
  if (m7 > res) {
    res = m7;
  }

  for (; i < n; i++) {
    if (data[i] > res) {
      res = data[i];
    }
  }

  return res;
}

float matlib_matrix_min(const Matrix *mat) {
  const size_t n = mat->rows * mat->cols;
  const float *restrict data = mat->data;

  // unroll to eight
  float m0 = data[0];
  float m1 = data[0];
  float m2 = data[0];
  float m3 = data[0];
  float m4 = data[0];
  float m5 = data[0];
  float m6 = data[0];
  float m7 = data[0];

  size_t i = 0;
  for (; i + 8 <= n; i += 8) {
    const float v0 = data[i];
    const float v1 = data[i + 1];
    const float v2 = data[i + 2];
    const float v3 = data[i + 3];
    const float v4 = data[i + 4];
    const float v5 = data[i + 5];
    const float v6 = data[i + 6];
    const float v7 = data[i + 7];

    if (v0 < m0) {
      m0 = v0;
    }
    if (v1 < m1) {
      m1 = v1;
    }
    if (v2 < m2) {
      m2 = v2;
    }
    if (v3 < m3) {
      m3 = v3;
    }
    if (v4 < m4) {
      m4 = v4;
    }
    if (v5 < m5) {
      m5 = v5;
    }
    if (v6 < m6) {
      m6 = v6;
    }
    if (v7 < m7) {
      m7 = v7;
    }
  }

  float res = m0;
  if (m1 < res) {
    res = m1;
  }
  if (m2 < res) {
    res = m2;
  }
  if (m3 < res) {
    res = m3;
  }
  if (m4 < res) {
    res = m4;
  }
  if (m5 < res) {
    res = m5;
  }
  if (m6 < res) {
    res = m6;
  }
  if (m7 < res) {
    res = m7;
  }

  for (; i < n; i++) {
    if (data[i] < res) {
      res = data[i];
    }
  }

  return res;
}

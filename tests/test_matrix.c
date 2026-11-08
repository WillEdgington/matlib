#include "matlib/matrix.h"
#include <ctest/ctest.h>
#include <stdint.h>

#define SUITE_NAME test_matrix
#define EPSILON_VAL 0.0001f

CTEST(SUITE_NAME, test_matrix_create_zero_dim) {
  Matrix *mat1 = matlib_matrix_create(0, 3);
  ASSERT_PTR_NULL(
      mat1, "Calling matlib_matrix_create() with rows == 0 should return NULL");

  Matrix *mat2 = matlib_matrix_create(5, 0);
  ASSERT_PTR_NULL(
      mat2, "Calling matlib_matrix_create() with cols == 0 should return NULL");

  Matrix *mat3 = matlib_matrix_create(0, 0);
  ASSERT_PTR_NULL(mat3, "Calling matlib_matrix_create() with rows == 0, cols "
                        "== 0 should return NULL");
}

CTEST(SUITE_NAME, test_matrix_create_valid_init) {
  Matrix *mat = matlib_matrix_create(3, 4);

  ASSERT_PTR_NOT_NULL(mat, "Calling matlib_matrix_create() with valid inputs "
                           "should return a pointer to a Matrix");
  ASSERT_INT_EQ(mat->rows, 3,
                "Returned matrix should have correct value for rows metadata");
  ASSERT_INT_EQ(mat->cols, 4,
                "Returned matrix should have correct value for cols metadata");
  ASSERT_PTR_NOT_NULL(mat->data, "Pointer to matrix data should not be NULL");
  ASSERT((uintptr_t)mat->data % 32 == 0,
         "Pointer to matrix data should be 32-byte alligned");

  matlib_matrix_free(mat);
}

CTEST(SUITE_NAME, test_matrix_zeros_zero_dim) {
  Matrix *mat1 = matlib_matrix_zeros(0, 4);
  ASSERT_PTR_NULL(
      mat1, "Calling matlib_matrix_zeros() with rows == 0 should return NULL");

  Matrix *mat2 = matlib_matrix_zeros(9, 0);
  ASSERT_PTR_NULL(
      mat2, "Calling matlib_matrix_zeros() with cols == 0 should return NULL");

  Matrix *mat3 = matlib_matrix_zeros(0, 0);
  ASSERT_PTR_NULL(mat3, "Calling matlib_matrix_zeros() with rows == 0, cols == "
                        "0 should return NULL");
}

CTEST(SUITE_NAME, test_matrix_zeros_valid_init) {
  Matrix *mat = matlib_matrix_zeros(4, 4);

  ASSERT_PTR_NOT_NULL(mat, "Calling matlib_matrix_zeros() with valid inputs "
                           "should return a pointer to a Matrix");
  ASSERT_INT_EQ(
      mat->rows, 4,
      "Zero-initialised matrix should have correct value for rows metadata");
  ASSERT_INT_EQ(
      mat->cols, 4,
      "Zero-initialised matrix should have correct value for cols metadata");
  ASSERT_PTR_NOT_NULL(
      mat->data, "Pointer to zero-initialised matrix data should not be NULL");
  ASSERT((uintptr_t)mat->data % 32 == 0,
         "Pointer to zero-initialised matrix data should be 32-byte alligned");

  matlib_matrix_free(mat);
}

CTEST(SUITE_NAME, test_matrix_zeros_all_items_zero) {
  Matrix *mat = matlib_matrix_zeros(1, 2);
  ASSERT_DOUBLE_EQ(mat->data[0], 0.0f, EPSILON_VAL, "");
  ASSERT_DOUBLE_EQ(mat->data[1], 0.0f, EPSILON_VAL, "");
  matlib_matrix_free(mat);
}

CTEST(SUITE_NAME, test_matrix_ones_zero_dim) {
  Matrix *mat1 = matlib_matrix_ones(0, 6);
  ASSERT_PTR_NULL(
      mat1, "Calling matlib_matrix_ones() with rows == 0 should return NULL");

  Matrix *mat2 = matlib_matrix_ones(11, 0);
  ASSERT_PTR_NULL(
      mat2, "Calling matlib_matrix_ones() with cols == 0 should return NULL");

  Matrix *mat3 = matlib_matrix_ones(0, 0);
  ASSERT_PTR_NULL(mat3, "Calling matlib_matrix_ones() with rows == 0, cols == "
                        "0 should return NULL");
}

CTEST(SUITE_NAME, test_matrix_ones_valid_init) {
  Matrix *mat = matlib_matrix_ones(5, 2);

  ASSERT_PTR_NOT_NULL(mat, "Calling matlib_matrix_ones() with valid inputs "
                           "should return a pointer to a Matrix");
  ASSERT_INT_EQ(
      mat->rows, 5,
      "One-initialised matrix should have correct value for rows metadata");
  ASSERT_INT_EQ(
      mat->cols, 2,
      "One-initialised matrix should have correct value for cols metadata");
  ASSERT_PTR_NOT_NULL(
      mat->data, "Pointer to one-initialised matrix data should not be NULL");
  ASSERT((uintptr_t)mat->data % 32 == 0,
         "Pointer to one-initialised matrix data should be 32-byte alligned");

  matlib_matrix_free(mat);
}

CTEST(SUITE_NAME, test_matrix_ones_all_items_one) {
  Matrix *mat = matlib_matrix_ones(2, 1);
  ASSERT_DOUBLE_EQ(mat->data[0], 1.0f, EPSILON_VAL, "");
  ASSERT_DOUBLE_EQ(mat->data[1], 1.0f, EPSILON_VAL, "");
  matlib_matrix_free(mat);
}

CTEST(SUITE_NAME, test_matrix_rand_zero_dim) {
  Matrix *mat1 = matlib_matrix_rand(0, 2, 0.0f, 10.0f);
  ASSERT_PTR_NULL(
      mat1, "Calling matlib_matrix_rand() with rows == 0 should return NULL");

  Matrix *mat2 = matlib_matrix_rand(8, 0, 1.0f, 5.0f);
  ASSERT_PTR_NULL(
      mat2, "Calling matlib_matrix_rand() with cols == 0 should return NULL");

  Matrix *mat3 = matlib_matrix_rand(0, 0, -4.0f, 8.0f);
  ASSERT_PTR_NULL(mat3, "Calling matlib_matrix_rand() with rows == 0, cols == "
                        "0 should return NULL");
}

CTEST(SUITE_NAME, test_matrix_rand_valid_init) {
  Matrix *mat = matlib_matrix_rand(3, 7, 0.0f, 9.0f);

  ASSERT_PTR_NOT_NULL(mat, "Calling matlib_matrix_rand() with valid inputs "
                           "should return a pointer to a Matrix");
  ASSERT_INT_EQ(mat->rows, 3,
                "Randomly-initialised matrix should have correct value for "
                "rows metadata");
  ASSERT_INT_EQ(mat->cols, 7,
                "Randomly-initialised matrix should have correct value for "
                "cols metadata");
  ASSERT_PTR_NOT_NULL(
      mat->data,
      "Pointer to randomly-initialised matrix data should not be NULL");
  ASSERT(
      (uintptr_t)mat->data % 32 == 0,
      "Pointer to randomly-initialised matrix data should be 32-byte alligned");

  matlib_matrix_free(mat);
}

CTEST(SUITE_NAME, test_matrix_rand_all_items_rand_in_bounds) {
  Matrix *mat = matlib_matrix_rand(1, 3, 1.5f, 2.2f);
  ASSERT(mat->data[0] >= 1.5f && mat->data[0] < 2.2f, "");
  ASSERT(mat->data[1] >= 1.5f && mat->data[1] < 2.2f, "");
  ASSERT(mat->data[2] >= 1.5f && mat->data[2] < 2.2f, "");
  ASSERT(mat->data[0] != mat->data[1] && mat->data[0] != mat->data[2] &&
             mat->data[1] != mat->data[2],
         "");
  matlib_matrix_free(mat);
}

CTEST(SUITE_NAME, test_matrix_get_in_bound) {
  Matrix *mat = matlib_matrix_zeros(7, 3);

  mat->data[(2 * 3) + 1] = 4.0f;
  ASSERT_DOUBLE_EQ(
      matlib_matrix_get(mat, 2, 1), 4.0f, EPSILON_VAL,
      "matlib_matrix_get() call should return the value at (row, col)");

  matlib_matrix_free(mat);
}

CTEST(SUITE_NAME, test_matrix_set_in_bound) {
  Matrix *mat = matlib_matrix_zeros(1, 3);

  matlib_matrix_set(mat, 0, 0, 5.0f);
  matlib_matrix_set(mat, 0, 2, 3.4f);

  ASSERT_DOUBLE_EQ(mat->data[0], 5.0f, EPSILON_VAL,
                   "matlib_matrix_set() call with (0, 0) and value 5.0f should "
                   "set element to 5.0f");
  ASSERT_DOUBLE_EQ(mat->data[2], 3.4f, EPSILON_VAL,
                   "matlib_matrix_set() call with (0, 2) and value 3.4f should "
                   "set element to 3.4f");
  ASSERT_DOUBLE_EQ(
      mat->data[1], 0.0f, EPSILON_VAL,
      "Unset (r, c) should remain unchanged after matlib_matrix_set() calls");

  matlib_matrix_free(mat);
}

CTEST(SUITE_NAME, test_matrix_transpose_square) {
  Matrix *mat = matlib_matrix_zeros(2, 2);

  matlib_matrix_set(mat, 0, 1, 1.0f);
  matlib_matrix_set(mat, 1, 0, 2.0f);
  matlib_matrix_set(mat, 1, 1, 3.0f);

  Matrix *t_mat = matlib_matrix_transpose(mat);
  ASSERT_PTR_NOT_NULL(
      t_mat,
      "Returned transposed matrix pointer should not be NULL (square type)");
  ASSERT_INT_EQ(t_mat->rows, mat->cols,
                "Transposed rows should match original columns");
  ASSERT_INT_EQ(t_mat->cols, mat->rows,
                "Transposed columns should match original rows");

  ASSERT_DOUBLE_EQ(t_mat->data[0], mat->data[0], EPSILON_VAL,
                   "Element at (0,0) should remain unchanged (square type)");
  ASSERT_DOUBLE_EQ(t_mat->data[1], mat->data[2], EPSILON_VAL,
                   "Element (0,1) in transposed should come from (1,0) in "
                   "original (square type)");
  ASSERT_DOUBLE_EQ(t_mat->data[2], mat->data[1], EPSILON_VAL,
                   "Element (1,0) in transposed should come from (0,1) in "
                   "original (square type)");
  ASSERT_DOUBLE_EQ(
      t_mat->data[3], mat->data[3], EPSILON_VAL,
      "Element at bottom-right should remain unchanged (square type)");

  matlib_matrix_free(mat);
  matlib_matrix_free(t_mat);
}

CTEST(SUITE_NAME, test_matrix_transpose_rectangle) {
  Matrix *mat = matlib_matrix_zeros(2, 3);

  matlib_matrix_set(mat, 0, 1, 1.0f);
  matlib_matrix_set(mat, 0, 2, 2.0f);
  matlib_matrix_set(mat, 1, 0, 3.0f);
  matlib_matrix_set(mat, 1, 1, 4.0f);
  matlib_matrix_set(mat, 1, 2, 5.0f);

  Matrix *t_mat = matlib_matrix_transpose(mat);
  ASSERT_PTR_NOT_NULL(
      t_mat,
      "Returned transposed matrix pointer should not be NULL (rectangle type)");
  ASSERT_INT_EQ(t_mat->rows, mat->cols,
                "Transposed rows should match original columns");
  ASSERT_INT_EQ(t_mat->cols, mat->rows,
                "Transposed columns should match original rows");

  ASSERT_DOUBLE_EQ(t_mat->data[0], mat->data[0], EPSILON_VAL,
                   "Top-left element (0,0) matches (rectangle type)");
  ASSERT_DOUBLE_EQ(
      t_mat->data[1], mat->data[3], EPSILON_VAL,
      "Transposed element at index 1 maps correctly (rectangle type)");
  ASSERT_DOUBLE_EQ(
      t_mat->data[2], mat->data[1], EPSILON_VAL,
      "Transposed element at index 2 maps correctly (rectangle type)");
  ASSERT_DOUBLE_EQ(
      t_mat->data[3], mat->data[4], EPSILON_VAL,
      "Transposed element at index 3 maps correctly (rectangle type)");
  ASSERT_DOUBLE_EQ(
      t_mat->data[4], mat->data[2], EPSILON_VAL,
      "Transposed element at index 4 maps correctly (rectangle type)");
  ASSERT_DOUBLE_EQ(t_mat->data[5], mat->data[5], EPSILON_VAL,
                   "Bottom-right element matches (rectangle type)");

  matlib_matrix_free(mat);
  matlib_matrix_free(t_mat);
}

CTEST(SUITE_NAME, test_matrix_add) {
  Matrix *a = matlib_matrix_rand(2, 2, -10.0f, 10.0f);
  Matrix *b = matlib_matrix_rand(2, 2, -10.0f, 10.0f);

  Matrix *c = matlib_matrix_add(a, b);

  ASSERT_PTR_NOT_NULL(
      c, "matlib_matrix_add() should return a valid Matrix pointer");
  ASSERT_INT_EQ(c->rows, a->rows, "Result matrix should match input rows");
  ASSERT_INT_EQ(c->cols, a->cols, "Result matrix should match input columns");

  ASSERT_DOUBLE_EQ(c->data[0], a->data[0] + b->data[0], EPSILON_VAL,
                   "Element at flat index 0 should equal sum of inputs");
  ASSERT_DOUBLE_EQ(c->data[1], a->data[1] + b->data[1], EPSILON_VAL,
                   "Element at flat index 1 should equal sum of inputs");
  ASSERT_DOUBLE_EQ(c->data[2], a->data[2] + b->data[2], EPSILON_VAL,
                   "Element at flat index 2 should equal sum of inputs");
  ASSERT_DOUBLE_EQ(c->data[3], a->data[3] + b->data[3], EPSILON_VAL,
                   "Element at flat index 3 should equal sum of inputs");

  matlib_matrix_free(a);
  matlib_matrix_free(b);
  matlib_matrix_free(c);
}

CTEST(SUITE_NAME, test_matrix_sub) {
  Matrix *a = matlib_matrix_rand(3, 1, -10.0f, 10.0f);
  Matrix *b = matlib_matrix_rand(3, 1, -10.0f, 10.0f);

  Matrix *c = matlib_matrix_sub(a, b);

  ASSERT_PTR_NOT_NULL(
      c, "matlib_matrix_sub() should return a valid Matrix pointer");
  ASSERT_INT_EQ(c->rows, a->rows, "Result matrix should match input rows");
  ASSERT_INT_EQ(c->cols, a->cols, "Result matrix should match input columns");

  ASSERT_DOUBLE_EQ(c->data[0], a->data[0] - b->data[0], EPSILON_VAL,
                   "Element at flat index 0 should equal difference of inputs");
  ASSERT_DOUBLE_EQ(c->data[1], a->data[1] - b->data[1], EPSILON_VAL,
                   "Element at flat index 1 should equal difference of inputs");
  ASSERT_DOUBLE_EQ(c->data[2], a->data[2] - b->data[2], EPSILON_VAL,
                   "Element at flat index 2 should equal difference of inputs");

  matlib_matrix_free(a);
  matlib_matrix_free(b);
  matlib_matrix_free(c);
}

CTEST(SUITE_NAME, test_matrix_mul_elementwise) {
  Matrix *a = matlib_matrix_rand(1, 3, -10.0f, 10.0f);
  Matrix *b = matlib_matrix_rand(1, 3, -10.0f, 10.0f);

  Matrix *c = matlib_matrix_mul_elementwise(a, b);

  ASSERT_PTR_NOT_NULL(
      c,
      "matlib_matrix_mul_elementwise() should return a valid Matrix pointer");
  ASSERT_INT_EQ(c->rows, a->rows, "Result matrix should match input rows");
  ASSERT_INT_EQ(c->cols, a->cols, "Result matrix should match input columns");

  ASSERT_DOUBLE_EQ(c->data[0], a->data[0] * b->data[0], EPSILON_VAL,
                   "Element at flat index 0 should equal product of inputs");
  ASSERT_DOUBLE_EQ(c->data[1], a->data[1] * b->data[1], EPSILON_VAL,
                   "Element at flat index 1 should equal product of inputs");
  ASSERT_DOUBLE_EQ(c->data[2], a->data[2] * b->data[2], EPSILON_VAL,
                   "Element at flat index 2 should equal product of inputs");

  matlib_matrix_free(a);
  matlib_matrix_free(b);
  matlib_matrix_free(c);
}

#include "matlib/tensor.h"
#include <ctest/ctest.h>
#include <stdint.h>

#define SUITE_NAME test_tensor
#define EPSILON_VAL 0.0001f

CTEST(SUITE_NAME, test_tensor_create_zero_dim) {
  size_t shape[] = {4, 0, 1};
  Tensor *t = matlib_tensor_create(3, shape);

  ASSERT_PTR_NULL(t, "matlib_tensor_create() should return a NULL pointer when "
                     "a dimension is zeros");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_create_0d) {
  size_t shape[1];
  Tensor *t = matlib_tensor_create(0, shape);

  ASSERT_PTR_NULL(
      t, "matlib_tensor_create() should return a NULL pointer when ndim is 0");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_create_3d_valid) {
  size_t shape[] = {2, 3, 4};
  Tensor *t = matlib_tensor_create(3, shape);

  ASSERT_PTR_NOT_NULL(
      t, "matlib_tensor_create() should return a valid Tensor pointer (3D)");
  ASSERT_INT_EQ(t->ndim, 3, "Tensor->ndim should match input ndim (3D)");
  ASSERT_INT_EQ(t->total_size, 24,
                "Tensor->total_size should be product of shape (3D)");
  ASSERT((uintptr_t)t->data % 32 == 0,
         "Pointer to tensor data should be 32-byte alligned");

  ASSERT_INT_EQ(t->shape[0], 2, "");
  ASSERT_INT_EQ(t->shape[1], 3, "");
  ASSERT_INT_EQ(t->shape[2], 4, "");

  ASSERT_INT_EQ(t->strides[0], 12, "");
  ASSERT_INT_EQ(t->strides[1], 4, "");
  ASSERT_INT_EQ(t->strides[2], 1, "");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_create_5d_valid) {
  size_t shape[] = {3, 4, 2, 5, 2};
  Tensor *t = matlib_tensor_create(5, shape);

  ASSERT_PTR_NOT_NULL(
      t, "matlib_tensor_create() should return a valid Tensor pointer (5D)");
  ASSERT_INT_EQ(t->ndim, 5, "Tensor->ndim should match input ndim (5D)");
  ASSERT_INT_EQ(t->total_size, 240,
                "Tensor->total_size should be product of shape (5D)");

  ASSERT_INT_EQ(t->shape[0], 3, "");
  ASSERT_INT_EQ(t->shape[1], 4, "");
  ASSERT_INT_EQ(t->shape[2], 2, "");
  ASSERT_INT_EQ(t->shape[3], 5, "");
  ASSERT_INT_EQ(t->shape[4], 2, "");

  ASSERT_INT_EQ(t->strides[0], 80, "");
  ASSERT_INT_EQ(t->strides[1], 20, "");
  ASSERT_INT_EQ(t->strides[2], 10, "");
  ASSERT_INT_EQ(t->strides[3], 2, "");
  ASSERT_INT_EQ(t->strides[4], 1, "");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_zeros_zero_dim) {
  size_t shape[] = {9, 4, 5, 0, 2};
  Tensor *t = matlib_tensor_zeros(5, shape);

  ASSERT_PTR_NULL(t, "matlib_tensor_zeros() should return a NULL pointer when "
                     "a dimension is zeros");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_zeros_0d) {
  size_t shape[1];
  Tensor *t = matlib_tensor_zeros(0, shape);

  ASSERT_PTR_NULL(
      t, "matlib_tensor_zeros() should return a NULL pointer when ndim is 0");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_zeros_valid) {
  size_t shape[] = {2, 6, 2, 3};
  Tensor *t = matlib_tensor_zeros(4, shape);

  ASSERT_PTR_NOT_NULL(
      t, "matlib_tensor_zeros() should return a valid Tensor pointer");
  ASSERT_INT_EQ(t->ndim, 4,
                "Zero-initialised tensor ndim value should match input ndim");
  ASSERT_INT_EQ(
      t->total_size, 72,
      "Zero-initialised tensor total_size value should be product of shape");

  ASSERT_INT_EQ(t->shape[0], 2, "");
  ASSERT_INT_EQ(t->shape[1], 6, "");
  ASSERT_INT_EQ(t->shape[2], 2, "");
  ASSERT_INT_EQ(t->shape[3], 3, "");

  ASSERT_INT_EQ(t->strides[0], 36, "");
  ASSERT_INT_EQ(t->strides[1], 6, "");
  ASSERT_INT_EQ(t->strides[2], 3, "");
  ASSERT_INT_EQ(t->strides[3], 1, "");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_zeros_all_items_zero) {
  size_t shape[] = {2, 1};
  Tensor *t = matlib_tensor_zeros(2, shape);
  ASSERT_DOUBLE_EQ(t->data[0], 0.0f, EPSILON_VAL, "");
  ASSERT_DOUBLE_EQ(t->data[1], 0.0f, EPSILON_VAL, "");
  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_ones_zero_dim) {
  size_t shape[] = {0, 7, 3};
  Tensor *t = matlib_tensor_ones(3, shape);

  ASSERT_PTR_NULL(t, "matlib_tensor_ones() should return a NULL pointer when a "
                     "dimension is zeros");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_ones_0d) {
  size_t shape[1];
  Tensor *t = matlib_tensor_ones(0, shape);

  ASSERT_PTR_NULL(
      t, "matlib_tensor_ones() should return a NULL pointer when ndim is 0");
}

CTEST(SUITE_NAME, test_tensor_ones_valid) {
  size_t shape[] = {2, 6};
  Tensor *t = matlib_tensor_ones(2, shape);

  ASSERT_PTR_NOT_NULL(
      t, "matlib_tensor_ones() should return a valid Tensor pointer");
  ASSERT_INT_EQ(t->ndim, 2,
                "One-initialised tensor ndim value should match input ndim");
  ASSERT_INT_EQ(
      t->total_size, 12,
      "One-initialised tensor total_size value should be product of shape");

  ASSERT_INT_EQ(t->shape[0], 2, "");
  ASSERT_INT_EQ(t->shape[1], 6, "");

  ASSERT_INT_EQ(t->strides[0], 6, "");
  ASSERT_INT_EQ(t->strides[1], 1, "");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_ones_all_items_one) {
  size_t shape[] = {2};
  Tensor *t = matlib_tensor_ones(1, shape);
  ASSERT_DOUBLE_EQ(t->data[0], 1.0f, EPSILON_VAL, "");
  ASSERT_DOUBLE_EQ(t->data[1], 1.0f, EPSILON_VAL, "");
  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_rand_zero_dim) {
  size_t shape[] = {3, 0, 0, 5};
  Tensor *t = matlib_tensor_rand(4, shape, 5.0f, 6.0f);

  ASSERT_PTR_NULL(t, "matlib_tensor_rand() should return a NULL pointer when a "
                     "dimension is zeros");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_rand_0d) {
  size_t shape[1];
  Tensor *t = matlib_tensor_rand(0, shape, 0.0f, 10.0f);

  ASSERT_PTR_NULL(
      t, "matlib_tensor_rand() should return a NULL pointer when ndim is 0");
}

CTEST(SUITE_NAME, test_tensor_rand_valid) {
  size_t shape[] = {4, 3, 7};
  Tensor *t = matlib_tensor_rand(3, shape, -1.0f, 5.0f);

  ASSERT_PTR_NOT_NULL(
      t, "matlib_tensor_rand() should return a valid Tensor pointer");
  ASSERT_INT_EQ(
      t->ndim, 3,
      "Randomly-initialised tensor ndim value should match input ndim");
  ASSERT_INT_EQ(t->total_size, 84,
                "Randomly-initialised tensor total_size value should be "
                "product of shape");

  ASSERT_INT_EQ(t->shape[0], 4, "");
  ASSERT_INT_EQ(t->shape[1], 3, "");
  ASSERT_INT_EQ(t->shape[2], 7, "");

  ASSERT_INT_EQ(t->strides[0], 21, "");
  ASSERT_INT_EQ(t->strides[1], 7, "");
  ASSERT_INT_EQ(t->strides[2], 1, "");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_rand_all_items_rand_in_bounds) {
  size_t shape[] = {1, 3, 1};
  Tensor *t = matlib_tensor_rand(3, shape, 2.5f, 3.1f);

  ASSERT(t->data[0] >= 2.5f && t->data[0] < 3.4f, "");
  ASSERT(t->data[1] >= 2.5f && t->data[1] < 3.4f, "");
  ASSERT(t->data[2] >= 2.5f && t->data[2] < 3.4f, "");
  ASSERT(t->data[0] != t->data[1] && t->data[0] != t->data[2] &&
             t->data[1] != t->data[2],
         "");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_get_3d_in_bound) {
  size_t shape[] = {2, 2, 3};
  Tensor *t = matlib_tensor_zeros(3, shape);

  size_t coords[] = {1, 0, 2};
  // t->strides = {6, 3, 1}
  t->data[(6 * 1) + (3 * 0) + 2] = 5.6f;
  ASSERT_DOUBLE_EQ(
      matlib_tensor_get(t, coords), 5.6f, EPSILON_VAL,
      "matlib_tensor_get() call should return the value at coords (3D)");
  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_get_4d_in_bound) {
  size_t shape[] = {5, 2, 4, 4};
  Tensor *t = matlib_tensor_zeros(4, shape);

  size_t coords[] = {3, 1, 2, 1};
  // t->strides = {32, 16, 4, 1}
  t->data[(32 * 3) + (16 * 1) + (4 * 2) + 1] = 5.6f;
  ASSERT_DOUBLE_EQ(
      matlib_tensor_get(t, coords), 5.6f, EPSILON_VAL,
      "matlib_tensor_get() call should return the value at coords (4D)");
  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_set_2d_in_bound) {
  size_t shape[] = {3, 2};
  Tensor *t = matlib_tensor_zeros(2, shape);

  size_t coords1[] = {2, 0};
  matlib_tensor_set(t, coords1, 3.2f);

  size_t coords2[] = {1, 1};
  matlib_tensor_set(t, coords2, -6.3f);

  // t->strides = {2, 1}
  ASSERT_DOUBLE_EQ(t->data[2 * 2], 3.2f, EPSILON_VAL, "");
  ASSERT_DOUBLE_EQ(t->data[(2 * 1) + 1], -6.3f, EPSILON_VAL, "");
  ASSERT_DOUBLE_EQ(t->data[2 * 1], 0.0f, EPSILON_VAL, "");
  ASSERT_DOUBLE_EQ(t->data[(2 * 2) + 1], 0.0f, EPSILON_VAL, "");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_reshape_valid) {
  size_t shape[] = {2, 3, 4};
  Tensor *t = matlib_tensor_zeros(3, shape);

  t->data[23] = 42.5f;

  size_t new_shape[] = {4, 6};
  Tensor *reshaped = matlib_tensor_reshape(t, 2, new_shape);

  ASSERT_PTR_NOT_NULL(reshaped, "matlib_tensor_reshape() should return a valid "
                                "Tensor pointer for valid total_size");
  ASSERT_INT_EQ(reshaped->ndim, 2, "Reshaped ndim should match new_ndim");
  ASSERT_INT_EQ(reshaped->total_size, 24, "Total size must remain invariant");

  ASSERT_INT_EQ(reshaped->shape[0], 4, "");
  ASSERT_INT_EQ(reshaped->shape[1], 6, "");

  ASSERT_DOUBLE_EQ(reshaped->data[23], 42.5f, EPSILON_VAL,
                   "Data values should map correctly after reshape");

  matlib_tensor_free(t);
  matlib_tensor_free(reshaped);
}

CTEST(SUITE_NAME, test_tensor_reshape_invalid_size) {
  size_t shape[] = {2, 3, 4}; // total_size = 24
  Tensor *t = matlib_tensor_zeros(3, shape);

  size_t invalid_shape[] = {5, 5}; // total_size = 25
  Tensor *reshaped = matlib_tensor_reshape(t, 2, invalid_shape);

  ASSERT_PTR_NULL(
      reshaped,
      "matlib_tensor_reshape() should return NULL when total_size mismatches");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_reshape_invalid_ndim) {
  size_t shape[] = {2, 2};
  Tensor *t = matlib_tensor_zeros(2, shape);

  Tensor *reshaped = matlib_tensor_reshape(t, 0, shape);
  ASSERT_PTR_NULL(
      reshaped,
      "matlib_tensor_reshape() should return NULL when new_ndim is 0");

  matlib_tensor_free(t);
}

CTEST(SUITE_NAME, test_tensor_permute_valid_3d) {
  size_t shape[] = {2, 3, 4};
  Tensor *t = matlib_tensor_zeros(3, shape);

  size_t coords_orig[] = {1, 2, 3};
  matlib_tensor_set(t, coords_orig, 99.0f);

  size_t axes[] = {2, 0, 1};
  Tensor *permuted = matlib_tensor_permute(t, axes);

  ASSERT_PTR_NOT_NULL(
      permuted, "matlib_tensor_permute() should return a valid tensor pointer");
  ASSERT_INT_EQ(permuted->ndim, 3, "Permuted ndim should match original ndim");

  // shape permutation = {4, 2, 3}
  ASSERT_INT_EQ(permuted->shape[0], 4, "");
  ASSERT_INT_EQ(permuted->shape[1], 2, "");
  ASSERT_INT_EQ(permuted->shape[2], 3, "");

  // stride permutation = {6, 3, 1}
  ASSERT_INT_EQ(permuted->strides[0], 6, "");
  ASSERT_INT_EQ(permuted->strides[1], 3, "");
  ASSERT_INT_EQ(permuted->strides[2], 1, "");

  size_t coords_perm[] = {3, 1, 2};
  ASSERT_DOUBLE_EQ(
      matlib_tensor_get(permuted, coords_perm), 99.0f, EPSILON_VAL,
      "Permuted tensor should access correct data via remapped coordinates");

  matlib_tensor_free(t);
  matlib_tensor_free(permuted);
}

CTEST(SUITE_NAME, test_tensor_add_2d) {
  size_t shape[] = {2, 2};
  Tensor *a = matlib_tensor_rand(2, shape, -10.0f, 10.0f);
  Tensor *b = matlib_tensor_rand(2, shape, -10.0f, 10.0f);

  Tensor *c = matlib_tensor_add(a, b);

  ASSERT_PTR_NOT_NULL(
      c, "matlib_tensor_add() should return a valid Tensor pointer");
  ASSERT_INT_EQ(c->ndim, 2, "Result tensor should match input ndims");
  ASSERT_INT_EQ(c->total_size, 4,
                "Result tensor should match input total_sizes");

  ASSERT_INT_EQ(c->shape[0], 2, "");
  ASSERT_INT_EQ(c->shape[1], 2, "");

  ASSERT_INT_EQ(c->strides[0], 2, "");
  ASSERT_INT_EQ(c->strides[1], 1, "");

  ASSERT_DOUBLE_EQ(c->data[0], a->data[0] + b->data[0], EPSILON_VAL,
                   "Element at flat index 0 should equal sum of inputs");
  ASSERT_DOUBLE_EQ(c->data[1], a->data[1] + b->data[1], EPSILON_VAL,
                   "Element at flat index 1 should equal sum of inputs");
  ASSERT_DOUBLE_EQ(c->data[2], a->data[2] + b->data[2], EPSILON_VAL,
                   "Element at flat index 2 should equal sum of inputs");
  ASSERT_DOUBLE_EQ(c->data[3], a->data[3] + b->data[3], EPSILON_VAL,
                   "Element at flat index 3 should equal sum of inputs");

  matlib_tensor_free(a);
  matlib_tensor_free(b);
  matlib_tensor_free(c);
}

CTEST(SUITE_NAME, test_tensor_sub_3d) {
  size_t shape[] = {1, 3, 1};
  Tensor *a = matlib_tensor_rand(3, shape, -10.0f, 10.0f);
  Tensor *b = matlib_tensor_rand(3, shape, -10.0f, 10.0f);

  Tensor *c = matlib_tensor_sub(a, b);

  ASSERT_PTR_NOT_NULL(
      c, "matlib_tensor_sub() should return a valid Tensor pointer");
  ASSERT_INT_EQ(c->ndim, 3, "Result tensor should match input ndims");
  ASSERT_INT_EQ(c->total_size, 3,
                "Result tensor should match input total_sizes");

  ASSERT_INT_EQ(c->shape[0], 1, "");
  ASSERT_INT_EQ(c->shape[1], 3, "");
  ASSERT_INT_EQ(c->shape[2], 1, "");

  ASSERT_INT_EQ(c->strides[0], 3, "");
  ASSERT_INT_EQ(c->strides[1], 1, "");
  ASSERT_INT_EQ(c->strides[2], 1, "");

  ASSERT_DOUBLE_EQ(c->data[0], a->data[0] - b->data[0], EPSILON_VAL,
                   "Element at flat index 0 should equal difference of inputs");
  ASSERT_DOUBLE_EQ(c->data[1], a->data[1] - b->data[1], EPSILON_VAL,
                   "Element at flat index 1 should equal difference of inputs");
  ASSERT_DOUBLE_EQ(c->data[2], a->data[2] - b->data[2], EPSILON_VAL,
                   "Element at flat index 2 should equal difference of inputs");

  matlib_tensor_free(a);
  matlib_tensor_free(b);
  matlib_tensor_free(c);
}

CTEST(SUITE_NAME, test_tensor_mul_elementwise_4d) {
  size_t shape[] = {1, 2, 1, 2};
  Tensor *a = matlib_tensor_rand(4, shape, -10.0f, 10.0f);
  Tensor *b = matlib_tensor_rand(4, shape, -10.0f, 10.0f);

  Tensor *c = matlib_tensor_mul_elementwise(a, b);

  ASSERT_PTR_NOT_NULL(
      c,
      "matlib_tensor_mul_elementwise() should return a valid Tensor pointer");
  ASSERT_INT_EQ(c->ndim, 4, "Result tensor should match input ndims");
  ASSERT_INT_EQ(c->total_size, 4,
                "Result tensor should match input total_sizes");

  ASSERT_INT_EQ(c->shape[0], 1, "");
  ASSERT_INT_EQ(c->shape[1], 2, "");
  ASSERT_INT_EQ(c->shape[2], 1, "");
  ASSERT_INT_EQ(c->shape[3], 2, "");

  ASSERT_INT_EQ(c->strides[0], 4, "");
  ASSERT_INT_EQ(c->strides[1], 2, "");
  ASSERT_INT_EQ(c->strides[2], 2, "");
  ASSERT_INT_EQ(c->strides[3], 1, "");

  ASSERT_DOUBLE_EQ(c->data[0], a->data[0] * b->data[0], EPSILON_VAL,
                   "Element at flat index 0 should equal product of inputs");
  ASSERT_DOUBLE_EQ(c->data[1], a->data[1] * b->data[1], EPSILON_VAL,
                   "Element at flat index 1 should equal product of inputs");
  ASSERT_DOUBLE_EQ(c->data[2], a->data[2] * b->data[2], EPSILON_VAL,
                   "Element at flat index 2 should equal product of inputs");
  ASSERT_DOUBLE_EQ(c->data[3], a->data[3] * b->data[3], EPSILON_VAL,
                   "Element at flat index 3 should equal product of inputs");

  matlib_tensor_free(a);
  matlib_tensor_free(b);
  matlib_tensor_free(c);
}

#include "matlib/tensor.h"
#include "matlib/random.h"
#include <stdlib.h>
#include <string.h>

#define ALIGNMENT_VAL 32

Tensor *matlib_tensor_create(size_t ndim, const size_t *shape) {
  if (ndim == 0) {
    return NULL;
  }

  Tensor *tensor = (Tensor *)malloc(sizeof(Tensor));
  if (tensor == NULL) {
    return NULL;
  }

  tensor->ndim = ndim;

  tensor->shape = (size_t *)malloc(ndim * sizeof(size_t));
  tensor->strides = (size_t *)malloc(ndim * sizeof(size_t));

  if (tensor->shape == NULL || tensor->strides == NULL) {
    free(tensor->shape);
    free(tensor->strides);
    free(tensor);
    return NULL;
  }

  // copy shape and calculate total_size (product of shape)
  size_t current_size = 1;
  for (size_t i = 0; i < ndim; i++) {
    tensor->shape[i] = shape[i];
    current_size *= shape[i];
  }
  tensor->total_size = current_size;

  if (current_size == 0) {
    free(tensor->shape);
    free(tensor->strides);
    free(tensor);
    return NULL;
  }

  // calculate strides
  // strides[i] = strides[i + 1] * shape[i + 1], where strides[ndim - 1] = 1
  if (ndim > 0) {
    tensor->strides[ndim - 1] = 1;
    for (int i = (int)ndim - 2; i >= 0; i--) {
      tensor->strides[i] = tensor->strides[i + 1] * tensor->shape[i + 1];
    }
  }

  // allocate 32-byte aligned data
  size_t alloc_size = tensor->total_size * sizeof(float);
  size_t rem = alloc_size % ALIGNMENT_VAL;
  alloc_size += rem == 0 ? 0 : ALIGNMENT_VAL - rem;

  tensor->data = (float *)aligned_alloc(ALIGNMENT_VAL, alloc_size);
  if (tensor->data == NULL) {
    free(tensor->shape);
    free(tensor->strides);
    free(tensor);
    return NULL;
  }

  return tensor;
}

void matlib_tensor_free(Tensor *tensor) {
  if (tensor != NULL) {
    free(tensor->shape);
    free(tensor->strides);
    free(tensor->data);
    free(tensor);
  }
}

Tensor *matlib_tensor_zeros(size_t ndim, const size_t *shape) {
  Tensor *tensor = matlib_tensor_create(ndim, shape);
  if (tensor == NULL) {
    return NULL;
  }

  memset(tensor->data, 0, tensor->total_size * sizeof(float));

  return tensor;
}

Tensor *matlib_tensor_ones(size_t ndim, const size_t *shape) {
  Tensor *tensor = matlib_tensor_create(ndim, shape);
  if (tensor == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < tensor->total_size; i++) {
    tensor->data[i] = 1.0f;
  }

  return tensor;
}

Tensor *matlib_tensor_rand(size_t ndim, const size_t *shape, float min,
                           float max) {
  Tensor *tensor = matlib_tensor_create(ndim, shape);
  if (tensor == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < tensor->total_size; i++) {
    tensor->data[i] = matlib_random_uniform_float(min, max);
  }

  return tensor;
}

static size_t flatten_coords(const Tensor *tensor, const size_t *coords) {
  size_t flat_idx = coords[0] * tensor->strides[0];

  for (size_t i = 1; i < tensor->ndim; i++) {
    flat_idx += coords[i] * tensor->strides[i];
  }
  return flat_idx;
}

float matlib_tensor_get(const Tensor *tensor, const size_t *coords) {
  size_t flat_idx = flatten_coords(tensor, coords);
  return tensor->data[flat_idx];
}

void matlib_tensor_set(Tensor *tensor, const size_t *coords, float val) {
  size_t flat_idx = flatten_coords(tensor, coords);
  tensor->data[flat_idx] = val;
}

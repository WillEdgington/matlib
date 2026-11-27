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

Tensor *matlib_tensor_reshape(const Tensor *tensor, size_t new_ndim,
                              const size_t *new_shape) {
  Tensor *new = matlib_tensor_create(new_ndim, new_shape);
  if (new == NULL || new->total_size != tensor->total_size) {
    matlib_tensor_free(new);
    return NULL;
  }

  memcpy(new->data, tensor->data, new->total_size * sizeof(float));
  return new;
}

Tensor *matlib_tensor_permute(const Tensor *tensor, const size_t *axes) {
  size_t new_shape[tensor->ndim];
  for (size_t i = 0; i < tensor->ndim; i++) {
    new_shape[i] = tensor->shape[axes[i]];
  }

  Tensor *new = matlib_tensor_create(tensor->ndim, new_shape);
  if (new == NULL) {
    return NULL;
  }

  size_t coords[tensor->ndim];
  for (size_t i = 0; i < new->total_size; i++) {
    size_t temp = i;
    for (int dim = (int)tensor->ndim - 1; dim >= 0; dim--) {
      coords[dim] = temp % new->shape[dim];
      temp /= new->shape[dim];
    }

    size_t orig_flat_idx = 0;
    for (size_t dim = 0; dim < tensor->ndim; dim++) {
      orig_flat_idx += coords[dim] * tensor->strides[axes[dim]];
    }

    new->data[i] = tensor->data[orig_flat_idx];
  }

  return new;
}

Tensor *matlib_tensor_add(const Tensor *a, const Tensor *b) {
  Tensor *c = matlib_tensor_create(a->ndim, a->shape);
  if (c == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < c->total_size; i++) {
    c->data[i] = a->data[i] + b->data[i];
  }

  return c;
}

Tensor *matlib_tensor_sub(const Tensor *a, const Tensor *b) {
  Tensor *c = matlib_tensor_create(a->ndim, a->shape);
  if (c == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < c->total_size; i++) {
    c->data[i] = a->data[i] - b->data[i];
  }

  return c;
}

Tensor *matlib_tensor_mul_elementwise(const Tensor *a, const Tensor *b) {
  Tensor *c = matlib_tensor_create(a->ndim, a->shape);
  if (c == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < c->total_size; i++) {
    c->data[i] = a->data[i] * b->data[i];
  }

  return c;
}

Tensor *matlib_tensor_add_scalar(const Tensor *tensor, float scalar) {
  Tensor *res = matlib_tensor_create(tensor->ndim, tensor->shape);
  if (res == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < res->total_size; i++) {
    res->data[i] = tensor->data[i] + scalar;
  }

  return res;
}

Tensor *matlib_tensor_sub_scalar(const Tensor *tensor, float scalar) {
  Tensor *res = matlib_tensor_create(tensor->ndim, tensor->shape);
  if (res == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < res->total_size; i++) {
    res->data[i] = tensor->data[i] - scalar;
  }

  return res;
}

Tensor *matlib_tensor_scale(const Tensor *tensor, float scalar) {
  Tensor *res = matlib_tensor_create(tensor->ndim, tensor->shape);
  if (res == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < res->total_size; i++) {
    res->data[i] = tensor->data[i] * scalar;
  }

  return res;
}

float matlib_tensor_sum(const Tensor *tensor) {
  const size_t n = tensor->total_size;
  const float *restrict data = tensor->data;

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

float matlib_tensor_mean(const Tensor *tensor) {
  return matlib_tensor_sum(tensor) / (float)tensor->total_size;
}

float matlib_tensor_max(const Tensor *tensor) {
  const size_t n = tensor->total_size;
  const float *restrict data = tensor->data;

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

float matlib_tensor_min(const Tensor *tensor) {
  const size_t n = tensor->total_size;
  const float *restrict data = tensor->data;

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

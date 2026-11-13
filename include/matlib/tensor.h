#ifndef MATLIB_TENSOR_H
#define MATLIB_TENSOR_H

#include <stddef.h>

typedef struct {
  size_t ndim;
  size_t *shape;
  size_t *strides;
  float *data;
  size_t total_size;
} Tensor;

Tensor *matlib_tensor_create(size_t ndim, const size_t *shape);
void matlib_tensor_free(Tensor *tensor);

Tensor *matlib_tensor_zeros(size_t ndim, const size_t *shape);
Tensor *matlib_tensor_ones(size_t ndim, const size_t *shape);
Tensor *matlib_tensor_rand(size_t ndim, const size_t *shape, float min,
                           float max);

float matlib_tensor_get(const Tensor *tensor, const size_t *coords);
void matlib_tensor_set(Tensor *tensor, const size_t *coords, float val);

Tensor *matlib_tensor_reshape(const Tensor *tensor, size_t new_ndim,
                              const size_t *new_shape);
Tensor *matlib_tensor_permute(const Tensor *tensor, const size_t *axes);

Tensor *matlib_tensor_add(const Tensor *a, const Tensor *b);
Tensor *matlib_tensor_sub(const Tensor *a, const Tensor *b);
Tensor *matlib_tensor_mul_elementwise(const Tensor *a, const Tensor *b);

Tensor *matlib_tensor_add_scalar(const Tensor *tensor, float scalar);
Tensor *matlib_tensor_sub_scalar(const Tensor *tensor, float scalar);
Tensor *matlib_tensor_scale(const Tensor *tensor, float scalar);

#endif

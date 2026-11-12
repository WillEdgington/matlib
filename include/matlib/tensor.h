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

#endif

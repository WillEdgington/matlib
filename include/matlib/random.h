#ifndef MATLIB_RANDOM_H
#define MATLIB_RANDOM_H

#include <stdint.h>

void matlib_seed(uint64_t seed);
float matlib_random_uniform_float(float min, float max);

#endif
#include "matlib/random.h"
#include <stdint.h>

static _Thread_local uint64_t rng_state = 0x853c49e6748fea9bULL;

void matlib_seed(uint64_t seed) {
  rng_state = seed ? seed : 0x853c49e6748fea9bULL;
}

// xorshift64* generator
static uint64_t next_random_u64(void) {
  uint64_t x = rng_state;
  x ^= x >> 12;
  x ^= x << 25;
  x ^= x >> 27;
  rng_state = x;
  return x * 0x2545F4914F6CDD1DULL;
}

float matlib_random_uniform_float(float min, float max) {
  uint64_t rand_u64 = next_random_u64();
  double norm = (double)rand_u64 / (double)UINT64_MAX;
  return min + (float)(norm * (double)(max - min));
}

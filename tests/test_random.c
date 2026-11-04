#include <ctest/ctest.h>
#include <matlib/random.h>

#define SUITE_NAME test_random

CTEST(SUITE_NAME, test_random_uniform_float_varies) {
  matlib_seed(11);
  float val1 = matlib_random_uniform_float(0.0f, 10.0f);
  float val2 = matlib_random_uniform_float(0.0f, 10.0f);

  ASSERT(val1 != val2, "Consecutive random draws should not be identical");
}

CTEST(SUITE_NAME, test_random_uniform_float_reproducible) {
  matlib_seed(42);
  float val1 = matlib_random_uniform_float(0.0f, 10.0f);

  matlib_seed(42);
  float val2 = matlib_random_uniform_float(0.0f, 10.0f);

  ASSERT_DOUBLE_EQ(
      val1, val2, 0.0001f,
      "Draw after seeding with same seed should produce identical numbers");
}

CTEST(SUITE_NAME, test_random_uniform_float_bounds) {
  matlib_seed(2);

  // [3.0f, 5.0f)
  float val1 = matlib_random_uniform_float(3.0f, 5.0f);
  ASSERT(val1 >= 3.0f && val1 < 5.0f,
         "Drawn number should be in bounds for range [3.0f, 5.0f)");

  // [-8.4f, 3.0f)
  float val2 = matlib_random_uniform_float(-8.4f, 3.0f);
  ASSERT(val2 >= -8.4f && val2 < 3.0f,
         "Drawn number should be in bounds for range [-8.4f, 3.0f)");
}

CTEST(SUITE_NAME, test_random_uniform_float_degenerate_bounds) {
  matlib_seed(99);
  float val = matlib_random_uniform_float(7.5f, 7.5f);
  ASSERT_DOUBLE_EQ(val, 7.5f, 0.0001f,
                   "Drawn number where min == max should equal min (and max)");
}
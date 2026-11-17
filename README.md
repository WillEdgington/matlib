# matlib

A high performance linear algebra and tensor library for the C programming language (currently no GPU acceleration).

Built as part of a [self-directed C curriculum](https://github.com/WillEdgington/c-curriculum).

---

## Contents

- [**Build**](#build)
- [**Pseudo-Random Number Generator**](#pseudo-random-number-generator)
- [**Matrix**](#matrix)
- [**Tensor**](#tensor)
- [**Contact**](#contact)

---

## Build

Requires `gcc` and `make`. Once the repository is cloned from the remote, you can run these build commands at the `matlib` directory location:

**runtime builds:**
```sh
make setup_deps # Clone the dependency repos
make            # Compiles the libmatlib.a library
```

**dev builds:**
```sh
make test       # Compiles and runs the test suite (./tests)
make debug      # Builds with debug symbols and sanitisers
```

**clean:**
```sh
make clean_deps # Cleans compiled binaries, .o, .a, and .d files (including compiled dependencies)
make clean      # Cleans compiled binaries, .o, and .d files (just for matlib)
```

---

## Pseudo-Random Number Generator

Uses a thread-local `xorshift64*` generator (`_Thread_local uint64_t` state) with a fixed default seed. Passing `0` to `matlib_seed` falls back to the default non-zero state.

- `void matlib_seed(uint64_t seed)`: Sets the thread-local PRNG state.
- `float matlib_random_uniform_float(float min, float max)`: Returns a uniform random float in the range `[min, max]`.

**Example:**
```C
#include <matlib/random.h>

// wrapped in some sort of method
matlib_seed(42);
float x = matlib_random_uniform_float(-1.0f, 1.0f);
```

---

## `Matrix`

Stores 2D single-precision floats in a contiguous, row-major, 32-byte aligned buffer.

- **Contract-based:** Performs no runtime bounds or dimension checks; callers must pass valid indices and compatible shapes.
- **Eager allocation:** All transformation and arithmetic operations return newly allocated matrices.

**Lifecycle**
- `Matrix *matlib_matrix_create(size_t rows, size_t cols)`: Allocates an uninitialised `Matrix` (`NULL` if a dimension is `0`).
- `void matlib_matrix_free(Matrix *mat)`: Frees `Matrix` metadata and aligned data.
- `Matrix *matlib_matrix_zeros(size_t rows, size_t cols)`: Creates a zero-filled `Matrix`.
- `Matrix *matlib_matrix_ones(size_t rows, size_t cols)`: Creates a one-filled `Matrix`.
- `Matrix *matlib_matrix_rand(size_t rows, size_t cols, float min, float max)`: Creates a `Matrix` with uniform random floats in range `[min, max]`.

**Access & Structure**
- `float matlib_matrix_get(const Matrix *mat, size_t row, size_t col)`: Reads element at (`row`, `col`).
- `void matlib_matrix_set(Matrix *mat, size_t row, size_t col, float val)`: Writes `val` to (`row`, `col`).
- `Matrix *matlib_matrix_transpose(const Matrix *mat)`: Returns a transposed copy.

**Arithmetic & Linear Algebra**
- `Matrix *matlib_matrix_add(const Matrix *a, const Matrix *b)`: Element-wise `a + b`.
- `Matrix *matlib_matrix_sub(const Matrix *a, const Matrix *b)`: Element-wise `a - b`.
- `Matrix *matlib_matrix_mul_elementwise(const Matrix *a, const Matrix *b)`: Hadamard product `a * b`.
- `Matrix *matlib_matrix_add_scalar(const Matrix *mat, float scalar)`: Adds `scalar` to every element.
- `Matrix *matlib_matrix_sub_scalar(const Matrix *mat, float scalar)`: Subtracts `scalar` from every element.
- `Matrix *matlib_matrix_scale(const Matrix *mat, float scalar)`: Multiplies every element by `scalar`.
- `Matrix *matlib_matrix_mul(const Matrix *a, const Matrix *b)`: Cache-friendly matrix multiplication.

**Reductions**
- `float matlib_matrix_sum(const Matrix *mat)`: Sum of all elements.
- `float matlib_matrix_mean(const Matrix *mat)`: Arithmetic mean of all elements.
- `float matlib_matrix_max(const Matrix *mat)`: Maximum element value.
- `float matlib_matrix_min(const Matrix *mat)`: Minimum element value.

**Example:**
```C
#include <matlib/matrix.h>

// wrapped in some sort of method
Matrix *a = matlib_matrix_rand(2, 3, 0.0f, 1.0f);
Matrix *b = matlib_matrix_ones(3, 2);
Matrix *c = matlib_matrix_mul(a, b);

float total = matlib_matrix_sum(c);

// free matrices after use
matlib_matrix_free(a);
matlib_matrix_free(b);
matlib_matrix_free(c);
```

---

## `Tensor`

Stores $N$-dimensional single-precision floats in a contiguous, row-major, 32-byte aligned buffer indexed via dynamic `shape` and `strides` arrays.

* **Contract-based:** Performs no runtime bounds or shape checks (except verifying matching `total_size` in `reshape`); callers must pass valid coordinates and compatible shapes.
* **Eager allocation:** All transformation and arithmetic operations return newly allocated, contiguous tensors.

**Lifecycle**
- `Tensor *matlib_tensor_create(size_t ndim, const size_t *shape)`: Allocates an uninitialised `Tensor` (`NULL` if `ndim == 0` or any dimension is `0`).
- `void matlib_tensor_free(Tensor *tensor)`: Frees shape, strides, aligned data, and `Tensor` metadata.
- `Tensor *matlib_tensor_zeros(size_t ndim, const size_t *shape)`: Creates a zero-filled `Tensor`.
- `Tensor *matlib_tensor_ones(size_t ndim, const size_t *shape)`: Creates a one-filled `Tensor`.
- `Tensor *matlib_tensor_rand(size_t ndim, const size_t *shape, float min, float max)`: Creates a `Tensor` with uniform random floats in `[min, max]`.

**Access & Structure**
- `float matlib_tensor_get(const Tensor *tensor, const size_t *coords)`: Reads element at `coords`.
- `void matlib_tensor_set(Tensor *tensor, const size_t *coords, float val)`: Writes `val` to `coords`.
- `Tensor *matlib_tensor_reshape(const Tensor *tensor, size_t new_ndim, const size_t *new_shape)`: Returns a reshaped copy (`NULL` on `total_size` mismatch).
- `Tensor *matlib_tensor_permute(const Tensor *tensor, const size_t *axes)`: Returns a copy with dimensions reordered per `axes`.

**Arithmetic**
- `Tensor *matlib_tensor_add(const Tensor *a, const Tensor *b)`: Element-wise `a + b`.
- `Tensor *matlib_tensor_sub(const Tensor *a, const Tensor *b)`: Element-wise `a - b`.
- `Tensor *matlib_tensor_mul_elementwise(const Tensor *a, const Tensor *b)`: Hadamard product `a * b`.
- `Tensor *matlib_tensor_add_scalar(const Tensor *tensor, float scalar)`: Adds `scalar` to every element.
- `Tensor *matlib_tensor_sub_scalar(const Tensor *tensor, float scalar)`: Subtracts `scalar` from every element.
- `Tensor *matlib_tensor_scale(const Tensor *tensor, float scalar)`: Multiplies every element by `scalar`.

**Reductions**
- `float matlib_tensor_sum(const Tensor *tensor)`: Sum of all elements.
- `float matlib_tensor_mean(const Tensor *tensor)`: Arithmetic mean of all elements.
- `float matlib_tensor_max(const Tensor *tensor)`: Maximum element value.
- `float matlib_tensor_min(const Tensor *tensor)`: Minimum element value.

**Example:**
```C
#include <matlib/tensor.h>

// wrapped in some sort of method
size_t shape[] = {2, 3, 4};
size_t axes[] = {2, 0, 1};

Tensor *t = matlib_tensor_rand(3, shape, 0.0f, 1.0f);
Tensor *p = matlib_tensor_permute(t, axes);

float avg = matlib_tensor_mean(p);

// free tensors after use
matlib_tensor_free(t);
matlib_tensor_free(p);
```

---

## Contact

Created by [**WillEdgington**](https://github.com/WillEdgington)

📧 [**willedge037@gmail.com**](mailto:willedge037@gmail.com) &nbsp;|&nbsp; 🔗 [**LinkedIn**](https://www.linkedin.com/in/williamedgington/)

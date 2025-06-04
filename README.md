# Bit counting

This small program measures and compares various bit counting algorithms.
It includes classic bit tests as well as hardware POPCNT, SIMD shuffling,
prefix-sum masking, de Bruijn scans and large precomputed lookup tables.

## Algorithms implemented

* **Iterated** – shifts the number right one bit at a time while adding the low bit.
  This simple approach is easy to understand but slow.
* **Sparse** – clears the lowest set bit with `n &= n - 1` until the value becomes zero.
  Works well when few bits are set.
* **Dense** – complements the input and then uses the sparse method to count zero bits.
  More efficient when most bits are set.
* **Parallel** – uses arithmetic masks to add neighboring bits together in groups.
  Produces the count after a few mask-and-add steps.
* **Nifty** – performs similar masking but finishes with a modulo 255 operation.
  This avoids carrying bits across word boundaries.
* **Hakmem** – algorithm from MIT's HAKMEM that folds shifted versions of the number together.
  It shows clever bit twiddling to count quickly.
* **Builtin** – calls the compiler intrinsic `__builtin_popcount`.
  The compiler chooses the best implementation.
* **POPCNT** – directly invokes the CPU POPCNT instruction available on modern processors.
* **SIMD** – uses SSSE3 byte shuffle instructions to count bits across four bytes in parallel.
* **Prefix** – computes prefix sums of bit pairs.
  Multiplying by a constant accumulates the sums across the whole word.
* **deBruijn** – isolates each lowest set bit using a de Bruijn table and counts one per iteration.
* **Precomp 8** – looks up each byte in a 256‑entry table of precomputed values and sums them.
* **Precomp 16** – extends the lookup approach to 16‑bit values, requiring two table accesses.
* **Precomp 24** – uses a huge table for 24 bits.
  A byte lookup handles the remaining bits.

More information about different approaches can be found in the following links:

* [Overview of bit counting method](http://demiurg.com.ua/blog/2010/04/11/bit-counting) (in russian)
* [Fast bit counting routines](http://gurmeet.net/puzzles/fast-bit-counting-routines)
* [Bit hacks](http://graphics.stanford.edu/~seander/bithacks.html)

## Project layout

```
bitcount.cpp           # benchmark driver
bitcount_algorithms.h  # declarations of the algorithms
bitcount_algorithms.cpp# implementations of the algorithms
bitcount_test.cpp      # simple correctness tests
Makefile               # build rules for the benchmark and tests
```

## Building and running

The repository provides a simple `Makefile`.

To build the benchmark and run it:

```sh
make bench
./bitcount        # runs the benchmark
```

To run the tests:

```sh
make test         # builds and executes bitcount_test
```

Use `make clean` to remove the generated binaries and object files.

## Adding new algorithms

Implement a function returning the bit count and register it in
`bitcount_algorithms.cpp`:

```cpp
int my_bitcount(unsigned int n) { /* ... */ }
REGISTER_BITCOUNT_ALGO("MyAlgo", my_bitcount, nullptr);
```

If the algorithm requires setup, provide an initialization function and pass it
as the third argument. Registered algorithms are automatically tested and run in
the benchmark.

Below are example timings obtained on a 2.26 GHz Core 2 Duo (gcc 4.2.1, `-O3`):

```
        8-bit lookup table calculation took 1.29 ms
        16-bit lookup table calculation took 2.08 ms

        ---> Iterated methods
        Iterated        4.545 sec   22.000 Mcps
        Sparse          3.467 sec   28.846 Mcps
        Dense           3.598 sec   27.790 Mcps
        ---> Parallel methods
        Parallel        1.227 sec   81.490 Mcps
        Nifty           1.299 sec   76.976 Mcps
        Hakmem          1.274 sec   78.504 Mcps
        ---> Table lookup methods
        Precomp 8       0.805 sec  124.176 Mcps
        Precomp 16      0.765 sec  130.661 Mcps
```


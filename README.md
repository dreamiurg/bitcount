# Bit Counting

`bitcount` times a variety of bit counting algorithms and shows how fast each one runs on your machine.

## Why?

Once upon a time I wanted to experiment with different bit counting algoithms to see how much of a time difference they have. 

## Project Layout

```
bitcount.cpp            # benchmark driver
bitcount_algorithms.h   # algorithm declarations
bitcount_algorithms.cpp # algorithm implementations
bitcount_test.cpp       # correctness tests
Makefile                # build rules for benchmarks and tests
```

## Quick Start

Build the benchmark and run it:

```sh
make bench
./bitcount
```

Run the tests:

```sh
make test
```

Clean generated binaries and object files:

```sh
make clean
```

## Implemented Algorithms

- **Iterated** — Shifts the input right and sums the low bit each time; easiest to follow, slowest.
- **Sparse** — Clears the lowest set bit with `n &= n - 1`; best when few bits are set.
- **Dense** — Complements the input and applies the sparse method to count zero bits.
- **Parallel** — Applies arithmetic masks to add neighboring bits in stages until the total emerges.
- **Nifty** — Performs similar masking but finishes with a modulo 255 to avoid cross-byte carries.
- **Hakmem** — Adapts the MIT HAKMEM trick that folds shifted versions together for speed.
- **Builtin** — Relies on the compiler intrinsic `__builtin_popcount` for a tuned implementation.
- **POPCNT** — Invokes the CPU POPCNT instruction directly when the hardware supports it.
- **SIMD** — Uses SSSE3 byte shuffles to count bits across four bytes in parallel.
- **Prefix** — Builds prefix sums of bit pairs and multiplies to accumulate partial counts.
- **deBruijn** — Isolates successive set bits with a de Bruijn table, counting one per iteration.
- **Precomp 8** — Looks up eight-bit chunks in a 256-entry table and sums the results.
- **Precomp 16** — Extends the lookup approach to 16-bit values with two table accesses.
- **Precomp 24** — Trades setup time for a large table covering 24-bit values plus a byte lookup.

Each algorithm is registered in `bitcount_algorithms.cpp`, which keeps the benchmark and tests in
sync automatically.

## Adding a New Algorithm

Implement a function that returns the population count and register it alongside the existing
entries:

```cpp
int my_bitcount(unsigned int n) {
  // ... custom implementation ...
}
REGISTER_BITCOUNT_ALGO("MyAlgo", my_bitcount, nullptr);
```

If your algorithm requires initialization, provide a setup function and pass it as the third
argument. Registered algorithms run in both the benchmark and the test suite without additional
changes.

## Benchmark Snapshot

Timings below were captured on an Apple M1 running macOS 15.5 (Darwin 24.5.0).
The machine had 8 cores, 8 GB RAM, and used Apple clang 17.0.0 with `-O2`:

```
Iterated        1.792 sec   55.798 Mcps
Sparse          0.279 sec  358.102 Mcps
Dense           1.886 sec   53.036 Mcps
Parallel        0.301 sec  332.237 Mcps
Nifty           0.289 sec  345.588 Mcps
Hakmem          0.287 sec  348.227 Mcps
Builtin         0.274 sec  364.832 Mcps
POPCNT          0.278 sec  359.783 Mcps
SIMD            0.275 sec  363.230 Mcps
Prefix          0.274 sec  365.245 Mcps
deBruijn        0.279 sec  359.032 Mcps
Precomp 8    setup took 0.00 ms
Precomp 8       0.276 sec  361.708 Mcps
Precomp 16   setup took 0.36 ms
Precomp 16      0.268 sec  372.630 Mcps
Precomp 24   setup took 133.60 ms
Precomp 24      0.526 sec  189.944 Mcps

Summary (fastest to slowest)
Precomp 16     7.0x
Prefix         6.9x
Builtin        6.9x
SIMD           6.8x
Precomp 8      6.8x
POPCNT         6.8x
deBruijn       6.8x
Sparse         6.8x
Hakmem         6.6x
Nifty          6.5x
Parallel       6.3x
Precomp 24     3.6x
Iterated       1.1x
Dense          1.0x
```

## Further Reading

- [Fast bit counting routines](http://gurmeet.net/puzzles/fast-bit-counting-routines)
- [Stanford bit hacks collection](http://graphics.stanford.edu/~seander/bithacks.html)

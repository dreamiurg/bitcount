# Bit counting

This small program measures and compares various bit counting algorithms.
It includes classic bit tests as well as hardware POPCNT, SIMD shuffling,
prefix-sum masking, de Bruijn scans and large precomputed lookup tables.

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


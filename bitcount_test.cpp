#include <cassert>
#include <random>
#include <iostream>
#include <vector>

#include "bitcount_algorithms.h"

int main() {
    // Initialize lookup tables for precomputed methods
    create_precomp8();
    create_precomp16();

    std::mt19937 rng(12345);
    const int trials = 100000;

    std::vector<unsigned int> test_values = {0u, 1u, 0xFFFFFFFFu};
    test_values.reserve(trials + test_values.size());
    for (int i = 0; i < trials; ++i) {
        test_values.push_back(rng());
    }

    for (unsigned int value : test_values) {
        int ref = __builtin_popcount(value);
        assert(bitcount(value) == ref);
        assert(bitcount_sparse(value) == ref);
        assert(bitcount_dense(value) == ref);
        assert(bitcount_precomp8(value) == ref);
        assert(bitcount_precomp16(value) == ref);
        assert(bitcount_parallel(value) == ref);
        assert(bitcount_nifty(value) == ref);
        assert(bitcount_hakmem(value) == ref);
    }
    std::cout << "All bitcount tests passed!" << std::endl;
    return 0;
}


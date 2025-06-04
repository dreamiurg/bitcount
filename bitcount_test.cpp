#include <cassert>
#include <random>
#include <iostream>
#include <vector>

#include "bitcount_algorithms.h"

int main() {
  for (auto& algo : get_algorithms()) {
    if (algo.init) {
      algo.init();
    }
  }

  std::mt19937 rng(12345);
  const int trials = 100000;

  std::vector<unsigned int> test_values = {0u, 1u, 0xFFFFFFFFu};
  test_values.reserve(trials + test_values.size());
  for (int i = 0; i < trials; ++i) {
    test_values.push_back(rng());
  }

  for (unsigned int value : test_values) {
    int ref = __builtin_popcount(value);
    for (auto& algo : get_algorithms()) {
      assert(algo.func(value) == ref);
    }
  }
  std::cout << "All bitcount tests passed!" << std::endl;
  return 0;
}


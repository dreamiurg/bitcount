#ifndef BITCOUNT_ALGORITHMS_H
#define BITCOUNT_ALGORITHMS_H

#include <vector>

struct BitCountAlgorithm {
  const char* name;
  int (*func)(unsigned int);
  void (*init)();
};

std::vector<BitCountAlgorithm>& get_algorithms();

#define REGISTER_BITCOUNT_ALGO(NAME, FUNC, INIT)           \
  namespace {                                              \
  struct Register_##FUNC {                                 \
    Register_##FUNC() {                                    \
      get_algorithms().push_back({NAME, FUNC, INIT});       \
    }                                                      \
  } register_##FUNC;                                       \
  }

int bitcount(unsigned int n);
int bitcount_sparse(unsigned int n);
int bitcount_dense(unsigned int n);

void create_precomp8();
int bitcount_precomp8(unsigned int n);

void create_precomp16();
int bitcount_precomp16(unsigned int n);

int bitcount_parallel(unsigned int n);
int bitcount_nifty(unsigned int n);
int bitcount_hakmem(unsigned int n);
int bitcount_builtin(unsigned int n);
int bitcount_popcnt(unsigned int n);
int bitcount_simd(unsigned int n);
int bitcount_prefix(unsigned int n);
int bitcount_debruijn(unsigned int n);

void create_precomp24();
int bitcount_precomp24(unsigned int n);

#endif // BITCOUNT_ALGORITHMS_H

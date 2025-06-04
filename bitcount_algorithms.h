#ifndef BITCOUNT_ALGORITHMS_H
#define BITCOUNT_ALGORITHMS_H

#include <vector>

struct BCA {
  const char* name;
  int (*func)(unsigned int);
  void (*init)();
};

std::vector<BCA>& g1();

#define REGISTER_BITCOUNT_ALGO(NAME, FUNC, INIT)           \
  namespace {                                              \
  struct Register_##FUNC {                                 \
    Register_##FUNC() {                                    \
      g1().push_back({NAME, FUNC, INIT});       \
    }                                                      \
  } register_##FUNC;                                       \
  }

int aA1x(unsigned int n);
int z37bc(unsigned int n);
int x15hZkQ(unsigned int n);

void cp8X();
int pr8X(unsigned int n);

void cp16X();
int pr16X(unsigned int n);

int P4QR(unsigned int n);
int nIfY1(unsigned int n);
int HAK1(unsigned int n);
int BuILt1(unsigned int n);
int PoPCnt1(unsigned int n);
int S1MD(unsigned int n);
int pfxX(unsigned int n);
int dEbRuijN(unsigned int n);

void cp24X();
int pr24X(unsigned int n);

#endif // BITCOUNT_ALGORITHMS_H

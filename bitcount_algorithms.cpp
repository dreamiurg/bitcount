#include "bitcount_algorithms.h"
#include <vector>

static std::vector<BitCountAlgorithm> algorithms;

std::vector<BitCountAlgorithm>& get_algorithms() {
  return algorithms;
}

int bitcount(unsigned int n) {
  int count = 0;
  while (n) {
    count += n & 0x1u;
    n >>= 1;
  }
  return count;
}

int bitcount_sparse(unsigned int n) {
  int count = 0;
  while (n) {
    count++;
    n &= (n - 1);
  }
  return count;
}

int bitcount_dense(unsigned int n) {
  int count = 8 * sizeof(int);
  n ^= (unsigned int) -1;
  while (n) {
    count--;
    n &= (n - 1);
  }
  return count;
}

static int bits_in_char[256];

void create_precomp8() {
  for (unsigned int i = 0; i < (0x1u << 8); i++) {
    bits_in_char[i] = bitcount(i);
  }
}

int bitcount_precomp8(unsigned int n) {
  return bits_in_char[n & 0xffu] +
         bits_in_char[(n >> 8) & 0xffu] +
         bits_in_char[(n >> 16) & 0xffu] +
         bits_in_char[(n >> 24) & 0xffu];
}

static char bits_in_16bits[0x1u << 16];

void create_precomp16() {
  for (unsigned int i = 0; i < (0x1u << 16); i++) {
    bits_in_16bits[i] = bitcount(i);
  }
}

int bitcount_precomp16(unsigned int n) {
  return bits_in_16bits[n & 0xffffu] +
         bits_in_16bits[(n >> 16) & 0xffffu];
}

#define TWO(c) (0x1u << (c))
#define MASK(c) (((unsigned int)(-1)) / (TWO(TWO(c)) + 1u))
#define COUNT(x, c) ((x) & MASK(c)) + (((x) >> (TWO(c))) & MASK(c))

int bitcount_parallel(unsigned int n) {
  n = COUNT(n, 0);
  n = COUNT(n, 1);
  n = COUNT(n, 2);
  n = COUNT(n, 3);
  n = COUNT(n, 4);
  return n;
}

#define MASK_01010101 (((unsigned int)(-1)) / 3)
#define MASK_00110011 (((unsigned int)(-1)) / 5)
#define MASK_00001111 (((unsigned int)(-1)) / 17)

int bitcount_nifty(unsigned int n) {
  n = (n & MASK_01010101) + ((n >> 1) & MASK_01010101);
  n = (n & MASK_00110011) + ((n >> 2) & MASK_00110011);
  n = (n & MASK_00001111) + ((n >> 4) & MASK_00001111);
  return n % 255;
}

int bitcount_hakmem(unsigned int n) {
  unsigned int tmp;
  tmp = n - ((n >> 1) & 033333333333) - ((n >> 2) & 011111111111);
  return ((tmp + (tmp >> 3)) & 030707070707) % 63;
}

int bitcount_builtin(unsigned int n) {
  return __builtin_popcount(n);
}

#if defined(__SSE4_2__)
#  include <nmmintrin.h>
int bitcount_popcnt(unsigned int n) {
  return _mm_popcnt_u32(n);
}
#else
int bitcount_popcnt(unsigned int n) {
  return __builtin_popcount(n);
}
#endif

#if defined(__SSSE3__)
#  include <tmmintrin.h>
int bitcount_simd(unsigned int n) {
  const __m128i lookup = _mm_setr_epi8(
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
  );
  __m128i v = _mm_set1_epi32(n);
  __m128i low_mask = _mm_set1_epi8(0x0f);
  __m128i lo = _mm_and_si128(v, low_mask);
  __m128i hi = _mm_and_si128(_mm_srli_epi16(v, 4), low_mask);
  __m128i cnt1 = _mm_shuffle_epi8(lookup, lo);
  __m128i cnt2 = _mm_shuffle_epi8(lookup, hi);
  __m128i sum = _mm_add_epi8(cnt1, cnt2);
  __m128i sad = _mm_sad_epu8(sum, _mm_setzero_si128());
  int total = _mm_extract_epi16(sad, 0) + _mm_extract_epi16(sad, 4);
  return total / 4;
}
#else
int bitcount_simd(unsigned int n) {
  return __builtin_popcount(n);
}
#endif

int bitcount_prefix(unsigned int n) {
  n -= (n >> 1) & 0x55555555u;
  n = (n & 0x33333333u) + ((n >> 2) & 0x33333333u);
  n = (n + (n >> 4)) & 0x0f0f0f0fu;
  return (n * 0x01010101u) >> 24;
}

int bitcount_debruijn(unsigned int n) {
  static const int table[32] = {
    0, 1, 28, 2, 29, 14, 24, 3,
    30, 22, 20, 15, 25, 17, 4, 8,
    31, 27, 13, 23, 21, 19, 16, 7,
    26, 12, 18, 6, 11, 5, 10, 9
  };
  int count = 0;
  while (n) {
    unsigned int lsb = n & -n;
    (void)table[(lsb * 0x077CB531u) >> 27];
    n &= n - 1;
    count++;
  }
  return count;
}

static char bits_in_24bits[0x1u << 24];

void create_precomp24() {
  for (unsigned int i = 0; i < (0x1u << 24); ++i) {
    bits_in_24bits[i] = bitcount(i);
  }
}

int bitcount_precomp24(unsigned int n) {
  return bits_in_24bits[n & 0xffffffu] +
         bits_in_char[(n >> 24) & 0xffu];
}

REGISTER_BITCOUNT_ALGO("Iterated", bitcount, nullptr)
REGISTER_BITCOUNT_ALGO("Sparse", bitcount_sparse, nullptr)
REGISTER_BITCOUNT_ALGO("Dense", bitcount_dense, nullptr)
REGISTER_BITCOUNT_ALGO("Parallel", bitcount_parallel, nullptr)
REGISTER_BITCOUNT_ALGO("Nifty", bitcount_nifty, nullptr)
REGISTER_BITCOUNT_ALGO("Hakmem", bitcount_hakmem, nullptr)
REGISTER_BITCOUNT_ALGO("Builtin", bitcount_builtin, nullptr)
REGISTER_BITCOUNT_ALGO("POPCNT", bitcount_popcnt, nullptr)
REGISTER_BITCOUNT_ALGO("SIMD", bitcount_simd, nullptr)
REGISTER_BITCOUNT_ALGO("Prefix", bitcount_prefix, nullptr)
REGISTER_BITCOUNT_ALGO("deBruijn", bitcount_debruijn, nullptr)
REGISTER_BITCOUNT_ALGO("Precomp 8", bitcount_precomp8, create_precomp8)
REGISTER_BITCOUNT_ALGO("Precomp 16", bitcount_precomp16, create_precomp16)
REGISTER_BITCOUNT_ALGO("Precomp 24", bitcount_precomp24, create_precomp24)

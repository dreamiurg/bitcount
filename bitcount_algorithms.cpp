#include "bitcount_algorithms.h"

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
    n ^= (unsigned int)-1;
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

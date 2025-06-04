#ifndef BITCOUNT_ALGORITHMS_H
#define BITCOUNT_ALGORITHMS_H

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

#endif // BITCOUNT_ALGORITHMS_H

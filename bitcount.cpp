//
// bitcount.cpp - comparison of various bit-counting approaches
// by Dmitry Guyvoronsky (http://demiurg.com.ua)
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

 class StopWatch
{
public:
	StopWatch() : m_start(0), m_stop(0) {}
	
	void Start() 	{ m_start = NowMicroseconds(); }
	void Stop()		{ m_stop  = NowMicroseconds(); }
	
	// returns number of microseconds elapsed between Start() and Stop() calls
	unsigned long ElapsedMicrosec() const { return (m_stop > m_start) ? (m_stop - m_start) : 0;	}

        static unsigned long NowMicroseconds()
        {
                struct timeval tv = {};

                if( gettimeofday(&tv, NULL) == 0)
                {
                        return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
                }

                return 0;
        }
		
private:
	unsigned long m_start;
	unsigned long m_stop;
};

// bit-counting function
typedef int (*bc_function)(unsigned int);

void run_test(const unsigned int iters, bc_function bcf, const char* name)
{
	StopWatch sw;
	sw.Start();
	
	int num_of_bits = 0;
	for(unsigned int i = 0; i < iters; ++i)
	{
		num_of_bits += (*bcf)(lrand48());
	}
	
	sw.Stop();
	const unsigned long elapsed = sw.ElapsedMicrosec();
	
	printf("%-12s %8.3f sec %8.3f Mcps\n", name, elapsed / 1000000.0, (double)iters / elapsed);
}

int bitcount (unsigned int n) {
   int count = 0;
   while (n) {
      count += n & 0x1u;
      n >>= 1;
   }
   return count;
}

int bitcount_sparse (unsigned int n)  {
   int count = 0 ;
   while (n)  {
      count++ ;
      n &= (n - 1) ;
   }
   return count ;
}

int bitcount_dense (unsigned int n)   {
   int count = 8 * sizeof(int) ;
   n ^= (unsigned int) - 1 ;
   while (n)  {
      count-- ;
      n &= (n - 1) ;
   }
   return count ;
}

static int bits_in_char [256] ;       

void create_precomp8()
{
	for(unsigned int i = 0; i < (0x1u << 8); i++)
	{
		bits_in_char[i] = bitcount(i);
	}
}    

int bitcount_precomp8 (unsigned int n)  {
  // works only for 32-bit ints

  return bits_in_char [n  & 0xffu]
      +  bits_in_char [(n >>  8 ) & 0xffu]
      +  bits_in_char [(n >> 16) & 0xffu]
      +  bits_in_char [(n >> 24) & 0xffu] ;
}

static char bits_in_16bits [0x1u << 16] ;      

void create_precomp16()
{
	for(unsigned int i = 0; i < (0x1u << 16); i++)
	{
		bits_in_16bits[i] = bitcount(i);
	}
}    

int bitcount_precomp16 (unsigned int n)  {
   // works only for 32-bit ints

   return bits_in_16bits [n         & 0xffffu]
       +  bits_in_16bits [(n >> 16) & 0xffffu] ;
}

#define TWO(c)     (0x1u << (c))
#define MASK(c) \
  (((unsigned int)(-1)) / (TWO(TWO(c)) + 1u))
#define COUNT(x,c) \
  ((x) & MASK(c)) + (((x) >> (TWO(c))) & MASK(c))

int bitcount_parallel (unsigned int n)  {
   n = COUNT(n, 0) ;
   n = COUNT(n, 1) ;
   n = COUNT(n, 2) ;
   n = COUNT(n, 3) ;
   n = COUNT(n, 4) ;
   /* n = COUNT(n, 5) ;    for 64-bit integers */
   return n ;
}

#define MASK_01010101 (((unsigned int)(-1))/3)
#define MASK_00110011 (((unsigned int)(-1))/5)
#define MASK_00001111 (((unsigned int)(-1))/17)
 int bitcount_nifty (unsigned int n) {
   n = (n & MASK_01010101) + ((n >> 1) & MASK_01010101) ;
   n = (n & MASK_00110011) + ((n >> 2) & MASK_00110011) ;
   n = (n & MASK_00001111) + ((n >> 4) & MASK_00001111) ;
   return n % 255 ;
}

int bitcount_hakmem (unsigned int n) {
   /* works for 32-bit numbers only    */
   /* fix last line for 64-bit numbers */

   register unsigned int tmp;

   tmp = n - ((n >> 1) & 033333333333)
           - ((n >> 2) & 011111111111);
   return ((tmp + (tmp >> 3)) & 030707070707) % 63;
}

int main()
{
	srand48(time(NULL));
	
	// precompute lookup tables
	{
		StopWatch sw;
		sw.Start();
		create_precomp8();
		sw.Stop();
		printf("8-bit lookup table calculation took %.2f ms\n", (double)sw.ElapsedMicrosec() / 1000);
	}
	{
		StopWatch sw;
		sw.Start();
		create_precomp16();
		sw.Stop();
		printf("16-bit lookup table calculation took %.2f ms\n", (double)sw.ElapsedMicrosec() / 1000);
	}

	const unsigned int iters = 100*1000*1000;
	printf("\n---> Iterated methods\n");
	run_test(iters, &bitcount, 			"Iterated");
	run_test(iters, &bitcount_sparse, 	"Sparse");
	run_test(iters, &bitcount_dense, 	"Dense");

	printf("---> Parallel methods\n");
	run_test(iters, &bitcount_parallel, "Parallel");
	run_test(iters, &bitcount_nifty, 	"Nifty");
	run_test(iters, &bitcount_hakmem, 	"Hakmem");

	printf("---> Table lookup methods\n");
	run_test(iters, &bitcount_precomp8, "Precomp 8");
	run_test(iters, &bitcount_precomp16,"Precomp 16");
	
	return 0;
}
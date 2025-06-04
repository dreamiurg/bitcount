//
// bitcount.cpp - comparison of various bit-counting approaches
// by Dmitry Guyvoronsky (http://demiurg.com.ua)
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include "bitcount_algorithms.h"

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
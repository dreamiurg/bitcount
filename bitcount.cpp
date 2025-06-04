//
// bitcount.cpp - comparison of various bit-counting approaches
// by Dmitry Guyvoronsky (http://demiurg.com.ua)
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include <string>
#include "bitcount_algorithms.h"

struct Result {
  std::string name;
  double mcps;
};

class StopWatch {
public:
  StopWatch() : m_start(0), m_stop(0) {}

  void Start() 	{
    m_start = NowMicroseconds();
  }
  void Stop()		{
    m_stop  = NowMicroseconds();
  }

  // returns number of microseconds elapsed between Start() and Stop() calls
  unsigned long ElapsedMicrosec() const {
    return (m_stop > m_start) ? (m_stop - m_start) : 0;
  }

  static unsigned long NowMicroseconds() {
    struct timeval tv = {};

    if ( gettimeofday(&tv, NULL) == 0) {
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

double run_test(const unsigned int iters, bc_function bcf, const char* name)
{
  StopWatch sw;
  sw.Start();

  unsigned long long num_of_bits = 0;
  for (unsigned int i = 0; i < iters; ++i)
  {
    num_of_bits += (*bcf)(lrand48());
  }

  sw.Stop();
  const unsigned long elapsed = sw.ElapsedMicrosec();

  const double mcps = (double)iters / elapsed;
  printf("%-12s %8.3f sec %8.3f Mcps\n", name, elapsed / 1000000.0, mcps);
  return mcps;
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

  const unsigned int iters = 100 * 1000 * 1000;
  std::vector<Result> results;

  printf("\n---> Iterated methods\n");
  results.push_back({"Iterated", run_test(iters, &bitcount, "Iterated")});
  results.push_back({"Sparse",   run_test(iters, &bitcount_sparse, "Sparse")});
  results.push_back({"Dense",    run_test(iters, &bitcount_dense, "Dense")});

  printf("---> Parallel methods\n");
  results.push_back({"Parallel", run_test(iters, &bitcount_parallel, "Parallel")});
  results.push_back({"Nifty",    run_test(iters, &bitcount_nifty, "Nifty")});
  results.push_back({"Hakmem",   run_test(iters, &bitcount_hakmem, "Hakmem")});

  printf("---> Builtin method\n");
  results.push_back({"Builtin",  run_test(iters, &bitcount_builtin, "Builtin")});

  printf("---> Table lookup methods\n");
  results.push_back({"Precomp 8",  run_test(iters, &bitcount_precomp8, "Precomp 8")});
  results.push_back({"Precomp 16", run_test(iters, &bitcount_precomp16, "Precomp 16")});

  std::sort(results.begin(), results.end(), [](const Result& a, const Result& b) { return a.mcps > b.mcps; });
  const double slowest = results.back().mcps;

  printf("\nSummary (fastest to slowest)\n");
  for (const auto& r : results) {
    printf("%-12s %5.1fx\n", r.name.c_str(), r.mcps / slowest);
  }

  return 0;
}
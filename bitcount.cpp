//
// bitcount.cpp - comparison of various bit-counting approaches
// by Dmitry Guyvoronsky (http://demiurg.com.ua)
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include "bitcount_algorithms.h"

struct Result {
  std::string name;
  double mcps;
};

class StopWatch {
public:
  void Start() { m_start = clock::now(); }
  void Stop() { m_stop = clock::now(); }

  unsigned long ElapsedMicrosec() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(
             m_stop - m_start).count();
  }

private:
  using clock = std::chrono::steady_clock;
  std::chrono::time_point<clock> m_start{};
  std::chrono::time_point<clock> m_stop{};
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

  const unsigned int iters = 100 * 1000 * 1000;
  std::vector<Result> results;
  for (auto& algo : get_algorithms()) {
    if (algo.init) {
      StopWatch sw;
      sw.Start();
      algo.init();
      sw.Stop();
      printf("%-12s setup took %.2f ms\n", algo.name, sw.ElapsedMicrosec() / 1000.0);
    }
    results.push_back({algo.name, run_test(iters, algo.func, algo.name)});
  }

  std::sort(results.begin(), results.end(), [](const Result& a, const Result& b) { return a.mcps > b.mcps; });
  const double slowest = results.back().mcps;

  printf("\nSummary (fastest to slowest)\n");
  for (const auto& r : results) {
    printf("%-12s %5.1fx\n", r.name.c_str(), r.mcps / slowest);
  }

  return 0;
}
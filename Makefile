CXX ? = g++
        CXXFLAGS ? = -std = c++11 - O2

                            BENCH_OBJS = bitcount.o bitcount_algorithms.o
TEST_OBJS = bitcount_test.o bitcount_algorithms.o

                              all : bitcount_test

                              bitcount.o : bitcount.cpp bitcount_algorithms.h
                              $(CXX) $(CXXFLAGS) - c bitcount.cpp - o $@

                              bitcount_algorithms.o : bitcount_algorithms.cpp bitcount_algorithms.h
                              $(CXX) $(CXXFLAGS) - c bitcount_algorithms.cpp - o $@

                              bitcount_test.o : bitcount_test.cpp bitcount_algorithms.h
                              $(CXX) $(CXXFLAGS) - c bitcount_test.cpp - o $@

                              bitcount : $(BENCH_OBJS)
                              $(CXX) $(CXXFLAGS) $(BENCH_OBJS) - o $@

                              bitcount_test : $(TEST_OBJS)
                              $(CXX) $(CXXFLAGS) $(TEST_OBJS) - o $@

                              .PHONY : clean test bench

                              test : bitcount_test
                              . / bitcount_test

                              bench : bitcount

                              clean :
                              rm - f * .o bitcount bitcount_test

/* 
 * LEON3 timing measurement utilities - mini demo
 * ----------------------------------------------
 * (c) 2011, Benedikt Huber <benedikt@vmars.tuwien.ac.at>. All Rights Reserved.
 */

#include <stdio.h>

#include "bench.h"

#define REPEAT 8

/* 
 * The routine we want to analyze:
 * It is quite stupid: 32 times, do 8 additions
 */
void simple()
{
  unsigned int i, r = 42;

  for(i = 0; i < REPEAT; ++i) {
    asm volatile("add %1, 1, %%g1\n"
                 "add %%g1, 1, %%g1\n"
                 "add %%g1, 1, %%g1\n"
                 "add %%g1, 1, %%g1\n"
                 "add %%g1, 1, %%g1\n"
                 "add %%g1, 1, %%g1\n"
                 "add %%g1, 1, %%g1\n"
                 "add %%g1, 1, %0\n"
                 :  "=r"(r) /* output */
                 :  "r" (r) /* input */
                 : "%g1");
  }
}

/*
 * A simple test driver for the routine we want to analyze
 */
void simple_test() {
  flush_icache_blocking();

  MEASURE_START();

  simple();

  MEASURE_STOP();

  printf("simple (%d.times 8xadd) estimated: %ld [incl.function call]\n",
         REPEAT, ELAPSED_CYCLES());
}

/*
 * The main entry point 
 */
int main(int argc, char ** argv)
{
  init_benchmarks(1);
  simple_test();
}

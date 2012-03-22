#include <stdio.h>
#include "insertion_sort.h"
#include "bench.h"

void run_insertion_sort(int*data, int size)
{
  MEASURE_START();
  insertion_sort(data,size);
  MEASURE_STOP();
  printf("Elapsed Cycles: %lu\n", ELAPSED_CYCLES());
}

void run()
{
	int i;
	static int data[32];

        /* TODO: write test cases */
}

int main(int argc, char** argv)
{
  init_benchmarks(1);
  run();
}

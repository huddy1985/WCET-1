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

void run16(int *data) {
	
	insertion_sort(data,16);
}

void run32(int *data) {
	insertion_sort(data,32);
}

void run64(int *data) {
	insertion_sort(data,64);
}

void run2() {
	static int data[64];
	run16(data);
	run32(data);
	run64(data);
}

void filldata(int*data,int size) {
	int i;
	for (i=size-1; i >= 0; i--)
		data[size-1-i] = i;
}

void run()
{
	int i;
	static int data[64];

        /* TODO: write test cases */
        filldata(data, 64);
        run_insertion_sort(data, 16);
        run_insertion_sort(data, 32);
        run_insertion_sort(data, 64);
}

int main(int argc, char** argv)
{
  init_benchmarks(1);
  run();
}

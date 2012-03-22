/*
 * test.c
 * WCET Analysis Lab
 * 
 * Copyright (c) 2010 Benedikt Huber <benedikt@vmars.tuwien.ac.at>
 *               Real Time System Group, Department of Computer Engineering
 *               Vienna University of Technology
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <math.h>
#include "task.h"
#include "test_data.h"
#include "bench.h"

/* Test Data */
sample_buffer_t sbuf_1;
sample_value_t  sbuf_1_storage[SAMPLE_BUF_SIZE];

/* Benchmarking statistics */

#define BENCH_COUNT 3
typedef enum { task_e, merge_e, fft_e } bench_t;
char*   bench_names[BENCH_COUNT] = { "task", "merge_samples", "fft" };

typedef struct {
    unsigned long min, max, total, samples;
} stats_t;

stats_t stats[BENCH_COUNT];

void update_stats(bench_t bench)
{
    long elapsed = ELAPSED_CYCLES();
    stats_t * s = &stats[bench];

    if(s->min == 0)           s->min = elapsed;
    else if(elapsed < s->min) s->min = elapsed;
    if(elapsed > s->max)      s->max = elapsed;

    s->samples += 1;
    s->total   += elapsed;
}

/* Benchmarking delegators */
status_t __wrap_task(input_t *input, state_t *state, const signal_spec_t* signal_spec) {
    MEASURE_START();status_t r = task(input,state,signal_spec);MEASURE_STOP();
    update_stats(task_e); return r;
}
void __wrap_merge_samples(input_t* in, sample_buffer_t* sbuf)
{
    MEASURE_START();merge_samples(in, sbuf);MEASURE_STOP();
    update_stats(merge_e);
}
status_t __wrap_fft(sample_buffer_t *buf, int16_t *fft_r_out, int16_t *fft_i_out)
{
    MEASURE_START();      status_t r = fft(buf,fft_r_out,fft_i_out);   MEASURE_STOP();
    update_stats(fft_e);  return r;
}


void sep(char* str)
{
  printf("******************************************************************************\n");
  printf("--- %-70s ---\n",str);
  printf("******************************************************************************\n");
}

void print_sample_buffer(FILE* out, char* header, sample_buffer_t* sbuf)
{
  int i,ptr;
  fprintf(out,"%s\n",header);
  fprintf(stdout,"sample_ptr: %d\n",sbuf->sample_ptr);
  ptr = NINDEX(sbuf);
  for(i = 0; i < sbuf->buf_size; i++)
  {
    if(i == ptr)                                       fprintf(out," ^");
    else if(i == ((ptr+(sbuf->buf_size>>1))%(sbuf->buf_size))) fprintf(out," |");
    else                                               fprintf(out,"  ");
    if(IS_VALUE_MISSING(sbuf->samples[i]))  fprintf(out, "%4s", "[M]");
    else                                    fprintf(out, "%d", sbuf->samples[i]);    
  }
  if(sbuf->sample_ptr < 0) fprintf(out, " [%d samples missing]", -sbuf->sample_ptr);
  fprintf(out,"\n");
}


void test_interpolate()
{
	int      indices[] = { 0        ,2        ,5         ,9        };
    int16_t values[]   = { FP_INT(0),FP_INT(3),FP_INT(-1),FP_INT(4)};
    int i,j;
    
    sep("FP - Interpolate test (16 bit)");
    for(i = 0; i < ARRAY_SIZE(indices)-1;++i)
    {
    	int i1 = indices[i];
    	int i2 = indices[i+1];
    	for(j = i1+1; j < i2;j++)
    	{
    		int16_t v1 = values[i];
    		int16_t v2 = values[i+1];
    		int16_t v = iinterpolate16(i1,v1,i2,v2,j);
    		printf("%d @ %d /interpolate %d/ %d @ %d ==> %d\n", v1, i1, j ,v2, i2, v);
    	}
    }
}

void test_sample_buf(sample_buffer_t* sbuf)
{
  int i,j;
  int32_t cnt = 0;

  input_t input;
  sample_value_t input_buffer[SAMPLE_COUNT*5];
  input.input_samples = &input_buffer[0]; 

  sep("Sample Buffer Test");
  clear_sample_buffer(sbuf);
  
  print_sample_buffer(stdout, "Init", sbuf);
  for(i = 0 ; i < 5 ; i++)
  {
    input.input_count = SAMPLE_COUNT*5/7;
    for(j = 0; j < input.input_count-1; j++) {
      input_buffer[j] = (cnt++ << 4);
    }
    input_buffer[j] = VALUE_MISSING;
    __wrap_merge_samples(&input, sbuf);
    print_sample_buffer(stdout, "Adding Samples", sbuf);
  }

  cnt = 0;
  clear_sample_buffer(sbuf);
  input.input_count = SAMPLE_COUNT*5;
  for(j = 0; j < SAMPLE_COUNT*5-1; j++) {
      input_buffer[j] = (cnt++ << 4);
  }
  input_buffer[j] = VALUE_MISSING;
  __wrap_merge_samples(&input, sbuf);
  print_sample_buffer(stdout, "Adding Samples", sbuf);
    
}

void test_fft(sample_buffer_t* sbuf)
{
  int16_t fft_r[SAMPLE_COUNT];
  int16_t fft_i[SAMPLE_COUNT];
  int32_t fft_abs[SAMPLE_COUNT];
  int i,p;

  input_t input;
  sample_value_t input_buffer[ARRAY_SIZE(TEST_DATA)*2];
  input.input_count   = ARRAY_SIZE(TEST_DATA)*2;
  input.input_samples = &input_buffer[0]; 

  sep("FP - FFT Test");
  clear_sample_buffer(sbuf);
  p = 0;
  for(i = 0; i < ARRAY_SIZE(TEST_DATA); i++) input_buffer[p++] = TEST_DATA[i];
  for(i = 0; i < ARRAY_SIZE(TEST_DATA); i++) input_buffer[p++] = TEST_DATA[i];
  __wrap_merge_samples(&input, sbuf);
  print_sample_buffer(stdout, "FFT Test Start", sbuf);
  __wrap_fft(sbuf, &fft_r[0], &fft_i[0]);
  for(i = 0; i < ARRAY_SIZE(fft_r); i++)
  {
    int32_t real = fft_r[i];
    int32_t imag = fft_i[i];
    int32_t absval = FP_SQRT( ((real*real)>>FP_FRAC) + ((imag*imag)>>FP_FRAC) );
    
    if(absval > INT16_MAX) absval = INT16_MAX;
    printf("%d: %d = |%d + %d i|\n",i,absval, real,imag);
    fft_abs[i] = absval;
  }  
}


#define GEN_COUNT 2
static input_t input[GEN_COUNT];
static sample_value_t inputbuf[SAMPLE_COUNT];
static const signal_spec_t * spec[GEN_COUNT] = { &spec1, &spec2 };
static state_t state[GEN_COUNT];
static status_t output[GEN_COUNT];
void test_task()
{
  int i;
  init_state(&state[0]);
  init_state(&state[1]);

  sep("Add half of data samples to the input (too few samples)");

  input[0].input_count   = SAMPLE_COUNT/2;
  input[0].input_samples = &TEST_DATA[0];

  input[1].input_count   = SAMPLE_COUNT/2;
  input[1].input_samples = &TEST_DATA_2[0];

  for(i = 0; i < GEN_COUNT; i++)
  {
    output[i] = __wrap_task(&input[i],&state[i],spec[i]);
    printf("Generator status %d: %s\n",i,status_message(output[i]));
  }

  sep("Add other half of data samples to the input (first: missing 1 sample, second generator ok)");

  input[0].input_count   = SAMPLE_COUNT/2;
  input[0].input_samples = &TEST_DATA[SAMPLE_COUNT/2];

  input[1].input_count   = SAMPLE_COUNT/2;
  input[1].input_samples = &TEST_DATA_2[SAMPLE_COUNT/2];

  for(i = 0; i < GEN_COUNT; i++)
  {
    output[i] = __wrap_task(&input[i],&state[i],spec[i]);
    printf("Generator status %d: %s\n",i,status_message(output[i]));
  }

  sep("Add first half of data samples to the input (both generators ok)");

  input[0].input_count   = SAMPLE_COUNT/2;
  input[0].input_samples = &TEST_DATA[0];

  input[1].input_count   = SAMPLE_COUNT/2;
  input[1].input_samples = &TEST_DATA_2[0];

  for(i = 0; i < GEN_COUNT; i++)
  {
    output[i] = __wrap_task(&input[i],&state[i],spec[i]);
    printf("Generator status %d: %s\n",i,status_message(output[i]));
  }

  sep("Add 10 more data samples to the input (both generators ok)");

  input[0].input_count   = 10;
  input[0].input_samples = &TEST_DATA[SAMPLE_COUNT/2];

  input[1].input_count   = 10;
  input[1].input_samples = &TEST_DATA_2[SAMPLE_COUNT/2];

  for(i = 0; i < GEN_COUNT; i++)
  {
    output[i] = __wrap_task(&input[i],&state[i],spec[i]);
    printf("Generator status %d: %s\n",i,status_message(output[i]));
  }

  sep("Constant signal (0) (Both generators broken)");
  for(i = 0; i < SAMPLE_COUNT; i++) inputbuf[i] = 0;
  input[0].input_count   = SAMPLE_COUNT/2;
  input[0].input_samples = &inputbuf[0];
  input[1].input_count   = SAMPLE_COUNT/2;
  input[1].input_samples = &inputbuf[0];
  
  for(i = 0; i < GEN_COUNT; i++)
  {
    output[i] = __wrap_task(&input[i],&state[i],spec[i]);
    printf("Generator status %d: %s\n",i,status_message(output[i]));
  }

  sep("Constant signal (2.00) (Both generators broken)");
  for(i = 0; i < SAMPLE_COUNT; i++)
    inputbuf[i] = FP_FROM_DOUBLE(2.00);
  input[0].input_samples = &inputbuf[0];
  input[1].input_samples = &inputbuf[0];

  for(i = 0; i < GEN_COUNT; i++)
  {
    output[i] = __wrap_task(&input[i],&state[i],spec[i]);
    printf("Generator status %d: %s\n",i,status_message(output[i]));
  }

  sep("Add both test data samples to the input (Both generators ok)");
  input[0].input_samples = &TEST_DATA[0];
  input[1].input_samples = &TEST_DATA_2[0];
  
  for(i = 0; i < GEN_COUNT; i++)
  {
    output[i] = __wrap_task(&input[i],&state[i],spec[i]);
    printf("Generator status %d: %s\n",i,status_message(output[i]));
  }

  sep("Add first half of samples of other generator (Both generators broken)");
  input[0].input_count   = SAMPLE_COUNT/2;
  input[1].input_count   = SAMPLE_COUNT/2;
  input[0].input_samples = &TEST_DATA_2[0];
  input[1].input_samples = &TEST_DATA[0];

  for(i = 0; i < GEN_COUNT; i++)
  {
    output[i] = __wrap_task(&input[i],&state[i],spec[i]);
    printf("Generator status %d: %s\n",i,status_message(output[i]));
  }

  sep("Add other samples from other generator (Both generators broken)");
  input[0].input_samples = &TEST_DATA_2[SAMPLE_COUNT/2];
  input[1].input_samples = &TEST_DATA[SAMPLE_COUNT/2];

  for(i = 0; i < GEN_COUNT; i++)
  {
    output[i] = __wrap_task(&input[i],&state[i],spec[i]);
    printf("Generator status %d: %s\n",i,status_message(output[i]));
  }

}

void print_stats()
{
    int i;
    printf("%20s | %6s | %6s | %8s | %6s\n","name","min","max","total","samples");
    for(i=0;i<BENCH_COUNT;i++)
    {
        stats_t* s = &stats[i];
        printf("%20s | %6ld | %6ld | %8ld | %6ld\n", 
               bench_names[i], s->min, s->max, s->total, s->samples);
    }
}

int main()
{
  init_benchmarks(1);
  init_sample_buffer(&sbuf_1, &sbuf_1_storage[0], SAMPLE_BUF_SIZE);
  
  test_interpolate();
  test_sample_buf(&sbuf_1);
  test_fft(&sbuf_1);
  test_task();
  
  print_stats();
  return 0;
}



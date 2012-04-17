/*
 * task.c
 * WCET Analysis Lab 2010ss
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

#include "task.h"
#include <string.h>

static const check_function_t checks[2] = { &check_sin, &check_square };

/** @brief signal monitor task
 * 
 * @param input new samples for the signal
 * @param state sample buffer for the signal
 * @param signal_spec signal specification
 * @return status of the signal (ok, broken, too few (valid) samples)
 * 
 * Assumptions for WCET Analysis:
 *   Refer to input->input_count as @inputcount
 *   In the lab, assume @inputcount <= 32
 * 
 * Algorithm:
 *  (1) Merge new samples and interpolate missing samples
 *  (2) Normalize samples and perform a FFT analysis
 *  (3) Run a check to see whether the generator works correctly
 *
 * Hint: The sampler should run with ~ f*20 Hz for best results. 
 *
 */
 
status_t task(input_t *input, state_t *state, const signal_spec_t* signal_spec)
{
     static int16_t fft_r[SAMPLE_COUNT];
     static int16_t fft_i[SAMPLE_COUNT];

     int16_t status;
     sample_buffer_t* sbuf = &state->sample_buffer;

     /* (1) merge samples and interpolate missing ones */
     merge_samples(input,sbuf);

     /* (2) normalize samples and perform fast fourier transform */
     status = fft(sbuf,&fft_r[0],&fft_i[0]);
     if(status != S_OK) return status;    
     /* (3) analyze result */
     status = checks[signal_spec->signal_type](signal_spec->signal_params, fft_r, fft_i);
     return status;
}

/****************************************************************************
 * Initialize state of task
 ***************************************************************************/
void init_state(state_t* st)
{
  init_sample_buffer(&st->sample_buffer, &st->sample_buffer_storage[0], SAMPLE_BUF_SIZE);
}


/****************************************************************************
 * Add a list of samples to the sample buffer (at most @inputcount)
 * Interpolate missing samples if possible
 ***************************************************************************/
 /* ai: instruction "merge_samples" is entered with @inputcount = 32; */
void merge_samples(input_t* in, sample_buffer_t* sbuf)
{
  int i, j, cnt, valid;
  sample_value_t  x;
  sample_value_t *xs; 
  
  if(in->input_count < 1) return;

  /* Find the last valid sample in the current buffer if neccessary */  
  for(valid = -1; valid >= -MAX_CONSECUTIVE_MISSING-1; --valid)
  {
      if(! IS_VALUE_MISSING(sample_buffer_get(sbuf,valid))) break;
  }
  cnt = in->input_count;
  xs = in->input_samples;

  for(i = valid+1;
      i < cnt;/* ai: loop here MAX (@inputcount+4); */
      i++)
  {
  	
  	if(i >= 0)
  	{
  		x = xs[i];
  		/* ai: flow (here) <= 32 ("merge_samples"); */
  		sample_buffer_set(sbuf,i,x);
  	}
  	else
  	{
  		x = sample_buffer_get(sbuf,i);
  		/* ai: flow (here) <= 4 ("merge_samples"); */
  	}

    /* If the sample is not missing, interpolate the ones before if the range is acceptable */	
  	if(! IS_VALUE_MISSING(x))
  	{
          /* Only interpolate if we interpolate at most MAX_CONSECUTIVE_MISSING samples */
  	  int missing_samples = i - valid - 1;
  	  if(missing_samples > 0 && missing_samples <= MAX_CONSECUTIVE_MISSING)
  	  {
  	  	  
    		for(j = i-1;j > valid;--j)
    		{
    	      /* At most once for each invalid input sample */
    		  
    		  sample_value_t y = sample_buffer_get(sbuf,j);
    		  /* ai: loop here MAX 4; */
    		  /* ai: flow each (here) / ("merge_samples") is max 32; */
    		  if(! IS_VALUE_MISSING(y)) break;
    		  y = iinterpolate16(valid,sample_buffer_get(sbuf,valid),i,x,j);
    		  sample_buffer_set(sbuf,j, y);
    		}
  	  }
  	  valid = i;
  	}
  }
  sample_buffer_incr_ptr(sbuf, cnt);
  return;
}

/****************************************************************************
 * FFT transform, N = SAMPLE_COUNT
 ***************************************************************************/

status_t fft(sample_buffer_t *buf, int16_t * fft_r_out, int16_t * fft_i_out)
{
  int i, offs;
  sample_value_t max = 0;
  int32_t multiplier;
  
  for(offs = -1; offs >= -MAX_CONSECUTIVE_MISSING; --offs)
  {
	  if(! IS_VALUE_MISSING(sample_buffer_get(buf,offs))) break;
  }
  if(offs < -MAX_CONSECUTIVE_MISSING) return S_TOO_FEW_VALID_SAMPLES;

  /* Copy and normalize real-valued input */
  offs -= SAMPLE_COUNT-1;
  for(i = 0; i < SAMPLE_COUNT; i++)
  {
	 int16_t val = sample_buffer_get(buf,offs+i);
	 if(IS_VALUE_MISSING(val)) return S_TOO_FEW_VALID_SAMPLES;
	 if(val < 0) val = -val;
	 else if(val > max) max = val;	 
  }
  if(max != 0)
  {
    multiplier = (((int32_t)NORM_MAX)<<16) / max;
    for(i = 0; i < SAMPLE_COUNT; i++)
    {
	    /* Normalize real-valued input */
	    int32_t old_val = sample_buffer_get(buf,i-SAMPLE_COUNT);
	    fft_r_out[i] = (int16_t)((old_val*multiplier) >> 16);
    }
  }
  
  /* Set imaginary input to 0 */
  /* memset(&fft_i_out[0],
   *      0,
   *      SAMPLE_COUNT * sizeof(fft_i_out[0])); */
  for(i = 0; i < SAMPLE_COUNT; i++)
  {
	  fft_i_out[i] = 0;
  }
  
 
  /* Run fixed-point FFT */
  /* ai: instruction here is entered with @n = 64; */
  /* ai: instruction here is entered with @t =  6; */
  fp_radix2fft_withscaling(
         &fft_r_out[0], &fft_i_out[0],
         BIT_REVERSE64, TWIDDLE_R64, TWIDDLE_I64,
         SAMPLE_COUNT, SAMPLE_COUNT_LOG2);
  return S_OK;
}

/****************************************************************************
 * Check heuristics
 ***************************************************************************/
static inline uint16_t abs_complex(int32_t real, int32_t imag)
{  
  return FP_SQRT( ((real*real)>>FP_FRAC) + ((imag*imag)>>FP_FRAC) );
}

status_t check_sin(const void* params, const int16_t * fft_r, const int16_t* fft_i)
{ 
  int i;
  const check_sin_param_t *ps = (const check_sin_param_t*) params;
  sample_value_t f0,fhi, f;
  f0 = abs_complex(fft_r[0],fft_i[0]);
  /* 1 frequency should be above, the rest below the threshold */
  if(f0 > ps->base_threshold) return S_BAD_SPECTRUM;
  fhi = 0;
  for(i = 1; i < SAMPLE_COUNT>>1;i++)
  {
	  f = abs_complex(fft_r[i],fft_i[i]);
	  if(f > ps->noise_threshold) {
		  if(fhi != 0) {
			  return S_BAD_SPECTRUM;
		  }
		  fhi = f;
	  }
  }
  if(fhi < ps->amplitude_minimum) return S_BAD_SPECTRUM;
  return S_OK;
}

status_t check_square(const void* params, const int16_t * fft_r, const int16_t* fft_i)
{
	  int i;
	  const check_square_param_t* ps = (const check_square_param_t*)params;

	  uint16_t base = abs_complex(fft_r[0],fft_i[0]);	  
	  uint16_t top  = base;
	  uint16_t f;
	  int count = 0;
	  if(base < ps->base_minimum) return S_BAD_SPECTRUM;
	  for(i = 1; i < SAMPLE_COUNT>>1;i++)
	  {
		  f = abs_complex(fft_r[i],fft_i[i]);
		  if(f > ps->noise_threshold) {
			  count++;
			  if(f > top) return S_BAD_SPECTRUM;			  
			  top = f;
			  if(count == 1) {
				  if(f < ps->f1_minimum) return S_BAD_SPECTRUM;
			  }
		  }
	  }
	  if(count == 0) return S_BAD_SPECTRUM;
	  return S_OK;
}

/****************************************************************************
 * Textual representation of a status code
 ***************************************************************************/
char* status_message(int status)
{
  switch(status)
  {
    case S_OK: return "status ok";
    case S_BAD_SPECTRUM: return "bad spectrum";
    case S_TOO_FEW_VALID_SAMPLES: return "too few valid samples";
    default: return "internal error";
  }
}

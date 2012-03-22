/*
 * task.h
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

/** Signal Monitor Task
 * 
 * @param input new samples for the signal
 * @param state sample buffer for the signal
 * @param signal_spec signal specification
 * @return status of the signal (ok, broken, too few (valid) samples)
 * 
 * Assumptions for WCET Analysis: input->input_count <= 32
 * 
 * Algorithm:
 *  (1) Merge new samples and interpolate missing samples
 *  (2) Normalize samples and perform a FFT analysis
 *  (3) Run a check to see whether the generator works correctly
 *
 * Hint: The sampler should run with ~ f*20 Hz for best results. 
 *
 */
#ifndef TASK_H_GNND1UZX
#define TASK_H_GNND1UZX

#include <stdio.h>
#include "macros.h"
#include "fixedpoint.h"
#include "sample_buffer.h"

/* Parameters */

//! @brief The binary logarithm of the number of samples used for the analysis
//         of the signal generator
#define SAMPLE_COUNT_LOG2 (6)

/* Error Codes */

//! @brief no error
#define E_OK                (0)

/* Status codes */

#define S_OK                     (0)
#define S_BAD_SPECTRUM          (-1)
#define S_PROCESSING_ERROR      (-2)
#define S_TOO_FEW_VALID_SAMPLES (-3)

/* Constants */

//! @brief The number of samples used for the FFT
#define SAMPLE_COUNT    (1<<SAMPLE_COUNT_LOG2)
//! @brief The maximum number of consecutive missing samples interpolated
#define MAX_CONSECUTIVE_MISSING 4
//! @brief The number of samples kept in the buffer
#define SAMPLE_BUF_SIZE (SAMPLE_COUNT*2)
//! @brief Normalized sample maximum
#define NORM_MAX 30000

/* Type aliases and composite types */

typedef int16_t status_t;

typedef enum _enum_signal_type_t { 
	SINE_SIGNAL, SQUARE_SIGNAL 
} signal_type_t;

/** @brief vector of samples */
typedef struct _struct_input
{  
  int input_count;
  sample_value_t* input_samples;
} input_t;

/** @brief one sample buffers for each signal */
typedef struct _struct_state
{
  sample_buffer_t sample_buffer;
  sample_value_t  sample_buffer_storage[SAMPLE_BUF_SIZE];
} state_t;

typedef struct _struct_check_sin_param {
  int16_t amplitude_minimum;
  int16_t base_threshold;
  int16_t noise_threshold;
} check_sin_param_t;

typedef struct _struct_check_square_param {
  int16_t base_minimum;
  int16_t f1_minimum;
  int16_t noise_threshold;
} check_square_param_t; 

/* ANSI C cannot intialize unions
typedef union _union_signal_param_t {
		check_sin_param_t sin_params;
		check_square_param_t square_params;
} signal_param_t;
*/
typedef status_t (*check_function_t)(const void* ,const int16_t *, const int16_t*);

/** @brief signal specification */
typedef struct _struct_signal_spec
{
	signal_type_t signal_type;
	void* signal_params;
} signal_spec_t;

/* Prototypes */

/** @brief signal monitor task
 * 
 * @param[in] input new samples for the signal
 * @param state sample buffer for the signal
 * @param signal_spec signal specification
 * @return status of the signal (ok, broken, too few (valid) samples)
 * 
 * Assumptions for WCET Analysis: input->input_count <= 32
 * 
 * Algorithm:
 *  (1) Merge new samples and interpolate missing samples
 *  (2) Normalize samples and perform a FFT analysis
 *  (3) Run a check to see whether the generator works correctly
 *
 * Hint: The sampler should run with ~ f*20 Hz for best results. 
 *
 */
status_t task(input_t *input, state_t *state, const signal_spec_t* signal_spec);

/** @brief return the status message */
char* status_message(int status);

/** @brief initialize the state of the monitor */
void init_state(state_t* st);

//! @brief Merge in new samples and interpolate missing values
void merge_samples(input_t* in, sample_buffer_t* sbuf);

//! @brief Run a fast fourier transform, first normalizing samples
status_t fft(sample_buffer_t *buf, int16_t *fft_r_out, int16_t *fft_i_out);

/* Checkers */

status_t check_sin(const void* param, const int16_t * fft_r, const int16_t* fft_i);
status_t check_square(const void* param, const int16_t * fft_r, const int16_t* fft_i);

#endif /* end of include guard: TASK_H_GNND1UZX */

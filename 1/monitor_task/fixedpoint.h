/*
 * fixedpoint.h
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
#ifndef FIXEDPOINT_H_G9EM7H5O
#define FIXEDPOINT_H_G9EM7H5O

#include "macros.h"

/* Fix point numbers library */

//! @brief The number of bits used for the fraction of fixed point numbers
#define FP_FRAC 12

#if (FP_FRAC & 1)
#error "fixedpoint.h: Fraction Length for Fixed Point has to be even"
#endif

//! @brief 16-bit fixed-point numbers
typedef int16_t fix16_t;

#define FP_INT(x)         (x<<FP_FRAC)
#define FP_TO_DOUBLE(x)   ((double)(x) / (1<<FP_FRAC))
#define FP_FROM_DOUBLE(x) ((fix16_t)(x*(1<<FP_FRAC)))
#define FP_SQRT(x)        (isqrt32u(x) << (FP_FRAC/2))

/** @brief Linear interpolation for 16 bit integers
 *  @param xi1 first index, < xi
 *  @param xv1 first value
 *  @param xi2 second index, > xi
 *  @param xv2 second value
 *  @param xi  interpolated index, > xi1, < xi2
 *  @return interpolated value
 */
int16_t iinterpolate16(int xi1, int16_t xv1, int xi2, int16_t xv2, int xi);

/** @brief fixed point square root
 * @param val x
 * @return y with y^2 <= x < (y+1)^2
 */
uint32_t isqrt32u(uint32_t val);

/** @brief fixed point FFT
  * @param xr  real part of length n (input/output)
  * @param xi  imaginary part of length n (input/output)
  * @param brv bit reverse matrix of length n
  * @param wr  real part of twiddle factors of length n
  * @param wi  imaginary part of twiddle factors of length n
  * @param n   length of the input
  * @param t   log2(n)
  * 
  * user registers: @n >= n, @t >= t
  * 
  * adapted from: http://www.mathworks.com/products/fixed/demos.html?file=/products/demos/shipping/fixedpoint/fi_radix2fft_demo.html
  * 
  */
void fp_radix2fft_withscaling(int16_t* xr, int16_t* xi,             // input
                              const int *brv,                       // bit reverse vector
                              const int16_t* wr, const int16_t* wi, // twiddle factors
                              int n,                                // length of the input
                              int t);                               // log2 of length of input

//! @brief 64-samples FFT twiddle factors, real part
extern const int16_t TWIDDLE_R64[64];

//! @brief 64-samples FFT twiddle factors, imaginary part
extern const int16_t TWIDDLE_I64[64];

//! @brief indices for 64-samples bit-reversal
extern const int BIT_REVERSE64[64];

#endif /* end of include guard: FIXEDPOINT_H_G9EM7H5O */

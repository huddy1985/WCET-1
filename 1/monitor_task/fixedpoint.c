/*
 * fixedpoint.c
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
#include "fixedpoint.h"
#include <stdio.h>

const int16_t TWIDDLE_R64[64] = {
  4096,  4096,     0,  4096,  2896,     0, -2896,  4096,
  3784,  2896,  1567,     0, -1567, -2896, -3784,  4096,
  4017,  3784,  3405,  2896,  2275,  1567,   799,     0,
  -799, -1567, -2275, -2896, -3405, -3784, -4017,  4096,
  4076,  4017,  3919,  3784,  3612,  3405,  3166,  2896,
  2598,  2275,  1930,  1567,  1188,   799,   401,     0,
  -401,  -799, -1188, -1567, -1930, -2275, -2598, -2896,
  -3166, -3405, -3612, -3784, -3919, -4017, -4076,  4096
};

const int16_t TWIDDLE_I64[64] = {
  0,     0, -4096,     0, -2896, -4096, -2896,     0,
  -1567, -2896, -3784, -4096, -3784, -2896, -1567,     0,
  -799, -1567, -2275, -2896, -3405, -3784, -4017, -4096,
  -4017, -3784, -3405, -2896, -2275, -1567,  -799,     0,
  -401,  -799, -1188, -1567, -1930, -2275, -2598, -2896,
  -3166, -3405, -3612, -3784, -3919, -4017, -4076, -4096,
  -4076, -4017, -3919, -3784, -3612, -3405, -3166, -2896,
  -2598, -2275, -1930, -1567, -1188,  -799,  -401,     0
};

const int BIT_REVERSE64[64] = {
  0, 32, 16, 48,  8, 40, 24, 56, 
  4, 36, 20, 52, 12, 44, 28, 60, 
  2, 34, 18, 50, 10, 42, 26, 58, 
  6, 38, 22, 54, 14, 46, 30, 62,
  1, 33, 17, 49,  9, 41, 25, 57,
  5, 37, 21, 53, 13, 45, 29, 61, 
  3, 35, 19, 51, 11, 43, 27, 59, 
  7, 39, 23, 55, 15, 47, 31, 63
};


/* Permute an array (bit reverse indices) prior to FFT */
static void bitreverse(int16_t* vs, const int* brv, int n);

/** @brief Linear interpolation for 16 bit integers
 *  @param xi1 first index, < xi
 *  @param xv1 first value
 *  @param xi2 second index, > xi
 *  @param xv2 second value
 *  @param xi  interpolated index, > xi1, < xi2
 *  @return interpolated value
 */
int16_t iinterpolate16(int xi1, int16_t xv1, int xi2, int16_t xv2, int xi)
{
  int di_i1 = xi  - xi1;
  int di_21 = xi2 - xi1;
  // Rearranging gives:  (di_21-di_i1) (xv1/di_21) + di_i1 (xv2/di_21)
  xv1 /= di_21;
  xv1 *= (di_21-di_i1);
  xv2 /= di_21;
  xv2 *= di_i1;
  return xv1+xv2;
}

/** @brief fixed point square root
 * @param val x
 * @return y with y^2 <= x < (y+1)^2
 */
uint32_t isqrt32u(uint32_t val) {
  /* taken from: http://www.azillionmonkeys.com/qed/sqroot.html */
  /* by Jim Ulery */
  int32_t temp, g=0, b = 0x8000, bshft = 15;
  do /* ai: loop here EXACTLY 16; */
    {
      if (val >= (temp = (((g << 1) + b)<<bshft--))) {
        g += b;
        val -= temp;
      }
    } while (b >>= 1);
  return g;
}


/** @brief fixed point FFT
 * @param xr  real part of length n (input/output)
 * @param xi  imaginary part of length n (input/output)
 * @param brv bit reverse matrix of length n
 * @param wr  real part of twiddle factors of length n
 * @param wi  imaginary part of twiddle factors of length n
 * @param n   length of the input
 * @param t   log2(n)
 * 
 * user registers: @n, @t
 * 
 * adapted from: http://www.mathworks.com/products/fixed/demos.html?file=/products/demos/shipping/fixedpoint/fi_radix2fft_demo.html
 * 
 */

void fp_radix2fft_withscaling(int16_t* xr, int16_t* xi,
                              const int *bvr,
                              const int16_t* wr, const int16_t* wi,
                              int n, // @n
                              int t) // @t
{
	/* ai: instruction "fp_radix2fft_withscaling" is entered with @n = 64; */
/* ai: instruction "fp_radix2fft_withscaling" is entered with @t =  6; */
  int32_t tempr, tempi;
  int q, j, k;
  int n1, n2, n3;
  int L, kL, r, L2;
  //int32_t lcnt = 0;
  //int32_t lcnt2 = 0;
  int32_t lcnt3 = 0;

  bitreverse(xr,bvr,n);
  bitreverse(xi,bvr,n);
  

  /*  Assignment 1.1:
   *  Work out the loop bounds for the FFT transform
   *  Hint: Use a debug statement to get an idea of values for q, r, L and L2 */
  for (q=1; 
       q<=t;      /* ai: loop here MAX (@t); */
       q++) {
  // lcnt++;
    L = 1 << q;
    r = 1 << (t-q);
    L2 = L>>1;
    kL = 0;
        
    for (k=0;
         k<r;  /* ai: loop here MAX (@n/2); */
         k++) {
    //lcnt2++;
    lcnt3 = 0;
      for (j=0; 
           j<L2; /* ai: loop here MAX (@n/2); */
           j++) { 
      //lcnt3++;
      // flow limit here is exactly 6 * (2^(1:t) * 2^(t:1)) = 192
        /* ai?: flow (here) <= 192 ("fp_radix2fft_withscaling"); */
        n3     = kL + j;  
        n2     = n3 + L2;
        n1     = L2 - 1 + j;
        tempr  = (int32_t)wr[n1]*(int32_t)xr[n2] - (int32_t)wi[n1]*(int32_t)xi[n2];
        tempi  = (int32_t)wr[n1]*(int32_t)xi[n2] + (int32_t)wi[n1]*(int32_t)xr[n2];
        xr[n2] = ((((int32_t)xr[n3])<<FP_FRAC) - tempr)>>(FP_FRAC+1);
        xi[n2] = ((((int32_t)xi[n3])<<FP_FRAC) - tempi)>>(FP_FRAC+1);
        xr[n3] = ((((int32_t)xr[n3])<<FP_FRAC) + tempr)>>(FP_FRAC+1);
        xi[n3] = ((((int32_t)xi[n3])<<FP_FRAC) + tempi)>>(FP_FRAC+1);
      }
      //printf("\n**************\nlcnt3: %d\n**************\n\n", lcnt3);
      kL += L;
    }
  }
  //printf("\n**************\nlcnt: %d, %d, %d\n**************\n\n", lcnt, lcnt2, lcnt3); 
}

/** @brief Permutation of the elements in vs for FFT 
 *  @param vs the value vector
 *  @param brv the bitreverse lookup table
 *  @param n the size of vs and brv
 *  User-Register: @n >= n
 */
static void bitreverse(int16_t* vs, const int* brv, int n)
{
  int i;
  for(i = 0; i < n; i++)
    {
      /* ai: loop here MAX (@n); */
      int j = brv[i];
      if(i < j)
        {
          int16_t tmp = vs[i];
          vs[i]       = vs[j];
          vs[j]       = tmp;
        }
    }
}


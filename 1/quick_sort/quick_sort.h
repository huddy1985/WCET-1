/*
 * quick_sort.h
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
#include "stdint.h"

//! @brief maximal number of elements quicksort can work with
#define QUICKSORT_MAX_ELEMENTS 64

//! @brief when there are less elements, insertions sort is used
#define INSERTION_SORT_THRESHOLD 7

/** @brief Sort K largest complex numbers in reverse order of their absolute value
  * @param[in]  fft_r the real values (fixed point format)
  * @param[in]  fft_i the imaginary values (fixed point format)
  * @param[out] index_tags_buffer a buffer for storing the indices of the elements in descending frequency order
  * @param      start the first index of fft_r and fft_i of interest
  * @param      end   the last index of  fft_r and fft_i of interest
  * @return     The indices of the complex numbers in reverse order of their absolute value (stored in index_tags_buffer)
  */
int* quick_sort_frequencies(const int16_t *fft_r, const int16_t* fft_i, int* index_tags_buffer, int start, int end);

/*
 * sample_buffer.h
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
#ifndef SAMPLE_BUFFER_H_T1LW1FZN
#define SAMPLE_BUFFER_H_T1LW1FZN

#include "macros.h"
#include "fixedpoint.h"

//! @brief index of the next sample to be stored in the buffer
#define NINDEX(buf)         ((buf->sample_ptr < 0) ? (buf->buf_size+buf->sample_ptr) : (buf->sample_ptr))
#define RINDEX(buf,rindex)  ((NINDEX(buf) + buf->buf_size + rindex) & (buf->buf_size-1))

//! @brief Special value used for missing samples
#define VALUE_MISSING        INT16_MIN

//! @brief Special value for invalid index
#define INVALID_INDEX        INT_MIN

#define IS_VALUE_MISSING(s)  (s == VALUE_MISSING)

typedef fix16_t sample_value_t;

/* Sample Buffer datatype
 * ~~~~~~~~~~~~~~~~~~~~~~
 * phase := (next_ptr < 0) ? INIT : POST_INIT
 * next_sample := (phase == INIT) ? (buf_size-next_ptr) : next_ptr
 * 
 */
typedef struct _struct_sample_buffer
{
  int             buf_size;         //! @brief size of the sample buffer
  sample_value_t* samples;          //! @brief sample data

  int             sample_ptr;       //! @brief storing the index of the next sample
} sample_buffer_t;

/* Library: Sample Buffers */

/** @brief initialize the given sample buffer using the given underlying memory region for storing values
 * @param buf A pointer to a (uninitialized) sample buffer
 * @param buf_storage Pointer to an array with buf_size elements
 * @param buf_size Maximum number of samples in the buffer
 */
void init_sample_buffer(sample_buffer_t* buf, sample_value_t* buf_storage, int buf_size);

/** @brief remove all samples from the sample buffer
 * @param buf A pointer to a sample buffer
 */
void clear_sample_buffer(sample_buffer_t* buf);


/** @brief Get the sample at a position relative to sample_ptr
 * @param buf A pointer to a sample buffer
 * @param rindex The offset (positive or negative) of the element to access, relative to buf->sample_ptr
 */
sample_value_t sample_buffer_get(sample_buffer_t *buf, int rindex);

/** @brief Set a sample at a position relative to sample_ptr
 * @param buf A pointer to a sample buffer
 * @param rindex The offset (positive or negative) of the element to access, relative to buf->sample_ptr
 * @param sample The new value at the given index
 */
void sample_buffer_set(sample_buffer_t *buf, int rindex, sample_value_t sample);

/** @brief Change the sample_ptr of the given buffer
 * @param buf A pointer to a sample buffer
 * @param offset The offset (positive or negative) to move buf->sample_ptr. The absolute value
 *               of offset has to be smaller than buf->buf_size
 */
void sample_buffer_incr_ptr(sample_buffer_t *buf, int offset);

#ifdef DEBUG
void print_sample_buffer(FILE* out, char* header, sample_buffer_t* sbuf);
#endif
#endif /* end of include guard: SAMPLE_BUFFER_H_T1LW1FZN */

/*
 * sample_buffer.c
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
#include "sample_buffer.h"

// ============================
// = Sample Buffer Operations =
// ============================

/** @brief initialize the given sample buffer using the given underlying memory region for storing values
 * @param buf A pointer to a (uninitialized) sample buffer
 * @param buf_storage Pointer to an array with buf_size elements
 * @param buf_size Maximum number of samples in the buffer (power of 2!)
 */
void init_sample_buffer(sample_buffer_t* buf, sample_value_t* buf_storage, int buf_size)
{
  buf->samples = buf_storage;
  buf->buf_size = buf_size;
  clear_sample_buffer(buf);
}

/** @brief remove all samples from the sample buffer
 * @param buf A pointer to a sample buffer
 */
void clear_sample_buffer(sample_buffer_t* buf)
{
  int i;
  for(i = 0; i < buf->buf_size;i++)
  {
    buf->samples[i] = VALUE_MISSING;
  }
  buf->sample_ptr = 0;
}

/** @brief Get the sample at a position relative to sample_ptr
 * @param buf A pointer to a sample buffer
 * @param offset The offset (positive or negative) of the element to access, relative to buf->sample_ptr
 */
sample_value_t sample_buffer_get(sample_buffer_t *buf, int offset)
{
	return buf->samples[RINDEX(buf,offset)];
}

/** @brief Set a sample at a position relative to sample_ptr
 * @param buf A pointer to a sample buffer
 * @param rioffsetndex The offset (positive or negative) of the element to access, relative to buf->sample_ptr
 * @param sample The new value at the given index
 */
void sample_buffer_set(sample_buffer_t *buf, int offset, sample_value_t value)
{
	buf->samples[RINDEX(buf,offset)] = value;
}

/** @brief Change the sample_ptr of the given buffer
 * @param buf A pointer to a sample buffer
 * @param offset The offset (positive or negative) to move buf->sample_ptr. The absolute value
 *               of offset has to be smaller than buf->buf_size
 */
void sample_buffer_incr_ptr(sample_buffer_t *buf, int offset)
{
	buf->sample_ptr = RINDEX(buf,offset);
}

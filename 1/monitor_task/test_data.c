/*
 * test_data.c
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
#include "task.h"
#include "test_data.h"

const signal_spec_t spec2;

check_sin_param_t params1 = { FP_FROM_DOUBLE(2.00), FP_FROM_DOUBLE(1.25), FP_FROM_DOUBLE(1.0) };
const signal_spec_t spec1 = { SINE_SIGNAL, &params1 };
check_square_param_t params2 = { FP_FROM_DOUBLE(1.50), FP_FROM_DOUBLE(0.7), FP_FROM_DOUBLE(0.8) };
const signal_spec_t spec2 = { SQUARE_SIGNAL, &params2 };

#define _M VALUE_MISSING
const sample_value_t TEST_DATA_INTERPOLATE_START = -1189;
sample_value_t TEST_DATA[SAMPLE_COUNT] = {
   _M,  1498,  3890,  3745,  4340,  4569,  5072,  4115,
  4874,  3941,  1214,  -282,   242, -1102, -3112, -2937,
 -3570, -2791,    _M, -1437, -1536,  -346,  1860,  2280,
  4675,  4300,  5051,  6057,  4157,  4836,  2282,  1855,
  1553,     4,  -771, -2167, -3439,    _M, -3527, -2862,
 -2853, -1870,    _M,  2183,  2859,  2726,  4945,  5932,
  4872,  3965,  4493,  4048,  3014,  2145,  -428,  -896,
 -1257, -2808, -2899, -2662, -3261, -1577,    _M,   353
};

sample_value_t TEST_DATA_2[SAMPLE_COUNT] = {
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
  5000,     5000,     5000,     5000,     5000,     5000,     5000,     5000,
  5000,     5000,     5000,     5000,     5000,     5000,     5000,     5000,
     0,     0,     0,     0,     _M,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     _M,
  5000,     5000,     5000,     5000,     5000,     5000,     5000,     5000,
  5000,     5000,     5000,     5000,     5000,     5000,     5000,     5000
};
#undef _M

sample_value_t QS_WORST_CASE[SAMPLE_COUNT] =
{
    /* TODO: test data (bonus problem for assignment 1.1) */
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0
};

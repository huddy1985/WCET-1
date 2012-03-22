/*
 * macros.h
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
#ifndef MACROS_H_EV19QIO5
#define MACROS_H_EV19QIO5

/* fixed size integers */
/* #include <stdint.h> */
#include "stdint.h"
#define __STDC_LIMIT_MACROS
#include <limits.h>

/* Compatibility with ADS compiler */
#define inline

/* debug print */
#ifdef DEBUG
#include <stdio.h>
#define debug(format, args...) fprintf (stderr, format, args)
#else
#define debug(format, args...)
#endif

/* Other useful macros */
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#endif /* end of include guard: MACROS_H_EV19QIO5 */

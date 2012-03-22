/*
 * quick_sort.c
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
#include "quick_sort.h"

/* Static storage for quicksort, as we do not want to
 * deal with malloc in this lab
 */
static uint32_t abs_squared[QUICKSORT_MAX_ELEMENTS];
static int istack[QUICKSORT_MAX_ELEMENTS+1];

/****************************************************************************
 * Sort frequencies by absolute value, in decreasing order.
 *
 * We used this in the benchmark previously, but it just too hard to analyze.
 *
 * Taken from the Maelardalen benchmarks:  http://www.mrtc.mdh.se/projects/wcet/wcet_bench/qsort-exam/qsort-exam.c
 *                  which was taken from: Numerical Recipes in C - The Second Edition
 * Fun Fact: the authors of the Maelardalen benchmark forgot that the algorithm uses 1-based array indices
 *
 ***************************************************************************/

/** @brief Sort K largest complex numbers in reverse order of their absolute value
 * @param[in]  fft_r the real values (fixed point format)
 * @param[in]  fft_i the imaginary values (fixed point format)
 * @param[out] index_tags_buffer a buffer for storing the indices of the elements in descending frequency order
 * @param      start the first index of fft_r and fft_i of interest
 * @param      end   the last index of  fft_r and fft_i of interest
 * @return     The indices of the complex numbers in reverse order of their absolute value (stored in index_tags_buffer)
 *
 * User registers for timing analysis: @size := max (end-start+1)
 */
int* quick_sort_frequencies(const int16_t *fft_r, const int16_t* fft_i, int* index_tags_buffer, int start, int end) {
	int*arr = index_tags_buffer;
	uint32_t tmp;

#define QS_SWAP(a,b) tmp=arr[a];arr[a]=arr[b];arr[b]=tmp;
#define QS_VAL(x)    abs_squared[arr[x]]

	int i, j, k;

	int right=end;
	int left=start;
	int jstack=0;
	int a_key;
	uint32_t a_val;

	if(end >= QUICKSORT_MAX_ELEMENTS) return NULL;

	/* initialize tags and absolute values */
	for (i = 0;
	     i <= end; /* loop here MAX @size; */
	     i++)
    {
		int32_t real = fft_r[i];
		int32_t imag = fft_i[i];
		abs_squared[i] = ((uint32_t)(real*real) + (uint32_t)(imag*imag));
		arr[i] = i;
	}
	/* while(there is an open sub problem) */
	/* Maximum number of subproblems: TODO (bonus problem) */
	for (;;) /* TODO: loop here (bonus problem) */
	{
		/* Base Problem (less than INSERTION_SORT_THRESHOLD elements) */
		if (right-left < INSERTION_SORT_THRESHOLD)
        {
            /* TODO: flow (here) (bonus problem) */
			/* Run insertion sort if there are less than INSERTION_SORT_THRESHOLD elements */
			for (j=left+1;
			     j<=right; /* TODO: loop here (bonus problem) */
			     j++)
            {
				a_key=arr[j];
				a_val=abs_squared[a_key];
				for (i = j-1;
				     i >= left; /* TODO: loop here (bonus problem) */
				     i--)
                {
					if (QS_VAL(i) >= a_val)
						break;

					/* TODO: flow (here) (bonus problem) */
					arr[i+1]=arr[i];
				}
				arr[i+1]=a_key;
			}

			/* Next subproblem */
			if (jstack == 0)
				break;
			right = istack[jstack--];
			left = istack[jstack--];
		}
        /* Recursive Problem: partition and update stack */
        else
        {
			/* TODO: flow (here) (bonus problem) */

			/* Select Pivot (median of three: l,(l+right) >> 1,right) */
			k = (left+right) >> 1;
			QS_SWAP(k,left+1); /* pivot is in (l+1) */
			if (QS_VAL(left) < QS_VAL(right))
				QS_SWAP(left,right);         /* max(l,right) | pivot | min(l,right) */
			if (QS_VAL(left+1) < QS_VAL(right)) {
				QS_SWAP(left+1,right);       /* max(l,right) | max(l+1,right) | min(l,l+1,right) */
			}
			if (QS_VAL(left) < QS_VAL(left+1)) {
				QS_SWAP(left,left+1);        /*  max(l,l+1,right) | median | min(l,l+1,right) */
			}
			/* We now have val(l) >= val(l+1) >= val(right) */
			i=left+1;               /* first element of array */
			j=right;                /* last element of  array */
			a_key = arr[left+1];    /* pivot key */
			a_val = QS_VAL(left+1); /* pivot value */

			/* Partition */
			for (;;)  /* ai: loop here MAX (@size);  */
            {
				/* Find next index ix, scanning from start, s.t. val(ix) <= pivot [guarantueed for last] */

				/* TODO: flow (here) (bonus problem) */
				i++;
				while (QS_VAL(i) > a_val) /* TODO: loop here (bonus problem) */
				{
					/* TODO: flow (here) (bonus problem) */
					i++;
				}
				// Find previous index ix, scanning from end, s.t. val(ix) >= pivot [guaranteed for first]
				j--;
				while (QS_VAL(j) < a_val) /* TODO: loop here (bonus problem) */
				{
					/* TODO: flow (here) (bonus problem) */
					j--;
				}
				if (j < i)
					break;
				QS_SWAP(i,j);
			}
			arr[left+1]=arr[j];
			arr[j]=a_key;
            
			/* Now we have for each index ix:
             *   ix >= j ==> val(i) <= pivot
			 *   ix <  j ==> val(i) >= pivot
             */

			/* Stack management: 'Recursive' Invocations  */
			jstack += 2;

			if (right-i+1 >= j-left)
            {
				istack[jstack]=right;
				istack[jstack-1]=i;
				right=j-1;
			}
            else
            {
				istack[jstack]=j-1;
				istack[jstack-1]=left;
				left=i;
			}
		}
	}

#undef QS_VAL
#undef QS_SWAP
	return &index_tags_buffer[0];
}

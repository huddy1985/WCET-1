/*
 * insertion_sort.c
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

/* required register values: R1 */
void insertion_sort(int*arr, int size)
{
  /* ai: instruction "insertion_sort" is entered with @size = "%i0"; */

  int i,j,v;
  j = 1;
  
  
  while (j < size) 
    {
    	/* ai: LABEL here = "outerloop"; */
    	/* ai: loop here MAX (@size-1); */
      v=arr[j];
      
      i = j - 1;

      
      while (i >= 0)
        {
        	/* ai: LABEL here = "innerloop"; */
        	/* ai: loop here MAX (@size-2); */
          /* 'loop bound' relative to insertion_sort' */      
          
          /* ai?: flow (here) <= (@size * (@size-1) / 2) ("insertion_sort"); */
          /* ai?: flow (here) <= 496 ("insertion_sort"); */
          if(arr[i]<v) break;
          arr[i+1]=arr[i];
          /* ai: loop here MAX (@size-2); */
          i = i - 1;
        }

      arr[i+1]=v;
      /* ai: loop here MAX (@size-1); */
      j = j + 1;
    }
}

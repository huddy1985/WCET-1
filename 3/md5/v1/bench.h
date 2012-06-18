/* 
 * LEON3 timing measurement utilities.
 * Currently restricted to measuring 32-bit cylce counts
 * -----------------------------------------------------
 * (c) 2011, Benedikt Huber <benedikt@vmars.tuwien.ac.at>. All Rights Reserved.
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include "config.h"

/* amba.h has inline function _definitions_ - needs -fgnu89-inline */
#include <asm-leon/amba.h>

/* measurement uncertainty = timer granularity - 1 */
#define TIMER_GRANULARITY 4
#define MDELTA            (TIMER_GRANULARITY - 1)

/* global for the start and the end of the timing measurement */
extern unsigned long start, end;

/* global storing the estimated measurement overhead */
extern unsigned long estimated_overhead;


/* 
 * Prototypes
 * ----------
 */

/* initialize timer */
void init_benchmarks(int verbose);

/* flush the instruction cache, blocking until it is ready again */
void flush_icache_blocking(void);


/* 
 * Measurement Macros
 * ------------------
 */
#define MEASURE_START() do{                           \
    LEON3_GpTimer_Regs->e[0].ctrl |= LEON3_GPTIMER_LD;\
    start    = LEON3_GpTimer_Regs->e[0].val; } while(0)

#define MEASURE_STOP()  do{                           \
    end = LEON3_GpTimer_Regs->e[0].val;               \
    } while (0)

/* First call MEASURE_START, then MEASURE_STOP(). Now
   ELASPED_CYCLES() gives you the number of cycles needed
   measured */
#define ELAPSED_CYCLES()   (((start-end)*TIMER_GRANULARITY)+MDELTA)

#define MEASURE_OVERHEAD() (estimated_overhead)


/*
 * Repeating Measurements
 * ----------------------
 */


/* Note: f() has to call MEASURE_START and MEASURE_STOP!
   This convenience function calls f() 'repeats' times,
   and collects the number of cycles in the first iteration
   (coldt), maximum number of cycles (maxt) in subsequent iterations,
   and the minimum number of cycles (mint) in any iteration */
void repeat_measurements(void (*f)(), int repeats,
                         unsigned long *coldt,
                         unsigned long *mint, unsigned long *maxt);



/*
 * Internal (or, advanced) API
 * ---------------------------
 */
void init_timer(void);
void bench_overhead(void);
void bench_overhead_call(void);
void do_nothing(void);
void disable_icache_bursts(void);

#define OVERHEAD_TEST_RUNS 64

#endif



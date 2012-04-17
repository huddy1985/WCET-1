/* 
 * LEON3 timing measurement utilities.
 * -----------------------------------
 * (c) 2011, Benedikt Huber <benedikt@vmars.tuwien.ac.at>. All Rights Reserved.
 */

#include <stdio.h>
#include "config.h"
#include <asm-leon/amba.h>
#include <asm-leon/timer.h>
#include <asm-leon/leon.h>
#include "bench.h"

/* Cache control registers */
#define CCR_DP 14
#define CCR_IP 15
#define CCR_FI 21
#define CCR_FD 22

/* Global variables used in assembler timing routines */
unsigned long start, end;
unsigned long estimated_overhead;

/* Missing prototype in leon headers */
void amba_init(void);

/* flush the instruction cache (if present in config.h) */
void flush_icache_blocking()
{
#ifdef HAS_ICACHE

  /* The easy way, if we do not care what is flushed, and do not need
     blocking */
  /*
  asm("flush");
  */

  /* Does not work as expected (broken lib?) */
  // sparc_leon23_icache_flush();

  /* Does not work as expected (broken lib?),
   * but the general idea is correct. */

  /*
  LEON_REG.Cache_Control |= (1 << CCR_FI);
  while(LEON_REG.Cache_Control & (1 << CCR_IP));
  */

  /* Unfortunately, need to do everything in assembler... */

  /* (i) set bit FI (21) by writing (1<<21) to 0 in ASI 2 (Cache Control Register)*/
  volatile int reg;
  unsigned mask = (1<<21);
  asm volatile("lda [%%g0] 2, %%g1 \n"   /* g1 = *CCR; */
               "or  %%g1, %0, %%g1 \n"   /* g1 = *CCR | (1<<21) */      
               "sta %%g1, [%%g0] 2"      /* *CCR = g1 */
             :           /* output */
             :"r" (mask) /* input */
             :"%g1"      /* clobbered register */
             );       

  /* (ii) block while cache is disabled due to cache flush */
  /*      to this end, check bit IP (15) of the CCR */
  do {
    asm volatile ("lda [%%g0] 2, %0\n"
         : "=r"(reg)  /* output */
         );
  } while(reg & (1 << CCR_IP));
  
#endif
}

void init_benchmarks(int verbose)
{
  init_timer();

  if(verbose)
  {
    printf("Due to prescaling, we have cycles = %d * ticks +- %d\n",
           TIMER_GRANULARITY, MDELTA); 
  }

#ifdef DISABLE_ICACHE_BURST
  disable_icache_bursts();
#endif

  /* Estimated Overhead -> minimum measured + MDELTA (3) */
  unsigned long mint,maxt,coldt;
  repeat_measurements(&bench_overhead, OVERHEAD_TEST_RUNS, &coldt, &mint, &maxt);

  estimated_overhead = mint - MDELTA;

  if(verbose)
  {
    printf("Assumed Overhead : %ld (max:%ld, cold: %ld)\n",
           estimated_overhead, maxt + MDELTA, coldt + MDELTA);
  }
  
  repeat_measurements(&bench_overhead_call, OVERHEAD_TEST_RUNS, &coldt, &mint, &maxt);
  if(verbose)
  {
    printf("Assumed Overhead (with function call): %ld (max:%ld, cold: %ld)\n",
           mint + MDELTA, maxt + MDELTA, coldt + MDELTA);
  }
}

/* Usually, we also want to disable instruction cache bursts,
   because they are not supported by aiT */
void disable_icache_bursts(void)
{
  unsigned mask = ~(1<<16);
  asm volatile("lda [%%g0] 2,   %%g1 \n"   /* g1 = *CCR; */
               "and  %%g1, %0, %%g1 \n"    /* g1 = *CCR & o7 */
               "sta %%g1, [%%g0] 2"        /* *CCR = g1 */
             :              /* output */
             : "r"(mask)    /* input */
             :"%g1"         /* clobbered register */
             );
}

/* Initialize Timer */
void init_timer()
{
  amba_init();

  /* Set prescaler to 4 (minimal value if 2 timers are used) */
  LEON3_GpTimer_Regs->scalar_reload = 3;
  LEON3_GpTimer_Regs->scalar = 0; 

  /* Set timer 0, so it underflows every 10 second */
  LEON3_GpTimer_Regs->e[0].rld = FREQ * 10 / 4;
  LEON3_GpTimer_Regs->e[0].ctrl = 
	LEON3_GPTIMER_EN |
        LEON3_GPTIMER_RL;

  /* Set timer 1 chained with timer 0 */
  LEON3_GpTimer_Regs->e[1].rld = 0xFFFFFFFF;
  LEON3_GpTimer_Regs->e[1].ctrl = 
	LEON3_GPTIMER_EN |
        0x20             | /* chain */
        LEON3_GPTIMER_RL;  /* rs */
  
  /* no interrupts: LEON3_GPTIMER_IRQEN; */
  LEON3_GpTimer_Regs->e[1].ctrl |= LEON3_GPTIMER_LD;
}

/* Repeat measurements
   Note: f() has to call MEASURE_START and MEASURE_STOP!
   This convenience function calls f() 'repeats' times,
   and collects the number of cycles in the first iteration
   (coldt), maximum number of cycles (maxt) in subsequent iterations,
   and the minimum number of cycles (mint) in any iteration */
void repeat_measurements(void (*f)(), int repeats,
                         unsigned long *coldt_out,
                         unsigned long *mint_out, unsigned long *maxt_out)
{
  int i;
  unsigned long coldt = 0;
  unsigned long mint  = 0xFFFFFFFF;
  unsigned long maxt  = 0;
  unsigned long tmp;
  for(i = 0; i < repeats; i++)
  {
    f();
    tmp = ELAPSED_CYCLES();
    if(tmp<mint) mint=tmp;
    if(i != 0)  /* Exclude cold cache for maxt */
    {
      if(tmp>maxt) maxt=tmp;
    }
    else 
    {
      coldt=tmp;
    }
  }
  *mint_out = mint;
  *maxt_out = maxt;
  *coldt_out = coldt;
}

void bench_overhead(void)
{
  MEASURE_START();
  MEASURE_STOP();
  return;
}

__attribute__((noinline))
void do_nothing() 
{
  asm volatile("nop");
}

void bench_overhead_call(void)
{
  MEASURE_START();
  do_nothing();
  MEASURE_STOP();
}

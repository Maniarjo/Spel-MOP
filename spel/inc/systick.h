#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

// SysTick base address
#define STK_BASE 0xE000F000

// SysTick pointer macros
#define STK_CTLR  ((volatile uint32_t *)(STK_BASE + 0x00))
#define STK_SR    ((volatile uint32_t *)(STK_BASE + 0x04))
#define STK_CNTLR ((volatile uint32_t *)(STK_BASE + 0x08))
#define STK_CNTHR ((volatile uint32_t *)(STK_BASE + 0x0C))
#define STK_CMPLR ((volatile uint32_t *)(STK_BASE + 0x10))
#define STK_CMPHR ((volatile uint32_t *)(STK_BASE + 0x14))

// SysTick struct and accompanying pointer
typedef struct {
  uint32_t CTLR;
  uint32_t SR;
  uint32_t CNTLR;
  uint32_t CNTHR;
  uint32_t CMPLR;
  uint32_t CMPHR;
} STK_t;

#define STK ((volatile STK_t *)STK_BASE)

// Function prototypes

// Start repeated SysTick interrupts with given periodicity in microseconds
void systick_periodic_micro(uint32_t us);

// Stop SysTick
void systick_stop();

#endif

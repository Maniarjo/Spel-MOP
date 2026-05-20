#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>

// EXTI base address
#define EXTI_BASE 0x40010400

// EXTI pointer macros
#define EXTI_INTENR ((volatile uint32_t *)(EXTI_BASE + 0x00))
#define EXTI_EVENR  ((volatile uint32_t *)(EXTI_BASE + 0x04))
#define EXTI_RTENR  ((volatile uint32_t *)(EXTI_BASE + 0x08))
#define EXTI_FTENR  ((volatile uint32_t *)(EXTI_BASE + 0x0C))
#define EXTI_SWIEVR ((volatile uint32_t *)(EXTI_BASE + 0x10))
#define EXTI_INTFR  ((volatile uint32_t *)(EXTI_BASE + 0x14))

// EXTI struct and accompanying pointer
typedef struct {
  uint32_t INTENR;
  uint32_t EVENR;
  uint32_t RTENR;
  uint32_t FTENR;
  uint32_t SWIEVR;
  uint32_t INTFR;
} EXTI_t;

#define EXTI ((volatile EXTI_t *)EXTI_BASE)

#endif

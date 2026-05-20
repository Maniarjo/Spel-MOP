#ifndef AFIO_H
#define AFIO_H

#include <stdint.h>

// AFIO base address
#define AFIO_BASE 0x40010000

// AFIO pointer macros
#define AFIO_ECR     ((volatile uint32_t *)(AFIO_BASE + 0x00))
#define AFIO_PCFR    ((volatile uint32_t *)(AFIO_BASE + 0x04))
#define AFIO_EXTICR1 ((volatile uint16_t *)(AFIO_BASE + 0x08))
#define AFIO_EXTICR2 ((volatile uint16_t *)(AFIO_BASE + 0x0C))
#define AFIO_EXTICR3 ((volatile uint16_t *)(AFIO_BASE + 0x10))
#define AFIO_EXTICR4 ((volatile uint16_t *)(AFIO_BASE + 0x14))
#define AFIO_PCFR2   ((volatile uint32_t *)(AFIO_BASE + 0x18))

// AFIO struct and accompanying pointer
typedef struct {
  uint32_t ECR;
  uint32_t PCFR;
  uint16_t EXTICR1;
  uint16_t : 16;
  uint16_t EXTICR2;
  uint16_t : 16;
  uint16_t EXTICR3;
  uint16_t : 16;
  uint16_t EXTICR4;
  uint16_t : 16;
  uint32_t PCFR2;
} AFIO_t;

#define AFIO ((volatile AFIO_t *)AFIO_BASE)

#endif

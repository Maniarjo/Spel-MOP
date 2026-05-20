#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// GPIO base addresses
#define GPIOD_BASE 0x40011400
#define GPIOE_BASE 0x40011800

// GPIO pointer macros
#define GPIOD_CFGLR ((volatile uint32_t *)(GPIOD_BASE + 0x00))
#define GPIOD_CFGHR ((volatile uint32_t *)(GPIOD_BASE + 0x04))
#define GPIOD_IDR   ((volatile uint16_t *)(GPIOD_BASE + 0x08))
#define GPIOD_ODR   ((volatile uint16_t *)(GPIOD_BASE + 0x0C))
#define GPIOD_BSR   ((volatile uint16_t *)(GPIOD_BASE + 0x10))
#define GPIOD_BCR   ((volatile uint16_t *)(GPIOD_BASE + 0x14))
#define GPIOD_LCKR  ((volatile uint32_t *)(GPIOD_BASE + 0x18))

#define GPIOE_CFGLR ((volatile uint32_t *)(GPIOE_BASE + 0x00))
#define GPIOE_CFGHR ((volatile uint32_t *)(GPIOE_BASE + 0x04))
#define GPIOE_IDR   ((volatile uint16_t *)(GPIOE_BASE + 0x08))
#define GPIOE_ODR   ((volatile uint16_t *)(GPIOE_BASE + 0x0C))
#define GPIOE_BSR   ((volatile uint16_t *)(GPIOE_BASE + 0x10))
#define GPIOE_BCR   ((volatile uint16_t *)(GPIOE_BASE + 0x14))
#define GPIOE_LCKR  ((volatile uint32_t *)(GPIOE_BASE + 0x18))

// GPIO struct and accompanying pointers
typedef struct {
  uint32_t CFGLR;
  uint32_t CFGHR;
  uint16_t IDR;
  uint16_t : 16;
  uint16_t ODR;
  uint16_t : 16;
  uint16_t BSR;
  uint16_t : 16;
  uint16_t BCR;
  uint16_t : 16;
  uint32_t LCKR;
} GPIO_t;

#define GPIOD ((volatile GPIO_t *)GPIOD_BASE)
#define GPIOE ((volatile GPIO_t *)GPIOE_BASE)

// Function prototypes

// Initialize GPIO
void init_gpio();

#endif

#ifndef PFIC_H
#define PFIC_H

#include <stdint.h>

// PFIC base address
#define PFIC_BASE 0xE000E000

// PFIC pointer macro
#define PFIC_IENR ((volatile uint32_t *)(PFIC_BASE + 0x100))

// Function macros

// Enable given IRQ no.
#define IENR_SET(irqn) PFIC_IENR[irqn / 32] |= (1 << (irqn % 32))

// Disable given IRQ no.
#define IENR_CLR(irqn) PFIC_IENR[irqn / 32] &= ~(1 << (irqn % 32))

#endif

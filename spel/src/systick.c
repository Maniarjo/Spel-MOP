#include "systick.h"
#include <stdint.h>

void systick_periodic_micro(uint32_t us) {
  STK->CTLR = 0;
  STK->CMPLR = 144 * us - 1;
  // set:
  // - INIT  (bit 5)
  // - STRE  (bit 3)
  // - STCLK (bit 2)
  // - STIE  (bit 1)
  // - STE   (bit 0)
  STK->CTLR = (1 << 5) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);
}

void systick_stop() { STK->CTLR = 0; }

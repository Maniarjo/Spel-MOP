#include "systick.h"
#include <stdint.h>

void systick_periodic_micro(uint32_t us) {
  // stoppa systick först
  STK->CTLR = 0;
  
  // beräkna reload value
  // CPU clock = 144 MHz, så 144 cycles = 1 µs
  STK->CMPLR = 144 * us - 1;
  
  // configure CTLR bits
  // bit 5: INIT   = load counter från CMPLR
  // bit 3: STRE   = enable systick reload
  // bit 2: STCLK  = use core clock (not AHB/8)
  // bit 1: STIE   = enable interrupt
  // bit 0: STE    = enable timer
  STK->CTLR = (1 << 5) |  // INIT
              (1 << 3) |  // STRE
              (1 << 2) |  // STCLK
              (1 << 1) |  // STIE (interrupt)
              (1 << 0);   // STE (enable)
}

void systick_stop() {
  // stäng av timern
  STK->CTLR = 0;
}

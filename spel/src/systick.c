#include "systick.h"
#include <stdint.h>

void systick_periodic_micro(uint32_t us) {
  if (us == 0) {
    systick_stop();
    return;
  }

  STK->CTLR = 0;
  STK->CMPLR = 144 * us - 1;
  STK->CTLR = (1 << 5) |
              (1 << 3) |
              (1 << 2) |
              (1 << 1) |
              (1 << 0);
}

void systick_stop(void) {
  STK->CTLR = 0;
}

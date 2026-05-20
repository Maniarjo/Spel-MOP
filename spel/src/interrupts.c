#include "afio.h"
#include "exti.h"
#include "pfic.h"

void init_interrupts() {
  // Enable systick interrupts
  IENR_SET(12);
  // Enable EXTI{0,1,2,3}
  IENR_SET(22);
  IENR_SET(23);
  IENR_SET(24);
  IENR_SET(25);
  // Connect EXTI{0,1,2,3} to port D
  AFIO->EXTICR1 = 0x3333;
  // Use rising and falling triggers on lines {0,1,2,3}
  EXTI->RTENR |= 0xF << 0;
  EXTI->FTENR |= 0xF << 0;
  // Enable interrupts on lines {0,1,2,3}
  EXTI->INTENR |= 0xF << 0;
}

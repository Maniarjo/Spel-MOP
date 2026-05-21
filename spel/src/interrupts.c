#include "afio.h"
#include "exti.h"
#include "pfic.h"

void init_interrupts(void) {
  IENR_SET(12);

  IENR_SET(22);
  IENR_SET(23);
  IENR_SET(24);
  IENR_SET(25);

  AFIO->EXTICR1 = 0x3333;

  EXTI->INTENR &= ~0xFU;
  EXTI->RTENR &= ~0xFU;
  EXTI->FTENR &= ~0xFU;
  EXTI->INTFR = 0xFU;

  EXTI->RTENR |= 0xFU;
  EXTI->FTENR |= 0xFU;
  EXTI->INTENR |= 0xFU;
}

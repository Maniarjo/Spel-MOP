#include "afio.h"
#include "exti.h"
#include "pfic.h"

void init_interrupts() {
  // enable systick interrupt
  // IENR_SET(12) = syscall interrupt (används för systick)
  IENR_SET(12);
  
  // enable externa interrupts
  // EXTI{0,1,2,3} = keypad interrupt lines
  IENR_SET(22);     // EXTI0
  IENR_SET(23);     // EXTI1
  IENR_SET(24);     // EXTI2
  IENR_SET(25);     // EXTI3
  
  // map EXTI lines till GPIOD
  // 0x3333 = all lines connected to port D
  AFIO->EXTICR1 = 0x3333;
  
  // trigger setup: rising AND falling edge
  // (för både knapptryck och release)
  EXTI->RTENR |= 0xF << 0;  // rising edge
  EXTI->FTENR |= 0xF << 0;  // falling edge
  
  // enable interrupts på lines 0-3
  EXTI->INTENR |= 0xF << 0;
}

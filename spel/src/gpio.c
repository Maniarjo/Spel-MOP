#include "gpio.h"

void init_gpio() {
  // GPIOD [0:7] keypad
  // - cols [0:3] (input, pull-up)
  // - rows [4:7] (output, open drain, 2 MHz)
  GPIOD->CFGLR = 0x66668888;
  GPIOD->BSR = 0xF << 0;
  // Activate all rows
  GPIOD->BCR = 0xF << 4;
  // GPIOE [0] buzzer (output, push/pull, 2 MHz)
  GPIOE->CFGLR &= ~(0xF << 0);
  GPIOE->CFGLR |=  (0x2 << 0);
  // GPIOD [8:15] bargraph (output, push/pull, 2 MHz)
  GPIOD->CFGHR = 0x22222222;
  // turn off all leds
  GPIOD->BCR = 0xFF << 8;
}

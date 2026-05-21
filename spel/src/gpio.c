#include "gpio.h"

void init_gpio() {
  // keypad setup
  // GPIOD [0:7] = keypad
  // cols [0:3] = input pull-up
  // rows [4:7] = output open-drain
  GPIOD->CFGLR = 0x66668888;    // sätt config för cols
  GPIOD->BSR = 0xF << 0;        // pull-up på cols
  GPIOD->BCR = 0xF << 4;        // aktivera alla rows
  
  // buzzer
  // GPIOE [0] = buzzer (output push/pull)
  GPIOE->CFGLR &= ~(0xF << 0);  // rensa bits
  GPIOE->CFGLR |=  (0x2 << 0);  // sätt output mode
  
  // LED bar
  // GPIOD [8:15] = bargraph LEDs (output push/pull)
  GPIOD->CFGHR = 0x22222222;    // config alla LEDs som outputs
  GPIOD->BCR = 0xFF << 8;       // släck alla LEDs från start
}

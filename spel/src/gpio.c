#include "gpio.h"

void init_gpio(void) {
  *RCC_APB2PCENR |= RCC_AFIOEN | RCC_IOPDEN | RCC_IOPEEN;

  // Keypad on GPIOD[0:7]:
  // columns D0-D3 = input pull-up, rows D4-D7 = output open-drain.
  GPIOD->CFGLR = 0x66668888;
  GPIOD->BSR = KEYPAD_COL_MASK;
  GPIOD->BCR = KEYPAD_ROW_MASK;

  // Buzzer on GPIOE[0].
  GPIOE->CFGLR &= ~(0xF << 0);
  GPIOE->CFGLR |= (0x2 << 0);
  GPIOE->BCR = BUZZER_MASK;

  // LED bar on GPIOD[8:15].
  GPIOD->CFGHR = 0x22222222;
  GPIOD->BCR = LED_MASK;
}

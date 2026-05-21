#include "gpio.h"
#include "keypad.h"

uint8_t keypad(void) {
  int row;
  int col;
  uint16_t idr;

  GPIOD->BSR = KEYPAD_ROW_MASK;

  for (row = 0; row < 4; row++) {
    GPIOD->BCR = 1U << (row + 4);

    for (volatile int i = 0; i < 100; i++) {
    }

    idr = GPIOD->IDR;

    for (col = 0; col < 4; col++) {
      if ((idr & (1U << col)) == 0) {
        GPIOD->BCR = KEYPAD_ROW_MASK;
        return (uint8_t)(4 * row + col);
      }
    }
  }

  GPIOD->BCR = KEYPAD_ROW_MASK;
  return 0xFF;
}

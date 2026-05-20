#include "gpio.h"
#include "keypad.h"

uint8_t keypad() {
  int row;
  int col;
  uint16_t idr;
  // Deactivate all rows
  GPIOD->BSR = 0xF << 4;
  // For each row:
  for (row = 0; row < 4; row++) {
    // Activate row
    GPIOD->BCR = 1 << (row + 4);
    // pin update speed set to 2 MHz
    // => need to wait for 500 ns before reading columns
    // but can't overwrite systick, so we'll waste time with an empty loop
    for (int i = 0; i < 100; i++) {
    }
    // For each column:
    // if column is active, activate all rows and return key index
    idr = GPIOD->IDR;
    for (col = 0; col < 4; col++) {
      if ((idr & (1 << col)) == 0) {
        GPIOD->BCR = 0xF << 4;
        return 4 * row + col;
      }
    }
  }
  // No key pressed. Activate all rows and return error code
  GPIOD->BCR = 0xF << 4;
  return 0xFF;
}

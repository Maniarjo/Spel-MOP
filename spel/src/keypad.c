#include "gpio.h"
#include "keypad.h"

uint8_t keypad() {
  int row;
  int col;
  uint16_t idr;
  
  // släck alla rows (pull-up aktiv)
  GPIOD->BSR = 0xF << 4;
  
  // scanna varje row
  for (row = 0; row < 4; row++) {
    // aktivera denna row
    GPIOD->BCR = 1 << (row + 4);
    
    // vänta på GPIO settling (500 ns)
    // 2 MHz = 500 ns per cycle, så ca 100 iterationer
    for (int i = 0; i < 100; i++) {
      // waste cycles
    }
    
    // läs kolumner
    idr = GPIOD->IDR;
    
    // scanna varje kolumn
    for (col = 0; col < 4; col++) {
      // kolumn är LOW om knappen trycks (pull-up)
      if ((idr & (1 << col)) == 0) {
        GPIOD->BCR = 0xF << 4;  // släck alla rows igen
        return 4 * row + col;   // returnera knapp-ID
      }
    }
  }
  
  // ingen knapp tryckt
  GPIOD->BCR = 0xF << 4;  // släck alla rows
  return 0xFF;            // error code
}

#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

// Function prototypes

// Read keypad (GPIOD [0:7])
// Returns key index if a key is pressed, otherwise error code 0xFF
uint8_t keypad();

#endif

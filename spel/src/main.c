#include "tftmd307.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// In Simserver, connect 
// GPIO E(0-15)/SPI Display -> 18 LC/TFT Display

int main(void) {
    // Initialize the display
    tft_init(TFT_INIT_ILI9488); 
    
    // Clear the screen with black color
    tft_lcd_rect(0, 0, LCD_W - 1, LCD_H - 1, 0x0000, 1); 
}
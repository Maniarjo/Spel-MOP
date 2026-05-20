#include "tftmd307.h"
#include "afio.h"
#include "exti.h"
#include "gpio.h"
#include "interrupts.h"
#include "keypad.h"
#include "music.h"
#include "pfic.h"
#include "systick.h"
#include "vector_table.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// GAME CONFIGURATION
// ============================================================================

#define MAX_SEQUENCE_LENGTH 32
#define TONE_DURATION_MS 300
#define PAUSE_BETWEEN_TONES_MS 100
#define PLAYER_INPUT_TIMEOUT_MS 3000

// ============================================================================
// FREQUENCY LOOKUP TABLE
// ============================================================================

// Period values for each keypad button (in microseconds)
// These create different frequencies when used with systick
uint32_t periods[] = {
  1516, 1431, 1351, 1275,    // Row 0: Buttons 0-3
  1203, 1136, 1072, 1012,    // Row 1: Buttons 4-7
   955,  901,  851,  803,    // Row 2: Buttons 8-11
   758,  715,  675,  637     // Row 3: Buttons 12-15
};

// ============================================================================
// GAME STATE VARIABLES
// ============================================================================

static uint8_t game_sequence[MAX_SEQUENCE_LENGTH];
static uint8_t player_sequence[MAX_SEQUENCE_LENGTH];
static uint8_t sequence_length = 0;
static uint8_t player_pos = 0;
static uint8_t game_over = 0;
static uint32_t remaining_duration = 0;
static uint32_t current_period = 0;
static uint8_t last_key_pressed = 0xFF;

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void display_binary_led(uint8_t value);

// ============================================================================
// INTERRUPTS AND HANDLERS
// ============================================================================

// Handle SysTick interrupt - generates square wave for sound
__attribute__((interrupt("machine")))
void systick_handler() {
  // Invert buzzer pin to create square wave
  *GPIOE_ODR ^= 1;
  
  // Acknowledge interrupt
  *STK_SR &= ~1;
  
  // Decrement remaining duration
  if (remaining_duration > 0) {
    remaining_duration -= current_period;
  }
}

// Handle external keypad interrupt (on press and release)
__attribute__((interrupt("machine")))
void exti_handler() {
  // Acknowledge EXTI interrupt
  *EXTI_INTFR = 0xFF;
  
  // Read keypad
  uint8_t key = keypad();
  
  if (key != 0xFF) {
    // Key pressed - display button number in binary on LEDs
    last_key_pressed = key;
    display_binary_led(key);
  } else {
    // Key released - turn off all LEDs
    display_binary_led(0);
  }
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Display button number in binary on LED bar (GPIOD [8:15])
// Button 0-15 displays as 4-bit binary on the 8 LEDs
// Example: Button 5 (0101 in binary) lights up LEDs [8, 10]
void display_binary_led(uint8_t value) {
  // Clear all LEDs first
  *GPIOD_BCR = 0xFF00;
  // Set LEDs according to binary representation of value
  // Bits [8:15] correspond to binary bits [0:7]
  *GPIOD_BSR = (value << 8);
}

// Play a tone for the given button
void play_tone(uint8_t button, uint32_t duration_ms) {
  if (button >= 16) return;
  
  current_period = periods[button];
  remaining_duration = duration_ms * 1000;  // Convert ms to us
  
  // Display button number in binary on LED bargraph
  display_binary_led(button);
  
  // Start the buzzer
  systick_periodic_micro(current_period);
  
  // Wait for tone to finish
  while (remaining_duration > 0) {
    // Spin wait
  }
  
  // Stop buzzer and clear LED display
  systick_stop();
  display_binary_led(0);  // Turn off all LEDs
}

// Wait for player input with timeout
uint8_t wait_for_player_input(uint32_t timeout_ms) {
  last_key_pressed = 0xFF;
  uint32_t wait_duration = timeout_ms * 1000;
  remaining_duration = wait_duration;
  current_period = 1000;  // 1ms ticks for timeout
  
  // Start timer
  systick_periodic_micro(1000);
  
  while (remaining_duration > 0 && last_key_pressed == 0xFF) {
    // Wait for key press or timeout
  }
  
  systick_stop();
  
  return last_key_pressed;
}

// Display text on screen
void display_text(uint16_t x, uint16_t y, const char *text, uint16_t color) {
  tft_lcd_putstr(x, y, (uint8_t *)text, color, 0x0000);
}

// Clear and setup display
void setup_display() {
  tft_lcd_rect(0, 0, LCD_W - 1, LCD_H - 1, 0x0000, 1);
  display_text(10, 10, "SIMON SAYS", 0xFFFF);
}

// Display game state
void display_game_state() {
  char buffer[50];
  
  // Clear game state area
  tft_lcd_rect(10, 50, LCD_W - 10, LCD_H - 10, 0x0000, 1);
  
  sprintf(buffer, "Level: %d", sequence_length);
  display_text(10, 60, buffer, 0x00FF);  // Green
  
  sprintf(buffer, "Press buttons in order");
  display_text(10, 100, buffer, 0xFFFF);  // White
}

// Display game over message
void display_game_over(uint8_t level) {
  tft_lcd_rect(10, 50, LCD_W - 10, LCD_H - 10, 0x0000, 1);
  
  display_text(10, 80, "GAME OVER", 0xF800);  // Red
  
  char buffer[50];
  sprintf(buffer, "You reached level: %d", level);
  display_text(10, 130, buffer, 0xFFFF);
  
  display_text(10, 180, "Press any key to restart", 0x00FF);
}

// Generate next button in sequence
void add_to_sequence() {
  if (sequence_length < MAX_SEQUENCE_LENGTH) {
    game_sequence[sequence_length] = rand() % 16;
    sequence_length++;
  }
}

// Play the current sequence to the player
void play_sequence() {
  for (uint8_t i = 0; i < sequence_length; i++) {
    // Play tone for this button
    play_tone(game_sequence[i], TONE_DURATION_MS);
    
    // Pause between tones
    remaining_duration = PAUSE_BETWEEN_TONES_MS * 1000;
    current_period = 10000;  // 10ms ticks
    systick_periodic_micro(10000);
    while (remaining_duration > 0) {
      // Wait
    }
    systick_stop();
  }
}

// Get player input sequence and validate
uint8_t get_player_sequence() {
  player_pos = 0;
  
  while (player_pos < sequence_length) {
    // Wait for player input
    uint8_t key = wait_for_player_input(PLAYER_INPUT_TIMEOUT_MS);
    
    if (key == 0xFF) {
      // Timeout - player didn't press in time
      return 0;
    }
    
    // Validate the input
    if (key != game_sequence[player_pos]) {
      // Wrong button pressed
      return 0;
    }
    
    // Play the button tone as feedback
    play_tone(key, TONE_DURATION_MS / 2);
    
    player_pos++;
    
    // Pause between inputs
    remaining_duration = 200000;  // 200ms
    current_period = 1000;
    systick_periodic_micro(1000);
    while (remaining_duration > 0) {
      // Wait
    }
    systick_stop();
  }
  
  return 1;  // Success
}

// ============================================================================
// MAIN GAME LOOP
// ============================================================================

int main(void) {
    // Initialize the display
    tft_init(TFT_INIT_ILI9488); 
    
    // Clear the screen with black color
    tft_lcd_rect(0, 0, LCD_W - 1, LCD_H - 1, 0x0000, 1); 

     init_gpio();
  init_vector_table();
  init_interrupts();
  
  // Initialize systick for timing
  systick_periodic_micro(1);
  
  // Game loop
  while (1) {
    setup_display();
    sequence_length = 0;
    game_over = 0;
    
    // Play until player makes a mistake
    while (!game_over && sequence_length < MAX_SEQUENCE_LENGTH) {
      display_game_state();
      
      // Add new button to sequence
      add_to_sequence();
      
      // Play the sequence to the player
      play_sequence();
      
      // Get player's input
      if (!get_player_sequence()) {
        game_over = 1;
      } else {
        // Success - add a slight pause before next round
        remaining_duration = 1000000;  // 1 second
        current_period = 100000;  // 100ms ticks
        systick_periodic_micro(100000);
        while (remaining_duration > 0) {
          // Wait
        }
        systick_stop();
      }
    }
    
    // Display game over screen
    display_game_over(sequence_length);
    
    // Wait for player to press any key to restart
    last_key_pressed = 0xFF;
    while (last_key_pressed == 0xFF) {
      uint8_t key = keypad();
      if (key != 0xFF) {
        last_key_pressed = key;
      }
    }
  }
  
  return 0;
}

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

// Simon Says game - spelaren upprepar växande knappsekvenser

#define MAX_SEQUENCE_LENGTH 32
#define TONE_DURATION_MS 300
#define PAUSE_BETWEEN_TONES_MS 100
#define PLAYER_INPUT_TIMEOUT_MS 3000

// Knappgrid layout
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 60
#define GRID_START_X 25
#define GRID_START_Y 80
#define BUTTON_SPACING 10

// Frekvenser för varje knapp (lägre = högre ljud)
uint32_t periods[] = {
  1516, 1431, 1351, 1275,
  1203, 1136, 1072, 1012,
   955,  901,  851,  803,
   758,  715,  675,  637
};

// Spelets tillstånd
static uint8_t game_sequence[MAX_SEQUENCE_LENGTH];
static uint8_t player_sequence[MAX_SEQUENCE_LENGTH];
static uint8_t sequence_length = 0;
static uint8_t player_pos = 0;
static uint8_t game_over = 0;
static uint32_t remaining_duration = 0;
static uint32_t current_period = 0;
static uint8_t last_key_pressed = 0xFF;


// Funktionsdeklarationer
extern void tft_lcd_putstr(uint16_t x, uint16_t y, uint8_t *text, uint16_t color, uint16_t bgcolor);

void draw_button(uint8_t button_id, uint16_t color, int filled);
uint16_t get_button_color(uint8_t button_id);
void draw_button_grid(void);
void draw_score(uint8_t level);

// Interrupt handlers
__attribute__((interrupt("machine")))
void systick_handler() {
  *GPIOE_ODR ^= 1;  // invertera buzzer
  *STK_SR &= ~1;    // acknowledge
  if (remaining_duration > 0) {
    remaining_duration -= current_period;
  }
}

__attribute__((interrupt("machine")))
void exti_handler() {
  *EXTI_INTFR = 0xFF;  // rensa interrupt
  uint8_t key = keypad();
  if (key != 0xFF) {
    last_key_pressed = key;
    display_binary_led(key);
  } else {
    display_binary_led(0);
  }
}

// Grafik-funktioner

void draw_button(uint8_t button_id, uint16_t color, int filled) {
  if (button_id >= 16) return;
  
  uint8_t row = button_id / 4;
  uint8_t col = button_id % 4;
  
  int x1 = GRID_START_X + col * (BUTTON_WIDTH + BUTTON_SPACING);
  int y1 = GRID_START_Y + row * (BUTTON_HEIGHT + BUTTON_SPACING);
  int x2 = x1 + BUTTON_WIDTH;
  int y2 = y1 + BUTTON_HEIGHT;
  
  tft_lcd_rect(x1, y1, x2, y2, color, filled);
}

uint16_t get_button_color(uint8_t button_id) {
  uint8_t row = button_id / 4;
  
  switch (row) {
    case 0: return TFT_RED;
    case 1: return TFT_GREEN;
    case 2: return TFT_BLUE;
    case 3: return TFT_YELLOW;
    default: return TFT_WHITE;
  }
}

void draw_button_grid(void) {
  for (uint8_t i = 0; i < 16; i++) {
    uint16_t color = get_button_color(i);
    draw_button(i, color, 1);
  }
}

void draw_score(uint8_t level) {
  char buffer[20];
  sprintf(buffer, "LEVEL: %d", level);
  tft_lcd_putstr(10, 10, (uint8_t *)buffer, TFT_WHITE, TFT_BLACK);
}

// LED och ljud

void display_binary_led(uint8_t value) {
  *GPIOD_BCR = 0xFF00;           // släck alla
  *GPIOD_BSR = (value << 8);     // tända rätt LED
}

void play_tone(uint8_t button, uint32_t duration_ms) {
  if (button >= 16) return;
  
  current_period = periods[button];
  remaining_duration = duration_ms * 1000;
  
  display_binary_led(button);
  systick_periodic_micro(current_period);
  
  while (remaining_duration > 0) {
    // vänta på interrupt
  }
  
  systick_stop();
  display_binary_led(0);
}

uint8_t wait_for_player_input(uint32_t timeout_ms) {
  last_key_pressed = 0xFF;
  remaining_duration = timeout_ms * 1000;
  current_period = 1000;
  
  systick_periodic_micro(1000);
  
  while (remaining_duration > 0 && last_key_pressed == 0xFF) {
    // vänta
  }
  
  systick_stop();
  return last_key_pressed;
}

// Spel-logik

void add_to_sequence(void) {
  if (sequence_length < MAX_SEQUENCE_LENGTH) {
    game_sequence[sequence_length] = rand() % 16;
    sequence_length++;
  }
}

void play_sequence(void) {
  for (uint8_t i = 0; i < sequence_length; i++) {
    play_tone(game_sequence[i], TONE_DURATION_MS);
    
    remaining_duration = PAUSE_BETWEEN_TONES_MS * 1000;
    current_period = 10000;
    systick_periodic_micro(10000);
    while (remaining_duration > 0) {
    }
    systick_stop();
  }
}

uint8_t get_player_sequence(void) {
  player_pos = 0;
  
  while (player_pos < sequence_length) {
    uint8_t key = wait_for_player_input(PLAYER_INPUT_TIMEOUT_MS);
    
    if (key == 0xFF) {
      return 0;  // timeout
    }
    
    if (key != game_sequence[player_pos]) {
      return 0;  // fel knapp
    }
    
    play_tone(key, TONE_DURATION_MS / 2);
    player_pos++;
    
    remaining_duration = 200000;
    current_period = 1000;
    systick_periodic_micro(1000);
    while (remaining_duration > 0) {
    }
    systick_stop();
  }
  
  return 1;  // ok
}

void display_game_over_screen(uint8_t final_level) {
  tft_lcd_rect(0, 0, LCD_W - 1, 60, TFT_BLACK, 1);
  tft_lcd_rect(0, 60, LCD_W - 1, LCD_H - 1, TFT_BLACK, 1);
  
  tft_lcd_putstr(160, 80, (uint8_t *)"GAME OVER", TFT_RED, TFT_BLACK);
  
  char buffer[40];
  sprintf(buffer, "You reached level: %d", final_level);
  tft_lcd_putstr(100, 150, (uint8_t *)buffer, TFT_WHITE, TFT_BLACK);
  
  tft_lcd_putstr(80, 220, (uint8_t *)"Press any key to restart", TFT_CYAN, TFT_BLACK);
}

int main(void) {
  // Spelets beskrivning:
  // - Simon Says: upprepa växande sekvenser av 4x4-knappar
  // - Spelare trycker på knappmatrisen för att repetera sekvensen
  // - Varje knapp ger ton, tänder motsvarande LED och visas grafiskt
  // - Nivå visas uppe; timeout eller fel avslutar spelet (Game Over)
  // - Starta om: tryck valfri knapp på Game Over-skärmen
  // Init allt
  tft_init(TFT_INIT_ILI9488);
  tft_lcd_rect(0, 0, LCD_W - 1, LCD_H - 1, TFT_BLACK, 1);
  
  init_gpio();
  init_vector_table();
  init_interrupts();
  
  systick_periodic_micro(1);
  
  // Huvudloop
  while (1) {
    sequence_length = 0;
    game_over = 0;
    
    tft_lcd_rect(0, 0, LCD_W - 1, LCD_H - 1, TFT_BLACK, 1);
    tft_lcd_putstr(150, 10, (uint8_t *)"SIMON SAYS", TFT_WHITE, TFT_BLACK);
    draw_button_grid();
    
    // Spela tills spelaren gör misstag
    while (!game_over && sequence_length < MAX_SEQUENCE_LENGTH) {
      draw_score(sequence_length);
      add_to_sequence();
      play_sequence();
      
      if (!get_player_sequence()) {
        game_over = 1;
      } else {
        remaining_duration = 1000000;
        current_period = 100000;
        systick_periodic_micro(100000);
        while (remaining_duration > 0) {
        }
        systick_stop();
      }
    }
    
    // Game over
    display_game_over_screen(sequence_length);
    
    // Vänta på restart
    last_key_pressed = 0xFF;
    while (last_key_pressed == 0xFF) {
      uint8_t key = keypad();
      if (key != 0xFF) {
        last_key_pressed = key;
        delay_ms(50);
      }
    }
  }
  
  return 0;
}



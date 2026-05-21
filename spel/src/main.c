#include "exti.h"
#include "gpio.h"
#include "interrupts.h"
#include "keypad.h"
#include "systick.h"
#define DBGCALL
#include "tftmd307.h"
#include "vector_table.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SEQUENCE_LENGTH 32
#define NO_KEY 0xFFU

#define TONE_DURATION_MS 300U
#define PAUSE_BETWEEN_TONES_MS 100U
#define PLAYER_INPUT_TIMEOUT_MS 3000U

#define BUTTON_WIDTH 90
#define BUTTON_HEIGHT 45
#define GRID_START_X 45
#define GRID_START_Y 92
#define BUTTON_SPACING 10

#ifndef LCD_W
#define LCD_W 480
#define LCD_H 320
#endif

#define FONT_WIDTH 5
#define FONT_HEIGHT 7

static const uint32_t periods[16] = {
  1516, 1431, 1351, 1275,
  1203, 1136, 1072, 1012,
   955,  901,  851,  803,
   758,  715,  675,  637
};

static const uint8_t digit_glyphs[10][FONT_WIDTH] = {
  {0x3E, 0x51, 0x49, 0x45, 0x3E},
  {0x00, 0x42, 0x7F, 0x40, 0x00},
  {0x42, 0x61, 0x51, 0x49, 0x46},
  {0x21, 0x41, 0x45, 0x4B, 0x31},
  {0x18, 0x14, 0x12, 0x7F, 0x10},
  {0x27, 0x45, 0x45, 0x45, 0x39},
  {0x3C, 0x4A, 0x49, 0x49, 0x30},
  {0x01, 0x71, 0x09, 0x05, 0x03},
  {0x36, 0x49, 0x49, 0x49, 0x36},
  {0x06, 0x49, 0x49, 0x29, 0x1E}
};

static const uint8_t letter_glyphs[26][FONT_WIDTH] = {
  {0x7E, 0x11, 0x11, 0x11, 0x7E},
  {0x7F, 0x49, 0x49, 0x49, 0x36},
  {0x3E, 0x41, 0x41, 0x41, 0x22},
  {0x7F, 0x41, 0x41, 0x22, 0x1C},
  {0x7F, 0x49, 0x49, 0x49, 0x41},
  {0x7F, 0x09, 0x09, 0x09, 0x01},
  {0x3E, 0x41, 0x49, 0x49, 0x7A},
  {0x7F, 0x08, 0x08, 0x08, 0x7F},
  {0x00, 0x41, 0x7F, 0x41, 0x00},
  {0x20, 0x40, 0x41, 0x3F, 0x01},
  {0x7F, 0x08, 0x14, 0x22, 0x41},
  {0x7F, 0x40, 0x40, 0x40, 0x40},
  {0x7F, 0x02, 0x0C, 0x02, 0x7F},
  {0x7F, 0x04, 0x08, 0x10, 0x7F},
  {0x3E, 0x41, 0x41, 0x41, 0x3E},
  {0x7F, 0x09, 0x09, 0x09, 0x06},
  {0x3E, 0x41, 0x51, 0x21, 0x5E},
  {0x7F, 0x09, 0x19, 0x29, 0x46},
  {0x46, 0x49, 0x49, 0x49, 0x31},
  {0x01, 0x01, 0x7F, 0x01, 0x01},
  {0x3F, 0x40, 0x40, 0x40, 0x3F},
  {0x1F, 0x20, 0x40, 0x20, 0x1F},
  {0x3F, 0x40, 0x38, 0x40, 0x3F},
  {0x63, 0x14, 0x08, 0x14, 0x63},
  {0x07, 0x08, 0x70, 0x08, 0x07},
  {0x61, 0x51, 0x49, 0x45, 0x43}
};

static const uint8_t blank_glyph[FONT_WIDTH] = {0, 0, 0, 0, 0};
static const uint8_t colon_glyph[FONT_WIDTH] = {0x00, 0x36, 0x36, 0x00, 0x00};

static uint8_t game_sequence[MAX_SEQUENCE_LENGTH];
static uint8_t sequence_length = 0;
static uint8_t game_over = 0;

static volatile uint32_t remaining_duration = 0;
static volatile uint32_t current_period = 0;
static volatile uint8_t buzzer_enabled = 0;
static volatile uint8_t last_key_pressed = NO_KEY;

static void init_display(void);
static void lcd_puts(int x, int y, const char *text, uint16_t color, uint16_t bgcolor, int scale);
static void draw_button(uint8_t button_id, uint16_t color, int filled);
static uint16_t get_button_color(uint8_t button_id);
static void draw_button_grid(void);
static void draw_score(uint8_t level);
static void display_binary_led(uint8_t value);
static void start_countdown(uint32_t duration_us, uint32_t tick_us, uint8_t sound);
static void wait_countdown_done(void);
static void delay_game_ms(uint32_t duration_ms);
static void play_tone(uint8_t button, uint32_t duration_ms);
static uint8_t wait_for_player_input(uint32_t timeout_ms);
static void add_to_sequence(void);
static void play_sequence(void);
static uint8_t get_player_sequence(void);
static void display_game_over_screen(uint8_t final_level);

__attribute__((interrupt("machine")))
void systick_handler(void) {
  *STK_SR &= ~1U;

  if (buzzer_enabled != 0) {
    *GPIOE_ODR ^= BUZZER_MASK;
  }

  if (remaining_duration > 0) {
    uint32_t step = current_period;
    if ((step == 0) || (remaining_duration <= step)) {
      remaining_duration = 0;
    } else {
      remaining_duration -= step;
    }
  }
}

__attribute__((interrupt("machine")))
void exti_handler(void) {
  *EXTI_INTFR = 0xFU;

  uint8_t key = keypad();
  if (key != NO_KEY) {
    last_key_pressed = key;
    display_binary_led(key);
  } else {
    display_binary_led(0);
  }
}

static void init_display(void) {
  if (!tft_init(TFT_INIT_ILI9488)) {
    (void)tft_init(TFT_INIT_ST7796S);
  }
}

static const uint8_t *glyph_for(char ch) {
  if ((ch >= 'a') && (ch <= 'z')) {
    ch = (char)(ch - ('a' - 'A'));
  }

  if ((ch >= 'A') && (ch <= 'Z')) {
    return letter_glyphs[ch - 'A'];
  }

  if ((ch >= '0') && (ch <= '9')) {
    return digit_glyphs[ch - '0'];
  }

  if (ch == ':') {
    return colon_glyph;
  }

  return blank_glyph;
}

static void lcd_draw_char(int x, int y, char ch, uint16_t color, uint16_t bgcolor, int scale) {
  const uint8_t *glyph = glyph_for(ch);

  for (int col = 0; col < FONT_WIDTH; col++) {
    for (int row = 0; row < FONT_HEIGHT; row++) {
      uint16_t pixel_color = ((glyph[col] & (1U << row)) != 0) ? color : bgcolor;
      for (int dx = 0; dx < scale; dx++) {
        for (int dy = 0; dy < scale; dy++) {
          tft_lcd_pixel(x + col * scale + dx, y + row * scale + dy, pixel_color);
        }
      }
    }
  }

  for (int row = 0; row < FONT_HEIGHT * scale; row++) {
    for (int dx = 0; dx < scale; dx++) {
      tft_lcd_pixel(x + FONT_WIDTH * scale + dx, y + row, bgcolor);
    }
  }
}

static void lcd_puts(int x, int y, const char *text, uint16_t color, uint16_t bgcolor, int scale) {
  while (*text != '\0') {
    lcd_draw_char(x, y, *text, color, bgcolor, scale);
    x += (FONT_WIDTH + 1) * scale;
    text++;
  }
}

static void draw_button(uint8_t button_id, uint16_t color, int filled) {
  if (button_id >= 16) {
    return;
  }

  uint8_t row = button_id / 4;
  uint8_t col = button_id % 4;

  int x1 = GRID_START_X + col * (BUTTON_WIDTH + BUTTON_SPACING);
  int y1 = GRID_START_Y + row * (BUTTON_HEIGHT + BUTTON_SPACING);
  int x2 = x1 + BUTTON_WIDTH - 1;
  int y2 = y1 + BUTTON_HEIGHT - 1;

  tft_lcd_rect(x1, y1, x2, y2, color, filled);
}

static uint16_t get_button_color(uint8_t button_id) {
  switch (button_id / 4) {
    case 0:
      return TFT_RED;
    case 1:
      return TFT_GREEN;
    case 2:
      return TFT_BLUE;
    case 3:
      return TFT_YELLOW;
    default:
      return TFT_WHITE;
  }
}

static void draw_button_grid(void) {
  for (uint8_t i = 0; i < 16; i++) {
    draw_button(i, get_button_color(i), 1);
  }
}

static void draw_score(uint8_t level) {
  char buffer[16];
  tft_lcd_rect(0, 0, 130, 35, TFT_BLACK, 1);
  sprintf(buffer, "LEVEL:%u", (unsigned int)level);
  lcd_puts(10, 10, buffer, TFT_WHITE, TFT_BLACK, 2);
}

static void display_binary_led(uint8_t value) {
  *GPIOD_BCR = LED_MASK;
  *GPIOD_BSR = ((uint16_t)value << 8) & LED_MASK;
}

static void start_countdown(uint32_t duration_us, uint32_t tick_us, uint8_t sound) {
  if (tick_us == 0) {
    tick_us = 1;
  }

  current_period = tick_us;
  remaining_duration = duration_us;
  buzzer_enabled = sound;
  systick_periodic_micro(tick_us);
}

static void wait_countdown_done(void) {
  while (remaining_duration > 0) {
  }

  systick_stop();
  buzzer_enabled = 0;
  *GPIOE_BCR = BUZZER_MASK;
}

static void delay_game_ms(uint32_t duration_ms) {
  start_countdown(duration_ms * 1000U, 1000U, 0);
  wait_countdown_done();
}

static void play_tone(uint8_t button, uint32_t duration_ms) {
  if (button >= 16) {
    return;
  }

  display_binary_led(button);
  start_countdown(duration_ms * 1000U, periods[button], 1);
  wait_countdown_done();
  display_binary_led(0);
}

static uint8_t wait_for_player_input(uint32_t timeout_ms) {
  last_key_pressed = NO_KEY;
  start_countdown(timeout_ms * 1000U, 1000U, 0);

  while ((remaining_duration > 0) && (last_key_pressed == NO_KEY)) {
    uint8_t key = keypad();
    if (key != NO_KEY) {
      last_key_pressed = key;
      display_binary_led(key);
    }
  }

  systick_stop();
  buzzer_enabled = 0;
  *GPIOE_BCR = BUZZER_MASK;
  return last_key_pressed;
}

static void add_to_sequence(void) {
  if (sequence_length < MAX_SEQUENCE_LENGTH) {
    game_sequence[sequence_length] = (uint8_t)(rand() % 16);
    sequence_length++;
  }
}

static void play_sequence(void) {
  for (uint8_t i = 0; i < sequence_length; i++) {
    play_tone(game_sequence[i], TONE_DURATION_MS);
    delay_game_ms(PAUSE_BETWEEN_TONES_MS);
  }
}

static uint8_t get_player_sequence(void) {
  for (uint8_t player_pos = 0; player_pos < sequence_length; player_pos++) {
    uint8_t key = wait_for_player_input(PLAYER_INPUT_TIMEOUT_MS);

    if (key == NO_KEY) {
      return 0;
    }

    if (key != game_sequence[player_pos]) {
      return 0;
    }

    play_tone(key, TONE_DURATION_MS / 2U);
    delay_game_ms(200U);
  }

  return 1;
}

static void display_game_over_screen(uint8_t final_level) {
  char buffer[24];
  tft_lcd_rect(0, 0, LCD_W - 1, LCD_H - 1, TFT_BLACK, 1);
  lcd_puts(160, 80, "GAME OVER", TFT_RED, TFT_BLACK, 3);
  sprintf(buffer, "LEVEL:%u", (unsigned int)final_level);
  lcd_puts(170, 150, buffer, TFT_WHITE, TFT_BLACK, 2);
  lcd_puts(150, 220, "PRESS ANY KEY", TFT_CYAN, TFT_BLACK, 2);
}

int main(void) {
  init_display();
  tft_lcd_rect(0, 0, LCD_W - 1, LCD_H - 1, TFT_BLACK, 1);

  init_gpio();
  init_vector_table();
  init_interrupts();

  while (1) {
    sequence_length = 0;
    game_over = 0;

    tft_lcd_rect(0, 0, LCD_W - 1, LCD_H - 1, TFT_BLACK, 1);
    lcd_puts(180, 10, "SIMON SAYS", TFT_WHITE, TFT_BLACK, 2);
    draw_button_grid();

    while ((!game_over) && (sequence_length < MAX_SEQUENCE_LENGTH)) {
      add_to_sequence();
      draw_score(sequence_length);
      delay_game_ms(300U);
      play_sequence();

      if (!get_player_sequence()) {
        game_over = 1;
      } else {
        delay_game_ms(1000U);
      }
    }

    display_game_over_screen(sequence_length);

    last_key_pressed = NO_KEY;
    while (last_key_pressed == NO_KEY) {
      uint8_t key = keypad();
      if (key != NO_KEY) {
        last_key_pressed = key;
        delay_ms(50);
      }
    }
  }
}

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

// Lab version - task-based Simon Says implementation

// frekvenser för knapparna
uint32_t periods[] = {
  1516, 1431, 1351, 1275,
  1203, 1136, 1072, 1012,
   955,  901,  851,  803,
   758,  715,  675,  637
};

// melodi placeholder
uint32_t melodi1[] = {
};

// musik data
Note notes[] = NOTES;
const int notes_length = sizeof(notes) / sizeof(Note);
int remaining_duration;
int current_period;

// systick handler - ljud
__attribute__((interrupt("machine")))
void systick_handler() {
  *GPIOE_ODR ^= 1;        // invertera buzzer för kvadratvåg
  *STK_SR &= ~1;          // acknowledge
  if (remaining_duration > 0) {
    remaining_duration -= current_period;  // räkna ned
  }
}

// exti handler - keypad interrupt
__attribute__((interrupt("machine")))
void exti_handler() {
  *EXTI_INTFR = 0xFF;     // rensa interrupt
  uint8_t key = keypad();
  
  if (key != 0xFF) {
    // knapp tryckt - starta ljud
    systick_periodic_micro(periods[key]);
    *GPIOD_BSR = (key << 8);    // tända LED
  } else {
    // knapp släppt - stoppa allt
    systick_stop();
    *GPIOD_BCR = 0xFF00;        // släck LED
  }
}

// entry point
int main(void) {
  // init hårdvara
  init_gpio();
  init_vector_table();
  init_interrupts();
  
  // starta systick
  systick_periodic_micro(1);
  
  // spela melodi
  for (int i = 0; i < notes_length; i++) {
    current_period = notes[i].period_micro;
    remaining_duration = notes[i].duration_micro;
    systick_periodic_micro(current_period);
    
    // vänta på att not är klar
    while (remaining_duration > 0) {
      // interrupt hanterar
    }
  }
  
  systick_stop();

  // vänta på interrupts
  while (1) {
    // keypad interrupts här
  }
}

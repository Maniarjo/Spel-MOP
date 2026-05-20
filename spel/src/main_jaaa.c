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

/* SUMMARY OF TASKS
 *
 * Task 1: Sound Check
 * - in systick_handler(), invert the buzzer pin and acknowledge interrupt
 * - in main(), add systick_periodic_micro(500) to test the buzzer
 * - remove test code before moving on
 *
 * Task 2: External Control
 * - Task 2.1: Reading the Keypad
 * --- in exti_handler(), read keypad and acknowledge interrupt
 * --- use debugger to verify being able to read key press AND release
 * - Task 2.2: Making Noise
 * --- add array with period values, in scope from exti_handler()
 * --- in exti_handler(), start/stop sound on key press/release
 *
 * Task 3: Lights, Please!
 * - in exti_handler() where sound is started, also turn on lights
 * - in exti_handler() where sound is stopped, also turn off lights
 *
 * Task 4: Melody Player
 * - add given global variables to main.c, above systick_handler()
 * - in main(), iterate through list of notes, starting each note and
 *   wait while remaining_duration > 0
 *   (WARNING: do NOT use while(remaining_duration != 0) to wait!)
 * - in systick_handler(), reduce remaining_duration by current_period
 */

// Globals for Task 2 - Period lookup table
uint32_t periods[] = {
  1516, 1431, 1351, 1275,
  1203, 1136, 1072, 1012,
   955,  901,  851,  803,
   758,  715,  675,  637
};

uint32_t melodi1[] = {
  
};


// Globals for Task 4
Note notes[] = NOTES;
const int notes_length = sizeof(notes) / sizeof(Note);
int remaining_duration;
int current_period;

// Handle internal SysTick interrupt
__attribute__((interrupt("machine")))
void systick_handler() {
  // Task 1: Invert buzzer pin to create square wave
  *GPIOE_ODR ^= 1;
  
  // Acknowledge SysTick interrupt
  *STK_SR &= ~1;
  
  // Task 4: Decrement remaining duration
  if (remaining_duration > 0) {
    remaining_duration -= current_period;
  }
}

// Handle external keypad interrupt (on press *and* on release)
__attribute__((interrupt("machine")))
void exti_handler() {
  // Acknowledge EXTI interrupt FIRST
  *EXTI_INTFR = 0xFF;
  
  // Task 2 & 3: Read keypad and control tone + lights
  uint8_t key = keypad();
  
  if (key != 0xFF) {
    // Key pressed: start tone with corresponding period and show lights
    systick_periodic_micro(periods[key]);
    *GPIOD_BSR = (key << 8);  // Set bargraph bits [8:15]
  } else {
    // Key released: stop tone and turn off lights
    systick_stop();
    *GPIOD_BCR = 0xFF00;  // Clear bargraph bits [8:15]
  }
}

// Entry point
int main(void) {
  // Initialize
  init_gpio();
  init_vector_table();
  init_interrupts();
  
  // Task 4: Play melody
  systick_periodic_micro(1);  // Start SysTick to enable interrupts
  
  for (int i = 0; i < notes_length; i++) {
    current_period = notes[i].period_micro;
    remaining_duration = notes[i].duration_micro;
    systick_periodic_micro(current_period);
    
    // Wait for note to finish
    while (remaining_duration > 0) {
      // Do nothing - interrupt handler counts down
    }
  }
  
  systick_stop();  // Stop after melody finishes

  // Wait for interrupts
  while (1) {
    // Could put concurrently running code here!
  }
}

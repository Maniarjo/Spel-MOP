#ifndef MUSIC_H
#define MUSIC_H

typedef struct {
  int period_micro;
  int duration_micro;
} Note;

// Frequency periods for each keypad button (in microseconds)
// These are used to generate the frequencies when buttons are pressed
extern uint32_t periods[];

#endif

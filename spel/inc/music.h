#ifndef MUSIC_H
#define MUSIC_H

typedef struct {
  int period_micro;
  int duration_micro;
} Note;

#define TEMPO 138 // Change this if you want!
#define UNIT 15000000 / TEMPO
#define NOTES                                                                  \
  {                                                                            \
        {568, 4 * UNIT}, {675, 2 * UNIT}, {637, 2 * UNIT}, {568, 4 * UNIT},    \
        {675, 2 * UNIT}, {637, 2 * UNIT}, {568, 2 * UNIT}, {1136, 2 * UNIT},   \
        {1012, 2 * UNIT}, {901, 2 * UNIT}, {851, 2 * UNIT}, {758, 2 * UNIT},   \
        {675, 2 * UNIT}, {637, 2 * UNIT}, {675, 4 * UNIT}, {851, 2 * UNIT},    \
        {758, 2 * UNIT}, {675, 4 * UNIT}, {1351, 2 * UNIT}, {1275, 2 * UNIT},  \
        {1136, 2 * UNIT}, {1012, 2 * UNIT}, {1136, 2 * UNIT},                  \
        {1275, 2 * UNIT}, {1136, 2 * UNIT}, {851, 2 * UNIT}, {901, 2 * UNIT},  \
        {851, 2 * UNIT},                                                       \
                                                                               \
        {1012, 4 * UNIT}, {851, 2 * UNIT}, {901, 2 * UNIT}, {1012, 4 * UNIT},  \
        {1136, 2 * UNIT}, {1275, 2 * UNIT}, {1136, 2 * UNIT},                  \
        {1275, 2 * UNIT}, {1351, 2 * UNIT}, {1275, 2 * UNIT},                  \
        {1136, 2 * UNIT}, {1012, 2 * UNIT}, {901, 2 * UNIT}, {851, 2 * UNIT},  \
        {1012, 4 * UNIT}, {851, 2 * UNIT}, {901, 2 * UNIT}, {851, 4 * UNIT},   \
        {901, 2 * UNIT}, {1012, 2 * UNIT}, {901, 2 * UNIT}, {1136, 2 * UNIT},  \
        {1012, 2 * UNIT}, {901, 2 * UNIT}, {851, 2 * UNIT}, {758, 2 * UNIT},   \
        {675, 2 * UNIT}, {637, 2 * UNIT},                                      \
                                                                               \
        {675, 4 * UNIT}, {851, 2 * UNIT}, {758, 2 * UNIT}, {675, 4 * UNIT},    \
        {758, 2 * UNIT}, {851, 2 * UNIT}, {758, 2 * UNIT}, {851, 2 * UNIT},    \
        {901, 2 * UNIT}, {851, 2 * UNIT}, {758, 1 * UNIT}, {675, 1 * UNIT},    \
        {758, 2 * UNIT}, {851, 2 * UNIT}, {901, 2 * UNIT}, {851, 4 * UNIT},    \
        {1012, 2 * UNIT}, {901, 2 * UNIT}, {851, 4 * UNIT}, {1351, 2 * UNIT},  \
        {1275, 2 * UNIT}, {1136, 2 * UNIT}, {1012, 2 * UNIT},                  \
        {1136, 2 * UNIT}, {1275, 2 * UNIT}, {1136, 2 * UNIT},                  \
        {1351, 2 * UNIT}, {1275, 2 * UNIT}, {1136, 2 * UNIT},                  \
                                                                               \
        {1275, 4 * UNIT}, {1012, 2 * UNIT}, {1136, 2 * UNIT},                  \
        {1275, 4 * UNIT}, {1351, 2 * UNIT}, {1516, 2 * UNIT},                  \
        {1351, 2 * UNIT}, {1516, 2 * UNIT}, {1702, 2 * UNIT},                  \
        {1516, 2 * UNIT}, {1351, 2 * UNIT}, {1275, 2 * UNIT},                  \
        {1136, 2 * UNIT}, {1012, 2 * UNIT}, {1275, 4 * UNIT},                  \
        {1012, 2 * UNIT}, {1136, 2 * UNIT}, {1012, 4 * UNIT}, {901, 2 * UNIT}, \
        {851, 2 * UNIT}, {1136, 2 * UNIT}, {1012, 2 * UNIT}, {901, 2 * UNIT},  \
        {851, 2 * UNIT}, {758, 2 * UNIT}, {675, 2 * UNIT}, {637, 2 * UNIT}, {  \
      568, 2 * UNIT                                                            \
    }                                                                          \
  }

#endif

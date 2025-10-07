// buzzer.h
#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

/* Types) */
typedef enum {
  BUZZ_IDLE = 0,
  BUZZ_PLAYING,
  BUZZ_SILENCE
} BuzzState;

typedef struct {
  const int *tone;
  const int *duration;
  const int *pause;   // may be NULL
  int size;

  volatile int idx;
  volatile uint32_t ms_left;
  volatile BuzzState state;
  volatile int running;
} Buzzer;

/* Timer clock */
#ifndef TIM_FREQ
#define TIM_FREQ 108000000
#endif

#define ROWS_OF(arr)  (sizeof(arr) / sizeof((arr)[0]))
#define COLS_OF(arr)  (sizeof((arr)[0]) / sizeof((arr)[0][0]))

#define buzzer_play_sound(song) \
  buzzer_play_sound_impl( (song)[0], (song)[1], (ROWS_OF(song) >= 3 ? (song)[2] : NULL), (int)COLS_OF(song) )

/* Exports main.c needs */
extern Buzzer buzzer;
int  calculate_pres(int frequency);
void buzzer_set_freq(int frequency);
void buzzer_play_sound_impl(const int *tones,
                            const int *durations,
                            const int *pause,  /* may be NULL */
                            int cols);

#endif /* BUZZER_H */

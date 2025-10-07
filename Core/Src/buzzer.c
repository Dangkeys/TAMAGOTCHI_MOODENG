// buzzer.c
#include "buzzer.h"
#include "tim.h"
#include "stm32f7xx_hal.h"

extern TIM_HandleTypeDef htim2;  // make htim2 visible from tim.c

/* The one global instance */
Buzzer buzzer;

/* Prescaler calculation (TIM_FREQ defined in buzzer.h) */
int calculate_pres(int frequency)
{
    if (frequency <= 0) return 0;
    return (TIM_FREQ / (1000 * frequency)) - 1;  // ARR fixed at 999
}

/* Set current note frequency (mute if 0) */
void buzzer_set_freq(int frequency)
{
    if (frequency <= 0) {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
        return;
    }
    int pres = calculate_pres(frequency);
    __HAL_TIM_SET_PRESCALER(&htim2, (uint32_t)pres);
}

/* Start a song (2 rows = tones+durations; 3rd row optional pause/beat) */
void buzzer_play_sound_impl(const int *tones,
                            const int *durations,
                            const int *pause,
                            int cols)
{
    if (!tones || !durations || cols <= 0) return;

    buzzer.tone     = tones;
    buzzer.duration = durations;
    buzzer.pause    = pause;
    buzzer.size     = cols;
    buzzer.idx      = 0;
    buzzer.state    = BUZZ_PLAYING;
    buzzer.running  = 1;

    buzzer_set_freq(buzzer.tone[0]);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 500);   // ~50% duty
    buzzer.ms_left = (uint32_t)buzzer.duration[0];
}

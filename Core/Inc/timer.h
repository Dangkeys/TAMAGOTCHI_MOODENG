#ifndef __TIMER_H
#define __TIMER_H

typedef struct {
    int hour;
    int minute;
    int second;
} Clock_t;

void Timer_Init(Clock_t* gameClock);
void Timer_Update(Clock_t* gameClock);
void Timer_Reset(Clock_t* gameClock);

#endif

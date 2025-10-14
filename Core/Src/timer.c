#include "timer.h"
#include "moodeng.h"
#include "ui_manager.h"
#include <stdio.h>

extern Moodeng_t moodeng;
extern UIManager_t ui;

void Timer_Init(Clock_t* gameClock) {
    gameClock->hour = 0;
    gameClock->minute = 0;
    gameClock->second = 0;
}

void Timer_Update(Clock_t* gameClock) {
    if (moodeng.evolution == 3 && moodeng.happy == 4){
        Moodeng_Handle_Win(moodeng);
    }
    if (moodeng.isAlive == false) {
        Moodeng_Endgame_animation(&moodeng);
    }
    gameClock->second++;
    if (moodeng.isSleeping == true) {
        if (ui.isLightOn == false) setSleepingTime(&moodeng, moodeng.sleepingTime + 1);
    }

    if (gameClock->second >= 60) {
        gameClock->second = 0;
        gameClock->minute++;

        Moodeng_Update(&moodeng);
        checkEvolution(&moodeng, gameClock);
    }

    if (gameClock->minute >= 60) {
        gameClock->minute = 0;
        gameClock->hour++;
    }

    if (gameClock->hour >= 24) {
        gameClock->hour = 0;
    }
}

void Timer_Reset(Clock_t* gameClock) {
    gameClock->hour = 0;
    gameClock->minute = 0;
    gameClock->second = 0;
}

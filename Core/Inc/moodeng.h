/*
 * moodeng.h
 *
 *  Created on: Oct 4, 2025
 *      Author: Gan
 */

#include <stdbool.h>
#include "timer.h"
#include "rng.h"

#ifndef INC_MOODENG_H_
#define INC_MOODENG_H_

typedef enum {
    NORMAL = 0,
    SILLY,
    SCOLDED,
} Emotion_t;

typedef struct {
    int happy;          // 0-4
    int weight;        // 5-99
    int hunger;        // 0-4
    int poopCount;     // 0-6
    float poopRate;    // 0.0 - 1.0
    bool isSick;
    float healRate;    // 0.0 - 1.0
    int discipline;    // 0-5
    bool isTired;
    int evolution;     // 0-2
    bool isAlive;
    Emotion_t emotion;
    int nextDecayHappy;
    int nextDecayHunger;
    int nextPoopTime;
    int nextSickTime;
    int nextHurtTime;
    int nextDirtyTime;
    int nextSleepyTime;
    int sleepingTime; //seconds
    bool isSleeping;
} Moodeng_t;

void Moodeng_Init(Moodeng_t* moodeng);
void Moodeng_WriteFlashData(Moodeng_t* moodeng);
void Moodeng_Reset(Moodeng_t* moodeng);
int Moodeng_GenerateRandomNumber(Moodeng_t* moodeng, int start, int end);
void setHappy(Moodeng_t* moodeng, int value);
void setWeight(Moodeng_t* moodeng, int value);
void setHunger(Moodeng_t* moodeng, int value);
void setPoopCount(Moodeng_t* moodeng, int value);
void setPoopRate(Moodeng_t* moodeng, float value);
void setIsSick(Moodeng_t* moodeng, bool value);
void setHealRate(Moodeng_t* moodeng, float value);
void setDiscipline(Moodeng_t* moodeng, int value);
void setisTired(Moodeng_t* moodeng, bool value);
void setEvolution(Moodeng_t* moodeng, int value);
void setIsAlive(Moodeng_t* moodeng, bool value);
void setEmotion(Moodeng_t* moodeng, Emotion_t value);
void setIsSleeping(Moodeng_t *moodeng, bool value);
void setSleepingTime(Moodeng_t *moodeng, int value);
void setNextSleepyTime(Moodeng_t *moodeng, int value);
float Moodeng_FeedingChance(Moodeng_t* moodeng);
float Moodeng_SickChance(Moodeng_t* moodeng);
float Moodeng_PlayingChance(Moodeng_t* moodeng);
void checkEvolution(Moodeng_t* moodeng, Clock_t* gameClock);
void Moodeng_Update(Moodeng_t* moodeng);
bool Moodeng_Minigame(Moodeng_t* moodeng, int guess);
void Moodeng_Heal(Moodeng_t* moodeng);
bool Moodeng_Check_Feed(Moodeng_t* moodeng);
bool Moodeng_Check_Play(Moodeng_t* moodeng);
void Moodeng_Handle_Lose(Moodeng_t* moodeng);
void Moodeng_Endgame_animation(Moodeng_t* moodeng);
void Moodeng_Handle_Win(Moodeng_t* moodeng);

#endif /* INC_MOODENG_H_ */

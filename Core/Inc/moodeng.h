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
    bool isTried;
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
} Moodeng_t;

void Moodeng_Init(Moodeng_t* moodeng);
int Moodeng_GenerateRandomNumber(Moodeng_t* moodeng, int start, int end);
void Moodeng_SetHappy(Moodeng_t* moodeng, int value);
void Moodeng_SetWeight(Moodeng_t* moodeng, int value);
void Moodeng_SetHunger(Moodeng_t* moodeng, int value);
void Moodeng_SetPoopCount(Moodeng_t* moodeng, int value);
void Moodeng_SetPoopRate(Moodeng_t* moodeng, float value);
void Moodeng_SetIsSick(Moodeng_t* moodeng, bool value);
void Moodeng_SetHealRate(Moodeng_t* moodeng, float value);
void Moodeng_SetDiscipline(Moodeng_t* moodeng, int value);
void Moodeng_SetIsTried(Moodeng_t* moodeng, bool value);
void Moodeng_SetEvolution(Moodeng_t* moodeng, int value);
void Moodeng_SetIsAlive(Moodeng_t* moodeng, bool value);
void Moodeng_SetEmotion(Moodeng_t* moodeng, Emotion_t value);
float Moodeng_FeedingChance(Moodeng_t* moodeng);
float Moodeng_SickChance(Moodeng_t* moodeng);
float Moodeng_PlayingChance(Moodeng_t* moodeng);
static void Moodeng_HandleDecay(int* timer, int* stat, int minRand, int maxRand, Moodeng_t* moodeng);
void checkEvolution(Moodeng_t* moodeng, Clock_t* gameClock);
void Moodeng_Update(Moodeng_t* moodeng);
bool Moodeng_Minigame(Moodeng_t* moodeng, int guess);
void Moodeng_Sleep(Moodeng_t* moodeng);
void Moodeng_Heal(Moodeng_t* moodeng);
bool Moodeng_Check_Feed(Moodeng_t* moodeng);
bool Moodeng_Check_Play(Moodeng_t* moodeng);

#endif /* INC_MOODENG_H_ */

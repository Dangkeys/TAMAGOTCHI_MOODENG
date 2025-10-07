#include "moodeng.h"
#include <stdbool.h>
#include "rng.h"
#include "ui_manager.h"
extern UIManager_t ui

// Default starting stats (easier tuning)
#define MOODENG_INIT_HAPPY        2
#define MOODENG_INIT_WEIGHT       5
#define MOODENG_INIT_HUNGER       2
#define MOODENG_INIT_POOPCOUNT    6
#define MOODENG_INIT_POOPRATE     0.0f
#define MOODENG_INIT_ISSICK       false
#define MOODENG_INIT_HEALRATE     0.5f
#define MOODENG_INIT_DISCIPLINE   3
#define MOODENG_INIT_ISTIRED      false
#define MOODENG_INIT_EVOLUTION    0
#define MOODENG_INIT_ISALIVE      true
#define MOODENG_INIT_EMOTION      NORMAL
#define START_DECAY_HAPPY         60
#define END_DECAY_HAPPY           90
#define START_DECAY_HUNGER        45
#define END_DECAY_HUNGER          75
#define START_POOPTIME            10
#define END_POOPTIME              30
#define MOODENG_INIT_SICKTIME     1
#define MOODENG_INIT_SLEEPYTIME   480
#define MOODENG_DIRTY_TIME        30
#define MOODENG_HURT_TIME         30


void Moodeng_Init(Moodeng_t* moodeng) {
    moodeng->happy = MOODENG_INIT_HAPPY;
    moodeng->weight = MOODENG_INIT_WEIGHT;
    moodeng->hunger = MOODENG_INIT_HUNGER;
    moodeng->poopCount = MOODENG_INIT_POOPCOUNT;
    moodeng->poopRate = MOODENG_INIT_POOPRATE; 
    moodeng->isSick = MOODENG_INIT_ISSICK;
    moodeng->healRate = MOODENG_INIT_HEALRATE;
    moodeng->discipline = MOODENG_INIT_DISCIPLINE;
    moodeng->isTried = MOODENG_INIT_ISTIRED;
    moodeng->evolution = MOODENG_INIT_EVOLUTION;
    moodeng->isAlive = MOODENG_INIT_ISALIVE;
    moodeng->emotion = MOODENG_INIT_EMOTION;
    moodeng->nextDecayHappy = Moodeng_GenerateRandomNumber(moodeng, START_DECAY_HAPPY, END_DECAY_HAPPY);
    moodeng->nextDecayHunger = Moodeng_GenerateRandomNumber(moodeng, START_DECAY_HUNGER, END_DECAY_HUNGER);
    moodeng->nextPoopTime = -1;
    moodeng->nextSickTime = MOODENG_INIT_SICKTIME;
    moodeng->nextHurtTime = -1;
    moodeng->nextDirtyTime = -1;
    moodeng->nextSleepyTime = MOODENG_INIT_SLEEPYTIME;
}

int Moodeng_GenerateRandomNumber(Moodeng_t* moodeng, int start, int end) {
    uint32_t randomNum;
    HAL_RNG_GenerateRandomNumber(&hrng, &randomNum);
    return start + (randomNum % (end - start + 1));
}

bool inRangeInt(int value, int min, int max) {
    return value >= min && value <= max;
}

bool inRangeFloat(float value, float min, float max) {
    return value >= min && value <= max;
}

void setHappy(Moodeng_t* moodeng, int emotion) {
    if (inRangeInt(emotion, 0, 4)) 
        moodeng->happy = emotion;
}

void setWeight(Moodeng_t* moodeng, int value) {
    if (inRangeInt(value, 5, 99)) 
        moodeng->weight = value;
}

void setHunger(Moodeng_t* moodeng, int value) {
    if (inRangeInt(value, 0, 4)) 
        moodeng->hunger = value;
}

void setPoopCount(Moodeng_t* moodeng, int value) {
    if (inRangeInt(value, 0, 6)) 
        moodeng->poopCount = value;
}

void setPoopRate(Moodeng_t* moodeng, float value) {
    if (inRangeFloat(value, 0.0f, 1.0f)) 
        moodeng->poopRate = value;
} 

void setIsSick(Moodeng_t* moodeng, bool value) {
    moodeng->isSick = value;
}

void setHealRate(Moodeng_t* moodeng, float value) {
    if (inRangeFloat(value, 0.0f, 1.0f)) 
        moodeng->healRate = value;
}

void setDiscipline(Moodeng_t* moodeng, int value) {
    if (inRangeInt(value, 0, 6))
        moodeng->discipline = value;
}

void setIsTried(Moodeng_t* moodeng, bool value) {
    moodeng->isTried = value;
}

void setEvolution(Moodeng_t* moodeng, int value) {
    if (inRangeInt(value, 0, 2))
        moodeng->evolution = value;
}

void setEmotion(Moodeng_t* moodeng, Emotion_t value) {
    moodeng->emotion = value;
}

float Moodeng_FeedingChance(Moodeng_t* moodeng) {
    return moodeng->discipline * 0.17f;
}

float Moodeng_SickChance(Moodeng_t* moodeng) {
    return moodeng->weight * 0.15f + moodeng->poopCount * 0.1f + moodeng->isTried * 0.15f + (20.0f - moodeng->happy * 0.05f) + (20.0f - moodeng->hunger * 0.05f);
}

float Moodeng_PlayingChance(Moodeng_t* moodeng) {
    return moodeng->discipline * 0.22f - moodeng->weight * 0.4f;
}

void checkEvolution(Moodeng_t* moodeng, Clock_t* gameClock) {
    int totalMinutes = (gameClock->hour * 60) + gameClock->minute;

    switch(moodeng->evolution) {
        case 0:
            if (totalMinutes >= 10) {
                moodeng->evolution = 1;
                moodeng->discipline += 2;
                if (moodeng->discipline > 6) moodeng->discipline = 6;
            }
            break;

        case 1:
            if (totalMinutes >= 180) {
                moodeng->evolution = 2;
                moodeng->discipline += 3;
                if (moodeng->discipline > 6) moodeng->discipline = 6;
            }
            break;

        case 2:
            if (totalMinutes >= 360) {
                moodeng->evolution = 3;
                moodeng->discipline += 4;
                if (moodeng->discipline > 6) moodeng->discipline = 6;
            }
            break;

        case 3:
            if (moodeng->happy == 6) {
                ui.activeAnim = &winAnim
            }
            break;

        default:
            break;
    }
}

static void Moodeng_HandleDecay(int* timer, int* stat, int minRand, int maxRand, Moodeng_t* moodeng) {
    if (*timer > 0) {
        (*timer)--;
        if (*timer == 0) {
            (*stat)--;
            if (*stat <= 0) {
                *stat = 0;
                moodeng->isAlive = false;
                ui.activeAnim = &loseAnim;
            }
            *timer = Moodeng_GenerateRandomNumber(moodeng, minRand, maxRand);
        }
    }
}

void Moodeng_Update(Moodeng_t* moodeng) {
    //poop
    if (moodeng->nextPoopTime == -1 && moodeng->poopRate > 0.0f) {
        moodeng->nextPoopTime = Moodeng_GenerateRandomNumber(moodeng, START_POOPTIME, END_POOPTIME);
    }
    if (moodeng->nextPoopTime > 0) {
        moodeng->nextPoopTime--;
        if (moodeng->nextPoopTime == 0) {
            float randomProb = (float)Moodeng_GenerateRandomNumber(moodeng, 0, 100) / 100.0f;

            if (randomProb < moodeng->poopRate) {
                moodeng->poopCount++;
                if (moodeng->poopCount > 6) moodeng->poopCount = 6;
                moodeng->nextPoopTime = -1;
            } else {
                moodeng->poopRate += 0.2f;
                if (moodeng->poopRate > 1.0f) moodeng->poopRate = 1.0f;
                moodeng->nextPoopTime = Moodeng_GenerateRandomNumber(moodeng, START_POOPTIME, END_POOPTIME);  
            }
        }
    }
    //sick
    if (moodeng->isSick == false && moodeng->nextSickTime > 0){
        moodeng->nextSickTime--;
        if (moodeng->nextSickTime == 0) {
            float randomProb = (float)Moodeng_GenerateRandomNumber(moodeng, 0, 100) / 100.0f;

            if (randomProb < Moodeng_SickChance(&moodeng)) {
                moodeng->isSick = true;
                moodeng->nextHurtTime = MOODENG_HURT_TIME;
            }
            moodeng->nextSickTime = MOODENG_INIT_SICKTIME;
        }
    }
    //hurt
    if (moodeng->isSick == true && moodeng->nextHurtTime > 0){
        moodeng->nextHurtTime--;
        if (moodeng->nextHurtTime == 0) {
            moodeng->happy--;
            moodeng->hunger--;
            if(moodeng->isSick == true) moodeng->nextHurtTime = MOODENG_HURT_TIME;
        }
    }
    //dirty
    if (moodeng->poopCount >= 2 && moodeng->nextDirtyTime == -1) {
        moodeng->nextDirtyTime = MOODENG_DIRTY_TIME;
    }
    if (moodeng->nextDirtyTime > 0 && moodeng->poopCount >= 2) {
        moodeng->nextDirtyTime--;
        if (moodeng->nextDirtyTime == 0){
            moodeng->happy -= moodeng->poopCount-1;
            moodeng->nextDirtyTime = MOODENG_DIRTY_TIME;
        }
    }
    //tired
    if (moodeng->isTried == false && moodeng->nextSleepyTime > 0){
        moodeng->nextSleepyTime--;
        if (moodeng->nextSleepyTime == 0){
            moodeng->isTried = true;
            moodeng->nextDirtyTime = MOODENG_INIT_SLEEPYTIME;
        } 
    }
    //Happy, Hunger
    Moodeng_HandleDecay(&moodeng->nextDecayHappy, &moodeng->happy, START_DECAY_HAPPY, END_DECAY_HAPPY, moodeng);
    Moodeng_HandleDecay(&moodeng->nextDecayHunger, &moodeng->hunger, START_DECAY_HUNGER, END_DECAY_HUNGER, moodeng);
}

bool Moodeng_Minigame(Moodeng_t* moodeng, int guess){
    bool win = (guess == Moodeng_GenerateRandomNumber(moodeng, 0, 1);)
    if (win){
        moodeng->happy += 2;
    } 
    else {
        moodeng->happy++;
    }
    moodeng->weight--;
    return win;
}

void Moodeng_Sleep(Moodeng_t* moodeng){

}

void Moodeng_Heal(Moodeng_t* moodeng){
    if (moodeng->isSick){
        float randomProb = (float)Moodeng_GenerateRandomNumber(moodeng, 0, 100) / 100.0f;
        if (randomProb < moodeng->healRate) {
            moodeng->isSick = false;
        }
        else {
            moodeng->healRate += 0.25;
            if (moodeng->healRate > 1.0) {moodeng->healRate = 1.0;}
        }
    }
    else {
        moodeng->happy--;
    }
}

bool Moodeng_Check_Feed(Moodeng_t* moodeng){
    if (moodeng->emotion == SCOLDED) {
        moodeng->emotion = NORMAL;
        return true;
    } 
    else if (moodeng->emotion == SILLY){
        return false;
    }
    float randomProb = (float)Moodeng_GenerateRandomNumber(&moodeng, 0, 100) / 100.0f;
    if (randomProb < Moodeng_FeedingChance(&moodeng)) {
        return true;
    }
    else {
        moodeng->emotion = SILLY;
        return false;
    }
}

bool Moodeng_Check_Play(Moodeng_t* moodeng){
    if (moodeng->emotion == SCOLDED) {
        moodeng->emotion = NORMAL;
        return true;
    }
    else if (moodeng->emotion == SILLY){
        return false;
    }

    float randomProb = (float)Moodeng_GenerateRandomNumber(moodeng, 0, 100) / 100.0f;
    if (randomProb < Moodeng_PlayingChance(&moodeng)) {
        return true;
    }
    else {
        moodeng->emotion = SILLY;
        return false;
    }
}
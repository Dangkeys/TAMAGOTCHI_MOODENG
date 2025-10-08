#include "moodeng.h"
#include <stdbool.h>
#include "rng.h"
#include "flash.h"

void Moodeng_Init(Moodeng_t* moodeng) {
    if (!Flash_IsDataValid(HAPPY_ADDRESS)) {
        Moodeng_Reset(moodeng);
    } else {
        Moodeng_LoadFlashData(moodeng);
    }
}

void Moodeng_Reset(Moodeng_t* moodeng) {
    moodeng->happy = 2;
    moodeng->weight = 5;
    moodeng->hunger = 2;
    moodeng->poopCount = 0;
    moodeng->poopRate = 0.0f; 
    moodeng->isSick = false;
    moodeng->healRate = 0.5f;
    moodeng->discipline = 3;
    moodeng->isTried = false;
    moodeng->evolution = 0;
    moodeng->isAlive = true;
    moodeng->emotion = NORMAL;
    moodeng->nextDecayHappy = Moodeng_GenerateRandomNumber(moodeng, 60, 90);
    moodeng->nextDecayHunger = Moodeng_GenerateRandomNumber(moodeng, 45, 75);
    moodeng->nextPoopTime = Moodeng_GenerateRandomNumber(moodeng, 10, 30);
    moodeng->nextSickTime = NULL;
    moodeng->nextHurtTime = NULL;
    moodeng->nextDirtyTime = NULL;
    moodeng->nextSleepyTime = 480;
    Moodeng_WriteFlashData(moodeng);
}

void Moodeng_LoadFlashData(Moodeng_t* moodeng) {
    moodeng->happy = (int)Flash_Read_NUM(HAPPY_ADDRESS);
    moodeng->weight = (int)Flash_Read_NUM(WEIGHT_ADDRESS);
    moodeng->hunger = (int)Flash_Read_NUM(HUNGER_ADDRESS);
    moodeng->poopCount = (int)Flash_Read_NUM(POOPCOUNT_ADDRESS);
    moodeng->poopRate = Flash_Read_NUM(POOPRATE_ADDRESS);
    moodeng->isSick = (bool)Flash_Read_NUM(ISSICK_ADDRESS);
    moodeng->healRate = Flash_Read_NUM(HEALRATE_ADDRESS);
    moodeng->discipline = (int)Flash_Read_NUM(DISCIPLINE_ADDRESS);
    moodeng->isTried = (bool)Flash_Read_NUM(ISTRIED_ADDRESS);
    moodeng->evolution = (int)Flash_Read_NUM(EVOLUTION_ADDRESS);
    moodeng->isAlive = (bool)Flash_Read_NUM(ISALIVE_ADDRESS);
    moodeng->emotion = (Emotion_t)(int)Flash_Read_NUM(EMOTION_ADDRESS);
    moodeng->nextDecayHappy = (int)Flash_Read_NUM(NEXTDECAYHAPPY_ADDRESS);
    moodeng->nextDecayHunger = (int)Flash_Read_NUM(NEXTDECAYHUNGER_ADDRESS);
    moodeng->nextPoopTime = (int)Flash_Read_NUM(NEXTPOOPTIME_ADDRESS);
    moodeng->nextSickTime = (int)Flash_Read_NUM(NEXTSICKTIME_ADDRESS);
    moodeng->nextHurtTime = (int)Flash_Read_NUM(NEXTHURTTIME_ADDRESS);
    moodeng->nextDirtyTime = (int)Flash_Read_NUM(NEXTDIRTYTIME_ADDRESS);
    moodeng->nextSleepyTime = (int)Flash_Read_NUM(NEXTSLEEPYTIME_ADDRESS);
}

void Moodeng_WriteFlashData(Moodeng_t* moodeng) {
    Flash_Write_NUM(HAPPY_ADDRESS, (float)moodeng->happy);
    Flash_Write_NUM(WEIGHT_ADDRESS, (float)moodeng->weight);
    Flash_Write_NUM(HUNGER_ADDRESS, (float)moodeng->hunger);
    Flash_Write_NUM(POOPCOUNT_ADDRESS, (float)moodeng->poopCount);
    Flash_Write_NUM(POOPRATE_ADDRESS, moodeng->poopRate);
    Flash_Write_NUM(ISSICK_ADDRESS, (float)moodeng->isSick);
    Flash_Write_NUM(HEALRATE_ADDRESS, moodeng->healRate);
    Flash_Write_NUM(DISCIPLINE_ADDRESS, (float)moodeng->discipline);
    Flash_Write_NUM(ISTRIED_ADDRESS, (float)moodeng->isTried);
    Flash_Write_NUM(EVOLUTION_ADDRESS, (float)moodeng->evolution);
    Flash_Write_NUM(ISALIVE_ADDRESS, (float)moodeng->isAlive);
    Flash_Write_NUM(EMOTION_ADDRESS, (float)moodeng->emotion);
    Flash_Write_NUM(NEXTDECAYHAPPY_ADDRESS, (float)moodeng->nextDecayHappy);
    Flash_Write_NUM(NEXTDECAYHUNGER_ADDRESS, (float)moodeng->nextDecayHunger);
    Flash_Write_NUM(NEXTPOOPTIME_ADDRESS, (float)moodeng->nextPoopTime);
    Flash_Write_NUM(NEXTSICKTIME_ADDRESS, (float)moodeng->nextSickTime);
    Flash_Write_NUM(NEXTHURTTIME_ADDRESS, (float)moodeng->nextHurtTime);
    Flash_Write_NUM(NEXTDIRTYTIME_ADDRESS, (float)moodeng->nextDirtyTime);
    Flash_Write_NUM(NEXTSLEEPYTIME_ADDRESS, (float)moodeng->nextSleepyTime);
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
    if (inRangeInt(emotion, 0, 4)) {
        moodeng->happy = emotion;
        Flash_Write_NUM(HAPPY_ADDRESS, (float)moodeng->happy);
    }
}

void setWeight(Moodeng_t* moodeng, int value) {
    if (inRangeInt(value, 5, 99)) {
        moodeng->weight = value;
        Flash_Write_NUM(WEIGHT_ADDRESS, (float)moodeng->weight);
    }
}

void setHunger(Moodeng_t* moodeng, int value) {
    if (inRangeInt(value, 0, 4)) {  
        moodeng->hunger = value;
        Flash_Write_NUM(HUNGER_ADDRESS, (float)moodeng->hunger);
    }
}

void setPoopCount(Moodeng_t* moodeng, int value) {
    if (inRangeInt(value, 0, 6)) {
        moodeng->poopCount = value;
        Flash_Write_NUM(POOPCOUNT_ADDRESS, (float)moodeng->poopCount);
    }
}

void setPoopRate(Moodeng_t* moodeng, float value) {
    if (inRangeFloat(value, 0.0f, 1.0f)) {
        moodeng->poopRate = value;
        Flash_Write_NUM(POOPRATE_ADDRESS, moodeng->poopRate);
    }
} 

void setIsSick(Moodeng_t* moodeng, bool value) {
    moodeng->isSick = value;
    Flash_Write_NUM(ISSICK_ADDRESS, (float)moodeng->isSick);
}

void setHealRate(Moodeng_t* moodeng, float value) {
    if (inRangeFloat(value, 0.0f, 1.0f)) {
        moodeng->healRate = value;
        Flash_Write_NUM(HEALRATE_ADDRESS, moodeng->healRate);
    }
}

void setDiscipline(Moodeng_t* moodeng, int value) {
    if (inRangeInt(value, 0, 6)) {
        moodeng->discipline = value;
        Flash_Write_NUM(DISCIPLINE_ADDRESS, (float)moodeng->discipline);
    }
}

void setIsTried(Moodeng_t* moodeng, bool value) {
    moodeng->isTried = value;
    Flash_Write_NUM(ISTRIED_ADDRESS, (float)moodeng->isTried);
}

void setEvolution(Moodeng_t* moodeng, int value) {
    if (inRangeInt(value, 0, 2)) {
        moodeng->evolution = value;
        Flash_Write_NUM(EVOLUTION_ADDRESS, (float)moodeng->evolution);
    }
}

void setEmotion(Moodeng_t* moodeng, Emotion_t value) {
    moodeng->emotion = value;
    Flash_Write_NUM(EMOTION_ADDRESS, (float)moodeng->emotion);
}

float Moodeng_FeedingChange(Moodeng_t* moodeng) {
    return moodeng->discipline * 0.17f;
}

float Moodeng_SickChange(Moodeng_t* moodeng) {
    return moodeng->weight * 0.15f + moodeng->poopCount * 0.1f + moodeng->isTried * 0.15f + (20.0f - moodeng->happy * 0.05f) + (20.0f - moodeng->hunger * 0.05f);
}

float Moodeng_PlayingChange(Moodeng_t* moodeng) {
    return moodeng->discipline * 0.22f - moodeng->weight * 0.4f;
}

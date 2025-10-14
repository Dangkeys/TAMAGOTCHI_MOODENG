#include "moodeng.h"
#include <stdbool.h>
#include "rng.h"
#include "ui_manager.h"
#include "buzzer.h"
#include "sound.h"
#include "flash.h"
extern UIManager_t ui;

bool losing = false;
uint32_t loseStart = 0;

// Default starting stats (easier tuning)
#define MOODENG_INIT_HAPPY 2
#define MOODENG_INIT_WEIGHT 5
#define MOODENG_INIT_HUNGER 2
#define MOODENG_INIT_POOPCOUNT 0
#define MOODENG_INIT_POOPRATE 0.0f
#define MOODENG_INIT_ISSICK false
#define MOODENG_INIT_HEALRATE 0.5f
#define MOODENG_INIT_DISCIPLINE 3
#define MOODENG_INIT_ISTIRED false
#define MOODENG_INIT_EVOLUTION 0
#define MOODENG_INIT_ISALIVE true
#define MOODENG_INIT_EMOTION NORMAL
#define START_DECAY_HAPPY 60
#define END_DECAY_HAPPY 90
#define START_DECAY_HUNGER 45
#define END_DECAY_HUNGER 75
#define START_POOPTIME 10
#define END_POOPTIME 30
#define MOODENG_INIT_SICKTIME 30
#define MOODENG_INIT_SLEEPYTIME 480
#define MOODENG_DIRTY_TIME 30
#define MOODENG_HURT_TIME 30

void Moodeng_Init(Moodeng_t *moodeng)
{
    if (!Flash_IsDataValid(HAPPY_ADDRESS))
    {
        Moodeng_Reset(moodeng);
    }
    else
    {
        Moodeng_LoadFlashData(moodeng);
    }
}

void Moodeng_LoadFlashData(Moodeng_t *moodeng)
{
    moodeng->happy = (int)Flash_Read_NUM(HAPPY_ADDRESS);
    moodeng->weight = (int)Flash_Read_NUM(WEIGHT_ADDRESS);
    moodeng->hunger = (int)Flash_Read_NUM(HUNGER_ADDRESS);
    moodeng->poopCount = (int)Flash_Read_NUM(POOPCOUNT_ADDRESS);
    moodeng->poopRate = Flash_Read_NUM(POOPRATE_ADDRESS);
    moodeng->isSick = (bool)Flash_Read_NUM(ISSICK_ADDRESS);
    moodeng->healRate = Flash_Read_NUM(HEALRATE_ADDRESS);
    moodeng->discipline = (int)Flash_Read_NUM(DISCIPLINE_ADDRESS);
    moodeng->isTired = (bool)Flash_Read_NUM(ISTIRED_ADDRESS);
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
    moodeng->sleepingTime = (int)Flash_Read_NUM(SLEEPINGTIME_ADDRESS);
    moodeng->isSleeping = (bool)Flash_Read_NUM(ISSLEEPING_ADDRESS);
}

void Moodeng_Reset(Moodeng_t *moodeng)
{
    moodeng->happy = MOODENG_INIT_HAPPY;
    moodeng->weight = MOODENG_INIT_WEIGHT;
    moodeng->hunger = MOODENG_INIT_HUNGER;
    moodeng->poopCount = MOODENG_INIT_POOPCOUNT;
    moodeng->poopRate = MOODENG_INIT_POOPRATE;
    moodeng->isSick = MOODENG_INIT_ISSICK;
    moodeng->healRate = MOODENG_INIT_HEALRATE;
    moodeng->discipline = MOODENG_INIT_DISCIPLINE;
    moodeng->isTired = MOODENG_INIT_ISTIRED;
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
    moodeng->sleepingTime = 0; // seconds
    moodeng->isSleeping = false;
    Moodeng_WriteFlashData(moodeng);
}

void Moodeng_WriteFlashData(Moodeng_t *moodeng)
{
    Flash_Write_NUM(HAPPY_ADDRESS, (float)moodeng->happy);
    Flash_Write_NUM(WEIGHT_ADDRESS, (float)moodeng->weight);
    Flash_Write_NUM(HUNGER_ADDRESS, (float)moodeng->hunger);
    Flash_Write_NUM(POOPCOUNT_ADDRESS, (float)moodeng->poopCount);
    Flash_Write_NUM(POOPRATE_ADDRESS, moodeng->poopRate);
    Flash_Write_NUM(ISSICK_ADDRESS, (float)moodeng->isSick);
    Flash_Write_NUM(HEALRATE_ADDRESS, moodeng->healRate);
    Flash_Write_NUM(DISCIPLINE_ADDRESS, (float)moodeng->discipline);
    Flash_Write_NUM(ISTIRED_ADDRESS, (float)moodeng->isTired);
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
    Flash_Write_NUM(SLEEPINGTIME_ADDRESS, (float)moodeng->sleepingTime);
    Flash_Write_NUM(ISSLEEPING_ADDRESS, (float)moodeng->isSleeping);
}

int Moodeng_GenerateRandomNumber(Moodeng_t *moodeng, int start, int end)
{
    uint32_t randomNum;
    HAL_RNG_GenerateRandomNumber(&hrng, &randomNum);
    return start + (randomNum % (end - start + 1));
}

int inRangeInt(int value, int min, int max)
{
    if (value > max) return max;
    if (value < min) return min;
    return value;
}

float inRangeFloat(float value, float min, float max)
{
    if (value > max) return max;
    if (value < min) return min;
    return value;
}

void setHappy(Moodeng_t *moodeng, int emotion)
{
    moodeng->happy = inRangeInt(emotion, 0, 4);
    if (moodeng->happy == 0) {
        Moodeng_Handle_Lose(moodeng);
    }
    Flash_Write_NUM(HAPPY_ADDRESS, (float)moodeng->happy);
}

void setWeight(Moodeng_t *moodeng, int value)
{
    moodeng->weight = inRangeInt(value, 5, 99);
    Flash_Write_NUM(WEIGHT_ADDRESS, (float)moodeng->weight);
}

void setHunger(Moodeng_t *moodeng, int value)
{
    moodeng->hunger = inRangeInt(value, 0, 4);
    if (moodeng->hunger == 0) {
        Moodeng_Handle_Lose(moodeng);
    }
    Flash_Write_NUM(HUNGER_ADDRESS, (float)moodeng->hunger);
}

void setPoopCount(Moodeng_t *moodeng, int value)
{
    moodeng->poopCount = inRangeInt(value, 0, 6);
    Flash_Write_NUM(POOPCOUNT_ADDRESS, (float)moodeng->poopCount);
}

void setPoopRate(Moodeng_t *moodeng, float value)
{
    moodeng->poopRate = inRangeFloat(value, 0.0f, 1.0f);
    Flash_Write_NUM(POOPRATE_ADDRESS, moodeng->poopRate);
}

void setIsSick(Moodeng_t *moodeng, bool value)
{
    moodeng->isSick = value;
    Flash_Write_NUM(ISSICK_ADDRESS, (float)moodeng->isSick);
}

void setHealRate(Moodeng_t *moodeng, float value)
{
    moodeng->healRate = inRangeFloat(value, 0.0f, 1.0f);
    Flash_Write_NUM(HEALRATE_ADDRESS, moodeng->healRate);
}

void setDiscipline(Moodeng_t *moodeng, int value)
{
    moodeng->discipline = inRangeInt(value, 0, 6);
    Flash_Write_NUM(DISCIPLINE_ADDRESS, (float)moodeng->discipline);
}

void setisTired(Moodeng_t *moodeng, bool value)
{
    moodeng->isTired = value;
    Flash_Write_NUM(ISTIRED_ADDRESS, (float)moodeng->isTired);
}

void setEvolution(Moodeng_t *moodeng, int value)
{
    moodeng->evolution = inRangeInt(value, 0, 3);
    Flash_Write_NUM(EVOLUTION_ADDRESS, (float)moodeng->evolution);
}

void setEmotion(Moodeng_t *moodeng, Emotion_t value)
{
    moodeng->emotion = value;
    Flash_Write_NUM(EMOTION_ADDRESS, (float)moodeng->emotion);
}

void setIsAlive(Moodeng_t *moodeng, bool value)
{
    moodeng->isAlive = value;
    Flash_Write_NUM(ISALIVE_ADDRESS, (float)moodeng->isAlive);
}

void setNextDecayHappy(Moodeng_t *moodeng, int value)
{
    moodeng->nextDecayHappy = value;
    Flash_Write_NUM(NEXTDECAYHAPPY_ADDRESS, (float)moodeng->nextDecayHappy);
}

void setNextDecayHunger(Moodeng_t *moodeng, int value)
{
    moodeng->nextDecayHunger = value;
    Flash_Write_NUM(NEXTDECAYHUNGER_ADDRESS, (float)moodeng->nextDecayHunger);
}

void setNextPoopTime(Moodeng_t *moodeng, int value)
{
    moodeng->nextPoopTime = value;
    Flash_Write_NUM(NEXTPOOPTIME_ADDRESS, (float)moodeng->nextPoopTime);
}

void setNextSickTime(Moodeng_t *moodeng, int value)
{
    moodeng->nextSickTime = value;
    Flash_Write_NUM(NEXTSICKTIME_ADDRESS, (float)moodeng->nextSickTime);
}

void setNextHurtTime(Moodeng_t *moodeng, int value)
{
    moodeng->nextHurtTime = value;
    Flash_Write_NUM(NEXTHURTTIME_ADDRESS, (float)moodeng->nextHurtTime);
}

void setNextDirtyTime(Moodeng_t *moodeng, int value)
{
    moodeng->nextDirtyTime = value;
    Flash_Write_NUM(NEXTDIRTYTIME_ADDRESS, (float)moodeng->nextDirtyTime);
}

void setNextSleepyTime(Moodeng_t *moodeng, int value)
{
    moodeng->nextSleepyTime = value;
    Flash_Write_NUM(NEXTSLEEPYTIME_ADDRESS, (float)moodeng->nextSleepyTime);
}

void setSleepingTime(Moodeng_t *moodeng, int value)
{
    moodeng->sleepingTime = value;
    Flash_Write_NUM(SLEEPINGTIME_ADDRESS, (float)moodeng->sleepingTime);
}

void setIsSleeping(Moodeng_t *moodeng, bool value)
{
    moodeng->isSleeping = value;
    Flash_Write_NUM(ISSLEEPING_ADDRESS, (float)moodeng->isSleeping);
}

float Moodeng_FeedingChance(Moodeng_t *moodeng)
{
    return moodeng->discipline * 0.17f;
}

float Moodeng_SickChance(Moodeng_t *moodeng)
{
    return moodeng->weight * 0.0015f + moodeng->poopCount * 0.1f + moodeng->isTired * 0.40f + (0.2f - moodeng->happy * 0.05f) + (0.2f - moodeng->hunger * 0.05f);
}

float Moodeng_PlayingChance(Moodeng_t *moodeng)
{
    return moodeng->discipline * 0.22f - moodeng->weight * 0.4f;
}

void checkEvolution(Moodeng_t *moodeng, Clock_t *gameClock)
{
    if (moodeng->isAlive == false) return; 
    int totalMinutes = (gameClock->hour * 60) + gameClock->minute;

    switch (moodeng->evolution)
    {
    case 0:
        if (totalMinutes >= 10)
        {
            setEvolution(moodeng, 1);
            setDiscipline(moodeng, moodeng->discipline + 2);
            // Sound Evolution
            buzzer_play_sound(sound_evolution);
        }
        break;

    case 1:
        if (totalMinutes >= 180)
        {
            setEvolution(moodeng, 2);
            setDiscipline(moodeng, moodeng->discipline + 3);
            // Sound Evolution
            buzzer_play_sound(sound_evolution);
        }
        break;

    case 2:
        if (totalMinutes >= 360)
        {
            setEvolution(moodeng, 3);
            setDiscipline(moodeng, moodeng->discipline + 4);
            // Sound Evolution
            buzzer_play_sound(sound_evolution);
        }
        break;

    case 3:
        if (moodeng->happy == 4)
        {
            ui.activeAnim = &winAnim;
            // Game Win
            buzzer_play_sound(sound_game_win);
        }
        break;

    default:
        break;
    }
}

void Moodeng_Update(Moodeng_t *moodeng)
{
    // skip when sleep / die
    if (moodeng->isSleeping) return;
    if (moodeng->isAlive == false) return;
    // poop
    if (moodeng->nextPoopTime == -1 && moodeng->poopRate > 0.0f)
    {
        setNextPoopTime(moodeng, Moodeng_GenerateRandomNumber(moodeng, START_POOPTIME, END_POOPTIME));
    }
    if (moodeng->nextPoopTime > 0)
    {
        setNextPoopTime(moodeng, moodeng->nextPoopTime - 1);
        if (moodeng->nextPoopTime == 0)
        {
            float randomProb = (float)Moodeng_GenerateRandomNumber(moodeng, 0, 100) / 100.0f;

            if (randomProb < moodeng->poopRate)
            {
                setPoopCount(moodeng, moodeng->poopCount + 1);
                setNextPoopTime(moodeng, -1);
            }
            else
            {
                setPoopRate(moodeng, moodeng->poopRate + 0.2f);
                setNextPoopTime(moodeng, Moodeng_GenerateRandomNumber(moodeng, START_POOPTIME, END_POOPTIME));
            }
        }
    }
    // sick
    if (moodeng->isSick == false && moodeng->nextSickTime > 0)
    {
        setNextSickTime(moodeng, moodeng->nextSickTime - 1);
        if (moodeng->nextSickTime == 0)
        {
            float randomProb = (float)Moodeng_GenerateRandomNumber(moodeng, 0, 100) / 100.0f;

            if (randomProb < Moodeng_SickChance(moodeng))
            {
                setIsSick(moodeng, true);
                setNextHurtTime(moodeng, MOODENG_HURT_TIME);
                // Sound Sick
                buzzer_play_sound(sound_sick);
            }
            setNextSickTime(moodeng, MOODENG_INIT_SICKTIME);
        }
    }
    // hurt
    if (moodeng->isSick == true && moodeng->nextHurtTime > 0)
    {
        setNextHurtTime(moodeng, moodeng->nextHurtTime - 1);
        if (moodeng->nextHurtTime == 0)
        {
            setHappy(moodeng, moodeng->happy - 1);
            setHunger(moodeng, moodeng->hunger - 1);
            // Sound Sick
            buzzer_play_sound(sound_sick);
            if (moodeng->isSick == true)
                setNextHurtTime(moodeng, MOODENG_HURT_TIME);
        }
    }
    // dirty
    if (moodeng->poopCount >= 2 && moodeng->nextDirtyTime == -1)
    {
        setNextDirtyTime(moodeng, MOODENG_DIRTY_TIME);
    }
    if (moodeng->nextDirtyTime > 0 && moodeng->poopCount >= 2)
    {
        setNextDirtyTime(moodeng, moodeng->nextDirtyTime - 1);
        if (moodeng->nextDirtyTime == 0)
        {
            setHappy(moodeng, moodeng->happy - (moodeng->poopCount - 1));
            setNextDirtyTime(moodeng, MOODENG_DIRTY_TIME);
        }
    }
    // tired
    if (!moodeng->isTired && moodeng->nextSleepyTime > 0)
    {
        setNextSleepyTime(moodeng, moodeng->nextSleepyTime - 1);
        if (moodeng->nextSleepyTime == 0)
        {
            setisTired(moodeng, true);
            setNextSleepyTime(moodeng, MOODENG_INIT_SLEEPYTIME);
        }
    }
    // Happy, Hunger
    if (moodeng->nextDecayHappy > 0)
    {
        setNextDecayHappy(moodeng, moodeng->nextDecayHappy - 1);
        if (moodeng->nextDecayHappy == 0)
        {
            setHappy(moodeng, moodeng->happy - 1);
            setNextDecayHappy(moodeng, Moodeng_GenerateRandomNumber(moodeng, START_DECAY_HAPPY, END_DECAY_HAPPY));
        }
    }

    if (moodeng->nextDecayHunger > 0)
    {
        setNextDecayHunger(moodeng, moodeng->nextDecayHunger - 1);
        if (moodeng->nextDecayHunger == 0)
        {
            setHunger(moodeng, moodeng->hunger - 1);
            setNextDecayHunger(moodeng, Moodeng_GenerateRandomNumber(moodeng, START_DECAY_HUNGER, END_DECAY_HUNGER));
        }
    }
}

bool Moodeng_Minigame(Moodeng_t *moodeng, int guess)
{
    bool win = (guess == Moodeng_GenerateRandomNumber(moodeng, 0, 1));
    if (win)
    {
        setHappy(moodeng, moodeng->happy + 2);
    }
    else
    {
        setHappy(moodeng, moodeng->happy + 1);
    }
    setWeight(moodeng, moodeng->weight - 1);
    return win;
}

void Moodeng_Heal(Moodeng_t *moodeng)
{
    if (moodeng->isSick)
    {
        float randomProb = (float)Moodeng_GenerateRandomNumber(moodeng, 0, 100) / 100.0f;
        if (randomProb < moodeng->healRate)
        {
            setIsSick(moodeng, false);
        }
        else
        {
            setHealRate(moodeng, moodeng->healRate + 0.25f);
        }
    }
    else
    {
        setHappy(moodeng, moodeng->happy - 1);
    }
}

bool Moodeng_Check_Feed(Moodeng_t *moodeng)
{
    if (moodeng->emotion == SCOLDED)
    {
        setEmotion(moodeng, NORMAL);
        return true;
    }
    else if (moodeng->emotion == SILLY)
    {
        return false;
    }

    float randomProb = (float)Moodeng_GenerateRandomNumber(moodeng, 0, 100) / 100.0f;
    if (randomProb < Moodeng_FeedingChance(moodeng))
    {
        return true;
    }
    else
    {
        setEmotion(moodeng, SILLY);
        return false;
    }
}

bool Moodeng_Check_Play(Moodeng_t *moodeng)
{
    if (moodeng->emotion == SCOLDED)
    {
        setEmotion(moodeng, NORMAL);
        return true;
    }
    else if (moodeng->emotion == SILLY)
    {
        return false;
    }

    float randomProb = (float)Moodeng_GenerateRandomNumber(moodeng, 0, 100) / 100.0f;
    if (randomProb < Moodeng_PlayingChance(moodeng))
    {
        return true;
    }
    else
    {
        setEmotion(moodeng, SILLY);
        return false;
    }
}

void Moodeng_Handle_Lose(Moodeng_t* moodeng) 
{
    if (moodeng->isAlive == true) {
        setIsAlive(moodeng, false);
        ui.activeAnim = &loseAnim;
        buzzer_play_sound(sound_game_lose);
        losing = true;
        loseStart = HAL_GetTick();
    }
}

void Moodeng_lose_animation(Moodeng_t* moodeng)
{
    if (moodeng->isAlive == false) {
        if (losing && HAL_GetTick() - loseStart >= 2000) {
            losing = false;
            Moodeng_Reset(moodeng);
            Timer_Reset(&gameClock);
            ui.menuState = MENU_MAIN;
            ui.activeAnim = &idleAnim;
        }
    }
}
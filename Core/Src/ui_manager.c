/*
 * ui_manager.c
 *
 *  Created on: Oct 4, 2025
 *      Author: Acer
 */

#include "ui_manager.h"
#include "pet_sprites.h" // include sprite data arrays
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "stdbool.h"
#include "main.h"
#include "moodeng.h"
#include <stdio.h>
SpriteAnimator_t idleAnim;
SpriteAnimator_t sleepAnimDay;
SpriteAnimator_t sleepAnimNight;
SpriteAnimator_t menuBarAnim;
SpriteAnimator_t feedSnackAnim;
SpriteAnimator_t feedMealAnim;
SpriteAnimator_t playGameAnim;
SpriteAnimator_t medicineAnim;
SpriteAnimator_t showerAnim;
SpriteAnimator_t winAnim;
SpriteAnimator_t loseAnim;
SpriteAnimator_t miniGameCorrectAnim;
SpriteAnimator_t miniGameWrongAnim;
SpriteAnimator_t sickAnim;
SpriteAnimator_t stubbornAnim;
SpriteAnimator_t selectStateAnim;
SpriteAnimator_t heartAnim;
SpriteAnimator_t hungerAnim;
SpriteAnimator_t shitAnim;
SpriteAnimator_t evolutionAnim;
SpriteAnimator_t injectAnim;
SpriteAnimator_t cleanAnim;
extern ADC_HandleTypeDef hadc1; // From main.c or auto-generated MX_ADC1_Init()
extern UART_HandleTypeDef huart3;
extern Moodeng_t moodeng;

#define MOODENG_X_POS 28
#define MOODENG_Y_POS 56
void UIManager_Init(UIManager_t *ui)
{
    static const uint16_t *idleFrames[] = {idle1, idle2, idle3, idle4};
    static const uint16_t *sleepFramesDay[] = {sleep_day1, sleep_day2};
    static const uint16_t *sleepFramesNight[] = {sleep_night1, sleep_night2};
    static const uint16_t *menuBarFrame[] = {menuBar};

    static const uint16_t *feedSnackFrames[] = {feedSnack1, feedSnack2};
    static const uint16_t *feedMealFrames[] = {feedMeal1, feedMeal2};

    static const uint16_t *playGameFrames[] = {playGame1};
    static const uint16_t *medicineFrames[] = {medicine1};
    static const uint16_t *showerFrames[] = {shower1};
    static const uint16_t *winFrames[] = {win1, win2};
    static const uint16_t *loseFrames[] = {lose1};
    static const uint16_t *miniGameCorrectFrames[] = {correctAnswer1};
    static const uint16_t *miniGameWrongFrames[] = {wrongAnswer1};
    static const uint16_t *sickFrames[] = {sickness1, sickness2};
    static const uint16_t *stubbornFrames[] = {stubborn1, stubborn2};
    static const uint16_t *injectFrames[] = {inject1, inject2, medicine1};
    static const uint16_t *cleanFrames[] = {clean1, clean2, shower1};
    static const uint16_t *selectStateFrames[] = {selectState1};
    static const uint16_t *heartFrames[] = {heart1};
    static const uint16_t *hungerFrames[] = {hunger1};
    static const uint16_t *shitFrames[] = {shit1};
    static const uint16_t *evolutionFrames[] = {evolution1};

    SpriteAnimator_Init(&idleAnim, idleFrames, 2, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&sleepAnimDay, sleepFramesDay, 2, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&sleepAnimNight, sleepFramesNight, 2, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);

    SpriteAnimator_Init(&feedSnackAnim, feedSnackFrames, 2, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&feedMealAnim, feedMealFrames, 2, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);

    SpriteAnimator_Init(&medicineAnim, medicineFrames, 1, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&showerAnim, showerFrames, 1, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&winAnim, winFrames, 2, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&loseAnim, loseFrames, 1, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&miniGameCorrectAnim, miniGameCorrectFrames, 1, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&miniGameWrongAnim, miniGameWrongFrames, 1, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&sickAnim, sickFrames, 2, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&stubbornAnim, stubbornFrames, 2, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&playGameAnim, playGameFrames, 1, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, true);
    SpriteAnimator_Init(&injectAnim, injectFrames, 3, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, false);
    SpriteAnimator_Init(&cleanAnim, cleanFrames, 3, MOODENG_X_POS, MOODENG_Y_POS, 180, 180, 300, false);

    SpriteAnimator_Init(&selectStateAnim, selectStateFrames, 1, 0, 244, 18, 15, 300, true);
    SpriteAnimator_Init(&menuBarAnim, menuBarFrame, 1, 0, SCREEN_HEIGHT - 56, SCREEN_WIDTH, 56, 300, true);

    SpriteAnimator_Init(&heartAnim, heartFrames, 1, 0, 5, 40, 40, 300, true);
    SpriteAnimator_Init(&hungerAnim, hungerFrames, 1, 55, 5, 40, 40, 300, true);

    SpriteAnimator_Init(&shitAnim, shitFrames, 1, 110, 5, 40, 40, 300, true);
    SpriteAnimator_Init(&evolutionAnim, evolutionFrames, 1, 165, 8, 64, 35, 300, true);

    ui->menuState = MENU_MAIN;
    ui->activeAnim = &idleAnim;
    ui->menuBarAnim = &menuBarAnim;
    ui->selectedStateAnim = &selectStateAnim;
    ui->heartAnim = &heartAnim;
    ui->hungerAnim = &hungerAnim;
    ui->shitAnim = &shitAnim;
    ui->evolutionAnim = &evolutionAnim;
    ui->selectedState = MENU_MAIN;
    ui->isLightOn = true; // Default day
    ui->lightLevel = 0;
}
void setActiveAnim(UIManager_t *ui, SpriteAnimator_t *anim)
{
    ui->activeAnim = anim;
    ui->activeAnim->currentFrame = 0; // Reset to first frame
}
void UIManager_SetState(UIManager_t *ui, MenuState_t newState)
{
    ui->menuState = newState;

    switch (newState)
    {
    case MENU_FEED:
        setActiveAnim(ui, &feedMealAnim);
        break;
    case MENU_PLAY:
        setActiveAnim(ui, &playGameAnim);
        break;
    case MENU_CLEAN:
        setActiveAnim(ui, &showerAnim);
        break;
    case MENU_MEDICINE:
        setActiveAnim(ui, &medicineAnim);
        break;

    case MENU_SLEEP:
        if (moodeng.isSleeping)
        {
            if (ui->isLightOn)
                setActiveAnim(ui, &sleepAnimDay);
            else
                setActiveAnim(ui, &sleepAnimNight);
        }
        else
        {
            setActiveAnim(ui, &idleAnim);
        }
        break;
    default:
        setActiveAnim(ui, moodeng.isSick ? &sickAnim : &idleAnim);
        ui->selectedState = MENU_MAIN;
        ui->selectedStateAnim->x = 0;
        break;
    }
}
void UIManager_Update(UIManager_t *ui, uint32_t currentTime)
{

    // If currently in sleep menu, react to LDR
    if (ui->menuState == MENU_SLEEP)
    {

        // Read LDR ADC1
        HAL_ADC_Start(&hadc1);
        while (HAL_ADC_PollForConversion(&hadc1, 100) != HAL_OK)
        {
        }
        if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
        {
            /* light on around 800 | light off around 2000+ */
            ui->lightLevel = HAL_ADC_GetValue(&hadc1);
        }
        HAL_ADC_Stop(&hadc1);

        // Determine light status (adjust threshold for your LDR circuit)
        bool newLightState = (ui->lightLevel < 1500); // Example threshold

        if (newLightState != ui->isLightOn)
        {
            ui->isLightOn = newLightState;
            // ✅ Reuse same function
            UIManager_SetState(ui, MENU_SLEEP);

            // Optional UART debug
            char msg[128];
            int n = snprintf(msg, sizeof(msg),
                             "Light change: %lu | isLightOn=%d\r\n",
                             (unsigned long)ui->lightLevel, ui->isLightOn);
            HAL_UART_Transmit(&huart3, (uint8_t *)msg, n, HAL_MAX_DELAY);
        }
    }

    // Update animation normally
    if (ui->activeAnim)
        SpriteAnimator_Update(ui->activeAnim, currentTime);
}
// Helper to draw stat icon and value
void drawStat(SpriteAnimator_t *anim, int value)
{
    if (anim != NULL)
    {
        SpriteAnimator_Draw(anim);
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", value);
        ILI9341_Draw_Text(buf, anim->x + anim->w + 2, anim->y + 12, WHITE, 2, BACKGROUND_COLOR);
    }
}
void UIManager_Draw(UIManager_t *ui)
{

    if (ui->activeAnim)
        SpriteAnimator_Draw(ui->activeAnim);

    if (ui->menuBarAnim)
        SpriteAnimator_Draw(ui->menuBarAnim);

    if (ui->selectedStateAnim)
    {
        ILI9341_Draw_Filled_Rectangle_Coord(0, 244, SCREEN_WIDTH, 244 + 15, BACKGROUND_COLOR);
        SpriteAnimator_Draw(ui->selectedStateAnim);
    }

    drawStat(ui->heartAnim, moodeng.happy);
    drawStat(ui->hungerAnim, moodeng.hunger);
    drawStat(ui->shitAnim, moodeng.poopCount);

    drawStat(ui->evolutionAnim, moodeng.evolution);
}

void uiManagerResetToIdle(UIManager_t *ui)
{
    ui->menuState = MENU_MAIN;
    ui->selectedState = MENU_MAIN;
    ui->selectedStateAnim->x = 0;
    setActiveAnim(ui, moodeng.isSick ? &sickAnim : &idleAnim);
}
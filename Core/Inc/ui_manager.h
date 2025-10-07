/*
 * ui_manager.h
 *
 *  Created on: Oct 4, 2025
 *      Author: Acer
 */

#ifndef INC_UI_MANAGER_H_
#define INC_UI_MANAGER_H_

#include "sprite_animator.h"
#include "ILI9341_GFX.h"
#include "stdbool.h"
#include "main.h"

extern SpriteAnimator_t idleAnim;
extern SpriteAnimator_t sleepAnimDay;
extern SpriteAnimator_t sleepAnimNight;
extern SpriteAnimator_t menuBarAnim;
extern SpriteAnimator_t feedSnackAnim;
extern SpriteAnimator_t feedMealAnim;
extern SpriteAnimator_t playGameAnim;
extern SpriteAnimator_t medicineAnim;
extern SpriteAnimator_t cleanAnim;
extern SpriteAnimator_t winAnim;
extern SpriteAnimator_t loseAnim;
extern SpriteAnimator_t miniGameCorrectAnim;
extern SpriteAnimator_t miniGameWrongAnim;
extern SpriteAnimator_t sickAnim;
extern SpriteAnimator_t stubbornAnim;

typedef enum {
    MENU_MAIN = 0,
    MENU_FEED,
    MENU_PLAY,
    MENU_SLEEP,
    MENU_CLEAN,
    MENU_MEDICINE
} MenuState_t;

typedef struct {
    MenuState_t menuState;       // Confirmed menu (active state)
    MenuState_t selectedState;   // Currently highlighted but not confirmed
    SpriteAnimator_t* activeAnim;
    SpriteAnimator_t* menuBarAnim;
    uint16_t lightLevel;         // Store latest LDR ADC value
    bool isLightOn;              // Cached state (to avoid flickering)
} UIManager_t;
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
void UIManager_Init(UIManager_t* ui);
void UIManager_SetState(UIManager_t* ui, MenuState_t newState);
void UIManager_Update(UIManager_t* ui, uint32_t currentTime);
void UIManager_Draw(UIManager_t* ui);
void UIManager_Display_text(UIManager_t* ui,bool shouldClear);
#endif /* INC_UI_MANAGER_H_ */

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

typedef enum {
    MENU_MAIN = 0,
    MENU_FEED,
    MENU_PLAY,
    MENU_MEDICINE,
    MENU_CLEAN,
    MENU_SLEEP
} MenuState_t;

typedef struct {
    MenuState_t menuState;       // Confirmed menu (active state)
    MenuState_t selectedState;   // Currently highlighted but not confirmed
    SpriteAnimator_t* activeAnim;
    SpriteAnimator_t* menuBarAnim;
    SpriteAnimator_t* selectedStateAnim;
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

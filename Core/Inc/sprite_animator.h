/*
 * sprite_animator.h
 *
 *  Created on: Oct 4, 2025
 *      Author: Acer
 */

#ifndef INC_SPRITE_ANIMATOR_H_
#define INC_SPRITE_ANIMATOR_H_

#include "ILI9341_GFX.h"
#include "stdint.h"
#include <stdbool.h>

typedef struct {
    const uint16_t** frames;   // Array of sprite frame pointers
    uint8_t frameCount;        // Number of frames
    uint8_t currentFrame;      // Current frame index
    uint16_t x, y;             // Draw position
    uint16_t w, h;             // Frame dimensions
    uint32_t frameDelay;       // Delay per frame (ms)
    uint32_t lastFrameTime;    // For animation timing
    bool shouldLoop;           // Whether the animation should loop
} SpriteAnimator_t;

void SpriteAnimator_Init(SpriteAnimator_t* anim,
                         const uint16_t** frames,
                         uint8_t frameCount,
                         uint16_t x, uint16_t y,
                         uint16_t w, uint16_t h,
                         uint32_t frameDelay, bool shouldLoop);

void SpriteAnimator_Update(SpriteAnimator_t* anim, uint32_t currentTime);
void SpriteAnimator_Draw(SpriteAnimator_t* anim);
void ILI9341_Draw_PartImage(const uint16_t* image, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

#endif /* INC_SPRITE_ANIMATOR_H_ */

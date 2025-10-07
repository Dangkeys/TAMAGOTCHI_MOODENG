/*
 * sprite_animator.c
 *
 *  Created on: Oct 4, 2025
 *      Author: Acer
 */


#include "sprite_animator.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"


void SpriteAnimator_Init(SpriteAnimator_t* anim,
                         const uint16_t** frames,
                         uint8_t frameCount,
                         uint16_t x, uint16_t y,
                         uint16_t w, uint16_t h,
                         uint32_t frameDelay)
{
    anim->frames = frames;
    anim->frameCount = frameCount;
    anim->x = x;
    anim->y = y;
    anim->w = w;
    anim->h = h;
    anim->currentFrame = 0;
    anim->frameDelay = frameDelay;
    anim->lastFrameTime = 0;
}

void SpriteAnimator_Update(SpriteAnimator_t* anim, uint32_t currentTime)
{
    if (currentTime - anim->lastFrameTime >= anim->frameDelay) {
        anim->currentFrame = (anim->currentFrame + 1) % anim->frameCount;
        anim->lastFrameTime = currentTime;
    }
}

void ILI9341_Draw_PartImage(const uint16_t* image, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    ILI9341_Set_Address(x, y, x + w - 1, y + h - 1);

    ILI9341_Write_Command(0x2C);
    for (uint32_t i = 0; i < w * h; i++)
    {
        ILI9341_Write_Data(image[i] >> 8);
        ILI9341_Write_Data(image[i] & 0xFF);
    }
}

// void ILI9341_Draw_PartImage(const uint16_t* image, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
// {
//     // Process row by row to maintain efficiency
//     for (uint16_t row = 0; row < h; row++) {
//         uint16_t segmentStart = 0;
        
//         while (segmentStart < w) {
//             // Find start of non-black segment
//             while (segmentStart < w && image[row * w + segmentStart] == 0x0000) {
//                 segmentStart++;
//             }
            
//             if (segmentStart >= w) break; // End of row
            
//             // Find end of non-black segment
//             uint16_t segmentEnd = segmentStart;
//             while (segmentEnd < w && image[row * w + segmentEnd] != 0x0000) {
//                 segmentEnd++;
//             }
            
//             // Draw the segment
//             uint16_t segmentWidth = segmentEnd - segmentStart;
//             ILI9341_Set_Address(x + segmentStart, y + row, x + segmentEnd - 1, y + row);
//             ILI9341_Write_Command(0x2C);
            
//             for (uint16_t i = segmentStart; i < segmentEnd; i++) {
//                 uint16_t pixel = image[row * w + i];
//                 ILI9341_Write_Data(pixel >> 8);
//                 ILI9341_Write_Data(pixel & 0xFF);
//             }
            
//             segmentStart = segmentEnd;
//         }
//     }
// }

void SpriteAnimator_Draw(SpriteAnimator_t* anim)
{
	ILI9341_Draw_PartImage((uint16_t*)anim->frames[anim->currentFrame], anim->x, anim->y, anim->w, anim->h);
}

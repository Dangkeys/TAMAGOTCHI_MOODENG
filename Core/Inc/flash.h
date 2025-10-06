/**
  ***************************************************************************************************************
  ***************************************************************************************************************
  ***************************************************************************************************************
  File:	      flash.c
  Modifier:   ControllersTech.com
  Updated:    27th MAY 2021
  ***************************************************************************************************************
  Copyright (C) 2017 ControllersTech.com
  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.
  ***************************************************************************************************************
*/

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#define HAPPY_ADDRESS 0x081E0000U
#define WEIGHT_ADDRESS 0x081E0004U
#define HUNGER_ADDRESS 0x081E0008U
#define POOPCOUNT_ADDRESS 0x081E000CU
#define POOPRATE_ADDRESS 0x081E0010U
#define ISSICK_ADDRESS 0x081E0014U
#define HEALRATE_ADDRESS 0x081E0018U
#define DISCIPLINE_ADDRESS 0x081E001CU
#define ISTRIED_ADDRESS 0x081E0020U
#define EVOLUTION_ADDRESS 0x081E0024U
#define ISALIVE_ADDRESS 0x081E0028U
#define EMOTION_ADDRESS 0x081E002CU
#define NEXTDECAYHAPPY_ADDRESS 0x081E0030U
#define NEXTDECAYHUNGER_ADDRESS 0x081E0034U
#define NEXTPOOPTIME_ADDRESS 0x081E0038U
#define NEXTSICKTIME_ADDRESS 0x081E003CU
#define NEXTHURTTIME_ADDRESS 0x081E0040U
#define NEXTDIRTYTIME_ADDRESS 0x081E0044U
#define NEXTSLEEPYTIME_ADDRESS 0x081E0048U

#include <stdint.h>
#include <stdbool.h>

uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords);

void Flash_Read_Data (uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords);

void Convert_To_Str (uint32_t *Data, char *Buf);

void Flash_Write_NUM (uint32_t StartSectorAddress, float Num);

float Flash_Read_NUM (uint32_t StartSectorAddress);

bool Flash_IsDataValid(uint32_t StartSectorAddress);


#endif /* INC_FLASH_H_ */
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

#define HAPPY_ADDRESS 0x081C0000U
#define WEIGHT_ADDRESS 0x081C0010U
#define HUNGER_ADDRESS 0x081C0020U
#define POOPCOUNT_ADDRESS 0x081C0030U
#define POOPRATE_ADDRESS 0x081C0040U
#define ISSICK_ADDRESS 0x081C0050U
#define HEALRATE_ADDRESS 0x081C0060U
#define DISCIPLINE_ADDRESS 0x0081C0070U
#define ISTIRED_ADDRESS 0x081C0080U
#define EVOLUTION_ADDRESS 0x081C0090U
#define ISALIVE_ADDRESS 0x081C00A0U
#define EMOTION_ADDRESS 0x081C00B0U
#define NEXTDECAYHAPPY_ADDRESS 0x081C00C0U
#define NEXTDECAYHUNGER_ADDRESS 0x081C00D0U
#define NEXTPOOPTIME_ADDRESS 0x081C00E0U
#define NEXTSICKTIME_ADDRESS 0x081C00F0U
#define NEXTHURTTIME_ADDRESS 0x081C0100U
#define NEXTDIRTYTIME_ADDRESS 0x081C0110U
#define NEXTSLEEPYTIME_ADDRESS 0x081C0120U
#define SLEEPINGTIME_ADDRESS 0x081C0130U
#define ISSLEEPING_ADDRESS 0x081C0140U
#define GAMECLOCK_HOUR_ADDRESS 0x081C0150U
#define GAMECLOCK_MINUTE_ADDRESS 0x081C0160U
#define GAMECLOCK_SECOND_ADDRESS 0x081C0170U

#include <stdint.h>
#include <stdbool.h>

uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords);

void Flash_Read_Data (uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords);

void Convert_To_Str (uint32_t *Data, char *Buf);

void Flash_Write_NUM (uint32_t StartSectorAddress, uint16_t Num);

uint16_t Flash_Read_NUM (uint32_t StartSectorAddress);

bool Flash_IsDataValid(uint32_t StartSectorAddress);


#endif /* INC_FLASH_H_ */
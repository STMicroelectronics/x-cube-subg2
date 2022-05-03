/**
  ******************************************************************************
  * @file    utils.c
  * @author  STMicroelectronics
  * @brief   Utility for CLI application
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include <stdint.h>
#include "utils.h"

IrqQueue s_xIrqQueue = { .cIndexPush = 0, .cIndexPop = 0, .cIrqQueueSize = 0 };

uint32_t BufferToUint32(uint8_t* ucBuffer)
{
  uint32_t nTmp = 0;

  for(int8_t cI=3; cI>=0; cI--)
    nTmp |= (uint8_t)((uint32_t)ucBuffer[3-cI])<<(8*cI);

  return nTmp;
}

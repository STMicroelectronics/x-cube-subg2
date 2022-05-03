
/**
  ******************************************************************************
  * @file    s2lp_management.h
  * @author  SRA Application Team
  * @brief   Management functions for S2LP.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __S2LP_MANAGEMENT_H__
#define __S2LP_MANAGEMENT_H__

/* Includes ------------------------------------------------------------------*/
#include "S2LP_Gpio.h"
#include "S2LP_Radio.h"
#include "s2lp.h"
#include "S2LP_Regs.h"
#include "S2LP_Types.h"
#include "s2868a1.h"

#include "s2lp_sdkapi_mapping.h"

#ifdef __cplusplus
  "C" {
#endif

/**
 * @addtogroup S2LP_MANAGEMENT
 * @{
 */
/* This is getting the base frequency from the band defined in the board.
 * For user board, define the desired frequency in Hz (e.g: 868000000) */
#define BOARD_BASE_FREQUENCY                    (S2LP_GetFrequencyBand())

#define BOARD_FREQUENCY_BAND 3 /* 868MHz */

typedef enum
{
  RANGE_EXT_NONE = 0,
  RANGE_EXT_SKY_66100_11 = 1,
  RANGE_EXT_SKYWORKS_SE2435L = 2,
  RANGE_EXT_SKYWORKS_SKY66420 = 3,
  RANGE_EXT_SKYWORKS_868 = 4
} RangeExtType;

typedef enum
{
  S2LP_CUT_2_1 = 0x91,
  S2LP_CUT_2_0 = 0x81,
  S2LP_CUT_3_0 = 0xC1,
} S2LPCutType;

/**
 * @addgroup S2LP_MANAGEMENT_FUNCTIONS
 * @{
 */

void S2LP_ManagementIdentificationRFBoard(void);
uint32_t S2LP_ManagementComputeXtalFrequency(void);

RangeExtType S2LP_ManagementGetRangeExtender(void);
void S2LP_ManagementSetRangeExtender(RangeExtType xRangeType);
void S2LP_ManagementRangeExtInit(void);
S2LPCutType S2LP_ManagementGetCut(void);
uint8_t S2LP_ManagementGetTcxo(void);
void S2LP_ManagementSetBand(uint8_t value);
uint8_t S2LP_ManagementGetBand(void);
uint32_t S2LP_GetFrequencyBand(void);
void S2LP_ManagementSetOffset(int32_t value);
int32_t S2LP_ManagementGetOffset(void);
void S2LP_ManagementSetTcxo(uint8_t tcxo);

uint8_t EepromIdentification(void);

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /*__S2LP_MANAGEMENT_H__*/

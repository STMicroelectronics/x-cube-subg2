
/**
  ******************************************************************************
  * @file    s2lp_interface.h
  * @author  SRA Application Team
  * @brief   Interface functions for S2LP.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under ODE Software License Agreement
  * SLA0094, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0094
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef S2LP_INTERFACE_H
#define S2LP_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include "s2lp_gpio.h"
#include "s2lp_radio.h"
#include "s2lp.h"
#include "s2lp_regs.h"
#include "s2lp_types.h"
#include "s2868a1.h"

#ifdef __cplusplus
  "C" {
#endif

/**
 * @addtogroup S2LP_INTERFACE
 * @{
 */

typedef struct
{
  uint16_t nS2LP_Version;
}S2LP_VersionMap;

#define CUT_MAX_NO 3
#define CUT_2_1v3 0x0103
#define CUT_2_1v4 0x0104
#define CUT_3_0   0x0130

/**
 * @addgroup S2LP_INTERFACE_FUNCTIONS
 * @{
 */

void S2LPManagementIdentificationRFBoard(void);
uint32_t S2LPManagementComputeXtalFrequency(void);

RangeExtType S2LP_ManagementGetRangeExtender(void);
void S2LP_ManagementSetRangeExtender(RangeExtType xRangeType);
void S2LP_ManagementRangeExtInit(void);
S2LPCutType S2LP_ManagementGetCut(void);
uint8_t S2LP_ManagementGetTcxo(void);
void S2LP_ManagementSetBand(uint8_t value);
uint8_t S2LP_ManagementGetBand(void);
void S2LP_ManagementSetOffset(int32_t value);
int32_t S2LP_ManagementGetOffset(void);
uint8_t EepromIdentification(void);
uint8_t SdkEvalGetHasEeprom(void);
void S2LP_ManagementSetTcxo(uint8_t tcxo);

void S2LPInterfaceInit(void);
void S2LPGpioIrqInit(S2LPIrqs *pGpioIRQ);
void S2LP_RadioInit(SRadioInit *pRadioInit);

void S2LP_Interface_IoIrqEnable(void);
void S2LP_Interface_IoIrqDisable(void);
void S2LPManagementRcoCalibration(void);

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

#endif /*S2LP_INTERFACE_H*/

 /******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/

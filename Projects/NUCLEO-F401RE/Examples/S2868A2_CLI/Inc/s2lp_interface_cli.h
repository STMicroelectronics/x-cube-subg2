
/**
  ******************************************************************************
  * @file    s2lp_interface_cli.h
  * @author  SRA Application Team
  * @brief   Interface functions for S2LP.
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
#ifndef S2LP_INTERFACE_H
#define S2LP_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include "S2LP_Gpio.h"
#include "S2LP_Radio.h"
#include "s2lp.h"
#include "S2LP_Regs.h"
#include "S2LP_Types.h"
#include "s2868a2.h"

#include "s2lp_sdkapi_mapping.h"

#ifdef __cplusplus
  "C" {
#endif

/**
 * @addtogroup S2LP_INTERFACE
 * @{
 */

/**
 * @addgroup S2LP_INTERFACE_FUNCTIONS
 * @{
 */
void S2LPInterfaceInit(void);
void S2LPGpioIrqInit(S2LPIrqs *pGpioIRQ);
void S2LP_RadioInit(SRadioInit *pRadioInit);

void S2LP_Interface_IoIrqEnable(void);
void S2LP_Interface_IoIrqDisable(void);

void Radio_interrupt_callback_0(void);
void Radio_interrupt_callback_1(void);
void Radio_interrupt_callback_2(void);
void Radio_interrupt_callback_3(void);

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

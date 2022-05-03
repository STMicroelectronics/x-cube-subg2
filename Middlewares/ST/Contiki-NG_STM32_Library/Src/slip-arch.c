 /**
  ******************************************************************************
  * @file    slip-arch.c
  * @author  SRA Application Team
  * @brief   SLIP (Serial Line IP) API implementation
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

/**
 * @defgroup slip
 * @ingroup Contiki-NG_STM32_Library
 * @{
 */

/**
 * @addtogroup slip
 * @ingroup Contiki-NG_STM32_Library
 * @{
 * @file Implementation of slip functions for stm32 platform
 */
/* Includes ------------------------------------------------------------------*/

#include "contiki.h"
#include "platform-conf.h"
#include <stdio.h>
#if BUILD_WITH_RPL_BORDER_ROUTER
#include "dev/slip.h"
#endif /*BUILD_WITH_RPL_BORDER_ROUTER*/
#include "main.h"
#include "contiki-platform.h"
#include "console.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/**
 * @brief  slip_arch_init
 * 		initialises slip device
 * @param  unsigned long baudrate
 * @retval none
 */
void
slip_arch_init(void)
{
#if BUILD_WITH_RPL_BORDER_ROUTER
  uart_set_input(0, slip_input_byte);
#endif /*BUILD_WITH_RPL_BORDER_ROUTER*/
}
/*--------------------------------------------------------------------------*/

/**
 * @brief  slip_arch_writeb
 * 		writes a byte to the slip
 * @param  uint8_t c
 * @retval none
 */
void
slip_arch_writeb(uint8_t c)
{
  (void) uart_send_char(c);
}
/*--------------------------------------------------------------------------*/
/** @} */
/** @} */

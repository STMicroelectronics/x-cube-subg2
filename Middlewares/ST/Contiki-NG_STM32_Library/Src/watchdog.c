 /**
  ******************************************************************************
  * @file    watchdog.c
  * @author  SRA Application Team
  * @brief   Watchdog API implementation
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "dev/watchdog.h"
#include "main.h"
#include "contiki-conf.h"

#if WATCHDOG_ENABLE
/** @defgroup watchdog
* @ingroup Contiki-NG_STM32_Library
* @{
*/

/** @addtogroup watchdog
* @{
* @file Implementation of watchdog functions for stm32 platform
*/

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* IWWDG handler declaration */
#endif /*WATCHDOG_ENABLE*/

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/**
 * @brief  watchdog_init
 * 		initialises watchdog
 * @param  none
 * @retval none
 */
void
watchdog_init(void)
{
//@TODO: add Watchdog init implementation
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  watchdog_start
 * 		starts watchdog
 * @param  none
 * @retval none
 */
void
watchdog_start(void)
{
#if WATCHDOG_ENABLE
//@TODO: add Watchdog start implementation
#endif /*WATCHDOG_ENABLE*/  
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  watchdog_periodic
 * 		periodic watchdog callback
 * @param  none
 * @retval none
 */
void
watchdog_periodic(void)
{
#if WATCHDOG_ENABLE
//@TODO: add Watchdog periodic implementation
#endif /*WATCHDOG_ENABLE*/
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  watchdog_stop
 * 		stops watchdog
 * @param  none
 * @retval none
 */
void
watchdog_stop(void)
{
//@TODO: add Watchdog stop implementation  
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  watchdog_reboot
 * 		reboots the system
 * @param  none
 * @retval none
 */
void
watchdog_reboot(void)
{
#if WATCHDOG_ENABLE  
//@TODO: add Watchdog reboot implementation
#endif /*WATCHDOG_ENABLE*/  
}
/*---------------------------------------------------------------------------*/
/**
* @}
*/

/**
* @}
*/

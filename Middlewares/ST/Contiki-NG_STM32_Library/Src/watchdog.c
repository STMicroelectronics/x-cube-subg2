 /**
  ******************************************************************************
  * @file    watchdog.c
  * @author  SRA Application Team
  * @brief   Watchdog API implementation
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Software License Agreement
  * SLA0055, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0055
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

 /**
  ******************************************************************************
  * @file    rtimer-arch.c
  * @author  SRA Application Team
  * @brief   RTimer Clock implementation
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
#include "contiki.h"
#include "platform-conf.h"

#include "sys/rtimer.h"
#include "sys/process.h"
#include "dev/watchdog.h"

#include "main.h"
#include "subg2_bsp_ip_conf.h"
#include "subg2_contiki_it.h"

/** @defgroup Rtimer_arch
* @ingroup Contiki-NG_STM32_Library
* @{
*/

/** @addtogroup Rtimer_arch
* @{
* @file Implementation of rtimer functions for stm32 platform
*/

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/*Main Contiki rtimer */
extern TIM_HandleTypeDef contiki_rtimer;
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/**
 * @brief  rtimer_arch_init
 * 		initialises rtimer 
 * @param  none
 * @retval none
 */
void rtimer_arch_init(void)
{
  TIM_platform_rtimer_init();
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  rtimer_arch_now
 * 		returns current rtime value
 * @param  none
 * @retval rtimer_clock_t current rtimer value
 */
rtimer_clock_t rtimer_arch_now(void)
{
  return  (rtimer_clock_t) __HAL_TIM_GET_COUNTER(&contiki_rtimer);
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  rtimer_arch_schedule
 * 		schedule next rtimer expiration
 * @param  rtimer_clock_t next value
 * @retval none
 */
void rtimer_arch_schedule(rtimer_clock_t t)
{
  TIM_platform_rtimer_schedule(t);
}
/*---------------------------------------------------------------------------*/
/**
* @}
*/

/**
* @}
*/

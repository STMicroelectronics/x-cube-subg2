/**
  ******************************************************************************
  * @file    clock.c
  * @author  SRA Application Team
  * @brief   Clock API Implementation
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
#include "contiki.h"
#include "platform-conf.h"
#include "contiki-conf.h"
#include "dev/leds.h"
#include "main.h"

/** @defgroup clock
* @ingroup Contiki-NG_STM32_Library
* @{
*/

/** @addtogroup clock
* @{
* @file Implementation of clock functions for stm32 platform
*/

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
volatile unsigned long seconds;
volatile clock_time_t ticks;
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/**
 * @brief  clock_init
 * 		initialises clock for Contiki-NG
 * @param  none
 * @retval none
 */
void clock_init(void)
{ 
  ticks = 0;
  seconds = 0;
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  (void) HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/((uint32_t)CLOCK_SECOND));
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  clock_seconds
 * 		returns current clock value in seconds
 * @param  none
 * @retval unsigned long current time in seconds
 */
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
unsigned long clock_seconds(void)
{
  return seconds;
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  clock_set_seconds
 * 		sets current clock value in seconds
 * @param  unsigned long time to set
 * @retval none
 */
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
void clock_set_seconds(unsigned long sec)
{
  seconds = sec;
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  clock_time
 * 		returns current clock value in ticks
 * @param  none
 * @retval clock_time_t current time in ticks
 */
clock_time_t clock_time(void)
{
  return ticks;
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  clock_delay_usec
 * 		delay for a specific amount of time (in us)
 * @param  uint16_t usec to delay
 * @retval none
 */
void
clock_delay_usec(uint16_t usec)
{
  HAL_Delay((uint32_t) usec/ ((uint32_t) 1000));
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  clock_wait
 * 		wait for a specific amount of time (in ticks)
 * @param  clock_time_t ticks to wait
 * @retval none
 */
void clock_wait(clock_time_t t)
{
  clock_time_t start;
  start = clock_time();
  while((clock_time() - start) < (clock_time_t)t ){
  }
}
/*---------------------------------------------------------------------------*/
/**
* @}
*/

/**
* @}
*/

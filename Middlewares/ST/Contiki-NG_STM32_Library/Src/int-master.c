 /**
  ******************************************************************************
  * @file    int-master.c
  * @author  SRA Application Team
  * @brief   Master interrupt for ST platform
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
#include "main.h" 
#include "contiki.h"
#include "sys/int-master.h"
#include <stdbool.h>

/** @defgroup int_master
* @ingroup Contiki-NG_STM32_Library
* @{
*/

/** @addtogroup int_master
* @{
* @file Implementation of int_master functions for stm32 platform
*/

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/**
 * @brief  int_master_enable
 * 		enables irq
 * @param  none
 * @retval none
 */
void
int_master_enable(void)
{
  __enable_irq();
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  int_master_read_and_disable
 * 		reads status and disable irq
 * @param  none
 * @retval int_master_status_t irq mask status 
 */
int_master_status_t
int_master_read_and_disable(void)
{
  int_master_status_t primask = __get_PRIMASK();

  __disable_irq();

  return primask;
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  int_master_status_set
 * 		sets irq status 
 * @param  int_master_status_t irq mask status
 * @retval none 
 */
void
int_master_status_set(int_master_status_t status)
{
  __set_PRIMASK(status);
}
/*---------------------------------------------------------------------------*/
/**
 * @brief  int_master_is_enabled
 * 		checks if irq is enabled
 * @param  none
 * @retval bool (true is enabled)
 */
bool
int_master_is_enabled(void)
{
  return ((__get_PRIMASK() != ((uint32_t) 0)) ? false : true);
}
/*---------------------------------------------------------------------------*/
/**
* @}
*/

/**
* @}
*/

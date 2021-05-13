/**
  ******************************************************************************
  * @file    leds-arch.c
  * @author  SRA Application Team
  * @brief    Contiki-NG LEGACY LEDs API implementation
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
  
/**
 * @defgroup led_sensor
 * @ingroup dev
 * @{
 */

/**
 * @addtogroup led_sensor
 * @ingroup dev
 * @{
 * @file Driver for the board LEDs
 */
/* Includes ------------------------------------------------------------------*/
#include "contiki-conf.h"
#include "dev/leds.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

void leds_arch_init(void)
{
  /* We have at least one led, the one on the Nucleo (GREEN)*/
  (void) BSP_LED_Init(LED2);
  (void) BSP_LED_Off(LED2);
}
/*---------------------------------------------------------------------------*/
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
unsigned char leds_arch_get(void)
{
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
  unsigned char ret = 0 ;
  if (BSP_LED_GetState(LED2) != 0) {
    ret |= LEDS_GREEN;
  }

  return ret;
}
/*---------------------------------------------------------------------------*/
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
void leds_arch_set(unsigned char leds)
{
  if ((leds & LEDS_GREEN) != ((uint8_t) 0)) {
    (void) BSP_LED_On(LED2);
  } else {
    (void) BSP_LED_Off(LED2);
  }
}
/*---------------------------------------------------------------------------*/
/** @} */
/** @} */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

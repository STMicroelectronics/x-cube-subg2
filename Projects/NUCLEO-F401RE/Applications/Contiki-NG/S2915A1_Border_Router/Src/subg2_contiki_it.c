
/**
  ******************************************************************************
  * @file    subg2_contiki_it.c
  * @author  SRA Application Team
  * @brief   Source file for IRQ callbacks code related to Contiki-NG module
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

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sys/rtimer.h"
#include "clock.h"
#include "radio-driver.h"
#include "subg2_bsp_ip_conf.h"
#include "subg2_contiki_it.h"
#include "contiki-platform.h"
#include "sys/energest.h"
#include "stm32f4xx_hal.h"

#if PLATFORM_HAS_BUTTON
#include "lib/sensors.h"
#endif /*PLATFORM_HAS_BUTTON*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
#if PLATFORM_HAS_BUTTON
extern const struct sensors_sensor button_sensor;
#endif /*PLATFORM_HAS_BUTTON*/

extern volatile unsigned long seconds;
extern volatile clock_time_t ticks;

/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
void TIM_platform_rtimer_schedule(rtimer_clock_t t)
{
  __HAL_TIM_SET_COMPARE(&contiki_rtimer, TIM_CHANNEL_1, t);
  __HAL_TIM_ENABLE_IT(&contiki_rtimer,TIM_IT_CC1);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief  Timer configuration
 * @param  None
 * @retval None
 */
void TIM_platform_rtimer_init(void)
{
/* TIMx is used for the Contiki rtimer implementation.
 * The rtimer is used for BUSYWAIT_UNTIL() macro in S2-LP radio driver, for the
 * S2-LP states transions, and by Contiki in some applications like scheduling
 * the radio operations in TSCH. For these reason, the rtimer_arch_now() API
 * is implemented with a simple register read. It is reccomended to choose a TIM
 * with a 32 bits counter on most of the platforms (@TODO: validate this)
 */
  TIM_ClockConfigTypeDef s_clock_source_config;
  TIM_OC_InitTypeDef s_config_oc;
  uint32_t uwPrescalerValue;
  uint32_t uwTimclock;
  RCC_ClkInitTypeDef sClokConfig;
  uint32_t uwAPB1Prescaler = 0;
  uint32_t pFLatency=0;

  /* Get clock configuration */
  HAL_RCC_GetClockConfig(&sClokConfig, &pFLatency);
  /* Get APB1 prescaler */
  uwAPB1Prescaler = sClokConfig.APB1CLKDivider;
  /* Compute TIMx clock */
  if (uwAPB1Prescaler == 0)
  {
    uwTimclock = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    uwTimclock = 2*HAL_RCC_GetPCLK1Freq();
  }
  /* Compute the prescaler value to have TIMx counter clock equal to RTIMER_ARCH_SECOND */
  uwPrescalerValue = (uint32_t) ((uwTimclock / (RTIMER_ARCH_SECOND)) - 1);

  contiki_rtimer.Init.Prescaler = uwPrescalerValue;
  contiki_rtimer.Init.CounterMode = TIM_COUNTERMODE_UP;
  contiki_rtimer.Init.Period = 0xFFFFFFFF;
  contiki_rtimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  if (HAL_OK != HAL_TIM_Base_Init(&contiki_rtimer)){
    printf("Error in TIM Base Init.\n");
    Error_Handler();
  }

  s_clock_source_config.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_OK != HAL_TIM_ConfigClockSource(&contiki_rtimer, &s_clock_source_config)){
    printf("Error in TIM Config Clock Source.\n");
    Error_Handler();
  }

  HAL_TIM_OC_Init(&contiki_rtimer);

  s_config_oc.OCMode = TIM_OCMODE_TIMING;
  s_config_oc.Pulse = 0;
  s_config_oc.OCPolarity = TIM_OCPOLARITY_HIGH;
  HAL_TIM_OC_ConfigChannel(&contiki_rtimer, &s_config_oc, TIM_CHANNEL_1);

  __HAL_TIM_CLEAR_FLAG(&contiki_rtimer, TIM_FLAG_UPDATE);

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  HAL_TIM_RegisterCallback(&SUBG2_RTIMER_TIM_Handle, HAL_TIM_PERIOD_ELAPSED_CB_ID, SUBG2_RTIMER_TIM_IC_CaptureCallback);
  HAL_TIM_RegisterCallback(&SUBG2_RTIMER_TIM_Handle, HAL_TIM_IC_CAPTURE_CB_ID, SUBG2_RTIMER_TIM_PeriodElapsedCallback);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* Enable TIMx Update interrupt */
  HAL_TIM_Base_Start_IT(&contiki_rtimer);
}
/*----------------------------------------------------------------------------*/
#if PLATFORM_HAS_BUTTON
/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
 if(Button==BUTTON_USER)
  {
    sensors_changed(&button_sensor);
  }
}
#endif /*PLATFORM_HAS_BUTTON*/
/*----------------------------------------------------------------------------*/
/**
  * @brief  Input Capture callback in non-blocking mode
  * @param  htim TIM IC handle
  * @retval None
  */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
void SUBG2_RTIMER_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
#else
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
#endif /*USE_HAL_TIM_REGISTER_CALLBACKS*/
{
  if(htim->Instance == SUBG2_RTIMER_INSTANCE)
  {
    rtimer_run_next();
  }
}
/*----------------------------------------------------------------------------*/
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
void SUBG2_RTIMER_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
#else
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
#endif /*USE_HAL_TIM_REGISTER_CALLBACKS*/
{
  if(htim->Instance == SUBG2_RTIMER_INSTANCE)
  {
    __HAL_TIM_CLEAR_FLAG(&contiki_rtimer, TIM_FLAG_UPDATE);
  }
}
/*----------------------------------------------------------------------------*/
/**
  * @brief This function handles USART global interrupt.
  *   Actual function name is defined in subg2_bsp_ip_conf.h
  */
void UART_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  UART_HandleTypeDef *huart = &UartHandle;

  if(__HAL_UART_GET_FLAG(huart, UART_FLAG_PE)){
    __HAL_UART_CLEAR_PEFLAG(huart);
  }

  if(__HAL_UART_GET_FLAG(huart, UART_FLAG_FE)){
    __HAL_UART_CLEAR_FEFLAG(huart);
  }

  if(__HAL_UART_GET_FLAG(huart, UART_FLAG_NE)){
    __HAL_UART_CLEAR_NEFLAG(huart);
  }

  if(__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)){
    __HAL_UART_CLEAR_OREFLAG(huart);
  }

  if(__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE)){
   input_handler(huart->Instance->DR);
  }

}
/*----------------------------------------------------------------------------*/
/**
  * @brief This function is called to increment  a global variable "uwTick"
  *        used as application time base.
  * @note In the default implementation, this variable is incremented each 1ms
  *       in SysTick ISR.
 * @note This function is reimplemented here like the default one, but adding
  *      the call to Contiki_SysTick_Handler()
  * @retval None
  */
void HAL_IncTick(void)
{
  uwTick += uwTickFreq;
  Contiki_SysTick_Handler();
}
/*----------------------------------------------------------------------------*/
/**
* @brief  Systick Handler for Contiki timings
* @param  None
* @retval None
*/
void Contiki_SysTick_Handler(void)
{
  ticks++;
  if((ticks % CLOCK_SECOND) == 0) {
    seconds++;
    energest_flush();
  }

  if(etimer_pending()) {
    etimer_request_poll();
  }
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

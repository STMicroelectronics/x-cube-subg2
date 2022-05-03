/**
  ******************************************************************************
  * @file    subg2_contiki_it.h
  * @author  SRA Application Team
  * @brief   Header file for IRQ callbacks code related to Contiki-NG module
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
#ifndef SUBG2_CONTIKI_IT_H
#define SUBG2_CONTIKI_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys/rtimer.h"
/*----------------------------------------------------------------------------*/
void Contiki_SysTick_Handler(void);
void TIM_platform_rtimer_schedule(rtimer_clock_t t);
void TIM_platform_rtimer_init(void);

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
void SUBG2_RTIMER_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void SUBG2_RTIMER_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#endif /*USE_HAL_TIM_REGISTER_CALLBACKS*/

#ifdef __cplusplus
}
#endif

#endif /*SUBG2_CONTIKI_IT_H*/

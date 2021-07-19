/**
  ******************************************************************************
  * @file    subg2_contiki_it.h
  * @author  SRA Application Team
  * @brief   Header file for IRQ callbacks code related to Contiki-NG module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under ODE Software License Agreement
  * SLA0094, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0094
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

#ifdef __cplusplus
}
#endif

#endif /*SUBG2_CONTIKI_IT_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

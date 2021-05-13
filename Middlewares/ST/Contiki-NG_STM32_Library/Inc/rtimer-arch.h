/**
  ******************************************************************************
  * @file    rtimer-arch.h
  * @author  SRA Application Team
  * @brief   Header for rtimer-arch.c module
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

#ifndef RTIMER_ARCH_H__
#define RTIMER_ARCH_H__
/* Includes ------------------------------------------------------------------*/
#include "contiki-conf.h"
#include "sys/clock.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define US_TO_RTIMERTICKS(US)  ((US) >= 0 ?                        \
                               (((int32_t)(US) * (RTIMER_ARCH_SECOND) + 500000) / 1000000L) :      \
                               ((int32_t)(US) * (RTIMER_ARCH_SECOND) - 500000) / 1000000L)

#define RTIMERTICKS_TO_US(T)   ((T) >= 0 ?                     \
                               (((int32_t)(T) * 1000000L + ((RTIMER_ARCH_SECOND) / 2)) / (RTIMER_ARCH_SECOND)) : \
                               ((int32_t)(T) * 1000000L - ((RTIMER_ARCH_SECOND) / 2)) / (RTIMER_ARCH_SECOND))

#define RTIMERTICKS_TO_US_64(T)  ((uint32_t)(((uint64_t)(T) * 1000000 + ((RTIMER_ARCH_SECOND) / 2)) / (RTIMER_ARCH_SECOND)))

/* Exported functions ------------------------------------------------------- */
rtimer_clock_t rtimer_arch_now(void);
void rtimer_arch_disable_irq(void);
void rtimer_arch_enable_irq(void);
/*--------------------------------------------------------------------------- */
#endif /* RTIMER_ARCH_H__ */
/*--------------------------------------------------------------------------- */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

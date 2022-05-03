/**
 ******************************************************************************
 * @file    custom_s2lp_conf.h
 * @author  SRA Application Team
 * @brief   This file contains definitions for the CUSTOM_S2LP components bus interfaces
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CUSTOM_S2LP_CONF_H__
#define __CUSTOM_S2LP_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo_bus.h"
#include "stm32l0xx_nucleo_errno.h"

#include "stm32l0xx_hal_exti.h"
#include "custom_s2lpgpio_setting.h"
#include "RTE_Components.h"

/* Global configuration ---------------------------------------------------*/
extern volatile int irq_disable_cnt;

/* Defines ---------------------------------------------------------------*/

/* CUSTOM_S2LP_RADIO_GPIO_0 is NOT set as EXTI */
#define S2LP_NVIC_GPIO_0_DisableIRQ()
#define S2LP_NVIC_GPIO_0_EnableIRQ()

/* CUSTOM_S2LP_RADIO_GPIO_1 is NOT set as EXTI */
#define S2LP_NVIC_GPIO_1_DisableIRQ()
#define S2LP_NVIC_GPIO_1_EnableIRQ()

/* CUSTOM_S2LP_RADIO_GPIO_2 is NOT set as EXTI */
#define S2LP_NVIC_GPIO_2_DisableIRQ()
#define S2LP_NVIC_GPIO_2_EnableIRQ()

/* CUSTOM_S2LP_RADIO_GPIO_3 is set as EXTI */
#define S2LP_NVIC_GPIO_3_DisableIRQ()	HAL_NVIC_DisableIRQ( CUSTOM_S2LP_RADIO_GPIO_3_IRQn)
#define S2LP_NVIC_GPIO_3_EnableIRQ()	HAL_NVIC_EnableIRQ( CUSTOM_S2LP_RADIO_GPIO_3_IRQn)

#define CUSTOM_S2LP_ENTER_CRITICAL()	\
   do{                                                                \
		 irq_disable_cnt++;		\
		 S2LP_NVIC_GPIO_0_DisableIRQ();        \
		 S2LP_NVIC_GPIO_1_DisableIRQ();        \
		 S2LP_NVIC_GPIO_2_DisableIRQ();        \
		 S2LP_NVIC_GPIO_3_DisableIRQ();        \
   }while(0)
#define CUSTOM_S2LP_EXIT_CRITICAL()                             \
   do{                                                                \
     if (irq_disable_cnt > 0)                                         \
       irq_disable_cnt--;                                             \
     if (irq_disable_cnt==0)                                          \
	 {	\
		S2LP_NVIC_GPIO_0_EnableIRQ();        \
		S2LP_NVIC_GPIO_1_EnableIRQ();        \
		S2LP_NVIC_GPIO_2_EnableIRQ();        \
		S2LP_NVIC_GPIO_3_EnableIRQ();        \
	}	\
   }while(0)

#define CUSTOM_S2LP_SPI_Init                                    BSP_SPI1_Init
#define CUSTOM_S2LP_SPI_DeInit                                  BSP_SPI1_DeInit
#define CUSTOM_S2LP_SPI_SendRecv                                BSP_SPI1_SendRecv
#define CUSTOM_S2LP_Delay                                       HAL_Delay
#define hspi                                                          hspi1

/* SPIx Bus Pin mapping */
#define CUSTOM_S2LP_RADIO_SPI_NSS_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#define CUSTOM_S2LP_RADIO_SPI_NSS_PORT                          GPIOA
#define CUSTOM_S2LP_RADIO_SPI_NSS_PIN                           GPIO_PIN_1
#define CUSTOM_S2LP_RADIO_SPI_NSS_SPEED                         GPIO_SPEED_FREQ_LOW
#define CUSTOM_S2LP_RADIO_SPI_NSS_PULL_MODE                     GPIO_NOPULL

#define CUSTOM_S2LP_RADIO_TCXO_CTL_PORT                         GPIOC
#define CUSTOM_S2LP_RADIO_TCXO_CTL_PIN                          GPIO_PIN_7
#define CUSTOM_S2LP_RADIO_TCXO_CTL_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()

/* GPIO3 as EXTI */
#define CUSTOM_S2LP_RADIO_GPIO_3_PORT                            GPIOC
#define CUSTOM_S2LP_RADIO_GPIO_3_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define CUSTOM_S2LP_RADIO_GPIO_3_PIN                             GPIO_PIN_0
#define CUSTOM_S2LP_RADIO_GPIO_3_EXTI_LINE                       EXTI_LINE_0
#define CUSTOM_S2LP_RADIO_GPIO_3_IRQn                            EXTI0_1_IRQn
#define H_EXTI_0                                             CUSTOM_S2LP_RADIO_GPIO_hexti[0]
#define CUSTOM_S2LP_RADIO_GPIO_3_MODE                            RADIO_MODE_EXTI_IN
#define CUSTOM_S2LP_RADIO_GPIO_3_EXTI_MODE                       GPIO_MODE_IT_FALLING
#define CUSTOM_S2LP_RADIO_GPIO_3_IT_PRIO                         0
#define CUSTOM_S2LP_RADIO_GPIO_3_IT_SUBPRIO                      0
#define CUSTOM_S2LP_RADIO_GPIO_3_SPEED                           GPIO_SPEED_FREQ_HIGH /* This value is not used for EXTI mode */
#define CUSTOM_S2LP_RADIO_GPIO_3_PULL_MODE                       GPIO_NOPULL

#define CUSTOM_S2LP_RADIO_GPIOn                                  (USE_CUSTOM_S2LP_RADIO_GPIO_0 + \
                                                            USE_CUSTOM_S2LP_RADIO_GPIO_1 + \
                                                            USE_CUSTOM_S2LP_RADIO_GPIO_2 + \
                                                            USE_CUSTOM_S2LP_RADIO_GPIO_3 + 1)

#if (CUSTOM_S2LP_RADIO_GPIOn == 1)
#error "At least one GPIO must be enabled (and in EXTI mode) for the radio to work."
#endif /*(CUSTOM_S2LP_RADIO_GPIOn == 1)*/

extern  EXTI_HandleTypeDef   CUSTOM_S2LP_RADIO_GPIO_hexti[CUSTOM_S2LP_RADIO_GPIOn -1];

#define CUSTOM_S2LP_RADIO_GPIO_SDN_PORT                          GPIOA
#define CUSTOM_S2LP_RADIO_GPIO_SDN_CLOCK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE()
#define CUSTOM_S2LP_RADIO_GPIO_SDN_PIN                           GPIO_PIN_8
#define CUSTOM_S2LP_RADIO_GPIO_SDN_SPEED                         GPIO_SPEED_FREQ_LOW
#define CUSTOM_S2LP_RADIO_GPIO_SDN_PULL_MODE                     GPIO_NOPULL

#ifdef __cplusplus
}
#endif

#endif /* __CUSTOM_S2LP_CONF_H__*/

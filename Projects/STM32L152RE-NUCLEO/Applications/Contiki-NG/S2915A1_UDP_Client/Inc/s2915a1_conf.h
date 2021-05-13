
/**
  ******************************************************************************
  * @file    s2915a1_conf.h
  * @author  SRA Application Team
  * @brief   This file contains definitions for the S2915A1 components bus interfaces
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __S2915A1_CONF_H__
#define __S2915A1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l1xx_hal.h"
#include "stm32l1xx_nucleo_bus.h"
#include "stm32l1xx_nucleo_errno.h"

#include "stm32l1xx_hal_exti.h"
#include "s2915a1_s2lpgpio_setting.h"
#include "RTE_Components.h"

/* Global configuration ---------------------------------------------------*/
extern volatile int irq_disable_cnt;

/* Defines ---------------------------------------------------------------*/

#if (USE_S2915A1_RADIO_GPIO_0 == 1)
#define S2915A1_RADIO_GPIO_0_IT_PRIO                         1
#define S2915A1_ENTER_CRITICAL()                            \
   do{                                                                \
     irq_disable_cnt++;                                               \
     HAL_NVIC_DisableIRQ( S2915A1_RADIO_GPIO_0_IRQn);        \
   }while(0)
#define S2915A1_EXIT_CRITICAL()                             \
   do{                                                                \
     if (irq_disable_cnt > 0)                                         \
       irq_disable_cnt--;                                             \
     if (irq_disable_cnt==0)                                          \
       HAL_NVIC_EnableIRQ( S2915A1_RADIO_GPIO_0_IRQn);       \
   }while(0)
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1)
#define S2915A1_RADIO_GPIO_1_IT_PRIO                         1
#define S2915A1_ENTER_CRITICAL()                            \
   do{                                                                \
     irq_disable_cnt++;                                               \
     HAL_NVIC_DisableIRQ( S2915A1_RADIO_GPIO_1_IRQn);        \
   }while(0)
#define S2915A1_EXIT_CRITICAL()                             \
   do{                                                                \
     if (irq_disable_cnt > 0)                                         \
       irq_disable_cnt--;                                             \
     if (irq_disable_cnt==0)                                          \
       HAL_NVIC_EnableIRQ( S2915A1_RADIO_GPIO_1_IRQn);       \
   }while(0)
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1)
#define S2915A1_RADIO_GPIO_2_IT_PRIO                         1
#define S2915A1_ENTER_CRITICAL()                            \
   do{                                                                \
     irq_disable_cnt++;                                               \
     HAL_NVIC_DisableIRQ( S2915A1_RADIO_GPIO_2_IRQn);        \
   }while(0)
#define S2915A1_EXIT_CRITICAL()                             \
   do{                                                                \
     if (irq_disable_cnt > 0)                                         \
       irq_disable_cnt--;                                             \
     if (irq_disable_cnt==0)                                          \
       HAL_NVIC_EnableIRQ( S2915A1_RADIO_GPIO_2_IRQn);       \
   }while(0)
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1)
#define S2915A1_RADIO_GPIO_3_IT_PRIO                         1
#define S2915A1_ENTER_CRITICAL()                            \
   do{                                                                \
     irq_disable_cnt++;                                               \
     HAL_NVIC_DisableIRQ( S2915A1_RADIO_GPIO_3_IRQn);        \
   }while(0)
#define S2915A1_EXIT_CRITICAL()                             \
   do{                                                                \
     if (irq_disable_cnt > 0)                                         \
       irq_disable_cnt--;                                             \
     if (irq_disable_cnt==0)                                          \
       HAL_NVIC_EnableIRQ( S2915A1_RADIO_GPIO_3_IRQn);       \
   }while(0)
#endif

#define S2915A1_SPI_Init                                    BSP_SPI1_Init
#define S2915A1_SPI_DeInit                                  BSP_SPI1_DeInit
#define S2915A1_SPI_SendRecv                                BSP_SPI1_SendRecv
#define EEPROM_SPI_SendRecv                                           BSP_SPI1_SendRecv
#define S2915A1_Delay                                       HAL_Delay
#define hspi                                                          hspi1

#define EEPROM_INSTANCE                                     (0)

#define S2915A1_EEPROM_SPI_CS_PORT                          GPIOB
#define S2915A1_EEPROM_SPI_CS_PIN                           GPIO_PIN_4
#define S2915A1_EEPROM_SPI_CS_RCC                           __HAL_RCC_GPIOB_CLK_ENABLE

#define S2915A1_PA_CSD_PIN                                  GPIO_PIN_0
#define S2915A1_PA_CSD_PORT                                 GPIOA
#define S2915A1_PA_CSD_GPIO_CLK()                           __HAL_RCC_GPIOA_CLK_ENABLE()

#define S2915A1_PA_CPS_PIN                                  GPIO_PIN_4
#define S2915A1_PA_CPS_PORT                                 GPIOA
#define S2915A1_PA_CPS_GPIO_CLK()                           __HAL_RCC_GPIOA_CLK_ENABLE()

#define S2915A1_PA_CTX_PIN                                  GPIO_PIN_0
#define S2915A1_PA_CTX_PORT                                 GPIOB
#define S2915A1_PA_CTX_GPIO_CLK()                           __HAL_RCC_GPIOB_CLK_ENABLE()

/* SPIx Bus Pin mapping */
#define S2915A1_RADIO_SPI_NSS_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#define S2915A1_RADIO_SPI_NSS_PORT                          GPIOA
#define S2915A1_RADIO_SPI_NSS_PIN                           GPIO_PIN_1

#define S2915A1_RADIO_TCXO_CTL_PORT                         GPIOC
#define S2915A1_RADIO_TCXO_CTL_PIN                          GPIO_PIN_7
#define S2915A1_RADIO_TCXO_CTL_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()

#if (USE_S2915A1_RADIO_GPIO_0 == 1)
#define S2915A1_RADIO_GPIO_0_PORT                            GPIOC
#define S2915A1_RADIO_GPIO_0_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define S2915A1_RADIO_GPIO_0_PIN                             GPIO_PIN_0
#define S2915A1_RADIO_GPIO_0_EXTI_LINE                       EXTI_LINE_0
#define S2915A1_RADIO_GPIO_0_IRQn                            EXTI0_IRQn
#define H_EXTI_0                                             S2915A1_RADIO_GPIO_hexti[0]
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1)
#define S2915A1_RADIO_GPIO_1_PORT                            GPIOC
#define S2915A1_RADIO_GPIO_1_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define S2915A1_RADIO_GPIO_1_PIN                             GPIO_PIN_0
#define S2915A1_RADIO_GPIO_1_EXTI_LINE                       EXTI_LINE_0
#define S2915A1_RADIO_GPIO_1_IRQn                            EXTI0_IRQn
#define H_EXTI_0                                             S2915A1_RADIO_GPIO_hexti[0]
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1)
#define S2915A1_RADIO_GPIO_2_PORT                            GPIOC
#define S2915A1_RADIO_GPIO_2_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define S2915A1_RADIO_GPIO_2_PIN                             GPIO_PIN_0
#define S2915A1_RADIO_GPIO_2_EXTI_LINE                       EXTI_LINE_0
#define S2915A1_RADIO_GPIO_2_IRQn                            EXTI0_IRQn
#define H_EXTI_0                                             S2915A1_RADIO_GPIO_hexti[0]
#endif

#if (USE_S2915A1_RADIO_GPIO_3 == 1)
#define S2915A1_RADIO_GPIO_3_PORT                            GPIOC
#define S2915A1_RADIO_GPIO_3_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define S2915A1_RADIO_GPIO_3_PIN                             GPIO_PIN_0
#define S2915A1_RADIO_GPIO_3_EXTI_LINE                       EXTI_LINE_0
#define S2915A1_RADIO_GPIO_3_IRQn                            EXTI0_IRQn
#define H_EXTI_0                                             S2915A1_RADIO_GPIO_hexti[0]
#endif

#define S2915A1_RADIO_GPIOn                                  (USE_S2915A1_RADIO_GPIO_0 + \
                                                            USE_S2915A1_RADIO_GPIO_1 + \
                                                            USE_S2915A1_RADIO_GPIO_2 + \
                                                            USE_S2915A1_RADIO_GPIO_3 + 1)

extern  EXTI_HandleTypeDef   S2915A1_RADIO_GPIO_hexti[S2915A1_RADIO_GPIOn -1];

#define S2915A1_RADIO_GPIO_SDN_PORT                          GPIOA
#define S2915A1_RADIO_GPIO_SDN_CLOCK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE()
#define S2915A1_RADIO_GPIO_SDN_PIN                           GPIO_PIN_8

#define S2915A1_LEDn                                        ((uint8_t)1)
#define S2915A1_LED_GPIO_PORT                               GPIOC
#define S2915A1_LED_GPIO_CLK_ENABLE()                       __HAL_RCC_GPIOC_CLK_ENABLE()              /*Rx Indicator LED, FOR LED Mount R17*/
#define S2915A1_LED_GPIO_PIN                                GPIO_PIN_7

#ifdef __cplusplus
}
#endif

#endif /* __S2915A1_CONF_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


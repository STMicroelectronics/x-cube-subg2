/**
  ******************************************************************************
  * File Name          : s2868A1_conf.h
  * Description        : This file provides code for the configuration
  *                      of the shield instances (pin mapping).
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

#ifndef __S2868A1_CONF_H__
#define __S2868A1_CONF_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f4xx_nucleo_bus.h"
#include "s2868a1.h"

/* Global configuration ---------------------------------------------------*/   
#define S2868A1_USE_EEPROM                         0
#define S2868A1_USE_WMBUS                          1
                                
/* Defines ---------------------------------------------------------------*/
#define S2868A1_RADIO_GPIO_0_IT_PRIO                        0
#define S2868A1_RADIO_GPIO_1_IT_PRIO                        0
#define S2868A1_RADIO_GPIO_2_IT_PRIO                        0
#define S2868A1_RADIO_GPIO_3_IT_PRIO                        0
/* Daughter board Pin mapping --------------------------------------------*/
#define S2868A1_ENTER_CRITICAL()                      HAL_NVIC_DisableIRQ( S2868A1_RADIO_GPIO_3_IRQn)
#define S2868A1_EXIT_CRITICAL()                       HAL_NVIC_EnableIRQ( S2868A1_RADIO_GPIO_3_IRQn)
   
/*  SPI functions redefinition */
#define S2868A1_SPI_Init                          BSP_SPI1_Init
#define S2868A1_SPI_DeInit                        BSP_SPI1_DeInit
#define S2868A1_SPI_SendRecv                      BSP_SPI1_SendRecv

/* SPIx Bus Pin mapping */ 
#define S2868A1_RADIO_SPI_NSS_CLK_ENABLE()                    __HAL_RCC_GPIOA_CLK_ENABLE()
#define S2868A1_RADIO_SPI_NSS_PORT                            GPIOA
#define S2868A1_RADIO_SPI_NSS_PIN                             GPIO_PIN_1


#define S2868A1_RADIO_TCXO_CTL_PORT                  GPIOC
#define S2868A1_RADIO_TCXO_CTL_PIN                   GPIO_PIN_7
#define S2868A1_RADIO_TCXO_CTL_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
   
   
#define S2868A1_RADIO_GPIOn                               4U

#define S2868A1_RADIO_GPIO_0_PORT                  GPIOA
#define S2868A1_RADIO_GPIO_0_PIN                   GPIO_PIN_0
#define S2868A1_RADIO_GPIO_0_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define S2868A1_RADIO_GPIO_0_EXTI_LINE             EXTI_LINE_0
#define S2868A1_RADIO_GPIO_0_IRQn                  EXTI0_IRQn                    
   
#define S2868A1_RADIO_GPIO_1_PORT                  GPIOA
#define S2868A1_RADIO_GPIO_1_PIN                   GPIO_PIN_4
#define S2868A1_RADIO_GPIO_1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define S2868A1_RADIO_GPIO_1_EXTI_LINE             EXTI_LINE_4
#define S2868A1_RADIO_GPIO_1_IRQn                  EXTI4_IRQn                    

#define S2868A1_RADIO_GPIO_2_PORT        	      GPIOB
#define S2868A1_RADIO_GPIO_2_PIN        			  GPIO_PIN_0
#define S2868A1_RADIO_GPIO_2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define S2868A1_RADIO_GPIO_2_EXTI_LINE             EXTI_LINE_0
#define S2868A1_RADIO_GPIO_2_IRQn                  EXTI0_IRQn                    
							

#define S2868A1_RADIO_GPIO_3_PORT     			   GPIOC
#define S2868A1_RADIO_GPIO_3_PIN      			   GPIO_PIN_0
#define S2868A1_RADIO_GPIO_3_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define S2868A1_RADIO_GPIO_3_EXTI_LINE             EXTI_LINE_0
#define S2868A1_RADIO_GPIO_3_IRQn                  EXTI0_IRQn 
  
/**** USED ONLY By CubeMX *****************************************************/   
#define H_EXTI_0                                  hRADIO_GPIO_exti
	
#define S2868A1_RADIO_GPIO_SDN_PORT                      GPIOA
#define S2868A1_RADIO_GPIO_SDN_PIN                       GPIO_PIN_8  
#define S2868A1_RADIO_GPIO_SDN_CLOCK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define S2868A1_RADIO_GPIO_SDN_SPEED                     GPIO_SPEED_HIGH

#define S2868A1_RADIO_TCXO_EN_PIN                             GPIO_PIN_7
#define S2868A1_RADIO_TCXO_EN_PORT                            GPIOC
#define S2868A1_RADIO_TCXO_EN_GPIO_CLK                        __GPIOC_CLK_ENABLE

#define S2868A1_LEDn                              ((uint8_t)1)
#define S2868A1_LED_GPIO_PIN                      GPIO_PIN_7                         /*Rx Indicator LED*/
#define S2868A1_LED_GPIO_PORT                     GPIOC                            // S2lp radio LED PC7
#define S2868A1_LED_GPIO_CLK_ENABLE()           __GPIOB_CLK_ENABLE()  
#define S2868A1_LED_GPIO_CLK_DISABLE()          __GPIOB_CLK_DISABLE()
 
/* Defines for EEPROM chip select pin (PA9 for STEVAL-FKI868V1 and PB4 for X-NUCLEO-S2868A1)*/
#define S2868A1_EEPROM_SPI_CS_PORT             GPIOB
#define S2868A1_EEPROM_SPI_CS_PIN              GPIO_PIN_4
#define S2868A1_EEPROM_SPI_CS_RCC              __GPIOB_CLK_ENABLE
#define S2868A1_EEPROM_SPI_CS_RCC_SOURCE       GPIO_PinSource4
#ifdef __cplusplus
}
#endif
#endif /* __SX1276MB1MAS_CONF_H__ */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

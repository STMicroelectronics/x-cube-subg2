/**
* @file    Platform_Configuration_CLI.h
* @author  AMS RF application team
* @version V1.0.0
* @date    April, 2020
* @brief   This file contains definitions for STM32L1xx_NUCLEO Eval Platform
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*
* <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIGURATION_H
#define __PLATFORM_CONFIGURATION_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
  * @brief MCU XO frequency definition
  */
#define CLOCK_FREQUENCY					(SystemCoreClock/1000)
#define CLOCK_FREQUENCY_MHZ				(CLOCK_FREQUENCY/1000)

/*****************************************************************************/
/*                              UART SECTION                                 */
/*****************************************************************************/
#define NUCLEO_UARTx_RX_QUEUE_SIZE			(380)
#define NUCLEO_UARTx_TX_QUEUE_SIZE			(380)

#ifdef __cplusplus
}
#endif

#endif /* __PLATFORM_CONFIGURATION_H */

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE*****/

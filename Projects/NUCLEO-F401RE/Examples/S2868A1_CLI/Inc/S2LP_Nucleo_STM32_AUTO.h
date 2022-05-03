
/**
* @file    S2LP_Nucleo_STM32_AUTO.h
* @author  AMS RF application team
* @version V1.0.0
* @date    December, 2018
* @brief   This file contains definitions for S2LP Shields + Nucleo64 Eval Platforms
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
#ifndef __S2LP_NUCLEO64_CLI_AUTO_H
#define __S2LP_NUCLEO64_CLI_AUTO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "s2868a1_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/*                            GENERAL CONFIG                                 */
/*****************************************************************************/

#define BOARD_VERSION					0x84

/*****************************************************************************/
/*                            S2-LP - GPIOs                                  */
/*****************************************************************************/

#define S2LP_M2S_SDN_MODE   GPIO_MODE_OUTPUT_PP
#define S2LP_M2S_SDN_PORT	S2868A1_RADIO_GPIO_SDN_PORT
#define S2LP_M2S_SDN_PIN	S2868A1_RADIO_GPIO_SDN_PIN
#define S2LP_M2S_SDN_CLOCK()	S2868A1_RADIO_GPIO_SDN_CLOCK_ENABLE()
#define S2LP_M2S_SDN_PUPD       S2868A1_RADIO_GPIO_SDN_PULL_MODE
#define S2LP_M2S_SDN_SPEED      S2868A1_RADIO_GPIO_SDN_SPEED

/**
* @brief Definitions for S2-LP IRQ line
*/

/* S2LP GPIO - IRQ Pin */
#define S2LP_GPIO_IRQ_PIN                       3

/* S2LP GPIO 0 */
#define M2S_GPIO_0_PORT_NUCLEO               S2868A1_RADIO_GPIO_0_PORT
#define M2S_GPIO_0_CLOCK_NUCLEO()            S2868A1_RADIO_GPIO_0_GPIO_CLK_ENABLE()
#define M2S_GPIO_0_PIN	                     S2868A1_RADIO_GPIO_0_PIN
#define M2S_GPIO_0_EXTI_IRQN	             S2868A1_RADIO_GPIO_0_IRQn
#define M2S_GPIO_0_SPEED                     S2868A1_RADIO_GPIO_0_SPEED
#define M2S_GPIO_0_PUPD                      S2868A1_RADIO_GPIO_0_PULL_MODE
#define M2S_GPIO_0_MODE                      S2868A1_RADIO_GPIO_0_MODE
#define M2S_GPIO_0_EXTI_MODE                 S2868A1_RADIO_GPIO_0_EXTI_MODE
#define M2S_GPIO_0_EXTI_PREEMPTION_PRIORITY  S2868A1_RADIO_GPIO_0_IT_PRIO
#define M2S_GPIO_0_EXTI_SUB_PRIORITY         S2868A1_RADIO_GPIO_0_IT_SUBPRIO
#define M2S_GPIO_0_EXTI_LINE                 S2868A1_RADIO_GPIO_0_EXTI_LINE

/* S2LP GPIO 1 */
#define M2S_GPIO_1_PORT_NUCLEO               S2868A1_RADIO_GPIO_1_PORT
#define M2S_GPIO_1_CLOCK_NUCLEO()            S2868A1_RADIO_GPIO_1_GPIO_CLK_ENABLE()
#define M2S_GPIO_1_PIN	                     S2868A1_RADIO_GPIO_1_PIN
#define M2S_GPIO_1_EXTI_IRQN	             S2868A1_RADIO_GPIO_1_IRQn
#define M2S_GPIO_1_SPEED                     S2868A1_RADIO_GPIO_1_SPEED
#define M2S_GPIO_1_PUPD                      S2868A1_RADIO_GPIO_1_PULL_MODE
#define M2S_GPIO_1_MODE                      S2868A1_RADIO_GPIO_1_MODE
#define M2S_GPIO_1_EXTI_MODE                 S2868A1_RADIO_GPIO_1_EXTI_MODE
#define M2S_GPIO_1_EXTI_PREEMPTION_PRIORITY  S2868A1_RADIO_GPIO_1_IT_PRIO
#define M2S_GPIO_1_EXTI_SUB_PRIORITY         S2868A1_RADIO_GPIO_1_IT_SUBPRIO
#define M2S_GPIO_1_EXTI_LINE                 S2868A1_RADIO_GPIO_1_EXTI_LINE

/* S2LP GPIO 2 */
#define M2S_GPIO_2_PORT_NUCLEO               S2868A1_RADIO_GPIO_2_PORT
#define M2S_GPIO_2_CLOCK_NUCLEO()            S2868A1_RADIO_GPIO_2_GPIO_CLK_ENABLE()
#define M2S_GPIO_2_PIN	                     S2868A1_RADIO_GPIO_2_PIN
#define M2S_GPIO_2_EXTI_IRQN	             S2868A1_RADIO_GPIO_2_IRQn
#define M2S_GPIO_2_SPEED                     S2868A1_RADIO_GPIO_2_SPEED
#define M2S_GPIO_2_PUPD                      S2868A1_RADIO_GPIO_2_PULL_MODE
#define M2S_GPIO_2_MODE                      S2868A1_RADIO_GPIO_2_MODE
#define M2S_GPIO_2_EXTI_MODE                 S2868A1_RADIO_GPIO_2_EXTI_MODE
#define M2S_GPIO_2_EXTI_PREEMPTION_PRIORITY  S2868A1_RADIO_GPIO_2_IT_PRIO
#define M2S_GPIO_2_EXTI_SUB_PRIORITY         S2868A1_RADIO_GPIO_2_IT_SUBPRIO
#define M2S_GPIO_2_EXTI_LINE                 S2868A1_RADIO_GPIO_2_EXTI_LINE

/* S2LP GPIO 3 */
#define M2S_GPIO_3_PORT_NUCLEO               S2868A1_RADIO_GPIO_3_PORT
#define M2S_GPIO_3_CLOCK_NUCLEO()            S2868A1_RADIO_GPIO_3_GPIO_CLK_ENABLE()
#define M2S_GPIO_3_PIN	                     S2868A1_RADIO_GPIO_3_PIN
#define M2S_GPIO_3_EXTI_IRQN	             S2868A1_RADIO_GPIO_3_IRQn
#define M2S_GPIO_3_SPEED                     S2868A1_RADIO_GPIO_3_SPEED
#define M2S_GPIO_3_PUPD                      S2868A1_RADIO_GPIO_3_PULL_MODE
#define M2S_GPIO_3_MODE                      S2868A1_RADIO_GPIO_3_MODE
#define M2S_GPIO_3_EXTI_MODE                 S2868A1_RADIO_GPIO_3_EXTI_MODE
#define M2S_GPIO_3_EXTI_PREEMPTION_PRIORITY  S2868A1_RADIO_GPIO_3_IT_PRIO
#define M2S_GPIO_3_EXTI_SUB_PRIORITY         S2868A1_RADIO_GPIO_3_IT_SUBPRIO
#define M2S_GPIO_3_EXTI_LINE                 S2868A1_RADIO_GPIO_3_EXTI_LINE

#ifdef __cplusplus
}
#endif

#endif /* __S2LP_NUCLEO64_CLI_AUTO_H */

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE*****/

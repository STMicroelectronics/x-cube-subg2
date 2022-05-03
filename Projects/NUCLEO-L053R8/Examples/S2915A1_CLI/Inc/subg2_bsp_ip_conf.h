/**
 ******************************************************************************
 * @file    subg2_bsp_ip_conf.h
 * @author  SRA Application Team
 * @brief   BSP IP configuration file
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
#ifndef __SUBG2_BSP_IP_CONF_H__
#define __SUBG2_BSP_IP_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/

/* Exported defines for USART -------------------------------------------------*/
#define UART_IRQHandler USART2_IRQHandler
#define NUCLEO_UARTx_IRQn USART2_IRQn
#define   NUCLEO_UARTx_TX_DMA_CHANNEL_IRQn DMA1_Channel4_5_6_7_IRQn
#define   NUCLEO_UARTx_RX_DMA_CHANNEL_IRQn DMA1_Channel4_5_6_7_IRQn
#define UartHandle hcom_uart[COM1]
#ifdef __cplusplus
}
#endif

#endif /* __SUBG2_BSP_IP_CONF_H__ */

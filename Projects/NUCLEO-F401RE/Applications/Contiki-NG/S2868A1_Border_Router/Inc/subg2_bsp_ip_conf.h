/**
 ******************************************************************************
 * @file    subg2_bsp_ip_conf.h
 * @author  SRA Application Team
 * @brief   BSP IP configuration file
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
#ifndef __BSP_IP_CONF_H__
#define __BSP_IP_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
/* Exported defines for TIMER -------------------------------------------------*/
extern TIM_HandleTypeDef htim5;
#define BSP_IP_TIM_Handle htim5
#define contiki_rtimer htim5
#define BSP_IP_TIM_Init MX_TIM5_Init
/* Exported defines for USART -------------------------------------------------*/
#define UART_IRQHandler USART2_IRQHandler
#define UartHandle hcom_uart[COM1]
#ifdef __cplusplus
}
#endif

#endif /* __BSP_IP_CONF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

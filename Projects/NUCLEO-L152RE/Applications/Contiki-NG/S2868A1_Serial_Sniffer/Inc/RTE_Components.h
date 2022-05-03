/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file
  * @author  MCD Application Team
  * @version V2.0.0
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
/* USER CODE END Header */
  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __RTE_COMPONENTS_H__
#define  __RTE_COMPONENTS_H__

/* Defines ------------------------------------------------------------------*/
/* STMicroelectronics.X-CUBE-SUBG2.5.0.0 */
#define CONTIKI_NG_STM32LIB_ENABLED
#define MAC_CONF_WITH_TSCH 0
#define MAC_CONF_WITH_CSMA 0
#define NETSTACK_CONF_MAC sniffer_mac_driver
#define PLATFORM_HAS_LEDS 1
#define PLATFORM_HAS_BUTTON 1
#define S2868A1
#define ROUTING_CONF_RPL_CLASSIC 0
#define ROUTING_CONF_RPL_LITE 0
#define ROUTING_CONF_NULLROUTING 1
#define NETSTACK_CONF_WITH_NULLNET 1
#define PROJECT_CONF_H 1
#define CONTIKI_NG_CORE_ENABLED
#define S2868A1_SNIFFER
#define USE_AUTOSTART_PROCESS
#define BUILD_WITH_SERIAL_SNIFFER 1
#define BUILD_WITH_SHELL 0

#endif /* __RTE_COMPONENTS_H__ */

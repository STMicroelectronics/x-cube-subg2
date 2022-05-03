
/**
  ******************************************************************************
  * @file    project-conf.h
  * @author  SRA Application Team
  * @brief   Project specific configuration file.
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
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

#include "radio-driver.h"

#define RTIMER_ARCH_SECOND 96000

#if BUILD_WITH_SERIAL_SNIFFER
#undef LLSEC802154_CONF_ENABLED
/* Disable Link Layer security */
#define LLSEC802154_CONF_ENABLED 0
#define NULLFRAMER_CONF_PARSE_802154 0
#endif /*BUILD_WITH_SERIAL_SNIFFER*/

#define FRAME802154_CONF_VERSION FRAME802154_IEEE802154_2015

#define WATCHDOG_ENABLE 0
#define RADIO_HW_CSMA 1
#define RADIO_SNIFF_MODE 0
#define RADIO_ADDRESS_FILTERING 1
#define CONTIKI_VERSION_STRING "Contiki-NG"
#define MCU_LOW_POWER 0
#define RADIO_LOW_POWER 0
#define RADIO_LONG_PREAMBLE 0

#if !BUILD_WITH_RPL_BORDER_ROUTER
#define LOG_CONF_LEVEL_MAC      LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_RPL   LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_FRAMER   LOG_LEVEL_ERR
#endif /*!BUILD_WITH_RPL_BORDER_ROUTER*/

//#define LOG_CONF_LEVEL_TCPIP   LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_IPV6     LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_MAC      LOG_LEVEL_INFO
//#define LOG_CONF_LEVEL_6LOWPAN  LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_LWM2M LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_COAP LOG_LEVEL_INFO
//#define LOG_CONF_LEVEL_DTLS LOG_LEVEL_INFO
//#define LOG_LEVEL_DTLS LOG_LEVEL_DBG

/*Set PANID if needed */
//#undef IEEE802154_CONF_PANID
//#define IEEE802154_CONF_PANID 0xBEEF

//#ifndef QUEUEBUF_CONF_NUM
//#define QUEUEBUF_CONF_NUM          16
//#endif

#if BUILD_WITH_RPL_BORDER_ROUTER

#define UIP_FALLBACK_INTERFACE         rpl_interface

#ifndef WEBSERVER_CONF_CFS_CONNS
#define WEBSERVER_CONF_CFS_CONNS 2
#endif

#ifndef BORDER_ROUTER_CONF_WEBSERVER
#define BORDER_ROUTER_CONF_WEBSERVER 1
#endif

#if BORDER_ROUTER_CONF_WEBSERVER
#undef UIP_CONF_TCP
#define UIP_CONF_TCP 1
#endif

#endif /* BORDER_ROUTER*/

#ifdef WITH_DTLS
#define COAP_DTLS_PSK_DEFAULT_IDENTITY "0123456789ABCDEF"
#define COAP_DTLS_PSK_DEFAULT_KEY      "stm32nucleo" //-> 73746d33326e75636c656f
#define DTLS_MAX_BUF 400
#endif

#endif /* PROJECT_CONF_H_ */

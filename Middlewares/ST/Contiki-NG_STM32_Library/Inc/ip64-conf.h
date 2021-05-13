/**
  ******************************************************************************
  * @file    ip64-conf.h
  * @author  SRA Application Team
  * @brief   Header for IP64 module
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
  
/**
 * @addtogroup configuration_files
 * @ingroup Contiki-NG_STM32_Library
 * @{
 * @file IP64 module configuration file
 */

#ifndef IP64_CONF_H
#define IP64_CONF_H
#if WITH_IP64
/*
#include "ip64-tap-driver.h"
#include "ip64-eth-interface.h"

#define IP64_CONF_UIP_FALLBACK_INTERFACE ip64_eth_interface
#define IP64_CONF_INPUT                  ip64_eth_interface_input

#define IP64_CONF_ETH_DRIVER             ip64_tap_driver


#undef UIP_FALLBACK_INTERFACE
#define UIP_FALLBACK_INTERFACE ip64_uip_fallback_interface
*/
#ifdef MY_DRIVERS
#include <my_wifi_interface.h>
#include <my_wifi_driver.h>

#define IP64_CONF_UIP_FALLBACK_INTERFACE_SLIP 0
#define IP64_CONF_UIP_FALLBACK_INTERFACE my_wifi_interface
#define IP64_CONF_INPUT                  my_wifi_interface_input
#define IP64_CONF_ETH_DRIVER             my_wifi_driver

#ifdef UIP_CONF_ROUTER
#undef UIP_CONF_ROUTER
#endif
#define UIP_CONF_ROUTER       1

#ifdef UIP_CONF_IPV6_RPL
#undef UIP_CONF_IPV6_RPL
#endif
#define UIP_CONF_IPV6_RPL		1

#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM          16
#endif

#undef IP64_CONF_DHCP
#define IP64_CONF_DHCP 0

#else


#include "net/ip64/ip64-slip-interface.h"
#include "net/ip64/ip64-null-driver.h"

#define IP64_CONF_UIP_FALLBACK_INTERFACE_SLIP 1
#define IP64_CONF_UIP_FALLBACK_INTERFACE ip64_slip_interface
#define IP64_CONF_INPUT                  ip64_slip_interface_input
#define IP64_CONF_ETH_DRIVER             ip64_null_driver

#undef UIP_CONF_ND6_RA_RDNSS
#define UIP_CONF_ND6_RA_RDNSS 1

#undef UIP_CONF_ND6_SEND_RA
#define UIP_CONF_ND6_SEND_RA  1

#undef UIP_CONF_ROUTER
#define UIP_CONF_ROUTER       1

#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM          16
#endif

#endif//MY_DRIVERS

#endif//WITH_IP64
#endif /* IP64_CONF_H */
/** @} */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

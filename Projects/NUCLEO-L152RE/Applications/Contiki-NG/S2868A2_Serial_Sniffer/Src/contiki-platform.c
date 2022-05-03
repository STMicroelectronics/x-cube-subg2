
/*
 * Copyright (c) 2017, George Oikonomou - http://www.spd.gr
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*---------------------------------------------------------------------------*/
/**
 * \addtogroup main
 * @{
 */
/*---------------------------------------------------------------------------*/
/**
 * \file
 *
 * Implementation of \os's main routine
 */
/*---------------------------------------------------------------------------*/

/**
* @defgroup Contiki-NG_STM32_Library
* @{
*/

/**
* @addtogroup Contiki-NG_STM32_Library
* @{
*/
/* Includes ------------------------------------------------------------------*/
#include "lib/sensors.h"
#include "net/linkaddr.h"
#include "sys/platform.h"
#include "sys/node-id.h"
#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "sys/autostart.h"
#include "sys/energest.h"
#if BUILD_WITH_RPL_BORDER_ROUTER
#include "dev/slip.h"
#endif /*BUILD_WITH_RPL_BORDER_ROUTER*/
#include "dev/watchdog.h"
#include "lib/random.h"
#include "net/netstack.h"

#include "net/ipv6/uip.h"
#include "net/mac/framer/frame802154.h"

#include "net/queuebuf.h"
#if BUILD_WITH_COAP
#include "net/app-layer/coap/coap-engine.h"
#endif /*BUILD_WITH_COAP*/
#if BUILD_WITH_RPL_BORDER_ROUTER
#include "services/rpl-border-router/rpl-border-router.h"
#endif /*BUILD_WITH_RPL_BORDER_ROUTER*/
#if BUILD_WITH_ORCHESTRA
#include "services/orchestra/orchestra.h"
#endif /*BUILD_WITH_ORCHESTRA*/
#if BUILD_WITH_SHELL
#include "services/shell/serial-shell.h"
#endif /*BUILD_WITH_SHELL*/

#include "dev/serial-line.h"
#include "services/simple-energest/simple-energest.h"
#if BUILD_WITH_TSCH_CS
#include "services/tsch-cs/tsch-cs.h"
#endif /*BUILD_WITH_TSCH_CS*/
#include "radio-driver.h"

#if MAC_CONF_WITH_TSCH
#include "net/mac/tsch/tsch.h"
#endif /*MAC_CONF_WITH_TSCH*/

#include "stdbool.h"
#include "main.h"
#include "contiki-platform.h"
#include "subg2_bsp_ip_conf.h"

#ifdef WITH_IP64
#include "ip64.h"
#include "ip64-addr.h"
#endif /*WITH_IP64*/
#include "subg2_contiki_it.h"

#include "RTE_Components.h"
/*---------------------------------------------------------------------------*/
#if NETSTACK_CONF_WITH_IPV6
#include "net/ipv6/uip-ds6.h"
#endif /*NETSTACK_CONF_WITH_IPV6*/
/*---------------------------------------------------------------------------*/
/* Log configuration */
#include "sys/log.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define LOG_MODULE "S2LP_platform"
#define LOG_LEVEL LOG_LEVEL_MAIN
#define BOARD_STRING "ST S2LP Platform"
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
static uint8_t node_mac[8];
int (* input_handler)(unsigned char c);
/*---------------------------------------------------------------------------*/
#if PLATFORM_HAS_BUTTON
#include "dev/button-sensor.h"
SENSORS(&button_sensor);
#else /*!PLATFORM_HAS_BUTTON*/
SENSORS(NULL);
#endif /*PLATFORM_HAS_BUTTON*/

#if PLATFORM_HAS_LEDS
#include "dev/leds.h"
#endif /*PLATFORM_HAS_LEDS*/

/* Private function prototypes -----------------------------------------------*/
static void print_parameters(void);

/* Functions Definition ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
  /**
  * @brief  MX_SUBG2_Contiki_NG_Process
  *         Contiki-NG scheduler
  * @retval None
  */
void MX_SUBG2_Contiki_NG_Process(void)
{
    int r = 0;
    do {
      r = process_run();
      watchdog_periodic();
    } while(r > 0);
    platform_idle();
}
/*----------------------------------------------------------------------------*/
  /**
  * @brief  MX_SUBG2_Contiki_NG_Init
  *         Initialization of the system (COM, LED, BUTTON and RADIO)
  * @retval None
  */
void MX_SUBG2_Contiki_NG_Init(void)
{
  BSP_COM_Init(COM1);
#if PLATFORM_HAS_LEDS
  /* Initialize USER LED */
  leds_arch_init();
#endif /*PLATFORM_HAS_LEDS*/

  SUBG2_RTIMER_TIM_Init();
  Stack_6LoWPAN_Init();
}
/*----------------------------------------------------------------------------*/
/**
* @brief  Stack_6LoWPAN_Init program
* 		initialises Contiki-NG structures, the radio and runs all the PROCESSes
* @param  None
* @retval None
*/
void Stack_6LoWPAN_Init(void) //former "main()" in contiki-main.c
{
  /*In STM32CubeMX integration, "stage_one" is done in our main*/
   platform_init_stage_one();

  clock_init();
  rtimer_init();
  process_init();
  process_start(&etimer_process, NULL);
  ctimer_init();
  watchdog_init();

  energest_init();

#if STACK_CHECK_ENABLED
  stack_check_init();
#endif

  platform_init_stage_two();

#if QUEUEBUF_ENABLED
  queuebuf_init();
#endif /* QUEUEBUF_ENABLED */
  netstack_init();
  node_id_init();

  LOG_INFO("Starting " CONTIKI_VERSION_STRING "\n");
  LOG_INFO("- Routing: %s\n", NETSTACK_ROUTING.name);
  LOG_INFO("- Net: %s\n", NETSTACK_NETWORK.name);
  LOG_INFO("- MAC: %s\n", NETSTACK_MAC.name);
  LOG_INFO("- 802.15.4 PANID: 0x%04x\n", IEEE802154_PANID);
#if MAC_CONF_WITH_TSCH
  LOG_INFO("- 802.15.4 TSCH default hopping sequence length: %u\n", (unsigned)sizeof(TSCH_DEFAULT_HOPPING_SEQUENCE));
#else /* MAC_CONF_WITH_TSCH */
  LOG_INFO("- 802.15.4 Default channel: %u\n", IEEE802154_DEFAULT_CHANNEL);
#endif /* MAC_CONF_WITH_TSCH */

  LOG_INFO("Node ID: %u\n", node_id);
  LOG_INFO("Link-layer address: ");
  LOG_INFO_LLADDR(&linkaddr_node_addr);
  LOG_INFO_("\n");

#if NETSTACK_CONF_WITH_IPV6
  {
    uip_ds6_addr_t *lladdr;
    memcpy(&uip_lladdr.addr, &linkaddr_node_addr, sizeof(uip_lladdr.addr));
    process_start(&tcpip_process, NULL);

    lladdr = uip_ds6_get_link_local(-1);
    LOG_INFO("Tentative link-local IPv6 address: ");
    LOG_INFO_6ADDR(lladdr != NULL ? &lladdr->ipaddr : NULL);
    LOG_INFO_("\n");
  }
#endif /* NETSTACK_CONF_WITH_IPV6 */

  platform_init_stage_three();

#if BUILD_WITH_RPL_BORDER_ROUTER
  rpl_border_router_init();
  LOG_DBG("With RPL Border Router\n");
#endif /* BUILD_WITH_RPL_BORDER_ROUTER */

#if BUILD_WITH_ORCHESTRA
  orchestra_init();
  LOG_DBG("With Orchestra\n");
#endif /* BUILD_WITH_ORCHESTRA */

#if BUILD_WITH_SHELL
  serial_shell_init();
  LOG_DBG("With Shell\n");
#endif /* BUILD_WITH_SHELL */

#if BUILD_WITH_COAP
  coap_engine_init();
  LOG_DBG("With CoAP\n");
#endif /* BUILD_WITH_COAP */

#if BUILD_WITH_SNMP
  snmp_init();
  LOG_DBG("With SNMP\n");
#endif /* BUILD_WITH_SNMP */

#if BUILD_WITH_SIMPLE_ENERGEST
  simple_energest_init();
#endif /* BUILD_WITH_SIMPLE_ENERGEST */

#if BUILD_WITH_TSCH_CS
  /* Initialize the channel selection module */
  tsch_cs_adaptations_init();
#endif /* BUILD_WITH_TSCH_CS */

#ifdef USE_AUTOSTART_PROCESS
  autostart_start(autostart_processes);
#endif /*USE_AUTOSTART_PROCESS*/

  watchdog_start();
}
/*---------------------------------------------------------------------------*/
/**
* @brief  UART_Platform_Transmit
* 		Sends a character over UART
* @param  uint8_t * msg
* @retval None
*/
void UART_Platform_Transmit(uint8_t * msg)
{
    HAL_UART_Transmit(&UartHandle, (uint8_t*)msg, 1, 5000);
}
/*----------------------------------------------------------------------------*/
/**
* @brief  UART_Platform_init
* 		Initializes UART
* @param  None
* @retval None
*/
void UART_Platform_init(void)
{
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
}
/*---------------------------------------------------------------------------*/
/**
* @brief  node_mac_init
* 		initialises MAC address of the node using its unique ID
* @param  None
* @retval None
*/
static void node_mac_init(void)
{
  uint32_t device_id[3];

  device_id[0] = HAL_GetUIDw0();
  device_id[1] = HAL_GetUIDw1();
  device_id[2] = HAL_GetUIDw2();

  (*(uint32_t*)node_mac) = device_id[1];
  (*(((uint32_t*)node_mac)+1)) = device_id[2];
  (*(((uint32_t*)node_mac)+1)) += device_id[0];
}
/*---------------------------------------------------------------------------*/
/**
* @brief  set_linkaddr
* 		initialises link address of the node using its MAC address
* @param  None
* @retval None
*/
static void
set_linkaddr(void)
{
  linkaddr_t addr;
  memset(&addr, 0, LINKADDR_SIZE);

#if NETSTACK_CONF_WITH_IPV6
  memcpy(addr.u8, node_mac, sizeof(addr.u8));
#else
  int i;
  for(i = 0; i < LINKADDR_SIZE; ++i) {
    addr.u8[i] = node_mac[LINKADDR_SIZE - 1 - i];
  }
#endif
  linkaddr_set_node_addr(&addr);
}
/*---------------------------------------------------------------------------*/
/**
* @brief  print_parameters
* 		prints main system parameters
* @param  None
* @retval None
*/
static void print_parameters(void)
{
#if (ROUTING_CONF_RPL_LITE | ROUTING_CONF_RPL_CLASSIC)
#include "rpl-conf.h"
#endif /*RPL*/

  /* A selected set of parameters to check the firmware configuration.         */
  /* To print float parameters use PRINT_PARAMETER_STR macro                   */
  printf("Platform and Contiki-NG parameters:\n");

  PRINT_PARAMETER_STR("Board: stm32l1xx_nucleo");

  PRINT_PARAMETER_STR("Expansion Board: S2868A2");

  printf("\n");
  PRINT_PARAMETER_STR(RSSI_RX_THRESHOLD);
  PRINT_PARAMETER_STR(RSSI_TX_THRESHOLD);
  PRINT_PARAMETER(RADIO_HW_CSMA, "%d");

  PRINT_PARAMETER_STR(POWER_DBM);
  PRINT_PARAMETER_STR(CHANNEL_SPACE);
  PRINT_PARAMETER_STR(FREQ_DEVIATION);
  PRINT_PARAMETER_STR(BANDWIDTH);
  PRINT_PARAMETER_STR(MODULATION_SELECT);
  PRINT_PARAMETER(DATARATE, "%d");
  PRINT_PARAMETER(IEEE802154_CONF_PANID, "%x");
  PRINT_PARAMETER_STR(BASE_FREQUENCY);
  PRINT_PARAMETER(CHANNEL_NUMBER, "%d");
  PRINT_PARAMETER(MAX_PACKET_LEN, "%d");

  printf("\n");
  PRINT_PARAMETER(UIP_CONF_BUFFER_SIZE, "%d");
  PRINT_PARAMETER(UIP_CONF_ROUTER, "%d");
  PRINT_PARAMETER(UIP_CONF_IPV6_RPL, "%d");
#if ROUTING_CONF_RPL_LITE
  PRINT_PARAMETER(RPL_DEFAULT_LEAF_ONLY, "%d");
#endif /*ROUTING_CONF_RPL_LITE*/
#if ROUTING_CONF_RPL_CLASSIC
  PRINT_PARAMETER(RPL_LEAF_ONLY, "%d");
#endif /*ROUTING_CONF_RPL_CLASSIC*/
  PRINT_PARAMETER(QUEUEBUF_CONF_NUM, "%d");
  PRINT_PARAMETER(UIP_ND6_REACHABLE_TIME, "%d");
  PRINT_PARAMETER(UIP_TCP_MSS, "%d");
  PRINT_PARAMETER(UIP_RECEIVE_WINDOW, "%d");
  PRINT_PARAMETER(UIP_CONF_TCP, "%d");
  PRINT_PARAMETER(UIP_MAX_ROUTES, "%d");
  PRINT_PARAMETER(NBR_TABLE_CONF_MAX_NEIGHBORS, "%d");
  PRINT_PARAMETER(UIP_CONF_ND6_SEND_RA, "%d");
  PRINT_PARAMETER(UIP_CONF_ND6_SEND_NA, "%d");
  PRINT_PARAMETER(UIP_CONF_ND6_SEND_NS, "%d");
#ifdef WITH_IP64
  PRINT_PARAMETER(WITH_IP64, "%d");
#else /*!WITH_IP64*/
  printf("IP64 is disabled.\n");
#endif /*WITH_IP64*/

#ifdef WITH_DTLS
  printf("DTLS is enabled.\n");
#else
  printf("DTLS is disabled.\n");
#endif /*WITH_DTLS*/

#if BUILD_WITH_COAP
  printf("\n");
  PRINT_PARAMETER(REST_MAX_CHUNK_SIZE, "%d");
  PRINT_PARAMETER(COAP_MAX_HEADER_SIZE, "%d");
  PRINT_PARAMETER(COAP_MAX_BLOCK_SIZE, "%d");
  PRINT_PARAMETER(COAP_MAX_PACKET_SIZE, "%d");
  PRINT_PARAMETER(COAP_LINK_FORMAT_FILTERING,  "%d");
  PRINT_PARAMETER(COAP_PROXY_OPTION_PROCESSING,  "%d");
  PRINT_PARAMETER(COAP_MAX_OPEN_TRANSACTIONS,  "%d");
  PRINT_PARAMETER(COAP_MAX_OBSERVERS,  "%d");
#ifdef COAP_OBSERVE_CLIENT
  printf("COAP_OBSERVE_CLIENT is enabled.\n");
  PRINT_PARAMETER(COAP_MAX_OBSERVEES,  "%d");
#else /*!COAP_OBSERVE_CLIENT*/
  printf("COAP_OBSERVE_CLIENT is disabled.\n");
#endif /*#ifdef COAP_OBSERVE_CLIENT*/
  PRINT_PARAMETER(COAP_OBSERVE_REFRESH_INTERVAL, "%d");
#endif /*BUILD_WITH_COAP*/

  PRINT_PARAMETER(RADIO_LOW_POWER, "%d");
  PRINT_PARAMETER(RADIO_SNIFF_MODE, "%d");
#ifdef RADIO_LONG_PREAMBLE
  PRINT_PARAMETER(RADIO_LONG_PREAMBLE, "%d");
#endif /*RADIO_LONG_PREAMBLE*/

#if MAC_CONF_WITH_TSCH
  PRINT_PARAMETER(RADIO_DELAY_BEFORE_TX, "%d");
  PRINT_PARAMETER(RADIO_DELAY_BEFORE_RX, "%d");
  PRINT_PARAMETER(RADIO_DELAY_BEFORE_DETECT, "%d");
  PRINT_PARAMETER(RADIO_PHY_OVERHEAD, "%d");
  PRINT_PARAMETER(RADIO_BYTE_AIR_TIME, "%d");
#endif /*MAC_CONF_WITH_TSCH*/

  printf("\n");
}
/*---------------------------------------------------------------------------*/
/**
* @brief  uart_set_input
* 		sets input handler for the UART
* @param  uint8_t uart
* @param  int (* input) Input handler
* @retval None
*/
void
uart_set_input(uint8_t uart, int (* input)(unsigned char c))
{
  UNUSED(uart);

  input_handler = input;
}
/*---------------------------------------------------------------------------*/
/**
* @brief  platform_init_stage_one
* 		First step of platform initializaion for Contiki-NG
* @param  None
* @retval None
*/
void
platform_init_stage_one(void)
{
  /*For the integration in STM32CubeMX this is done in our main.c */
}
/*---------------------------------------------------------------------------*/
/**
* @brief  platform_init_stage_two
* 		Second step of platform initializaion for Contiki-NG
* @param  None
* @retval None
*/
void
platform_init_stage_two()
{

  node_mac_init();
  set_linkaddr();

#if UART_CONF_ENABLE
  /*In case of Border router the callback will be ovewritten*/
  UART_Platform_init();
  uart_set_input(0, serial_line_input_byte);
#endif /*UART_CONF_ENABLE*/

  serial_line_init();
}
/*---------------------------------------------------------------------------*/
/**
* @brief  platform_init_stage_three
* 		Third step of platform initializaion for Contiki-NG
* @param  None
* @retval None
*/
void
platform_init_stage_three()
{
  LOG_INFO("%s\n", BOARD_STRING);

  random_init(node_id);

  print_parameters();

  LOG_INFO("Contiki-NG and Sub-GHz Radio correctly configured... Starting all processes\n");

  process_start(&sensors_process, NULL);

#if WITH_IP64
  uip_ip4addr_t ipv4addr, netmask, ip4addr;
  uip_ip6addr_t ip6addr;

  uip_ipaddr(&ipv4addr, 192, 168, 0, 1);
  uip_ipaddr(&netmask, 255, 255, 255, 0);
  ip64_set_ipv4_address(&ipv4addr, &netmask);
  /*  printf("IPv4 address %d.%d.%d.%d\n",
  ipv4addr.u8[0], ipv4addr.u8[1],
  ipv4addr.u8[2], ipv4addr.u8[3]);
  */
  uip_ipaddr(&ip4addr, 8,8,8,8);
  ip64_addr_4to6(&ip4addr, &ip6addr);
  uip_nameserver_update((uip_ipaddr_t *)&ip6addr, UIP_NAMESERVER_INFINITE_LIFETIME);
#endif /*WITH_IP64*/
}
/*---------------------------------------------------------------------------*/
/**
* @brief  platform_idle
* 		Contiki-NG idle loop
* @param  None
* @retval None
*/
void
platform_idle()
{
  __WFI();
}
/*---------------------------------------------------------------------------*/
/**
 * @}
 */
/** @} */

/** @} */


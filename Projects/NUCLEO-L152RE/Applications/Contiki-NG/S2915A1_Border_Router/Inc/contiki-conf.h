/**
  ******************************************************************************
  * @file    contiki-conf.h
  * @author  SRA Application Team
  * @brief   Contiki-NG configuration parameters
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

/*---------------------------------------------------------------------------*/
#ifndef CONTIKI_CONF_H__
#define CONTIKI_CONF_H__
/*---------------------------------------------------------------------------*/
#include "platform-conf.h"
#include "RTE_Components.h"
/**
 * @defgroup configuration_files
 * @ingroup Contiki-NG_STM32_Library
 * @{
 */

/**
 * @addtogroup configuration_files
 * @ingroup Contiki-NG_STM32_Library
 * @{
 * @file Main Contiki-NG configuration file
 */

/*---------------------------------------------------------------------------*/
#ifdef __CC_ARM
#pragma anon_unions //Needed by Keil
#endif /*__CC_ARM*/

/* Next define is needed for IAR */
#ifndef CC_CONF_ALIGN
#define CC_CONF_ALIGN(n) __attribute__((__aligned__(n)))
#endif /*CC_CONF_ALIGN*/

/*To avoid a potential problem with RPL Classic in Storing Mode. */
#ifndef UIP_SR_CONF_LINK_NUM
#define UIP_SR_CONF_LINK_NUM NETSTACK_MAX_ROUTE_ENTRIES
#endif /*UIP_SR_CONF_LINK_NUM*/

#define LEDS_CONF_LEGACY_API 1

//Needed here to avoid compilation problems from KEIL. Changed from original code to avoid warnings.
#define RTIMER_BUSYWAIT_UNTIL_ABS(cond, t0, max_time) \
  {                                                                \
    bool c;                                                         \
    do {c=(cond);} while((!c) && RTIMER_CLOCK_LT(RTIMER_NOW(), (t0) + (max_time))); \
  }

/*TSCH macros*/
//@TODO: next macros needs to be validated
#define S2LP_PREAMBLE_TIME (2 * PREAMBLE_LENGTH * 1000000 / DATARATE) //Preamble is in chip sequence (2 bits)
#define S2LP_SYNCH_TIME (SYNC_LENGTH * 1000000 / DATARATE)
#define RADIO_DELAY_BEFORE_TX  ((unsigned)US_TO_RTIMERTICKS(90 + S2LP_PREAMBLE_TIME +S2LP_SYNCH_TIME ))
#define RADIO_DELAY_BEFORE_RX ((unsigned)US_TO_RTIMERTICKS(90))
//#define TSCH_CONF_DEFAULT_TIMESLOT_LENGTH ...
#define RADIO_BYTE_AIR_TIME    (1000000 / ( DATARATE / 8))
#define RADIO_PHY_OVERHEAD         2//Should be 2: 1 Byte for LEN and 1 Byte for CRC...
#define RADIO_DELAY_BEFORE_DETECT ((unsigned)US_TO_RTIMERTICKS(20))//dummy value
#define RADIO_TO_RTIMER(X) ((uint32_t)((uint64_t)(X) * RTIMER_ARCH_SECOND / S2LPTimerGetRcoFrequency()))
//#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE TSCH_HOPPING_SEQUENCE_1_1
/* 6TiSCH minimal schedule length.
 * Larger values result in less frequent active slots: reduces capacity and saves energy. */
//#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 3
#define TSCH_CONF_JOIN_MY_PANID_ONLY 0
#define TSCH_LOG_CONF_PER_SLOT                     0
/* Do not start TSCH at init, wait for NETSTACK_MAC.on() */
#define TSCH_CONF_AUTOSTART 0

/*CSMA macros*/
#define CSMA_CONF_SEND_SOFT_ACK 1
#define CSMA_CONF_ACK_WAIT_TIME (RTIMER_SECOND/2)
#define CSMA_CONF_AFTER_ACK_DETECTED_WAIT_TIME (RTIMER_SECOND/10)
#define CSMA_CONF_MAX_BE 8

/* Enable Link Layer security */
#define LLSEC802154_CONF_ENABLED 1

#define SICSLOWPAN_CONF_COMPRESS_EXT_HDR 0

#define PACKETBUF_CONF_SIZE MAX_PACKET_LEN

/* Network setup for IPv6 */

/* radio driver blocks until ACK is received */
#define IEEE802154_CONF_PANID       0xABCD

#define PROCESS_CONF_NUMEVENTS 8
//#define PROCESS_CONF_STATS 1

#define LINKADDR_CONF_SIZE              8

#define UIP_CONF_LL_802154              1

#define UIP_CONF_ROUTER                 1

#ifndef UIP_CONF_IPV6_QUEUE_PKT
#define UIP_CONF_IPV6_QUEUE_PKT         0
#endif /* UIP_CONF_IPV6_QUEUE_PKT */

#define UIP_CONF_MAX_LISTENPORTS 8
#define UIP_CONF_UDP_CONNS       12
#define UIP_CONF_BROADCAST       1
#define UIP_ARCH_IPCHKSUM        0
#define UIP_CONF_UDP             1
#define UIP_CONF_UDP_CHECKSUMS   1
#define UIP_CONF_TCP		 0
/*---------------------------------------------------------------------------*/
/* include the project config */
/* PROJECT_CONF_H might be defined in the project Makefile */
#ifdef PROJECT_CONF_H
#include "project-conf.h"
#endif /* PROJECT_CONF_H */
/*---------------------------------------------------------------------------*/
#endif /* CONTIKI_CONF_H */
/*---------------------------------------------------------------------------*/
/** @} */
/** @} */

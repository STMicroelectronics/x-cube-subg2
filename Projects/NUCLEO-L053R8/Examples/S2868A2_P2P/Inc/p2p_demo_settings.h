
/**
  ******************************************************************************
  * @file    p2p_demo_settings.h
  * @author  SRA Application Team
  * @brief   This file contains radio settings for the P2P Demo Example
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
#ifndef P2P_DEMO_SETTINGS_H__
#define P2P_DEMO_SETTINGS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo_bus.h"
#include "stm32l0xx_nucleo_errno.h"

#include "stm32l0xx_hal_exti.h"
#include "s2868a2_s2lpgpio_setting.h"
#include "RTE_Components.h"

/* Global configuration ---------------------------------------------------*/

#define USE_BASIC_PROTOCOL

#define USE_BASIC_PROTOCOL_ADDRESS          /* to activate Basic protocol with Address field, to be activated with USE_BASIC_PROTOCOL */

#define        RadioPacketConfig                            S2LP_PacketConfig
#define        RadioSetPayloadlength                        S2LP_SetPayloadlength
#define        RadioEnableTxIrq                             S2LP_EnableTxIrq
#define        RadioEnableRxIrq                             S2LP_EnableRxIrq
#define        RadioSetDestinationAddress                   S2LP_SetDestinationAddress
#define        RadioSetRxTimeout                            S2LP_SetRxTimeout
#define        RadioEnableSQI                               S2LP_EnableSQI
#define        RadioStartRx                                 S2LP_StartRx
#define        RadioStartTx                                 S2LP_StartTx
#define        RadioGetRxPacket                             S2LP_GetRxPacket

#define    RADIO_GPIO_MODE_DIGITAL_OUTPUT_LP                S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP
#define    RADIO_GPIO_DIG_OUT_IRQ                           S2LP_GPIO_DIG_OUT_IRQ

#define    RadioCsmaInit                                    SCsmaInit

#define    RadioIrqs                                        S2LPIrqs

#define    RadioGetReceivedDestinationAddress()             S2LP_PCKT_HNDL_GetReceivedDestinationAddress()

#define    Radio_PktStackLlpInit                            S2LP_PCKT_STACK_LlpInit

#define    Radio_PktStackFilterOnSourceAddress              S2LP_PCKT_STACK_FilterOnSourceAddress
#define    Radio_PktStackSetRxSourceMask                    S2LP_PCKT_STACK_SetRxSourceMask
#define    Radio_PktStackSetSourceReferenceAddress          S2LP_PCKT_STACK_SetSourceReferenceAddress

#define    RadioPktStackInit                                S2LP_PCKT_STACK_Init
#define    RadioPktBasicInit                                S2LP_PCKT_BASIC_Init

#define    RadioCsma                                        S2LP_CSMA_Enable
#define    RadioPersistenRx                                 S2LP_PCKT_HNDL_SetRxPersistentMode

#define    RadioCsBlanking                                  S2LP_RADIO_QI_CsBlanking

#if defined(USE_STack_PROTOCOL)
//#define USE_STack_LLP  /*Uncomment if LLP featured need to be used*/
#endif
/******************************************************************************/

#ifdef USE_BASIC_PROTOCOL
  #define CSMA_ENABLE  /* Comment this line to disable the CSMA */
#endif

#ifdef CSMA_ENABLE
#include "S2LP_Csma.h"
/* CSMA configuration parameters */
#define PERSISTENT_MODE_EN                                  S_DISABLE
#define CS_PERIOD                                           CSMA_PERIOD_64TBIT
#define CS_TIMEOUT                                          3
#define MAX_NB                                              5
#define BU_COUNTER_SEED                                     0xFA21
#define CU_PRESCALER                                        32

extern RadioCsmaInit xCsmaInit;
 #endif

/* Defines ---------------------------------------------------------------*/

#define XTAL_OFFSET_PPM             0
#define INFINITE_TIMEOUT            0.0

#define BASE_FREQUENCY              868.0e6

#define CHANNEL_SPACE               100e3
#define CHANNEL_NUMBER              0
#define DATARATE                    38400
#define FREQ_DEVIATION              20e3
#define BANDWIDTH                   100E3
#define POWER_INDEX                 7
#define RECEIVE_TIMEOUT             2000.0 /*change the value for required timeout period*/

#define RSSI_THRESHOLD              -120  /* Default RSSI at reception, more
                                                             than noise floor */
#define CSMA_RSSI_THRESHOLD         -90   /* Higher RSSI to Transmit.
                              If it's lower, the Channel will be seen as busy */

/*  Packet configuration parameters  */

#define SYNC_WORD                       0x88888888
#define LENGTH_WIDTH                    7
#define CRC_MODE                        PKT_CRC_MODE_8BITS
#define EN_FEC                          S_DISABLE
#define EN_WHITENING                    S_ENABLE

#define MODULATION_SELECT               MOD_2FSK
#define POWER_DBM                       12.0
#define PREAMBLE_LENGTH                 PREAMBLE_BYTE(4)
#define SYNC_LENGTH                     SYNC_BYTE(4)
#define CONTROL_LENGTH                  0x00 //PKT_CONTROL_LENGTH_0BYTES
#define VARIABLE_LENGTH                 S_ENABLE
#define EXTENDED_LENGTH_FIELD           S_DISABLE

#define PREAMBLE_BYTE(v)                (4*v)
#define SYNC_BYTE(v)                    (8*v)
#define VARIABLE_LENGTH                 S_ENABLE

#ifndef USE_BASIC_PROTOCOL_ADDRESS
  /*  Addresses configuration parameters  */
  #define EN_ADDRESS                    S_DISABLE
  #define EN_FILT_MY_ADDRESS            S_DISABLE
  #define EN_FILT_MULTICAST_ADDRESS     S_DISABLE
  #define EN_FILT_BROADCAST_ADDRESS     S_DISABLE
  #define EN_FILT_SOURCE_ADDRESS        S_DISABLE//S_ENABLE
  #define SOURCE_ADDR_MASK              0xf0
  #define SOURCE_ADDR_REF               0x37
  #define MULTICAST_ADDRESS             0xEE
  #define BROADCAST_ADDRESS             0xFF

#else

  #define EN_ADDRESS                    S_ENABLE
  #define EN_FILT_MY_ADDRESS            S_ENABLE
  #define EN_FILT_MULTICAST_ADDRESS     S_ENABLE
  #define EN_FILT_BROADCAST_ADDRESS     S_ENABLE
  #define EN_FILT_SOURCE_ADDRESS        S_ENABLE
  #define SOURCE_ADDR_MASK              0xf0
  #define SOURCE_ADDR_REF               0x37
  #define MULTICAST_ADDRESS             0xEE
  #define BROADCAST_ADDRESS             0xFF

#endif

#define EN_AUTOACK                      S_DISABLE
#define EN_PIGGYBACKING                 S_DISABLE
#define MAX_RETRANSMISSIONS             PKT_DISABLE_RETX

#define PAYLOAD_LEN                     25 /*20 bytes data + tag + cmd_type + cmd + cmdlen + datalen*/
#define APPLI_CMD                       0x11
#define NWK_CMD                         0x22
#define LED_TOGGLE                      0xff
#define ACK_OK                          0x01
#define MAX_BUFFER_LEN                  96
#define TIME_TO_EXIT_RX                 3000
#define DELAY_RX_LED_TOGGLE             100
#define DELAY_TX_LED_GLOW               100
#define LPM_WAKEUP_TIME                 100
#define DATA_SEND_TIME                  50

#ifdef __cplusplus
}
#endif

#endif /* P2P_DEMO_SETTINGS_H__*/

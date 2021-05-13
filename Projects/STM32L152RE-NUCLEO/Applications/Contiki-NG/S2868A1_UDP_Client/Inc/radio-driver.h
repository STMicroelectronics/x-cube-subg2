
/**
  ******************************************************************************
  * @file    radio-driver.h
  * @author  SRA Application Team
  * @brief   Header file for S2LP radio configuration/driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under ODE Software License Agreement
  * SLA0094, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0094
  *
  ******************************************************************************
  */
/*---------------------------------------------------------------------------*/
#ifndef RADIO_DRIVER_H__
#define RADIO_DRIVER_H__
/*---------------------------------------------------------------------------*/

#include "s2lp.h"
#include "RTE_Components.h"
#include "contiki-conf.h"
/*---------------------------------------------------------------------------*/
extern const struct radio_driver subGHz_radio_driver;
/*---------------------------------------------------------------------------*/
/**
 * @addtogroup ST_Radio
 * @ingroup Contiki-NG_STM32_Library
 * @{
 * @file subGHz radio configuration file for Contiki
 */
/*---------------------------------------------------------------------------*/
void Radio_interrupt_callback(void);
/*---------------------------------------------------------------------------*/
#if (defined S2868A1) || (defined S2868A2)
  #define USE_RADIO_868MHz
#elif defined(S2915A1)
  #define USE_RADIO_915MHz
#else /*!X_NUCLEO_S2868A1 && !X_NUCLEO_S2915A1*/
#error RADIO Nucleo Shield undefined or unsupported
#endif /*X_NUCLEO_S2868A1 || X_NUCLEO_S2915A1*/

/* Exported constants --------------------------------------------------------*/

/*  Radio configuration parameters  */
#define XTAL_OFFSET_PPM             0
#define INFINITE_TIMEOUT            0.0

//@TODO: Validate CHANNEL_NUMBER_MIN / MAX values
#ifdef USE_RADIO_433MHz
#define BASE_FREQUENCY              433.0e6
#define CHANNEL_NUMBER_MIN          0
#define CHANNEL_NUMBER_MAX          13
#endif /*USE_RADIO_433MHz*/

#ifdef USE_RADIO_868MHz
#define BASE_FREQUENCY              868.0e6
#define CHANNEL_NUMBER_MIN          0
#define CHANNEL_NUMBER_MAX          13
#endif /*USE_RADIO_868MHz*/

#ifdef USE_RADIO_915MHz
#define BASE_FREQUENCY              915.0e6
#define CHANNEL_NUMBER_MIN          0
#define CHANNEL_NUMBER_MAX          13
#endif /*USE_RADIO_915MHz*/

#define RADIO_IRQ_ENABLE()    S2868A1_EXIT_CRITICAL()
#define RADIO_IRQ_DISABLE()   S2868A1_ENTER_CRITICAL()

#define    RADIO_GPIO_MODE_DIGITAL_OUTPUT_LP                S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP
#define    RADIO_GPIO_DIG_OUT_IRQ                           S2LP_GPIO_DIG_OUT_IRQ

#define radio_spi_busy() (HAL_SPI_GetState(&hspi) != HAL_SPI_STATE_READY)

#define CHANNEL_SPACE               100e3
#define CHANNEL_NUMBER              0
#define IEEE802154_CONF_DEFAULT_CHANNEL CHANNEL_NUMBER
#define MODULATION_SELECT           MOD_2FSK
#define DATARATE                    38400 /*bps*/
#define FREQ_DEVIATION              20e3
#define BANDWIDTH                   100.0e3

#define RADIO_POWER_DBM_MAX      14
#define RADIO_POWER_DBM_MIN      -31

#define POWER_DBM                   12.0
#define POWER_INDEX                 7

#define RSSI_RX_THRESHOLD          -118.0   /* dBm */
#define RSSI_TX_THRESHOLD          -90.0   /* dBm */

/*  Packet configuration parameters  */
#if RADIO_LONG_PREAMBLE
#define PREAMBLE_LENGTH             PREAMBLE_BYTE(64)
#else /*!RADIO_LONG_PREAMBLE*/
#define PREAMBLE_LENGTH             PREAMBLE_BYTE(4)
#endif/*RADIO_LONG_PREAMBLE*/

#define SYNC_LENGTH                 SYNC_BYTE(4)
#define SYNC_WORD                   0x88888888
#define VARIABLE_LENGTH             S_ENABLE
#define EXTENDED_LENGTH_FIELD       S_DISABLE
#define CRC_MODE                    PKT_CRC_MODE_8BITS
#define EN_FEC                      S_DISABLE
#define EN_WHITENING                S_ENABLE

#if RADIO_ADDRESS_FILTERING
#define EN_ADDRESS                  S_ENABLE
#else /*!RADIO_ADDRESS_FILTERING*/
#define EN_ADDRESS                  S_DISABLE
#endif /*RADIO_ADDRESS_FILTERING*/

/*  Addresses configuration parameters  */
#define EN_FILT_MY_ADDRESS          S_ENABLE
#define EN_FILT_MULTICAST_ADDRESS   S_DISABLE //We use "MY" and BROADCAST
#define MULTICAST_ADDRESS           0xEE
#define EN_FILT_BROADCAST_ADDRESS   S_ENABLE
#define BROADCAST_ADDRESS           0xFF

#define PREAMBLE_BYTE(v)           (4*v)
#define SYNC_BYTE(v)               (8*v)

#if RADIO_SNIFF_MODE
#define MIN_PERIOD_WAKEUP_MS ((8000*((PREAMBLE_LENGTH/4)-2))/DATARATE)
#define RX_TIMEOUT_MS        30
#endif /*RADIO_SNIFF_MODE*/

/**
 * The MAX_PACKET_LEN is the max allowed len for the packet.
 * If it is more than S2LP_RX_FIFO_SIZE it will be handled with FIFO thresholds.
 * The RADIO supports with its packet handler a length of 65,535 bytes,
 * and in direct mode (without packet handler) there is no limit of data.
 * At system level, the max len is set in PACKETBUF_SIZE, so we se it
 * with PACKETBUF_CONF_SIZE = MAX_PACKET_LEN in contiki-conf.h
 */
#define MAX_PACKET_LEN              S2LP_RX_FIFO_SIZE

/*---------------------------------------------------------------------------*/
#endif /* RADIO_DRIVER_H__ */
/*---------------------------------------------------------------------------*/
/** @} */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

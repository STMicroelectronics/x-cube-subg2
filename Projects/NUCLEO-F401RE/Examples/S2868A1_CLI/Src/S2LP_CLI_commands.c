/**
  ******************************************************************************
  * @file    S2LP_CLI_Commands.c
  * @author  STMicroelectronics
  * @date    March 2022
  * @brief   S2LP CLI Commands source file
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

#include "S2LP_CLI_commands.h"
#include "S2LP_Config.h"
#include "command-interpreter2.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "utils.h"
#include "SDK_UTILS_Timers.h"
#include "SDK_CLI_commands.h"

#include "S2LP_CORE_GPIO.h"
#include "SDK_EVAL_Com.h"
#include "s2lp_management.h"
#include "s2lp_sdkapi_mapping.h"

#define READ_NUM_ELEMENTS_FIFO()	S2LPFifoReadNumberBytesRxFifo()
#define REFRESH_STATUS()        	S2LPRefreshStatus()
#define SET_IRQ_MASK            	S2LPGpioIrqConfig
#define CLEAR_IRQ_STATUS        	S2LPGpioIrqClearStatus
#define GET_PREAMBLE_LENGTH     	S2LPGetPreambleLength
#define GET_SYNC_LENGTH         	S2LPGetSyncLength
#define GET_RX_DEST_ADDR        	S2LPGetReceivedDestinationAddress
#define GET_RX_SOURCE_ADDR      	S2LPGetReceivedSourceAddress
#define GET_MY_ADDR             	S2LPGetMyAddress
#define GET_RX_BROADCAST_ADDR   	S2LPGetBroadcastAddress
#define GET_RX_MULTICAST_ADDR   	S2LPGetMulticastAddress
#define GET_RX_SOURCE_MASK      	S2LPGetRxSourceMask
#define GET_RX_SOURCE_REF       	S2LPGetRxSourceReferenceAddress
#define GET_RX_CTRL()           	0
#define GET_CTRL_MASK()         	0
#define GET_CTRL_REF()          	0
#define GET_CTRL_LEN()          	0

#define GP_BUFFER_SIZE			3000

#if (defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
	#define SdkEvalTimersState(a, b, c) SdkEvalTimersState(a, c)
#endif

extern uint8_t responsePrintf(const char * formatString, ...);
extern IrqQueue s_xIrqQueue;
extern volatile uint8_t raw_input;
extern volatile uint8_t xApplTimerIRQRaised;

static uint8_t getNBytesReportAll = 0;
static uint8_t vectcBuff[GP_BUFFER_SIZE];
volatile FlagStatus xIRQRaised = RESET;
volatile uint8_t stopCmdFlag=0;
static volatile uint8_t SendBatchLP=0, GetBatchLP=0;

#if (defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
    #define S2LP_Middleware_GpioInterruptCmd(pA, pB, pC, pD) S2LP_Middleware_GpioInterruptCmd(pA, pB, pC, pD, M2S_GPIO_EDGE_EVENT)
#endif

#define SdkDelayMs(...) HAL_Delay(__VA_ARGS__)

uint8_t checkStop(void)
{
  uint8_t c;

  stopCmdFlag = 0;

  if (__io_getcharNonBlocking(&c)) {
    stopCmdFlag = (c == 'S');
  }

  return stopCmdFlag;
}

void S2LPGetRcoFrequencyAction(void)
{
  uint16_t rco_freq = S2LPTimerGetRcoFrequency();
  responsePrintf("{&N API call...&t2x}\r\n","S2LPGetRcoFrequency", "value", rco_freq);
}

float S2LPGetMsUntilSync(void)
{
  uint32_t dr=S2LPRadioGetDatarate();

  return ((1000.0*(uint32_t)(GET_PREAMBLE_LENGTH()*2+GET_SYNC_LENGTH()))/dr);
}

static void S2LPFillIRQQueue(void)
{
  if(s_xIrqQueue.cIrqQueueSize < (IRQ_QUEUE_MAX_SIZE-1))  {
    S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, (uint8_t *)s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPush].irq_status);
    s_xIrqQueue.cIndexPush = (s_xIrqQueue.cIndexPush+1)%IRQ_QUEUE_MAX_SIZE;
    s_xIrqQueue.cIrqQueueSize++;
  }
  else {
    /* Overflow IRQ queue */
    uint8_t tempRegValue[4];
    S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tempRegValue);
  }
}

/**
 * @addtogroup CLI
 * @{
 */

/**
 * @defgroup S2LP_CLI_Functions                   S2-LP Routines
 * @brief  This module contains a set of routines strictly related to the S2-LP.
 *         Some of them make use of the uC, some of them are just register setting for the device.
 * @{
 */

/**
 * @brief  Returns the packet Format.
 *         <p><b>HowToCall</b>:  S2LPGetPktFrmt</p>
 * @param  None.
 * @retval uint8_t Number of elements in the Rx FIFO.
 *          This value can be: 0: Basic,  2: WM-Bus,  3: STack.
 */
void S2LPGetPktFrmtAction(void)
{
  uint8_t tmp;
  tmp = S2LPGetPacketFormat();
  responsePrintf("{&N API call...&tx}\r\n", "S2LPGetPktFrmt", "value", tmp );
}

/**
 * @brief  Initialize the BASIC packet Format.
 *         <p><b>HowToCall</b>:  S2LPPktBasicInit v u w u u u u u u</p>
 * @param  preamble_length: preamble length in bit.
 * @param  sync_length: sync length in bit.
 * @param  sync_word: sync word.
 * @param  fix_var_length: (0: fixed, 1: var)
 * @param  extended_length: only for variable length(0: 1byte address, 1: 2bytes address)
 * @param  crc_mode: it can be one of the following:
 *           <ul><li>0: PKT_NO_CRC</li>
 *           <li>0x20: PKT_CRC_MODE_8BITS (poly 0x07)</li>
 *           <li>0x40: PKT_CRC_MODE_16BITS_1 (poly 0x8005)</li>
 *           <li>0x60: PKT_CRC_MODE_16BITS_2 (poly 0x1021)</li>
 *           <li>0x80: PKT_CRC_MODE_24BITS (poly 0x864CFB)</li>
 *           <li>0xA0: PKT_CRC_MODE_32BITS (poly 0x04C011BB7)</li></ul>
 * @param  address: specifies if address must be used (1) or not (0).
 * @param  fec: specifies if FEC must be used (1) or not (0).
 * @param  whitening: specifies if whitening must be used (1) or not (0).
 * @retval None.
 */
void S2LPPktBasicInitAction(void)
{
  PktBasicInit xBasicInit;

  xBasicInit.xPreambleLength = unsignedCommandArgument(0);
  xBasicInit.xSyncLength = unsignedCommandArgument(1);
  xBasicInit.lSyncWords = unsignedCommandArgument(2);
  xBasicInit.xFixVarLength = (SFunctionalState)unsignedCommandArgument(3);
  xBasicInit.cExtendedPktLenField = (SFunctionalState)unsignedCommandArgument(4);
  xBasicInit.xCrcMode = (BasicCrcMode)(unsignedCommandArgument(5));
  xBasicInit.xAddressField = (SFunctionalState)unsignedCommandArgument(6);
  xBasicInit.xFec = (SFunctionalState)unsignedCommandArgument(7);
  xBasicInit.xDataWhitening = (SFunctionalState)unsignedCommandArgument(8);

  S2LPPktBasicInit(&xBasicInit);
  responsePrintf("{&N API call...}\r\n", "S2LPPktBasicInit");
}

/**
 * @brief  Returns the BASIC packet info.
 *         <p><b>HowToCall</b>:  S2LPPktBasicGetInfo</p>
 * @param  None.
 * @retval All the packet set in the chip paramaters.
 */
void S2LPPktBasicGetInfoAction(void)
{
  PktBasicInit xBasicInit;
  S2LPPktBasicGetInfo(&xBasicInit);
  responsePrintf("{&N API call...&t2x&tx&t4x&tx&tx&tx&tx&tx&tx}\r\n",
   "S2LPPktBasicGetInfo",
     "preamble_length", xBasicInit.xPreambleLength,
     "sync_length", xBasicInit.xSyncLength,
     "sync_word", xBasicInit.lSyncWords,
     "length_mode", xBasicInit.xFixVarLength,
     "length_size", xBasicInit.cExtendedPktLenField,
     "crc_mode", xBasicInit.xCrcMode,
     "address", xBasicInit.xAddressField,
     "fec", xBasicInit.xFec,
     "whitening", xBasicInit.xDataWhitening);
}

/**
 * @brief  Initialize the STack packet Format.
 *         <p><b>HowToCall</b>:  S2LPPktStackInit v u w u u u u u</p>
 * @param  preamble_length: preamble length in bits.
 * @param  sync_length: sync length in bits.
 * @param  sync_word: sync word.
 * @param  fix_var_length: (0: fixed, 1: var)
 * @param  extended_length: only for variable length(0: 1byte address, 1: 2bytes address)
 * @param  crc_mode: it can be one of the following:
 *           <ul><li>0: PKT_NO_CRC</li>
 *           <li>0x20: PKT_CRC_MODE_8BITS (poly 0x07)</li>
 *           <li>0x40: PKT_CRC_MODE_16BITS_1 (poly 0x8005)</li>
 *           <li>0x60: PKT_CRC_MODE_16BITS_2 (poly 0x1021)</li>
 *           <li>0x80: PKT_CRC_MODE_24BITS (poly 0x864CFB)</li>
 *           <li>0xA0: PKT_CRC_MODE_32BITS (poly 0x04C011BB7)</li></ul>
 * @param  fec: specifies if FEC must be used (1) or not (0).
 * @param  whitening: specifies if whitening must be used (1) or not(0).
 * @retval None.
 */
void S2LPPktStackInitAction(void)
{
  PktStackInit xPktStackInit;

  xPktStackInit.xPreambleLength = unsignedCommandArgument(0);
  xPktStackInit.xSyncLength = unsignedCommandArgument(1);
  xPktStackInit.lSyncWords = unsignedCommandArgument(2);
  xPktStackInit.xFixVarLength = (SFunctionalState)unsignedCommandArgument(3);
  xPktStackInit.cExtendedPktLenField = (SFunctionalState)unsignedCommandArgument(4);
  xPktStackInit.xCrcMode = (PktCrcMode)(unsignedCommandArgument(5));
  xPktStackInit.xFec = (SFunctionalState)unsignedCommandArgument(6);
  xPktStackInit.xDataWhitening = (SFunctionalState)unsignedCommandArgument(7);

  S2LPPktStackInit(&xPktStackInit);
  responsePrintf("{&N API call...}\r\n", "S2LPPktStackInit");
}

/**
 * @brief  Returns the STack packet info.
 *         <p><b>HowToCall</b>:  S2LPPktStackGetInfo</p>
 * @param  None.
 * @retval All the packet set in the chip paramaters.
 */
void S2LPPktStackGetInfoAction(void)
{
  PktStackInit xPktStackInit;
  S2LPPktStackGetInfo(&xPktStackInit);
  responsePrintf("{&N API call...&t2x&tx&t4x&tx&tx&tx&tx&tx}\r\n",
   "S2LPPktStackGetInfo",
     "preamble_length", xPktStackInit.xPreambleLength,
     "sync_length", xPktStackInit.xSyncLength,
     "sync_word", xPktStackInit.lSyncWords,
     "length_mode", xPktStackInit.xFixVarLength,
     "length_size", xPktStackInit.cExtendedPktLenField,
     "crc_mode", xPktStackInit.xCrcMode,
     "fec", xPktStackInit.xFec,
     "whitening", xPktStackInit.xDataWhitening);
}

/**
 * @brief  Initialize the wM-Bus packet Format.
 *         <p><b>HowToCall</b>:  S2LPPktMbusInit u u u</p>
 * @note   The chip does not have a wM-BUS packet format , but the S2-LP library provide a set of API to
 *         configure the single blocks in order to be compilent to the WMBUS packets.
 * @param  wmbus_submode:
 *           <ul><li>0: WMBUS_SUBMODE_NOT_CONFIGURED</li>
 *           <li>1: WMBUS_SUBMODE_S1_S2_LONG_HEADER</li>
 *           <li>2: WMBUS_SUBMODE_S1_M_S2_T2_OTHER_TO_METER</li>
 *           <li>3: WMBUS_SUBMODE_T1_T2_METER_TO_OTHER</li>
 *           <li>4: WMBUS_SUBMODE_R2_SHORT_HEADER</li></ul>
 * @param  preamble_length: preamble len in chips.
 *         Some offset are predefined depending on the submode:
 *           <ul><li>279: WMBUS_SUBMODE_S1_S2_LONG_HEADER</li>
 *           <li>15: WMBUS_SUBMODE_S1_M_S2_T2_OTHER_TO_METER</li>
 *           <li>19:    WMBUS_SUBMODE_T1_T2_METER_TO_OTHER</li>
 *           <li>39:     WMBUS_SUBMODE_R2_SHORT_HEADER</li></ul>
 * @param  postamble_length: preamble len in chips.
 * @retval None.
 */
void S2LPPktMbusInitAction(void)
{
  PktWMbusInit xMbusInit;
  xMbusInit.xWMbusSubmode = (WMbusSubmode)unsignedCommandArgument(0);
  xMbusInit.cPreambleLength = unsignedCommandArgument(1);
  xMbusInit.cPostambleLength = unsignedCommandArgument(2);
  S2LPPktWMbusInit(&xMbusInit);

  responsePrintf("{&N API call...}\r\n", "S2LPPktMbusInit");
}

/**
 * @brief  Returns the wM-Bus packet info.
 *         <p><b>HowToCall</b>:  S2LPPktMbusGetInfo</p>
 * @param  None.
 * @retval All the packet set in the chip paramaters.
 */
void S2LPPktMbusGetInfoAction(void)
{
  PktWMbusInit xMbusInit;
  S2LPPktWMbusGetInfo(&xMbusInit);
  responsePrintf("{&N API call...&tx&tx&tx}\r\n", "S2LPPktMbusGetInfo",
                 "submode", xMbusInit.xWMbusSubmode,
                 "preamble", xMbusInit.cPreambleLength,
                 "postamble", xMbusInit.cPostambleLength);
}

/**
 * @brief  Set the payload length for Basic packets.
 *         <p><b>HowToCall</b>:  S2LPPktBasicSetPayloadLength v</p>
 * @param  payload_len: payload length to be set
 * @retval None
 */
void S2LPPktBasicSetPayloadLengthAction(void)
{
  S2LPPktBasicSetPayloadLength(unsignedCommandArgument(0));
  responsePrintf("{&N API call...}\r\n", "S2LPPktBasicSetPayloadLength");
}

/**
 * @brief  Get the payload length for Basic packets.
 *         <p><b>HowToCall</b>:  S2LPPktBasicGetPayloadLength</p>
 * @param  None
 * @retval payload length
 */
void S2LPPktBasicGetPayloadLengthAction(void)
{
  uint16_t nPayload = S2LPPktBasicGetPayloadLength();
  responsePrintf("{&N API call...&t2x}\r\n", "S2LPPktBasicGetPayloadLength",
  "value", nPayload);
}

/**
 * @brief  Set the payload length for STack packets.
 *         <p><b>HowToCall</b>:  S2LPPktStackSetPayloadLength v</p>
 * @param  payload_len: payload length to be set
 * @retval None
 */
void S2LPPktStackSetPayloadLengthAction(void)
{
  S2LPPktStackSetPayloadLength(unsignedCommandArgument(0));
  responsePrintf("{&N API call...}\r\n", "S2LPPktStackSetPayloadLength");
}

/**
 * @brief  Get the payload length for STack packets.
 *         <p><b>HowToCall</b>:  S2LPPktStackGetPayloadLength</p>
 * @param  None
 * @retval payload length
 */
void S2LPPktStackGetPayloadLengthAction(void)
{
  uint16_t nPayload = S2LPPktStackGetPayloadLength();
  responsePrintf("{&N API call...&t2x}\r\n", "S2LPPktStackGetPayloadLength",
  "value", nPayload);
}

/**
 * @brief  Set the payload length for wM-Bus packets.
 *         <p><b>HowToCall</b>:  SpiritPktMbusSetPayloadLength v</p>
 * @param  payload_len: payload length to be set
 * @retval None
 */
void S2LPPktMbusSetPayloadLengthAction(void)
{
  S2LPPktWMbusSetPayloadLength(unsignedCommandArgument(0));
  responsePrintf("{&N API call...}\r\n", "SpiritPktMbusSetPayloadLength" );
}

/**
 * @brief  Get the payload length for wM-Bus packets.
 *         <p><b>HowToCall</b>:  SpiritPktMbusGetPayloadLength</p>
 * @param  None
 * @retval payload length
 */
void S2LPPktMbusGetPayloadLengthAction(void)
{
  uint16_t nPayload = S2LPPktWMbusGetPayloadLength();
  responsePrintf("{&N API call...&t2x}\r\n", "SpiritPktMbusGetPayloadLength",
                 "value", nPayload);
}

/**
 * @brief  Set the RX timeout.
 *         <p><b>HowToCall</b>:  S2LPTimerSetRxTimeoutUs w</p>
 * @param  rx_timeout: RX timeout in us.
 * @retval None
 */
void S2LPTimerSetRxTimeoutUsAction(void)
{
  uint32_t rx_timeout = unsignedCommandArgument(0);

  if(rx_timeout == 0) {
    SET_INFINITE_RX_TIMEOUT();
  }
  else
  {
    S2LPTimerSetRxTimerUs(rx_timeout);
  }
  responsePrintf("{&N API call...}\r\n", "S2LPTimerSetRxTimeoutUs");
}

/**
 * @brief  Get the RX tiemout.
 *         <p><b>HowToCall</b>:  S2LPTimerGetRxTimeout</p>
 * @param  None
 * @retval Rx timeout in us.
 */
void S2LPTimerGetRxTimeoutAction(void)
{
  uint32_t lRxTimeoutUs;
  uint8_t cCounter, cPrescaler;

  S2LPTimerGetRxTimerUs(&lRxTimeoutUs, &cCounter, &cPrescaler);

  responsePrintf("{&N API call...&t4x&tx&tx}\r\n", "S2LPTimerGetRxTimeout",
                 "period", lRxTimeoutUs,
                 "counter", cCounter,
                 "prescaler", cPrescaler);
}

/**
 * @brief  Initialize the S2-LP radio.
 *         <p><b>HowToCall</b>:  S2LPRadioInit w u w w w w</p>
 * @param  frequency_base: base frequency in Hz.
 * @param  modualtion_select:
 *        <ul><li>0x70: MOD_NO_MOD  (CW modulation selected)</li>
 *        <li>0x00: MOD_2FSK  (2-FSK modulation selected)</li>
 *        <li>0x10: MOD_4FSK  (4-FSK modulation selected)</li>
 *        <li>0xA0: MOD_2GFSK_BT05 (2GFSK modulation selected with BT = 0.5)</li>
 *        <li>0x20: MOD_2GFSK_BT1 (2GFSK modulation selected with BT = 1)</li>
 *        <li>0xB0: MOD_4GFSK_BT05 (G4FSK modulation selected with BT = 0.5)</li>
 *        <li>0x30: MOD_4GFSK_BT1 (G4FSK modulation selected with BT = 1)</li>
 *        <li>0x50: MOD_ASK_OOK (OOK modulation selected)</li>
 *        <li>0x60: MOD_POLAR (Polar mode selected)</li></ul>
 * @param  data_rate: datarate in sym/s
 * @param  fdev: frequency deviation in Hz
 * @param  rx_bandwidth: rx filter bandwidth in Hz
 * @param  xtal_frequency: suggested value is 0 (in this case the preset XTAL value will be used)
 * @retval None.
 */
void S2LPRadioInitAction(void)
{
  SRadioInit xRadioInit;
  uint8_t err;

  xRadioInit.lFrequencyBase = unsignedCommandArgument(0);
  xRadioInit.xModulationSelect = (ModulationSelect)unsignedCommandArgument(1);
  xRadioInit.lDatarate = unsignedCommandArgument(2);
  xRadioInit.lFreqDev = unsignedCommandArgument(3);
  xRadioInit.lBandwidth = unsignedCommandArgument(4);

  err = S2LPRadioInit(&xRadioInit);
  responsePrintf("{&N API call...&tx}\r\n",
         "S2LPRadioInit", "error", err );
}

/**
 * @brief  Get Radio parameters.
 *         <p><b>HowToCall</b>:  S2LPRadioGetInfo</p>
 * @param  None
 * @retval Radio parameters.
 */
void S2LPRadioGetInfoAction(void)
{
  SRadioInit xRadioInit;
  S2LPRadioGetInfo(&xRadioInit);
  uint32_t xtal = S2LPRadioGetXtalFrequency();

  responsePrintf("{&N API call...&t4x&tx&t4x&t4x&t4x&t4x}\r\n", "S2LPRadioGetInfo",
   "Frequency_base", (uint32_t)xRadioInit.lFrequencyBase,
   "Modulation", xRadioInit.xModulationSelect,
   "Data_rate", (uint32_t)xRadioInit.lDatarate,
   "Frequency_deviation", (uint32_t)xRadioInit.lFreqDev,
   "Channel_filter_bandwidth", (uint32_t)xRadioInit.lBandwidth,
   "XTAL_frequency", xtal);
}

/**
 * @brief  Get the crystal frequency.
 *         <p><b>HowToCall</b>:  S2LPRadioGetXtalFrequency</p>
 * @param  None
 * @retval crystal frequency in Hz.
 */
void S2LPRadioGetXtalFrequencyAction(void)
{
  responsePrintf("{&N API call...&t4x}\r\n",
   "S2LPRadioGetXtalFrequency",
    "value", S2LPRadioGetXtalFrequency() );
}

/**
 * @brief  Initialize the S2-LP radio base frequency.
 *         <p><b>HowToCall</b>:  S2LPRadioSetFrequencyBase w</p>
 * @param  frequency_base: base frequency in Hz.
 * @retval None
 */
void S2LPRadioSetFrequencyBaseAction(void)
{
  uint32_t f_base = unsignedCommandArgument(0);

  S2LPRadioSetFrequencyBase(f_base);
  responsePrintf("{&N API call...}\r\n",
   "S2LPRadioSetFrequencyBase" );
}

/**
 * @brief  Get the radio base frequency.
 *         <p><b>HowToCall</b>:  S2LPRadioGetFrequencyBase</p>
 * @param  None
 * @retval base frequency in Hz.
 */
void S2LPRadioGetFrequencyBaseAction(void)
{
  uint32_t f_base = S2LPRadioGetFrequencyBase();

  responsePrintf("{&N API call...&t4x}\r\n",
   "S2LPRadioGetFrequencyBase", "value", f_base );
}

/**
 * @brief  Initialize the S2-LP modulation scheme.
 *         <p><b>HowToCall</b>:  S2LPRadioSetModulation u</p>
 * @param  modulation scheme:
 *        <ul><li>0x70: MOD_NO_MOD  (CW modulation selected)</li>
 *        <li>0x00: MOD_2FSK  (2-FSK modulation selected)</li>
 *        <li>0x10: MOD_4FSK  (4-FSK modulation selected)</li>
 *        <li>0xA0: MOD_2GFSK_BT05 (2GFSK modulation selected with BT = 0.5)</li>
 *        <li>0x20: MOD_2GFSK_BT1 (2GFSK modulation selected with BT = 1)</li>
 *        <li>0xB0: MOD_4GFSK_BT05 (G4FSK modulation selected with BT = 0.5)</li>
 *        <li>0x30: MOD_4GFSK_BT1 (G4FSK modulation selected with BT = 1)</li>
 *        <li>0x50: MOD_ASK_OOK (OOK modulation selected)</li>
 *        <li>0x60: MOD_POLAR (OOK modulation selected)</li></ul>
 * @retval None
 */
void S2LPRadioSetModulationAction(void)
{
  ModulationSelect mod = (ModulationSelect)unsignedCommandArgument(0);
  S2LPRadioSetModulation(mod);
  responsePrintf("{&N API call...}\r\n", "S2LPRadioSetModulation");
}

/**
 * @brief Get the S2-LP modulation scheme.
 *         <p><b>HowToCall</b>:  S2LPRadioGetModulation</p>
 * @param None
 * @retval  modulation scheme:
 *        <ul><li>0x70: MOD_NO_MOD  (CW modulation selected)</li>
 *        <li>0x00: MOD_2FSK  (2-FSK modulation selected)</li>
 *        <li>0x10: MOD_4FSK  (4-FSK modulation selected)</li>
 *        <li>0xA0: MOD_2GFSK_BT05 (2GFSK modulation selected with BT = 0.5)</li>
 *        <li>0x20: MOD_2GFSK_BT1 (2GFSK modulation selected with BT = 1)</li>
 *        <li>0xB0: MOD_4GFSK_BT05 (G4FSK modulation selected with BT = 0.5)</li>
 *        <li>0x30: MOD_4GFSK_BT1 (G4FSK modulation selected with BT = 1)</li>
 *        <li>0x50: MOD_ASK_OOK (OOK modulation selected)</li>
 *        <li>0x60: MOD_POLAR (OOK modulation selected)</li></ul>
 */
void S2LPRadioGetModulationAction(void)
{
  uint8_t mod = (uint8_t)S2LPRadioGetModulation();
  responsePrintf("{&N API call...&tx}\r\n",
  "S2LPRadioGetModulation", "value", mod );
}

/**
 * @brief  Get the RSSI dBm value.
 *         <p><b>HowToCall</b>:  S2LPQiGetRssidBm</p>
 * @param  None
 * @retval RSSI in dBm.
 */
void S2LPQiGetRssidBmAction(void)
{
  float fRssi = S2LPRadioGetRssidBm();
  responsePrintf("{&N API call...", "S2LPQiGetRssidBm");
  printf ("{%s:%.1f}}\r\n", "value", fRssi);
}

/**
 * @brief  Set the S2-LP PA level.
 *         <p><b>HowToCall</b>:  S2LPRadioSetPALevelNdBm w u</p>
 * @param  pa_level: float indicating the dBm value
 * @param  pa_index: index of the PA register from 0 to 7.
 * @retval None
 */
void S2LPRadioSetPALevelNdBmAction(void)
{
  int32_t lPaLevelx = (int32_t)signedCommandArgument(0);
  uint8_t cLevel = (uint8_t)unsignedCommandArgument(1);
  S2LPRadioSetPALeveldBm(cLevel, lPaLevelx);
  responsePrintf("{&N API call...}\r\n", "S2LPRadioSetPALevelNdBm");
}

/**
 * @brief  Set the S2-LP PA index.
 *         <p><b>HowToCall</b>:  S2LPRadioSetPALevelMaxInd u</p>
 * @param  pa_index: index of the PA register from 0 to 7.
 * @retval None.
 */
void S2LPRadioSetPALevelMaxIndAction(void)
{
  S2LPRadioSetPALevelMaxIndex((uint8_t)unsignedCommandArgument(0));
  responsePrintf("{&N API call...}\r\n", "S2LPRadioSetPALevelMaxInd");
}

/**
 * @brief  Set the S2-LP PA power level and the passed index as maxindex.
 *         <p><b>HowToCall</b>:  S2LPRadioSetPALeveldBm w u</p>
 * @param  pa_level: float indicating the dBm value
 * @param  pa_index: index of the PA register from 0 to 7.
 * @retval None
 */
void S2LPRadioSetPALeveldBmAction(void)
{
  uint8_t cPaLevelIndex= unsignedCommandArgument(1);
  S2LPRadioSetPALevelMaxIndex(cPaLevelIndex);
  S2LPRadioSetPALeveldBm(cPaLevelIndex, signedCommandArgument(0));

  responsePrintf("{&N API call...}\r\n", "S2LPRadioSetPALeveldBm");
}

/**
 * @brief  Get the S2-LP PA level.
 *         <p><b>HowToCall</b>:  S2LPRadioGetPALeveldBm u</p>
 * @param  pa_index: index of the PA register from 0 to 7.
 * @retval pa_level: float indicating the dBm value.
 */
void S2LPRadioGetPALeveldBmAction(void)
{
  float fPaLevel = S2LPRadioGetPALeveldBm(unsignedCommandArgument(0));

  responsePrintf("{&N API call...&td}\r\n", "S2LPRadioGetPALeveldBm",
                 "value", (int32_t)(fPaLevel*10));
}

/**
 * @brief  Set the S2-LP whitening.
 *         <p><b>HowToCall</b>:  S2LPWhitening u</p>
 * @param  whitening: 1: enable , 0: disable
 * @retval None.
 */
void S2LPWhiteningAction(void)
{
  S2LPPacketHandlerWhitening((SFunctionalState)unsignedCommandArgument(0));
  responsePrintf("{&N API call...}\r\n", "S2LPWhitening");
}

/**
 * @brief  Set the S2-LP DirectRf mode in RX.
 *         <p><b>HowToCall</b>:  S2LPDirectRfSetRxMode u</p>
 * @param  rx_mode:
 *        <ul><li>0: NORMAL_RX_MODE (Normal mode, no direct reception is used)</li>
 *        <li>0x10: DIRECT_RX_FIFO_MODE (Destination is FIFO: payload bits are continuously written to the RX FIFO and not subjected to any processing)</li>
 *        <li>0x20: DIRECT_RX_GPIO_MODE (Destination is GPIO: payload bits are continuously written to one of the GPIO ports and not subjected to any processing)</li></ul>
 * @retval None.
 */
void S2LPDirectRfSetRxModeAction(void)
{
  S2LPPacketHandlerSetRxMode((DirectRx)unsignedCommandArgument(0));
  responsePrintf("{&N API call...}\r\n", "S2LPDirectRfSetRxMode");
}

/**
 * @brief  Set the S2-LP DirectRf mode in TX.
 *         <p><b>HowToCall</b>:  S2LPDirectRfSetTxMode u</p>
 * @param  tx_mode:
 *        <ul><li>0: NORMAL_TX_MODE (Normal mode, no direct reception is used)</li>
 *        <li>0x04: DIRECT_TX_FIFO_MODE (Destination is FIFO: payload bits are continuously written to the RX FIFO and not subjected to any processing)</li>
 *        <li>0x08: DIRECT_TX_GPIO_MODE (Destination is GPIO: payload bits are continuously written to one of the GPIO ports and not subjected to any processing)</li></ul>
 *        <li>0x0C: PN9_TX_MODE (A pseudorandom binary sequence is generated internally)</li></ul>
 * @retval None.
 */
void S2LPDirectRfSetTxModeAction(void)
{
  S2LPPacketHandlerSetTxMode((DirectTx)unsignedCommandArgument(0));
  responsePrintf("{&N API call...}\r\n", "S2LPDirectRfSetTxMode");
}

/**
 * @brief  Set the S2-LP GPIOs.
 *         <p><b>HowToCall</b>:  S2LPGpioInit u u u</p>
 * @param  gpio_pin:
 *        <ul><li>0: GPIO0</li>
 *        <li>1: GPIO1</li>
 *        <li>2: GPIO2</li>
 *        <li>3: GPIO3</li></ul>
 * @param gpio_mode:
 *        <ul><li>1: S2LP_GPIO_MODE_DIGITAL_INPUT (Digital Input on GPIO)</li>
 *        <li>2: S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP (Digital Output on GPIO (low current))</li>
 *        <li>3: S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP (Digital Output on GPIO (high current))</li></ul>
 * @param gpio_io function:
 *      If configured as output:
 *       <ul>
 *        <li>0x00: S2LP_GPIO_DIG_OUT_IRQ - nIRQ (Interrupt Request, active low) , default configuration after POR</li>
 *        <li>0x08: S2LP_GPIO_DIG_OUT_POR_INV - POR inverted (active low)</li>
 *        <li>0x10:  S2LP_GPIO_DIG_OUT_WUT_EXP - Wake-Up Timer expiration: "1" when WUT has expired</li>
 *        <li>0x18:  S2LP_GPIO_DIG_OUT_LBD - Low battery detection: "1" when battery is below threshold setting</li>
 *        <li>0x20:  S2LP_GPIO_DIG_OUT_TX_DATA - TX data internal clock output (TX data are sampled on the rising edge of it)</li>
 *        <li>0x28:  S2LP_GPIO_DIG_OUT_TX_STATE - TX state indication: "1" when S2LP1 is passing in the TX state</li>
 *        <li>0x30:  S2LP_GPIO_DIG_OUT_TXRX_FIFO_ALMOST_EMPTY - TX/RX FIFO Almost Empty Flag</li>
 *        <li>0x38:  S2LP_GPIO_DIG_OUT_TXRX_FIFO_ALMOST_FULL - TX/RX FIFO Almost Full Flag</li>
 *        <li>0x40:  S2LP_GPIO_DIG_OUT_RX_DATA - RX data output </li>
 *        <li>0x48:  S2LP_GPIO_DIG_OUT_RX_CLOCK - RX clock output (recovered from received data)</li>
 *        <li>0x50:  S2LP_GPIO_DIG_OUT_RX_STATE - RX state indication: "1" when demodulator is ON</li>
 *        <li>0x58:  S2LP_GPIO_DIG_OUT_NOT_STANDBY_SLEEP - VDD when the device is not in SLEEP or STANDBY</li>
 *        <li>0x60:  S2LP_GPIO_DIG_OUT_STANDBY - VDD when device is in STANDBY</li>
 *        <li>0x68:  S2LP_GPIO_DIG_OUT_ANTENNA_SWITCH - Antenna switch used for antenna diversity</li>
 *        <li>0x70:  S2LP_GPIO_DIG_OUT_VALID_PREAMBLE - Valid Preamble Detected Flag</li>
 *        <li>0x78:  S2LP_GPIO_DIG_OUT_SYNC_DETECTED - Sync WordSync Word Detected Flag</li>
 *        <li>0x80:  S2LP_GPIO_DIG_OUT_RSSI_THRESHOLD - RSSI above threshold</li>
 *        <li>0x88:  S2LP_GPIO_DIG_OUT_MCU_CLOCK - MCU Clock</li>
 *        <li>0x90:  S2LP_GPIO_DIG_OUT_TX_RX_MODE - TX or RX mode indicator (to enable an external range extender)</li>
 *        <li>0x98:  S2LP_GPIO_DIG_OUT_VDD - VDD (to emulate an additional GPIO of the MCU, programmable by SPI)</li>
 *        <li>0xA0:  S2LP_GPIO_DIG_OUT_GND - GND (to emulate an additional GPIO of the MCU, programmable by SPI)</li>
 *        <li>0xA8:  S2LP_GPIO_DIG_OUT_SMPS_EXT - External SMPS enable signal (active high)</li>
 *        <li>0xB0:  S2LP_GPIO_DIG_OUT_SLEEP - Device in SLEEP (active high)</li>
 *        <li>0xB8:  S2LP_GPIO_DIG_OUT_READY - Device in READY (active high)</li>
 *        <li>0xC0:  S2LP_GPIO_DIG_OUT_LOCK - Device in LOCK (active high)</li>
 *        <li>0xC8:  S2LP_GPIO_DIG_OUT_WAIT_FOR_LOCK_SIG - Device waiting for LOCK (active high)</li>
 *        <li>0xD0:  S2LP_GPIO_DIG_OUT_TX_DATA_OOK_SIGNAL </li>
 *        <li>0xD8:  S2LP_GPIO_DIG_OUT_WAIT_FOR_READY2_SIG</li>
 *        <li>0xE0:  S2LP_GPIO_DIG_OUT_WAIT_FOR_TIMER_FOR_PM_SET</li>
 *        <li>0xE8:  S2LP_GPIO_DIG_OUT_WAIT_VCO_CALIBRATION</li>
 *        <li>0xF0:  S2LP_GPIO_DIG_OUT_ENABLE_SYNTH_FULL_CIRCUIT</li>
 *        <li>0xF8:  S2LP_GPIO_DIG_OUT_WAIT_FOR_RCCAL_OK_SIG</li>
 *      </ul>
 *  If configured as input:
 * <ul>
 *        <li>0x00:  S2LP_GPIO_DIG_IN_TX_COMMAND</li>
 *        <li>0x08:  S2LP_GPIO_DIG_IN_RX_COMMAND</li>
 *        <li>0x10:  S2LP_GPIO_DIG_IN_TX_DATA_INPUT_FOR_DIRECTRF</li>
 *        <li>0x18:  S2LP_GPIO_DIG_IN_DATA_WAKEUP</li>
 *        <li>0x20:  S2LP_GPIO_DIG_IN_EXT_CLOCK_AT_34_7KHZ</li>
 * </ul>
 * @retval None.
 */
void S2LPGpioInitAction(void)
{
  SGpioInit xGpio;
  xGpio.xS2LPGpioPin = (S2LPGpioPin)(unsignedCommandArgument(0));
  xGpio.xS2LPGpioMode = (S2LPGpioMode)(unsignedCommandArgument(1));
  xGpio.xS2LPGpioIO = (S2LPGpioIO)(unsignedCommandArgument(2));
  S2LPGpioInit(&xGpio);

  responsePrintf("{&N API call...}\r\n", "S2LPGpioInit" );
}

/**
 * @brief  Set the S2-LP GPIOs.
 *         <p><b>HowToCall</b>:  S2LPIrq w u</p>
 * @param irq code:
 *    <ul>
 *       <li>0x00000001: RX_DATA_READY - IRQ: RX data ready</li>
 *       <li>0x00000002: RX_DATA_DISC - RX data discarded (upon filtering)</li>
 *       <li>0x00000004: TX_DATA_SENT - TX data sent</li>
 *       <li>0x00000008:  MAX_RE_TX_REACH - Max re-TX reached</li>
 *       <li>0x00000010  CRC_ERROR - CRC error</li>
 *       <li>0x00000020  TX_FIFO_ERROR - TX FIFO underflow/overflow error</li>
 *       <li>0x00000040  RX_FIFO_ERROR - RX FIFO underflow/overflow error</li>
 *       <li>0x00000080  TX_FIFO_ALMOST_FULL - TX FIFO almost full</li>
 *       <li>0x00000100  TX_FIFO_ALMOST_EMPTY - TX FIFO almost empty</li>
 *       <li>0x00000200  RX_FIFO_ALMOST_FULL - RX FIFO almost full</li>
 *       <li>0x00000400  RX_FIFO_ALMOST_EMPTY - RX FIFO almost empty</li>
 *       <li>0x00000800  MAX_BO_CCA_REACH - Max number of back-off during CCA</li>
 *       <li>0x00001000  VALID_PREAMBLE - Valid preamble detected</li>
 *       <li>0x00002000  VALID_SYNC - Sync word detected</li>
 *       <li>0x00004000  RSSI_ABOVE_TH - RSSI above threshold</li>
 *       <li>0x00008000  WKUP_TOUT_LDC - Wake-up timeout in LDC mode</li>
 *       <li>0x00010000  READY - READY state</li>
 *       <li>0x00020000  STANDBY_DELAYED - STANDBY state after MCU_CK_CONF_CLOCK_TAIL_X clock cycles</li>
 *       <li>0x00040000  LOW_BATT_LVL - Battery level below threshold</li>
 *       <li>0x00080000  POR - Power On Reset</li>
 *       <li>0x00100000  BOR - IRQ: Brown out event (both accurate and inaccurate)</li>
 *       <li>0x00200000  LOCK - IRQ: LOCK state</li>
 *       <li>0x00400000  VCO_CALIBRATION_END - only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5)</li>
 *       <li>0x00800000  PA_CALIBRATION_END - only for debug; Crystal oscillator settling time counter expired</li>
 *       <li>0x01000000  PM_COUNT_EXPIRED - only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5)</li>
 *       <li>0x02000000  XO_COUNT_EXPIRED - only for debug; Crystal oscillator settling time counter expired </li>
 *       <li>0x04000000  TX_START_TIME - only for debug; TX circuitry startup time; see TX_START_COUNTER</li>
 *       <li>0x08000000  RX_START_TIME - only for debug; RX circuitry startup time; see TX_START_COUNTER</li>
 *       <li>0x10000000  RX_TIMEOUT - RX operation timeout</li>
 *       <li>0x20000000  RX_SNIFF_TIMEOUT - IRQ: RX sniff operation timeout</li>
 *       <li>0x7FFFFFFF  ALL_IRQ - All the above mentioned IRQs</li>
 *      </ul>
 * @param enable (1), disable (0)
 * @retval None.
 */
void S2LPIrqAction(void)
{
  IrqList xIrq;
  SFunctionalState xState;
  xIrq = (IrqList)unsignedCommandArgument(0);
  xState = (SFunctionalState)unsignedCommandArgument(1);
  S2LPGpioIrqConfig(xIrq, xState);

  responsePrintf("{&N API call...}\r\n", "S2LPIrq" );
}

/**
 * @brief  Get the S2-LP IRQ status (interrupt raised from the last IRQ_STATUS registers reading).
 *         <p><b>HowToCall</b>:  S2LPIrqGetStatus</p>
 * @param  None.
 * @retval IRQ status registers in a 32bits word.
 */
void S2LPIrqGetStatusAction(void)
{
  S2LPIrqs xIrqStatus;
  S2LPGpioIrqGetStatus(&xIrqStatus);
  /*Modified in order to avoid a warning in STM32CubeIDE when -Os is enabled */
  /*uint32_t nIrqStatus = *((uint32_t *)&xIrqStatus);*/
  uint32_t nIrqStatus;
  memcpy(&nIrqStatus, &xIrqStatus, sizeof(nIrqStatus));

  responsePrintf("{&N API call...&t4x}\r\n",
  "S2LPIrqGetStatus", "value", nIrqStatus );
}

/**
 * @brief  Set the RX Almost Full threshold.
 *         <p><b>HowToCall</b>:  S2LPLinearFifoSetAlmostFullThresholdRx u</p>
 * @param  ae_thr FIFO almost Empty threshold.
 * @retval None.
 */
void S2LPLinearFifoSetAlmostFullThresholdRxAction(void)
{
  S2LPFifoSetAlmostFullThresholdRx(unsignedCommandArgument(0));
  responsePrintf("{&N API call...}\r\n", "S2LPLinearFifoSetAlmostFullThresholdRx");
}

/**
 * @brief  Set the TX Almost Empty threshold.
 *         <p><b>HowToCall</b>:  S2LPLinearFifoSetAlmostEmptyThresholdTx u</p>
 * @param  ae_thr FIFO almost Empty threshold.
 * @retval None.
 */
void S2LPLinearFifoSetAlmostEmptyThresholdTxAction(void)
{
  S2LPFifoSetAlmostEmptyThresholdTx(unsignedCommandArgument(0));
  responsePrintf("{&N API call...}\r\n", "S2LPLinearFifoSetAlmostEmptyThresholdTx");
}

/**
 * @brief  Get the S2-LP version (silicon cut and ver).
 *         <p><b>HowToCall</b>:  S2LPGetVersion</p>
 * @param  None.
 * @retval 16 bit word done by the 2 bytes <i>PN</i> and <i>VER</i>: 0x(PN)(VER).
 */
void S2LPGetVersionAction(void)
{
  uint16_t nVersion;

  nVersion = ((uint16_t)S2LPGeneralGetDevicePN())<<8;
  nVersion |= S2LPGeneralGetVersion();

  responsePrintf("{&N API call...&t2x}\r\n",
         "S2LPGetVersion", "value", nVersion );
}

/**
 * @brief  Get the S2-LP library version.
 *         <p><b>HowToCall</b>:  S2LPGetLibVersion</p>
 * @param  None.
 * @retval library version string.
 */
void S2LPGetLibVersionAction(void)
{
  char libVer[30] = S2LPGeneralLibraryVersion();
  char tmp[2]={0,'\0'};
  uint32_t lVer=0;
  uint8_t k=2;

  for(uint8_t i=0;libVer[i]!='\0';i++)
    if(libVer[i]=='.') {
      tmp[0]=libVer[i+1];
      lVer|=((uint32_t)atoi(tmp)<<(8*k));
      k--;
    }

  responsePrintf("{&N API call...&t4x}\r\n",
  "S2LPGetLibVersion", "value", lVer );
}

/**
 * @brief  This will cause the low power STANDBY mode to be used in the SendNbytesBatch routines.
 *         <p><b>HowToCall</b>:  S2LPSendBatchLP u</p>
 * @param  en: enable(1), disable(0).
 * @retval None.
 */
void S2LPSendBatchLPAction(void)
{
  SendBatchLP = unsignedCommandArgument(0);
  responsePrintf("{&N API call...}\r\n", "S2LPSendBatchLP" );
}

/**
 * @brief  This will cause the low power STANDBY mode to be used in the GetNbytesBatch routines.
 *         <p><b>HowToCall</b>:  S2LPSendBatchLP u</p>
 * @param  en: enable(1), disable(0).
 * @retval None.
 */
void S2LPGetBatchLPAction(void)
{
  GetBatchLP = unsignedCommandArgument(0);
  responsePrintf("{&N API call...}\r\n", "S2LPGetBatchLP" );
}

/**
 * @brief  Wait for a S2-LP IRQ raised and returns the IRQ_STATUS when it is raised.
 *              If other interrupts are in the IRQ queue it pops and returns the last one.
 *         <p><b>HowToCall</b>:  SdkEvalIrqHandler</p>
 * @param  bloking if 1 it is blocked until the interrupt is raised (or stop cmd comes), if 0 it is not blocking.
 * @retval irq_status registers in a 32bits word.
 * @retval cli_timer.
 */
void IrqHandlerAction(void)
{
  uint8_t blocking=unsignedCommandArgument(0);

  if(blocking)  {

    WHILE(!xIRQRaised);
    if(!xIRQRaised)    {
      stopCmdFlag=0;
      xIRQRaised=RESET;
      S2LPSpiCommandStrobes(CMD_SABORT);
      S2LPSpiCommandStrobes(CMD_FLUSHRXFIFO);
      responsePrintf("{&N API call...}\r\n","StopCmd");
      return;
    }

    S2LPFillIRQQueue();
    xIRQRaised=RESET;

    responsePrintf("{&N API call...&t4x &t4x}\r\n","SdkEvalIrqHandler",
                   "irq_status",BufferToUint32(s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status),
                   "timer",s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].timer);

    if(s_xIrqQueue.cIrqQueueSize!=0)    {
      s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
      s_xIrqQueue.cIrqQueueSize--;
    }
  }
  else {
    if(xIRQRaised) {
      S2LPFillIRQQueue();
      xIRQRaised=RESET;
    }
    if(s_xIrqQueue.cIrqQueueSize!=0) {
      responsePrintf("{&N API call...&t4x &t4x &t4x}\r\n","SdkEvalIrqHandler",
               "irq_status",BufferToUint32(s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status),
               "timer",s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].timer,
               "dimension",s_xIrqQueue.cIrqQueueSize);

      if(s_xIrqQueue.cIrqQueueSize!=0){
        s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
        s_xIrqQueue.cIrqQueueSize--;
      }
    }
    else {
      responsePrintf("{&N API call...&t4x &t4x &t4x}\r\n","SdkEvalIrqHandler",
               "irq_status",0x00000000,
               "timer",SdkEvalGetTimerValue(),
               "dimension",s_xIrqQueue.cIrqQueueSize);
    }
  }
}

/**
 * @brief  Set the wake up timer.
 *         <p><b>HowToCall</b>:  S2LPTimerSetWakeUpTimerUs w</p>
 * @param  wake_up timer in us.
 * @retval None.
 */
void S2LPTimerSetWakeUpTimerUsAction(void)
{
  uint32_t wake_up_timer = unsignedCommandArgument(0);

  S2LPTimerSetWakeUpTimerUs(wake_up_timer);

  responsePrintf("{&N API call...}\r\n", "S2LPTimerSetWakeUpTimerUs");
}

/**
 * @brief  Enable the LDCR mode.
 *         <p><b>HowToCall</b>:  S2LPTimerLdcrMode u u</p>
 * @param  en enable (1), disable (0).
 * @param  en_autoreload enable autoreload (1), disable autoreload (0)
 * @retval None.
 */
void S2LPTimerLdcrModeAction(void)
{
  S2LPTimerLdcrMode((SFunctionalState)unsignedCommandArgument(0));
  S2LPTimerLdcrAutoReload((SFunctionalState)unsignedCommandArgument(1));

  responsePrintf("{&N API call...}\r\n", "S2LPTimerLdcrMode");
}

static uint8_t S2LPSendNBytes(uint8_t *vectcTxBuff, uint16_t cNbBytes)
{
  FlagStatus xAE=RESET;
  uint16_t nTxIndex;
  uint8_t cInFifoData, max_fifo=128;

  while(s_xIrqQueue.cIrqQueueSize>0){
    s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
    s_xIrqQueue.cIrqQueueSize--;
  }

  if((S2LPGeneralGetVersion()&0x80)==0)
    max_fifo=96;

  if(cNbBytes>max_fifo)
  {
    SET_IRQ_MASK(TX_FIFO_ALMOST_EMPTY, S_ENABLE);
    S2LPFifoMuxRxFifoIrqEnable(S_DISABLE);
    S2LPSpiWriteFifo(max_fifo, vectcTxBuff);
    nTxIndex=max_fifo;
  }
  else
  {
    SET_IRQ_MASK(TX_FIFO_ALMOST_EMPTY, S_DISABLE);
    S2LPFifoMuxRxFifoIrqEnable(S_DISABLE);
    S2LPSpiWriteFifo(cNbBytes, vectcTxBuff);
    nTxIndex=cNbBytes;
  }

  S2LPSpiReadRegisters(0x3F, 1, &cInFifoData);
  cInFifoData &= 0x7F;
  cInFifoData--;

  if(SendBatchLP==1)
  {
    /* if low power is enabled a ready will trigger TX because
      we had sent TX pending */
    CMD_STROBE_READY();
  }
  else
  {
    CMD_STROBE_TX();
  }

  while (1){
    do{
      WHILE(!xIRQRaised);
      if(stopCmdFlag)
      {
        stopCmdFlag=0;
        responsePrintf("{&N API call...}\r\n","StopCmd");
        return 0xff;
      }
      xIRQRaised=RESET;

      S2LPFillIRQQueue();

      if(((uint8_t)s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status[2]) & 0x01)
        xAE=SET;

      if(((uint8_t)s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status[3]) & 0x06)      //tx done
      {
        s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
        s_xIrqQueue.cIrqQueueSize--;

        if(SendBatchLP!=3)
        {
          S2LPSpiCommandStrobes(CMD_FLUSHTXFIFO);
        }
        return 0;
      }

      s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
      s_xIrqQueue.cIrqQueueSize--;

    }while(!xAE);
    xAE=RESET;

    if((cNbBytes-nTxIndex+1)+cInFifoData<max_fifo)
    {
      SET_IRQ_MASK(TX_FIFO_ALMOST_EMPTY , S_DISABLE);
      S2LPSpiWriteFifo(cNbBytes-nTxIndex+1, &vectcTxBuff[nTxIndex]);
      nTxIndex=cNbBytes;
    }
    else
    {
      S2LPSpiWriteFifo(max_fifo-cInFifoData, &vectcTxBuff[nTxIndex]);
      nTxIndex+=max_fifo-cInFifoData;
    }
  }
}

static uint8_t DataDiscardedReason(uint8_t *irq_mask)
{
  /* Get the destination address of the packet */
  uint8_t rxDestAddress = GET_RX_DEST_ADDR();
  uint8_t disc_vs_addr=0,filtMask,crcMode;

  S2LPSpiReadRegisters(0x40, 1, &filtMask);
  S2LPSpiReadRegisters(0x30, 1, &crcMode); crcMode&=0xe0;

  if(irq_mask[0]&0x10)// RX TIMEOUT
  {
    return 0x01;
  }

  /* Check if the packet has been discarded for CRC automatic filtering */
  if(((uint8_t)crcMode!=0) && (irq_mask[3]&0x10))
    return 0x02;

  /* Check if the packet has been discarded for my address automatic filtering */
  if((filtMask & 0x08) && (GET_MY_ADDR() != rxDestAddress))
    disc_vs_addr++;
  /* Check if the packet has been discarded for multicast address automatic filtering */
  if((filtMask & 0x04) && (GET_RX_MULTICAST_ADDR() != rxDestAddress))
    disc_vs_addr++;
  /* Check if the packet has been discarded for broadcast address automatic filtering */
  if((filtMask & 0x02) && (GET_RX_BROADCAST_ADDR() != rxDestAddress))
    disc_vs_addr++;

  if(disc_vs_addr!=0 && disc_vs_addr==((filtMask & 0x02)==0x02)+((filtMask & 0x04)==0x04)+((filtMask & 0x08)==0x08))
    return 0x03;

  /* Check if the packet has been discarded for address source automatic filtering */
  if(filtMask & 0x10)
  {
    uint8_t cSourceMask = GET_RX_SOURCE_MASK();
    if((GET_RX_SOURCE_ADDR() & cSourceMask) != (GET_RX_SOURCE_REF() & cSourceMask))
      return 0x04;
  }
  if(irq_mask[0]&0x20)
    return 0x06;

  return 0x00;
}

static uint8_t S2LPGetNBytes(uint8_t* vectcRxBuff, uint16_t* pnNBytesRec , uint16_t nNBytesToRec, uint32_t onSyncTimerMs)
{
  FlagStatus xAF=RESET;
  uint16_t nNBytesRec=0;
  uint8_t cRxData=0;

  while(s_xIrqQueue.cIrqQueueSize>0){
    s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
    s_xIrqQueue.cIrqQueueSize--;
  }

  SET_IRQ_MASK(RX_FIFO_ALMOST_FULL , S_ENABLE);
  S2LPFifoMuxRxFifoIrqEnable(S_ENABLE);

  if(GetBatchLP==1) {
    CMD_STROBE_READY(); /* If low power is enabled a ready will trigger RX because we had sent RX pending */
  } else if(GetBatchLP==0) {
    CMD_STROBE_RX();
  }

  while (1){
    do{
      WHILE(!xIRQRaised);
      if(stopCmdFlag) {
        stopCmdFlag=0;

        if(GetBatchLP!=3) {
          S2LPSpiCommandStrobes(CMD_SABORT);
          S2LPSpiCommandStrobes(CMD_FLUSHRXFIFO);
        }
        responsePrintf("{&N API call...}\r\n","StopCmd");

        return 0xff;
      }
      xIRQRaised=RESET;

      if(GetBatchLP==2 && S2LPManagementGetCut()==S2LP_CUT_2_0) {
        S2LPTimerLdcIrqWa(S_ENABLE);
      }

      S2LPFillIRQQueue();

      //af FIFO almost full
      if(((uint8_t)s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status[2]) & 0x02)
        xAF=SET;

      if(nNBytesToRec==0xffff) {
        //sync
        if(((uint8_t)s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status[2]) & 0x20)
        {
          SdkEvalTimersTimConfig_ms(APPL_TIMER_ID, onSyncTimerMs);
          SdkEvalTimersState(APPL_TIMER_ID, NULL, ENABLE);
          xApplTimerIRQRaised=0;
        }
        //data ready
        if(((uint8_t)s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status[3]) & 0x01)
	  {
          s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
          s_xIrqQueue.cIrqQueueSize--;

          SET_IRQ_MASK(RX_FIFO_ALMOST_FULL, S_DISABLE);
          cRxData = READ_NUM_ELEMENTS_FIFO();

	    if(cRxData>0){
            S2LPSpiReadFifo(cRxData, &vectcRxBuff[nNBytesRec]);
          }

          nNBytesRec+=cRxData;

          (*pnNBytesRec)=nNBytesRec;
          SdkEvalUpdateTimerValue();

          if(GetBatchLP==2 || GetBatchLP==3) {
            S2LPSpiReadRegisters(RSSI_LEVEL_ADDR, 1, &vectcRxBuff[nNBytesRec]);
            S2LPCmdStrobeSleep();

            if(S2LPManagementGetCut()==S2LP_CUT_2_0)
            {
              HAL_Delay(10);
              S2LPTimerLdcIrqWa(S_DISABLE);
            }
          }
          return 0;
        }
        //data disc
        if((GetBatchLP!=2 && ((uint8_t)s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status[3] & 0x02)) ||
           ((GetBatchLP==2) &&
            (((uint8_t)s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status[3] & 0x10)||
            ((uint8_t)s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status[0] & 0x10))))
        {
          SdkEvalUpdateTimerValue();
          SET_IRQ_MASK(RX_FIFO_ALMOST_FULL , S_DISABLE);
          S2LPSpiCommandStrobes(CMD_FLUSHRXFIFO);

	    uint8_t tempRegValue[4];
	    S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tempRegValue);
	    (*pnNBytesRec)=nNBytesRec;

	    if(GetBatchLP==2 || GetBatchLP==3) {
            S2LPCmdStrobeSleep();
            if(S2LPManagementGetCut()==S2LP_CUT_2_0)
            {
              SdkDelayMs(10);
              S2LPTimerLdcIrqWa(S_DISABLE);
            }
          }

          return DataDiscardedReason(s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status);
        }
      }

      s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
      s_xIrqQueue.cIrqQueueSize--;

    }while(!xAF);
    xAF=RESET;

    cRxData = READ_NUM_ELEMENTS_FIFO();

    if(nNBytesRec+cRxData>GP_BUFFER_SIZE)
    {
      S2LPSpiCommandStrobes(CMD_SABORT);
      while(s_xIrqQueue.cIrqQueueSize>0){
        s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
        s_xIrqQueue.cIrqQueueSize--;
      }
      SdkEvalUpdateTimerValue();
      S2LPSpiCommandStrobes(CMD_FLUSHRXFIFO);
      uint8_t tempRegValue[4];
	S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tempRegValue);
      (*pnNBytesRec)=nNBytesRec;

	return 7;
    }

    if(cRxData-1>0){
      S2LPSpiReadFifo(cRxData-1, &vectcRxBuff[nNBytesRec]);
    }

    nNBytesRec+=(cRxData-1);

    if(nNBytesRec>=nNBytesToRec)
    {
      S2LPSpiCommandStrobes(CMD_SABORT);
      while(s_xIrqQueue.cIrqQueueSize>0){
        s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
        s_xIrqQueue.cIrqQueueSize--;
      }

      SdkEvalUpdateTimerValue();
      S2LPSpiCommandStrobes(CMD_FLUSHRXFIFO);
      uint8_t tempRegValue[4];
	S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tempRegValue);
      (*pnNBytesRec)=nNBytesRec;

	return 0;
    }
  }
}

/**
 * @brief  Send N-bytes according to the configured FIFO TX mode (direct, packet...).
 *         <p><b>HowToCall</b>:  S2LPSendNBytes b</p>
 * @param  buffer data to be send through FIFO.
 * @retval None.
 */
void S2LPSendNBytesHandler(void)
{
  uint16_t cNbBytes = copyStringArgument(0, vectcBuff, 255, 0);

  S2LPSendNBytes(vectcBuff,cNbBytes);
  responsePrintf("{&N API call...}\r\n", "S2LPSendNBytes");
}

/**
 * @brief  Send N-bytes in batch mode according to the configured FIFO TX mode (direct, packet...).
 *         <p><b>HowToCall</b>:  S2LPSendNBytesBatch w w b</p>
 * @param  ref_timer data (the buffer that is passed as argument) will be sent every ref_timer ms.
 * @param  num_packets the number of times the packets must be sent.
 * @param  buffer data to be send through FIFO.
 * @details Each time a single packet is sent, the routine will answer with a <i>S2LPSendNBytes</i> notification.
 *              The routine can be stopped before its end with a <i>stopCmd</i>.
 * @retval None.
 */
void S2LPSendNBytesBatchHandler(void)
{
  uint32_t lRefTimer=unsignedCommandArgument(0);
  uint32_t lNumPackets=unsignedCommandArgument(1);
  uint16_t cNbBytes = copyStringArgument(2, vectcBuff, 255, 0);
  uint8_t error=0;

  /* Added for STM32CubeMX porting: currently the S2LP_WriteFIFO implementation uses same buffer for i/o.
   * If looping over the same data, actual tx buffer will be different.
   */
  uint8_t locbuff[255]; //Validate dimension.

  xIRQRaised=RESET;
  if(SendBatchLP==3)
  {
    while(s_xIrqQueue.cIrqQueueSize>0){
      s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
      s_xIrqQueue.cIrqQueueSize--;
    }

    uint8_t tmp[4];
    S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tmp);

    /* Set the wakeup timer */
    S2LPTimerSetWakeUpTimerUs(1000*lRefTimer);

    S2LPTimerLdcrAutoReload(S_DISABLE);

    /* LDC mode enabled */
    S2LPTimerLdcrMode(S_ENABLE);

    memcpy(locbuff, vectcBuff, cNbBytes); /* Added for STM32CubeMX */
	S2LPSpiWriteFifo(cNbBytes, locbuff);
//	S2LPSpiWriteFifo(cNbBytes, vectcBuff);

    CMD_STROBE_TX();
  }
  else
  {
    SdkEvalTimersTimConfig_ms(APPL_TIMER_ID, lRefTimer);
    SdkEvalTimersState(APPL_TIMER_ID, NULL, ENABLE);
  }

  for(uint32_t i=0;i<lNumPackets;i++)
  {
    memcpy(locbuff, vectcBuff, cNbBytes); /* Added for STM32CubeMX */

    if(SendBatchLP==3)
    {
      WHILE(!xIRQRaised);
      if(stopCmdFlag)
      {
        uint8_t tmp[4];
        stopCmdFlag=0;
        responsePrintf("{&N API call...}\r\n","StopCmd");
        error=0xff;

	  /* LDC mode enabled */
        S2LPTimerLdcrMode(S_DISABLE);

        CMD_STROBE_READY();
        CMD_STROBE_SABORT();

        S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tmp);

        S2LPSpiCommandStrobes(CMD_FLUSHTXFIFO);

        break;
      }
      xIRQRaised=RESET;

      S2LPFillIRQQueue();

      if(((uint8_t)s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPop].irq_status[3]) & 0x06)      //tx done
      {
        s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
        s_xIrqQueue.cIrqQueueSize--;

	  S2LPSpiWriteFifo(cNbBytes, locbuff); /* Uses locbuff for STM32CubeMX */
	  //S2LPSpiWriteFifo(cNbBytes, vectcBuff);
	  CMD_STROBE_SLEEP();
      }
    }
    else
    {
      if(SendBatchLP==1)
      {
        /* If low power is enabled we can sleep and send the TX in pending */
        CMD_STROBE_STANDBY();
        CMD_STROBE_TX();
        /* at this point, if the radio will be woken up with READY it automatically
        goes in TX */
      }
      WHILE(!xApplTimerIRQRaised);
      if(stopCmdFlag)
      {
        stopCmdFlag=0;

        SdkEvalTimersState(APPL_TIMER_ID, NULL, DISABLE);
        if(SendBatchLP==1)
        {
          /* Goes in READY state */
          CMD_STROBE_READY();
          do{
            /* Delay for state transition */
            for(volatile uint8_t i=0; i!=0xFF; i++);

            /* Reads the MC_STATUS register */
            REFRESH_STATUS();
          }while(g_xStatus.MC_STATE!=MC_STATE_READY);
        }
        responsePrintf("{&N API call...}\r\n","StopCmd");
        return;
      }
      xApplTimerIRQRaised=0;
      error=S2LPSendNBytes(locbuff,cNbBytes); /* Uses locbuff for STM32CubeMX */
      //error=S2LPSendNBytes(vectcBuff,cNbBytes);
      if(error==0xff)
        break;
    }
    responsePrintf("{&N API call...}\r\n","S2LPSendNBytes");
  }
  SdkEvalTimersState(APPL_TIMER_ID, NULL, DISABLE);

  if(SendBatchLP==3)
  {
    uint8_t tmp[4];
    S2LPTimerLdcrMode(S_DISABLE);

    CMD_STROBE_READY();

    CMD_STROBE_SABORT();

    S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tmp);

    CMD_STROBE_READY();
  }

  if(error!=0xff)
    responsePrintf("{&N API call...}\r\n","S2LPSendNBytesBatch");
}

/**
 * @brief  Get N-bytes routine should report all the packet data.
 *         <p><b>HowToCall</b>:  S2LPGetNBytesReportAll</p>
 *      So it will report not only RSSI and received buffer but also:
 *     <ul>
 *       <li>seq_num: received packet sequence number</li>
 *       <li>nack_rx: received NO_ACK bit</li>
 *       <li>source_addr: received source address</li>
 *       <li>dest_addr: received destination address</li>
 *       <li>agc_word: received agc_word</li>
 *       <li>crc: received crc word</li>
 *       <li>packet_len: received packet length</li>
 *     </ul>
 * @param  en enable(1), disable(0).
 * @retval None.
 */
void S2LPGetNBytesRepAllAction(void)
{
  getNBytesReportAll=unsignedCommandArgument(0);
  responsePrintf("{&N API call...}\r\n","S2LPGetNBytesReportAll");
}

/**
 * @brief  Get N-bytes batch routine. It will receive the desired number of packets.
 *         <p><b>HowToCall</b>:  S2LPGetNBytesBatch w w</p>
 * @param  ref_timer data (the buffer that is passed as argument) will be sent every ref_timer ms.
 * @param  num_packets the number of times the packets must be sent.
 * @retval  None As usual the end of the routine will be notified but for each received packet, notifications will be reported. See details.
 * @details Each time a single packet is received, the routine will answer with a <i>S2LPGetNBytes</i> notification.
 *          The single notification will contain the following return parameters:
 *           <ul>
 *              <li><b>error</b>: error code. It can be:
 *                  <ul>
 *                    <li><b>0</b>: no error</li>
 *                    <li><b>1</b>: RX timeout</li>
 *                    <li><b>2</b>: CRC does not match</li>
 *                    <li><b>3</b>: destination address mismatch</li>
 *                    <li><b>4</b>: source address mismatch</li>
 *                  </ul>
 *              </li>
 *              <li><b>rssi</b>: RSSI register (not dBm!) value</li>
 *              <li><b>bytes</b>: received bytes in FIFO</li>
 *              <li>if the @ref S2LPGetNBytesRepAllAction has been enabled some other parameters will be added.
 *                     See the @ref S2LPGetNBytesRepAllAction doc for details</li>.
 *           </ul>
 *           The routine can be stopped before its end with a <i>stopCmd</i>.
 *
 *           <h3>How it is implemented</h3>
 *           When called, the routine will stay in indefinite RX until the first packet is received.
 *              FIFO RX AF is used to grab the bytes from the RX FIFO.
 *              From there on, it will stay in READY (or STANDBY if the @ref S2LPGetBatchLPAction has been enabled) until the expiration
 *               of the ref_timer timer (implemented in SW by the uC).
 *              To avoid drift due to the XTAL mismatch, the timer of the uC is reloaded at the SYNC detected interrupt with the value:
 *                      <i>ref_timer-SYNC_PREAMBLE_time-4ms(guard_time)</i>.
 * @retval None.
 */
void S2LPGetNBytesBatchHandler(void)
{
  uint8_t error=0;
  uint16_t nNBytesRec;
  uint32_t lRefTimer=unsignedCommandArgument(0);
  uint32_t lNumPackets=unsignedCommandArgument(1);
  uint8_t cRxTimeoutC;
  uint32_t lOnSyncTimerMs=0;
  float msUntilSync;

  xIRQRaised=RESET;
  while(s_xIrqQueue.cIrqQueueSize>0){
    s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
    s_xIrqQueue.cIrqQueueSize--;
  }

  if(GetBatchLP==3)
  {
    uint8_t m, nS2LPRefTimerP, nS2LPRefTimerC;
    uint8_t nOnSyncTimerP,nOnSyncTimerC;

    S2LPSpiReadRegisters(TIMERS5_ADDR, 1, &cRxTimeoutC);

    SET_INFINITE_RX_TIMEOUT();
    msUntilSync = S2LPGetMsUntilSync();

    lOnSyncTimerMs=(uint32_t)((float)lRefTimer-msUntilSync-8.0f);

    S2LPTimerSetWakeUpTimerUs(lRefTimer*1000);

    /* Get the counter and the prescaler of the WU timer */
    S2LPTimerGetWakeUpTimerUs(&lRefTimer,&nS2LPRefTimerC,&nS2LPRefTimerP,&m);

    nOnSyncTimerP=nS2LPRefTimerP;
    nOnSyncTimerC=(uint8_t)(lOnSyncTimerMs*(float)S2LPTimerGetRcoFrequency()/(1<<m)/1000/(nOnSyncTimerP+1))-1;

    /* Set the Wake up reload timer */
    S2LPTimerSetWakeUpTimerReload(nOnSyncTimerC,nOnSyncTimerP,m);

    S2LPTimerLdcrAutoReload(S_ENABLE);

    /* Enable the LDC with autoreload */
    S2LPTimerLdcrMode(S_ENABLE);
  }
  else if(lRefTimer)
  {
    S2LPSpiReadRegisters(TIMERS5_ADDR, 1, &cRxTimeoutC);
    SET_INFINITE_RX_TIMEOUT();
    msUntilSync = S2LPGetMsUntilSync();

    lOnSyncTimerMs=(uint32_t)((float)lRefTimer-msUntilSync-4.0f);

    SET_IRQ_MASK(VALID_SYNC, S_ENABLE);
    SdkEvalTimersTimConfig_ms(APPL_TIMER_ID, lOnSyncTimerMs);
    SdkEvalTimersState(APPL_TIMER_ID, NULL, ENABLE);
  }

  if(GetBatchLP==1)
  {
    /* If low power is enabled we can sleep and send the RX in pending */
    CMD_STROBE_STANDBY();
    CMD_STROBE_RX();
    /* At this point, if the radio will be woken up with READY it automatically
    goes in RX */
  }
  if(GetBatchLP==2 || GetBatchLP==3)
  {
    lRefTimer=0;
    CMD_STROBE_RX();
  }

  for(uint32_t i=0;i<lNumPackets;i++)
  {
    if(lRefTimer && i!=0)
    {
      WHILE(!xApplTimerIRQRaised);
      if(stopCmdFlag)
      {
        stopCmdFlag=0;
        SdkEvalTimersState(APPL_TIMER_ID, NULL, DISABLE);
        SdkEvalTimersResetCounter(APPL_TIMER_ID);
        SET_IRQ_MASK(VALID_SYNC , S_DISABLE);

        if(GetBatchLP==1 || GetBatchLP==2)
        {
          /* Goes in READY state */
          CMD_STROBE_READY();
          do{
            /* Delay for state transition */
            for(volatile uint8_t i=0; i!=0xFF; i++);

            /* Reads the MC_STATUS register */
            REFRESH_STATUS();
          }while(g_xStatus.MC_STATE!=MC_STATE_READY);
        }
        S2LPSpiCommandStrobes(CMD_SABORT);
        S2LPSpiCommandStrobes(CMD_FLUSHRXFIFO);
        responsePrintf("{&N API call...}\r\n","StopCmd");
        return;
      }
      xApplTimerIRQRaised=0;
      SdkEvalTimersTimConfig_ms(APPL_TIMER_ID, lRefTimer);
      SdkEvalTimersState(APPL_TIMER_ID, NULL, ENABLE);
    }

    error=S2LPGetNBytes(vectcBuff, &nNBytesRec , 0xffff, lOnSyncTimerMs);

    if(error!=0xff)       /* If stopped we should no respond to the SW because 0xff means STOP_CMD received, with the corresponding macro responding */
    {
      /* Restore the rx timeout set by user */
      if((lRefTimer || GetBatchLP==3) && i==0) S2LPSpiWriteRegisters(TIMERS5_ADDR, 1, &cRxTimeoutC);

      if(getNBytesReportAll && lRefTimer==0 && i!=lNumPackets-1)
      {
        CMD_STROBE_RX();
      }

      responsePrintf("{&N API call...\r\n", "S2LPGetNBytes");
      uint8_t rssi;
      if(GetBatchLP==2 || GetBatchLP==3)
      {
        rssi=vectcBuff[nNBytesRec];
      }
      else
      {
        S2LPSpiReadRegisters(RSSI_LEVEL_ADDR, 1, &rssi);
      }
      responsePrintf("&tx\r\n","error",error);
      if(!error)
      {
        responsePrintf("{%t: ","bytes");

        for(uint16_t i = 0; i < nNBytesRec-1; i++)
        {
          responsePrintf("0x%x,", vectcBuff[i]);
        }
        responsePrintf("0x%x", vectcBuff[nNBytesRec-1]);
        responsePrintf("}\r\n");
      }
      responsePrintf("&tx\r\n","rssi",rssi);

	if(getNBytesReportAll)
	{
	  uint8_t tmp[16];

	  S2LPSpiReadRegisters(0x9C,16,tmp);

	  responsePrintf("&tx&tx&tx&tx","seq_num", tmp[0x9D-0x9C]&0x03,\
	    "nack_rx", (tmp[0x9D-0x9C]&0x04)!=0,\
		"source_addr", tmp[0xAA-0x9C],\
		  "dest_addr", tmp[0xAB-0x9C]);
	  responsePrintf("&tx&t4x","agc_word", (tmp[0xA1-0x9C]&0x0F),\
	    "crc", (((uint32_t)tmp[0xA6-0x9C])<<24)+ \
		(((uint32_t)tmp[0xA7-0x9C])<<16) + \
		  (((uint32_t)tmp[0xA8-0x9C])<<8) + \
		    (uint32_t)tmp[0xA9-0x9C]);
	  responsePrintf("&tx","packet_len", (((uint32_t)tmp[0xA4-0x9C])<<8)+(uint32_t)tmp[0xA5-0x9C]);
	}

      responsePrintf("&t4x\r\n","timer", SdkEvalGetTimerValue());
      responsePrintf("}\r\n");
    }
    else
    {
      break;
    }

    if(GetBatchLP==1)
    {
      /* If low power is enabled we can sleep and send the RX in pending */
      CMD_STROBE_STANDBY();
      CMD_STROBE_RX();
      /* At this point, if the radio will be woken up with READY it automatically goes in RX */
    }

  }

  /* For cut 2.0: since the 0x7c is not passed if the S2LPTimerLdcIrqWa is not applied
  we need to clear the last irq in firmware */
  if(GetBatchLP==2 || GetBatchLP==3)
  {
    S2LP_Middleware_GpioInterruptCmd(M2S_GPIO_3, 0x0A, 0x0A, DISABLE);
    S2LPTimerLdcrMode(S_DISABLE);
    S2LpTimerFastRxTermTimer(S_DISABLE);
    if(GetBatchLP==3)
    {
      CMD_STROBE_SABORT();
      CMD_STROBE_READY();
    }

    if(xIRQRaised)
    {
      xIRQRaised=RESET;
      if(S2LPManagementGetCut()==S2LP_CUT_2_0)
      {/* Exit from SLEEP (1st part) */
        S2LPTimerLdcIrqWa(S_ENABLE);
      }

      S2LPFillIRQQueue();
      s_xIrqQueue.cIndexPop = (s_xIrqQueue.cIndexPop+1)%IRQ_QUEUE_MAX_SIZE;
      s_xIrqQueue.cIrqQueueSize--;

      if(S2LPManagementGetCut()==S2LP_CUT_2_0)
      {/* Exit from SLEEP (2nd part) */
        S2LPTimerLdcIrqWa(S_DISABLE);
      }
    }
  }

  SdkEvalTimersState(APPL_TIMER_ID, NULL, DISABLE);
  SET_IRQ_MASK(VALID_SYNC, S_DISABLE);

  if(error!=0xff)	/* If stopped we should no respond to the SW because 0xff means STOP_CMD received, with the corresponding macro responding */
  {
    responsePrintf("{&N API call...}\r\n","S2LPGetNBytesBatch");
  }
}

void S2LPGetRssiRunBatchAction(void)
{
  uint8_t rssi[2];
  uint32_t ref_time=unsignedCommandArgument(0),rssi_avg_t=0;
  uint8_t rx_tim_cnt, tmp=0;

  DirectRx old_mode=S2LPPacketHandlerGetRxMode();
  S2LPPacketHandlerSetRxMode(DIRECT_RX_GPIO_MODE);
  S2LPSpiReadRegisters(TIMERS5_ADDR, 1, &rx_tim_cnt);
  S2LPSpiWriteRegisters(TIMERS5_ADDR, 1, &tmp);
  CMD_STROBE_RX();
  SdkEvalTimersTimConfig_ms(APPL_TIMER_ID,ref_time);
  SdkEvalTimersState(APPL_TIMER_ID, NULL, ENABLE);

  while(1)
  {
    /* Wait for timer to expire */
    WHILE(!xApplTimerIRQRaised);
    xApplTimerIRQRaised=RESET;
    {
      uint32_t n_samples = 0;
      S2LPSpiReadRegisters(RSSI_LEVEL_RUN_ADDR, sizeof(rssi), rssi);
      rssi_avg_t=0;
      for(uint32_t i=1;i<sizeof(rssi);i++) {
        if (rssi[i] > 0) {
          rssi_avg_t+=(uint32_t)rssi[i];
          n_samples++;
        }
      }

      /* Check that we have at least 1 valid sample */
      if (n_samples > 0)
        rssi_avg_t/=n_samples;
      else
        continue;
    }

    if(stopCmdFlag)
    {
      stopCmdFlag=0;
      SdkEvalTimersState(APPL_TIMER_ID, NULL, DISABLE);
      SdkEvalTimersResetCounter(APPL_TIMER_ID);
      S2LPSpiCommandStrobes(CMD_SABORT);
      S2LPPacketHandlerSetRxMode(old_mode);
      S2LPSpiWriteRegisters(TIMERS5_ADDR, 1, &rx_tim_cnt);
      responsePrintf("{&N API call...}\r\n","StopCmd");
      return;
    }
    responsePrintf("{&N API call...&tx}\r\n","S2LPGetRssiRun","rssi",(uint32_t)rssi_avg_t);
  }
}

/**
 * @brief  Get N-bytes routine. It will receive the desired number of bytes.
 *         <p><b>HowToCall</b>:  S2LPGetNBytes w</p>
 * @param  n_bytes number of bytes to be received.
 * @retval  recv_buffer buffer containing the received bytes.
 * @details The routine can be stopped before its end with a <i>stopCmd</i>.
 *           When called, the routine will stay in RX the number of bytes received is reached. FIFO AF is used to get the bytes from the RX FIFO.
 */
void S2LPGetNBytesHandler(void)
{
  uint16_t nNBytesRec,nNBytesToRec=unsignedCommandArgument(0);
  uint8_t error;

  CLEAR_IRQ_STATUS();
  error = S2LPGetNBytes(vectcBuff, &nNBytesRec , nNBytesToRec, 1000);
  uint8_t rssi; S2LPSpiReadRegisters(RSSI_LEVEL_ADDR, 1, &rssi);

  if(error!=0xff) {	/* If stopped we should no respond to the SW because 0xff means STOP_CMD received, with the corresponding macro responding! */

    responsePrintf("{&N API call...\r\n", "S2LPGetNBytes");
    responsePrintf("&tx\r\n","error",error);
    responsePrintf("&tx\r\n","rssi",rssi);
    if(!error) {
      responsePrintf("{%t: ","bytes");

      for(uint16_t i = 0; i < nNBytesRec-1; i++) {
        responsePrintf("0x%x,", vectcBuff[i]);
      }
      responsePrintf("0x%x", vectcBuff[nNBytesRec-1]);

      responsePrintf("}\r\n");
    }
    responsePrintf("&t4x\r\n", "timer", SdkEvalGetTimerValue());
    responsePrintf("}\r\n");
  }
}

void S2LPCalibrateRcoAction(void)
{
  S2LP_RcoCalibration();

  responsePrintf("{&N API call... }\r\n",
		 "S2LPCalibrateRco");
}

/**
 *@}
 */

/**
 *@}
 */

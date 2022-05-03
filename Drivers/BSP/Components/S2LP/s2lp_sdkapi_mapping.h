/**
 * @file    s2lp_sdkapi_mapping.h
 * @author  ST Microelectronics
 * @brief   Header file to map S2LP Library API to X-CUBE legacy API
 * @details
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2021 STMicroelectronics</center></h2>
 */
#ifndef __S2LP_SDKAPI_MAPPING_H__
#define __S2LP_SDKAPI_MAPPING_H__


/*** includes ***/

#include "S2LP_Commands.h"
#include "S2LP_Config.h"
#include "S2LP_Csma.h"
#include "S2LP_Fifo.h"
#include "S2LP_General.h"
#include "S2LP_Gpio.h"
#include "S2LP_PacketHandler.h"
#include "S2LP_PktBasic.h"
#include "S2LP_PktStack.h"
#include "S2LP_PktWMbus.h"
#include "S2LP_Qi.h"
#include "S2LP_Radio.h"
#include "S2LP_Regs.h"
#include "S2LP_Timer.h"
#include "S2LP_Timer_ex.h"
#include "S2LP_Types.h"

//Reverse mapping for legagy SDK Applications
#define S2LPManagementGetCut(...) S2LP_ManagementGetCut(__VA_ARGS__)
#define S2LPManagementIdentificationRFBoard(...) S2LP_ManagementIdentificationRFBoard(__VA_ARGS__)

//This is now in s2lp.c
#define S2LPManagementRcoCalibration(...) S2LP_RcoCalibration(__VA_ARGS__)

//	s2lp_timer.h		S2LP_Timer.h
#define	S2LP_TIM_SetRxTimerStopCondition(...)	S2LPTimerSetRxTimerStopCondition(__VA_ARGS__)
#define	S2LP_TIM_LdcrMode(...)	S2LPTimerLdcrMode(__VA_ARGS__)
#define	S2LP_TIM_LdcrAutoReload(...)	S2LPTimerLdcrAutoReload(__VA_ARGS__)
#define	S2LP_TIM_LdcrGetAutoReload(...)	S2LPTimerLdcrGetAutoReload(__VA_ARGS__)
#define	S2LP_TIM_FastRxTermTimer(...)	S2LpTimerFastRxTermTimer(__VA_ARGS__)
#define	S2LP_TIM_SetFastRxTermTimer(...)	S2LpSetTimerFastRxTermTimer(__VA_ARGS__)
#define	S2LP_TIM_SetFastRxTermTimerUs(...)	S2LpSetTimerFastRxTermTimerUs(__VA_ARGS__)
#define	S2LP_TIM_LdcrMode(...)	S2LPTimerLdcrMode(__VA_ARGS__)
#define	S2LP_TIM_LdcrAutoReload(...)	S2LPTimerLdcrAutoReload(__VA_ARGS__)
#define	S2LP_TIM_LdcrGetAutoReload(...)	S2LPTimerLdcrGetAutoReload(__VA_ARGS__)
#define	S2LP_TIM_SetRxTimer(...)	S2LPTimerSetRxTimer(__VA_ARGS__)
#define	S2LP_TIM_SetRxTimerUs(...)	S2LPTimerSetRxTimerUs(__VA_ARGS__)
#define	S2LP_TIM_SetRxTimerCounter(...)	S2LPTimerSetRxTimerCounter(__VA_ARGS__)
#define	S2LP_TIM_SetRxTimerPrescaler(...)	S2LPTimerSetRxTimerPrescaler(__VA_ARGS__)
#define	S2LP_TIM_GetRxTimerUs(...)	S2LPTimerGetRxTimerUs(__VA_ARGS__)
#define	S2LP_TIM_SetWakeUpTimer(...)	S2LPTimerSetWakeUpTimer(__VA_ARGS__)
#define	S2LP_TIM_SetWakeUpTimerUs(...)	S2LPTimerSetWakeUpTimerUs(__VA_ARGS__)
#define	S2LP_TIM_SetWakeUpTimerCounter(...)	S2LPTimerSetWakeUpTimerCounter(__VA_ARGS__)
#define	S2LP_TIM_SetWakeUpTimerPrescaler(...)	S2LPTimerSetWakeUpTimerPrescaler(__VA_ARGS__)
#define	S2LP_TIM_SetWakeUpTimerReloadUs(...)	S2LPTimerSetWakeUpTimerReloadUs(__VA_ARGS__)
#define	S2LP_TIM_GetWakeUpTimerUs(...)	S2LPTimerGetWakeUpTimerUs(__VA_ARGS__)
#define	S2LP_TIM_SetWakeUpTimerReload(...)	S2LPTimerSetWakeUpTimerReload(__VA_ARGS__)
#define	S2LP_TIM_SetWakeUpTimerReloadCounter(...)	S2LPTimerSetWakeUpTimerReloadCounter(__VA_ARGS__)
#define	S2LP_TIM_SetWakeUpTimerReloadPrescaler(...)	S2LPTimerSetWakeUpTimerReloadPrescaler(__VA_ARGS__)
#define	S2LP_TIM_GetWakeUpTimerReloadUs(...)	S2LPTimerGetWakeUpTimerReloadUs(__VA_ARGS__)
#define	S2LP_TIM_GetRcoFrequency(...)	S2LPTimerGetRcoFrequency(__VA_ARGS__)
#define	S2LP_TIM_CalibrationRco(...)	S2LPTimerCalibrationRco(__VA_ARGS__)
#define	S2LP_TIM_SleepB(...)	S2LPTimerSleepB(__VA_ARGS__)
#define	S2LP_TIM_LdcIrqWa(...)	S2LPTimerLdcIrqWa(__VA_ARGS__)

//			S2LP_Timer_ex.h
#define	S2LP_TIM_SetRxTimerMs(...)	S2LPTimerSetRxTimerMs(__VA_ARGS__)
#define	S2LP_TIM_GetRxTimer(...)	S2LPTimerGetRxTimer(__VA_ARGS__)
#define	S2LP_TIM_SetWakeUpTimerMs(...)	S2LPTimerSetWakeUpTimerMs(__VA_ARGS__)
#define	S2LP_TIM_SetWakeUpTimerReloadMs(...)	S2LPTimerSetWakeUpTimerReloadMs(__VA_ARGS__)
#define	S2LP_TIM_GetWakeUpTimer(...)	S2LPTimerGetWakeUpTimer(__VA_ARGS__)
#define	S2LP_TIM_GetWakeUpTimerReload(...)	S2LPTimerGetWakeUpTimerReload(__VA_ARGS__)
#define	S2LP_TIM_ComputeWakeUpValues(...)	S2LPTimerComputeWakeUpValues(__VA_ARGS__)
#define	S2LP_TIM_SetRxTimerCounter(...)	S2LPTimerSetRxTimerCounter(__VA_ARGS__)

//	s2lp_radio.h		S2LP_Radio.h
#define	S2LP_RADIO_Init(...)	S2LPRadioInit(__VA_ARGS__)
#define	S2LP_RADIO_GetInfo(...)	S2LPRadioGetInfo(__VA_ARGS__)
#define	S2LP_RADIO_SetSynthWord(...)	S2LPRadioSetSynthWord(__VA_ARGS__)
#define	S2LP_RADIO_GetSynthWord(...)	S2LPRadioGetSynthWord(__VA_ARGS__)
#define	S2LP_RADIO_SetChannel(...)	S2LPRadioSetChannel(__VA_ARGS__)
#define	S2LP_RADIO_GetChannel(...)	S2LPRadioGetChannel(__VA_ARGS__)
#define	S2LP_RADIO_SetRefDiv(...)	S2LPRadioSetRefDiv(__VA_ARGS__)
#define	S2LP_RADIO_GetRefDiv(...)	S2LPRadioGetRefDiv(__VA_ARGS__)
#define	S2LP_RADIO_SetDigDiv(...)	S2LPRadioSetDigDiv(__VA_ARGS__)
#define	S2LP_RADIO_GetDigDiv(...)	S2LPRadioGetDigDiv(__VA_ARGS__)
#define	S2LP_RADIO_SetChannelSpace(...)	S2LPRadioSetChannelSpace(__VA_ARGS__)
#define	S2LP_RADIO_GetChannelSpace(...)	S2LPRadioGetChannelSpace(__VA_ARGS__)
#define	S2LP_RADIO_SetFrequencyBase(...)	S2LPRadioSetFrequencyBase(__VA_ARGS__)
#define	S2LP_RADIO_GetFrequencyBase(...)	S2LPRadioGetFrequencyBase(__VA_ARGS__)
#define	S2LP_RADIO_SetDatarate(...)	S2LPRadioSetDatarate(__VA_ARGS__)
#define	S2LP_RADIO_GetDatarate(...)	S2LPRadioGetDatarate(__VA_ARGS__)
#define	S2LP_RADIO_SetFrequencyDev(...)	S2LPRadioSetFrequencyDev(__VA_ARGS__)
#define	S2LP_RADIO_GetFrequencyDev(...)	S2LPRadioGetFrequencyDev(__VA_ARGS__)
#define	S2LP_RADIO_SetChannelBW(...)	S2LPRadioSetChannelBW(__VA_ARGS__)
#define	S2LP_RADIO_GetChannelBW(...)	S2LPRadioGetChannelBW(__VA_ARGS__)
#define	S2LP_RADIO_SetModulation(...)	S2LPRadioSetModulation(__VA_ARGS__)
#define	S2LP_RADIO_GetModulation(...)	S2LPRadioGetModulation(__VA_ARGS__)
#define	S2LP_RADIO_SetXtalFrequency(...)	S2LPRadioSetXtalFrequency(__VA_ARGS__)
#define	S2LP_RADIO_GetXtalFrequency(...)	S2LPRadioGetXtalFrequency(__VA_ARGS__)
#define	S2LP_RADIO_SetIsiEqualizationMode(...)	S2LPRadioSetIsiEqualizationMode(__VA_ARGS__)
#define	S2LP_RADIO_GetIsiEqualizationMode(...)	S2LPRadioGetIsiEqualizationMode(__VA_ARGS__)
#define	S2LP_RADIO_CalibrationVco(...)	S2LPRadioCalibrationVco(__VA_ARGS__)
#define	S2LP_RADIO_SetTxCalibVcoAmpWord(...)	S2LPRadioSetTxCalibVcoAmpWord(__VA_ARGS__)
#define	S2LP_RADIO_SetRxCalibVcoAmpWord(...)	S2LPRadioSetRxCalibVcoAmpWord(__VA_ARGS__)
#define	S2LP_RADIO_SetTxCalibVcoFreqWord(...)	S2LPRadioSetTxCalibVcoFreqWord(__VA_ARGS__)
#define	S2LP_RADIO_SetRxCalibVcoFreqWord(...)	S2LPRadioSetRxCalibVcoFreqWord(__VA_ARGS__)
#define	S2LP_RADIO_SetMaxPALevel(...)	S2LPRadioSetMaxPALevel(__VA_ARGS__)
#define	S2LP_RADIO_SetPALeveldBm(...)	S2LPRadioSetPALeveldBm(__VA_ARGS__)
#define	S2LP_RADIO_GetPALeveldBm(...)	S2LPRadioGetPALeveldBm(__VA_ARGS__)
#define	S2LP_RADIO_GetPALevelMaxIndex(...)	S2LPRadioGetPALevelMaxIndex(__VA_ARGS__)
#define	S2LP_RADIO_SetPALevelMaxIndex(...)	S2LPRadioSetPALevelMaxIndex(__VA_ARGS__)
#define	S2LP_RADIO_SetManualRampingMode(...)	S2LPRadioSetManualRampingMode(__VA_ARGS__)
#define	S2LP_RADIO_SetAutoRampingMode(...)	S2LPRadioSetAutoRampingMode(__VA_ARGS__)
#define	S2LP_RADIO_AfcInit(...)	S2LPRadioAfcInit(__VA_ARGS__)
#define	S2LP_RADIO_GetAfcInfo(...)	S2LPRadioGetAfcInfo(__VA_ARGS__)
#define	S2LP_RADIO_SymClkRecoverInit(...)	S2LPRadioSymClkRecoverInit(__VA_ARGS__)
#define	S2LP_RADIO_GetSymClkRecoverInfo(...)	S2LPRadioGetSymClkRecoverInfo(__VA_ARGS__)

//	s2lp_qi.h		S2LP_Qi.h
#define	S2LP_RADIO_QI_GetRssidBm(...)	S2LPRadioGetRssidBm(__VA_ARGS__)
#define	S2LP_RADIO_QI_GetRssidBmRun(...)	S2LPRadioGetRssidBmRun(__VA_ARGS__)
#define	S2LP_RADIO_QI_SetRssiThreshdBm(...)	S2LPRadioSetRssiThreshdBm(__VA_ARGS__)
#define	S2LP_RADIO_QI_CsBlanking(...)	S2LPRadioCsBlanking(__VA_ARGS__)
#define	S2LP_RADIO_QI_RssiInit(...)	S2LPRadioRssiInit(__VA_ARGS__)
#define	S2LP_RADIO_QI_GetRssiInfo(...)	S2LPRadioGetRssiInfo(__VA_ARGS__)
#define	S2LP_RADIO_QI_AntennaSwitching(...)	S2LPRadioAntennaSwitching(__VA_ARGS__)
#define	S2LP_RADIO_QI_SetPQIThreshold(...)	S2LPRadioSetPQIThreshold(__VA_ARGS__)
#define	S2LP_RADIO_QI_SetSQIThreshold(...)	S2LPRadioSetSQIThreshold(__VA_ARGS__)
#define	S2LP_RADIO_QI_EnableSQI(...)	S2LPRadioEnableSQI(__VA_ARGS__)
#define	S2LP_RADIO_QI_GetCs(...)	S2LPQiGetCs(__VA_ARGS__)
#define	S2LP_RADIO_QI_GetLqi(...)	S2LPQiGetLqi(__VA_ARGS__)

//	s2lp_pktwmbus.h		 S2LP_PktWMbus.h
#define	S2LP_PACKET_WMBUS_Init(...)	S2LPPktWMbusInit(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_GetInfo(...)	S2LPPktWMbusGetInfo(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_SetFormat(...)	S2LPPktWMbusSetFormat(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_SetPostamble(...)	S2LPPktWMbusSetPostamble(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_GetPostamble(...)	S2LPPktWMbusGetPostamble(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_SetPostamblePattern(...)	S2LPPktWMbusSetPostamblePattern(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_GetPostamblePattern(...)	S2LPPktWMbusGetPostamblePattern(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_SetSubmode(...)	S2LPPktWMbusSetSubmodeWMbusSubmode(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_GetSubmode(...)	S2LPPktWMbusGetSubmode(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_SetPayloadLength(...)	S2LPPktWMbusSetPayloadLength(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_GetPayloadLength(...)	S2LPPktWMbusGetPayloadLength(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_SetPreambleLength(...)	S2LPPktWMbusSetPreambleLength(__VA_ARGS__)
#define	S2LP_PACKET_WMBUS_GetPreambleLength(...)	S2LPPktWMbusGetPreambleLength(__VA_ARGS__)

//	s2lp_pktstack.h		S2LP_PktStack.h
#define	S2LP_PCKT_STACK_SetPreambleLength(...)	S2LPPktStackSetPreambleLength(__VA_ARGS__)
#define	S2LP_PCKT_STACK_GetPreambleLength(...)	S2LPPktStackGetPreambleLength(__VA_ARGS__)
#define	S2LP_PCKT_STACK_SetSyncLength(...)	S2LPPktStackSetSyncLength(__VA_ARGS__)
#define	S2LP_PCKT_STACK_GetSyncLength(...)	S2LPPktStackGetSyncLength(__VA_ARGS__)
#define	S2LP_PCKT_STACK_FilterOnCrc(...)	S2LPPktStackFilterOnCrc(__VA_ARGS__)
#define	S2LP_PCKT_STACK_Whitening(...)	S2LPPktStackWhitening(__VA_ARGS__)
#define	S2LP_PCKT_STACK_Fec(...)	S2LPPktStackFec(__VA_ARGS__)
#define	S2LP_PCKT_STACK_SetSyncWords(...)	S2LPPktStackSetSyncWords(__VA_ARGS__)
#define	S2LP_PCKT_STACK_Init(...)	S2LPPktStackInit(__VA_ARGS__)
#define	S2LP_PCKT_STACK_GetInfo(...)	S2LPPktStackGetInfo(__VA_ARGS__)
#define	S2LP_PCKT_STACK_AddressesInit(...)	S2LPPktStackAddressesInit(__VA_ARGS__)
#define	S2LP_PCKT_STACK_GetAddressesInfov(...)	S2LPPktStackGetAddressesInfo(__VA_ARGS__)
#define	S2LP_PCKT_STACK_SetFormat(...)	S2LPPktStackSetFormat(__VA_ARGS__)
#define	S2LP_PCKT_STACK_SetPayloadLength(...)	S2LPPktStackSetPayloadLength(__VA_ARGS__)
#define	S2LP_PCKT_STACK_GetPayloadLength(...)	S2LPPktStackGetPayloadLength(__VA_ARGS__)
#define	S2LP_PCKT_STACK_GetReceivedPktLength(...)	S2LPPktStackGetReceivedPktLength(__VA_ARGS__)
#define	S2LP_PCKT_STACK_AckRequest(...)	S2LPPktStackAckRequest(__VA_ARGS__)
#define	S2LP_PCKT_STACK_AutoAck(...)	S2LPPktStackAutoAck(__VA_ARGS__)
#define	S2LP_PCKT_STACK_NRetx(...)	S2LPPktStackNRetx(__VA_ARGS__)
#define	S2LP_PCKT_STACK_Piggybacking(...)	S2LPPktStackPiggybacking(__VA_ARGS__)
#define	S2LP_PCKT_STACK_GetTXAckRequest(...)	S2LPPktStackGetTXAckRequest(__VA_ARGS__)
#define	S2LP_PCKT_STACK_GetNReTx(...)	S2LPPktStackGetNReTx(__VA_ARGS__)


//	s2lp_pktbasic.h		S2LP_PktBasic.h
#define	S2LP_PCKT_BASIC_SetPreambleLength(...)	S2LPPktBasicSetPreambleLength(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_GetPreambleLength(...)	S2LPPktBasicGetPreambleLength(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_SetSyncLength(...)	S2LPPktBasicSetSyncLength(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_GetSyncLength(...)	S2LPPktBasicGetSyncLength(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_FilterOnCrc(...)	S2LPPktBasicFilterOnCrc(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_Whitening(...)	S2LPPktBasicWhitening(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_Fec(...)	S2LPPktBasicFec(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_SetSyncWords(...)	S2LPPktBasicSetSyncWords(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_Init(...)	S2LPPktBasicInit(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_GetInfo(...)	S2LPPktBasicGetInfo(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_AddressesInit(...)	S2LPPktBasicAddressesInit(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_GetAddressesInfo(...)	S2LPPktBasicGetAddressesInfo(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_SetFormat(...)	S2LPPktBasicSetFormat(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_AddressField(...)	S2LPPktBasicAddressField(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_GetAddressField(...)	S2LPPktBasicGetAddressField(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_SetPayloadLength(...)	S2LPPktBasicSetPayloadLength(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_GetPayloadLength(...)	S2LPPktBasicGetPayloadLength(__VA_ARGS__)
#define	S2LP_PCKT_BASIC_GetReceivedPktLength(...)	S2LPPktBasicGetReceivedPktLength(__VA_ARGS__)


//	s2lp_packethandler.h		S2LP_PacketHandler.h
#define	S2LP_PCKT_HNDL_SetPreambleLength(...)	S2LPSetPreambleLength(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetPreambleLength(...)	S2LPGetPreambleLength(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetSyncLength(...)	S2LPSetSyncLength(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetSyncLength(...)	S2LPGetSyncLength(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetSyncWords(...)	S2LPSetSyncWords(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetSyncWords(...)	S2LPGetSyncWords(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_Whitening(...)	S2LPPacketHandlerWhitening(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_Fec(...)	S2LPPacketHandlerFec(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_3OutOf6(...)	S2LPPacketHandler3OutOf6(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_Manchester(...)	S2LPPacketHandlerManchester(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetPacketFormat(...)	S2LPGetPacketFormat(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_FilterOnCrc(...)	S2LPPktCommonFilterOnCrc(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetReceivedDestinationAddress(...)	S2LPGetReceivedDestinationAddress(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetReceivedSourceAddress(...)	S2LPGetReceivedSourceAddress(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetMyAddress(...)	S2LPGetMyAddress(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetBroadcastAddress(...)	S2LPGetBroadcastAddress(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetMulticastAddress(...)	S2LPGetMulticastAddress(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetRxSourceMask(...)	S2LPGetRxSourceMask(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetRxSourceReferenceAddress(...)	S2LPGetRxSourceReferenceAddress(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetTxMode(...)	S2LPPacketHandlerSetTxMode(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetRxMode(...)	S2LPPacketHandlerSetRxMode(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetTxMode(...)	S2LPPacketHandlerGetTxMode(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetRxMode(...)	S2LPPacketHandlerGetRxMode(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetExtendedLenField(...)	S2LPPacketHandlerSetExtendedLenField(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_Swap4FSKSymbol(...)	S2LPPacketHandlerSwap4FSKSymbol(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SwapFifoEndianess(...)	S2LPPacketHandlerSwapFifoEndianess(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SwapPreamblePattern(...)	S2LPPacketHandlerSwapPreamblePattern(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetCrcMode(...)	S2LPPacketHandlerSetCrcMode(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetCrcMode(...)	S2LPPacketHandlerGetCrcMode(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SelectSecondarySync(...)	S2LPPacketHandlerSelectSecondarySync(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetAutoPcktFilter(...)	S2LPPacketHandlerSetAutoPcktFilter(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetRxPersistentMode(...)	S2LPPacketHandlerSetRxPersistentMode(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetSrcAddrFlt(...)	S2LPPacketHandlerSetSrcAddrFlt(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetVariableLength(...)	S2LPPacketHandlerSetVariableLength(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetDualSyncWords(...)	S2LPSetDualSyncWords(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetDualSyncWords(...)	S2LPGetDualSyncWords(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetRxSourceMask(...)	S2LPSetRxSourceMask(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetRxSourceReferenceAddress(...)	S2LPSetRxSourceReferenceAddress(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetBroadcastAddress(...)	S2LPSetBroadcastAddress(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetMulticastAddress(...)	S2LPSetMulticastAddress(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_SetMyAddress(...)	S2LPSetMyAddress(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetMyAddress(...)	S2LPGetMyAddress(__VA_ARGS__)
#define	S2LP_PCKT_HNDL_GetTransmittedSeqNumber(...)	S2LPPacketHandlerGetTransmittedSeqNumber(__VA_ARGS__)

//	s2lp_gpio.h		S2LP_Gpio.h
#define	  S2LP_GPIO_Pin		 S2LPGpioPin
#define	 S2LP_GPIO_Mode		 S2LPGpioMode
#define	 S2LP_GPIO_IO		 S2LPGpioIO
#define	xS2LP_GPIO_Pin		xS2LPGpioPin
#define	xS2LP_GPIO_Mode		xS2LPGpioMode
#define	xS2LP_GPIO_IO		xS2LPGpioIO
#define	S2LP_GPIO_Init(...)	S2LPGpioInit(__VA_ARGS__)
#define	S2LP_GPIO_SetLevel(...)	S2LPGpioSetLevel(__VA_ARGS__)
#define	S2LP_GPIO_GetLevel(...)	S2LPGpioGetLevel(__VA_ARGS__)
#define	S2LP_GPIO_ClockOutput(...)	S2LPGpioClockOutput(__VA_ARGS__)
#define	S2LP_GPIO_ClockOutputInit(...)	S2LPGpioClockOutputInit(__VA_ARGS__)
#define	S2LP_GPIO_SetXOPrescaler(...)	S2LPGpioSetXOPrescaler(__VA_ARGS__)
#define	S2LP_GPIO_GetXOPrescaler(...)	S2LPGpioGetXOPrescaler(__VA_ARGS__)
#define	S2LP_GPIO_SetRCOPrescaler(...)	S2LPGpioSetRCOPrescaler(__VA_ARGS__)
#define	S2LP_GPIO_GetRCOPrescaler(...)	S2LPGpioGetRCOPrescaler(__VA_ARGS__)
#define	S2LP_GPIO_SetExtraClockCycles(...)	S2LPGpioSetExtraClockCycles(__VA_ARGS__)
#define	S2LP_GPIO_GetExtraClockCycles(...)	S2LPGpioGetExtraClockCycles(__VA_ARGS__)
#define	S2LP_GPIO_IrqDeInit(...)	S2LPGpioIrqDeInit(__VA_ARGS__)
#define	S2LP_GPIO_IrqInit(...)	S2LPGpioIrqInit(__VA_ARGS__)
#define	S2LP_GPIO_IrqConfig(...)	S2LPGpioIrqConfig(__VA_ARGS__)
#define	S2LP_GPIO_IrqGetMask(...)	S2LPGpioIrqGetMask(__VA_ARGS__)
#define	S2LP_GPIO_IrqGetStatus(...)	S2LPGpioIrqGetStatus(__VA_ARGS__)
#define	S2LP_GPIO_IrqClearStatus(...)	S2LPGpioIrqClearStatus(__VA_ARGS__)
#define	S2LP_GPIO_IrqCheckFlag(...)	S2LPGpioIrqCheckFlag(__VA_ARGS__)



//	s2lp_fifo.h		S2LP_Fifo.h
#define	S2LP_FIFO_ReadNumberBytesRxFifo(...)	S2LPFifoReadNumberBytesRxFifo(__VA_ARGS__)
#define	S2LP_FIFO_ReadNumberBytesTxFifo(...)	S2LPFifoReadNumberBytesTxFifo(__VA_ARGS__)
#define	S2LP_FIFO_SetAlmostFullThresholdRx(...)	S2LPFifoSetAlmostFullThresholdRx(__VA_ARGS__)
#define	S2LP_FIFO_GetAlmostFullThresholdRx(...)	S2LPFifoGetAlmostFullThresholdRx(__VA_ARGS__)
#define	S2LP_FIFO_SetAlmostEmptyThresholdRx(...)	S2LPFifoSetAlmostEmptyThresholdRx(__VA_ARGS__)
#define	S2LP_FIFO_GetAlmostEmptyThresholdRx(...)	S2LPFifoGetAlmostEmptyThresholdRx(__VA_ARGS__)
#define	S2LP_FIFO_SetAlmostFullThresholdTx(...)	S2LPFifoSetAlmostFullThresholdTx(__VA_ARGS__)
#define	S2LP_FIFO_GetAlmostFullThresholdTx(...)	S2LPFifoGetAlmostFullThresholdTx(__VA_ARGS__)
#define	S2LP_FIFO_SetAlmostEmptyThresholdTx(...)	S2LPFifoSetAlmostEmptyThresholdTx(__VA_ARGS__)
#define	S2LP_FIFO_GetAlmostEmptyThresholdTx(...)	S2LPFifoGetAlmostEmptyThresholdTx(__VA_ARGS__)
#define	S2LP_FIFO_MuxRxFifoIrqEnable(...)	S2LPFifoMuxRxFifoIrqEnable(__VA_ARGS__)


//	s2lp_csma.h		S2LP_Csma.h
#define	S2LP_CSMA_Init(...)	S2LPCsmaInit(__VA_ARGS__)
#define	S2LP_CSMA_GetInfo(...)	S2LPCsmaGetInfo(__VA_ARGS__)
#define	S2LP_CSMA_Enable(...)	S2LPCsma(__VA_ARGS__)
#define	S2LP_CSMA_GetCsma(...)	S2LPCsmaGetCsma(__VA_ARGS__)
#define	S2LP_CSMA_PersistentMode(...)	S2LPCsmaPersistentMode(__VA_ARGS__)
#define	S2LP_CSMA_GetPersistentMode(...)	S2LPCsmaGetPersistentMode(__VA_ARGS__)
#define	S2LP_CSMA_SeedReloadMode(...)	S2LPCsmaSeedReloadMode(__VA_ARGS__)
#define	S2LP_CSMA_GetSeedReloadMode(...)	S2LPCsmaGetSeedReloadMode(__VA_ARGS__)
#define	S2LP_CSMA_SetBuCounterSeed(...)	S2LPCsmaSetBuCounterSeed(__VA_ARGS__)
#define	S2LP_CSMA_GetBuCounterSeed(...)	S2LPCsmaGetBuCounterSeed(__VA_ARGS__)
#define	S2LP_CSMA_SetBuPrescaler(...)	S2LPCsmaSetBuPrescaler(__VA_ARGS__)
#define	S2LP_CSMA_GetBuPrescaler(...)	S2LPCsmaGetBuPrescaler(__VA_ARGS__)
#define	S2LP_CSMA_SetCcaPeriod(...)	S2LPCsmaSetCcaPeriod(__VA_ARGS__)
#define	S2LP_CSMA_GetCcaPeriod(...)	S2LPCsmaGetCcaPeriod(__VA_ARGS__)
#define	S2LP_CSMA_SetCcaLength(...)	S2LPCsmaSetCcaLength(__VA_ARGS__)
#define	S2LP_CSMA_GetCcaLength(...)	S2LPCsmaGetCcaLength(__VA_ARGS__)
#define	S2LP_CSMA_SetMaxNumberBackoff(...)	S2LPCsmaSetMaxNumberBackoff(__VA_ARGS__)
#define	S2LP_CSMA_GetMaxNumberBackoff(...)	S2LPCsmaGetMaxNumberBackoff(__VA_ARGS__)



//	s2lp_commands.h		S2LP_Commands.h
#define	 S2LP_CMD_		 S2LPCmd
#define	S2LP_CMD_StrobeTx(...)	S2LPCmdStrobeTx(__VA_ARGS__)
#define	S2LP_CMD_StrobeRx(...)	S2LPCmdStrobeRx(__VA_ARGS__)
#define	S2LP_CMD_StrobeCommand(...)	S2LPCmdStrobeCommand(__VA_ARGS__)
#define	S2LP_CMD_StrobeReady(...)	S2LPCmdStrobeReady(__VA_ARGS__)
#define	S2LP_CMD_StrobeStandby(...)	S2LPCmdStrobeStandby(__VA_ARGS__)
#define	S2LP_CMD_StrobeSleep(...)	S2LPCmdStrobeSleep(__VA_ARGS__)
#define	S2LP_CMD_StrobeLockRx(...)	S2LPCmdStrobeLockRx(__VA_ARGS__)
#define	S2LP_CMD_StrobeLockTx(...)	S2LPCmdStrobeLockTx(__VA_ARGS__)
#define	S2LP_CMD_StrobeSabort(...)	S2LPCmdStrobeSabort(__VA_ARGS__)
#define	S2LP_CMD_StrobeLdcReload(...)	S2LPCmdStrobeLdcReload(__VA_ARGS__)
#define	S2LP_CMD_StrobeSequenceUpdate(...)	S2LPCmdStrobeSequenceUpdate(__VA_ARGS__)
#define	S2LP_CMD_StrobeSres(...)	S2LPCmdStrobeSres(__VA_ARGS__)
#define	S2LP_CMD_StrobeFlushRxFifo(...)	S2LPCmdStrobeFlushRxFifo(__VA_ARGS__)
#define	S2LP_CMD_StrobeFlushTxFifo(...)	S2LPCmdStrobeFlushTxFifo(__VA_ARGS__)

//	S2LP_Types.h
#define S2LP_RefreshStatus(...) S2LPRefreshStatus(__VA_ARGS__)

#endif /*__S2LP_SDKAPI_MAPPING_H__*/

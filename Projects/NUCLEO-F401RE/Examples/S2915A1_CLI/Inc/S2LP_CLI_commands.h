/**
  ******************************************************************************
  * @file    S2LP_CLI_Commands.h
  * @author  STMicroelectronics
  * @date    March 2022
  * @brief   S2LP CLI Commands header file
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
#ifndef __S2LP_CLI_COMMANDS_H__
#define __S2LP_CLI_COMMANDS_H__

#include "S2LP_Config.h"

#define WHILE(WAIT_VAR) {while(!checkStop() && WAIT_VAR);}

#define S2LP_COMMAND_TABLE \
{ "S2LPRadioInit",      S2LPRadioInitAction,    "wuwwww", "\r\nuint32 frequency base [Hz]\r\nbyte modulation\r\nuint32 data rate [bps]\r\nuint32 frequency deviation [Hz]\r\nuint32 channel filter bandwidth [Hz]\r\nuint32 XTAL frequency in Hz, 0 means auto [Hz]" }, \
{ "S2LPRadioGetInfo",   S2LPRadioGetInfoAction, "", "" }, \
{ "S2LPRadioGetXtalFrequency",   S2LPRadioGetXtalFrequencyAction, "", "\r\nGet the XTAL frequency [Hz]" }, \
{ "S2LPRadioSetFrequencyBase",   S2LPRadioSetFrequencyBaseAction, "w", "\r\nuint32 Frequency base [Hz]" }, \
{ "S2LPRadioGetFrequencyBase",   S2LPRadioGetFrequencyBaseAction, "", "" }, \
{ "S2LPRadioSetModulation",   S2LPRadioSetModulationAction, "u", "\r\nuint8 Modulation code" }, \
{ "S2LPRadioGetModulation",   S2LPRadioGetModulationAction, "", "" }, \
{ "S2LPRadioSetPALeveldBm",   S2LPRadioSetPALeveldBmAction, "wu", "\r\nfloatoutput power in dBm\r\nbyte pa level max index used" }, \
{ "S2LPRadioGetPALeveldBm",   S2LPRadioGetPALeveldBmAction, "u", "\r\npa level index" }, \
{ "S2LPRadioSetPALevelNdBm", S2LPRadioSetPALevelNdBmAction, "wu", "\r\nfloat power in dBm\r\nbyte pa level index used for ramping"}, \
{ "S2LPRadioSetPALevelMaxInd", S2LPRadioSetPALevelMaxIndAction, "u", "to set the max index for PA"}, \
{ "S2LPGetPktFrmt",   S2LPGetPktFrmtAction, "", "" }, \
{ "S2LPPktBasicInit",   S2LPPktBasicInitAction, "vuwuuuuuu", "\r\nuint16 preamble length\r\nbyte sync length\r\nuint32 sync word\r\n - 0 no length field\r\n - 0 1 byte\r\n - 1 2 bytes\r\n(1/0) address field enable\r\nbyte CRC mode\r\n(1/0) FEC enable\r\n(1/0) whitening enable" }, \
{ "S2LPPktBasicGetInfo", S2LPPktBasicGetInfoAction, "", "" }, \
{ "S2LPPktStackInit",   S2LPPktStackInitAction, "vuwuuuuu", "\r\nuint16 preamble length\r\nbyte sync length\r\nuint32 sync word\r\n - 0 no length field\r\n - 0 1 byte\r\n - 1 2 bytes\r\nbyte CRC mode\r\n(1/0) FEC enable\r\n(1/0) whitening enable" }, \
{ "S2LPPktStackGetInfo", S2LPPktStackGetInfoAction, "", "" }, \
{ "S2LPPktBasicSetPayloadLength", S2LPPktBasicSetPayloadLengthAction, "v", "\r\nuint16 byte preamble length" }, \
{ "S2LPPktBasicGetPayloadLength", S2LPPktBasicGetPayloadLengthAction, "", "" }, \
{ "S2LPPktStackSetPayloadLength", S2LPPktStackSetPayloadLengthAction, "v", "\r\nuint16 byte preamble length" }, \
{ "S2LPPktStackGetPayloadLength", S2LPPktStackGetPayloadLengthAction, "", "" }, \
{ "S2LPPktMbusSetPayloadLength",    S2LPPktMbusSetPayloadLengthAction, "v", "payload length" }, \
{ "S2LPPktMbusGetPayloadLength",    S2LPPktMbusGetPayloadLengthAction, "", "" }, \
{ "S2LPPktMbusInit",    S2LPPktMbusInitAction, "uuu", "\r\nbyte WMBUS mode :\r\n - 0 MBUS_SUBMODE_S1_S2_LONG_HEADER\r\n - 1 MBUS_SUBMODE_S1_M_S2_T2_OTHER_TO_METER\r\n - 3 MBUS_SUBMODE_T1_T2_METER_TO_OTHER\r\n - 5 MBUS_SUBMODE_R2_SHORT_HEADER\r\nbyte preamble to add to the standard default preamble\r\nbyte postamble to add to the standard default postamble" }, \
{ "S2LPPktMbusGetInfo", S2LPPktMbusGetInfoAction, "", "" }, \
{ "S2LPTimerSetRxTimeoutUs", S2LPTimerSetRxTimeoutUsAction, "w", "float rx timeout in us" }, \
{ "S2LPTimerGetRxTimeout",   S2LPTimerGetRxTimeoutAction, "", "RX timeout in us" }, \
{ "S2LPGpioInit",       S2LPGpioInitAction, "uuu", "\r\ngpio pin (0,1,2,3)\r\ngpio mode:\r\n - 0 input\r\n - 1 output lp\r\n - 2 output hp\r\nsignal (code)" }, \
{ "S2LPIrq",       S2LPIrqAction, "wu", "\r\nuint32_t irq code\r\n(1/0) enable/disable" }, \
{ "S2LPIrqGetStatus",   S2LPIrqGetStatusAction, "", "" }, \
{ "S2LPQiGetRssidBm",   S2LPQiGetRssidBmAction, "", "" }, \
{ "S2LPGetVersion",   S2LPGetVersionAction, "", "" }, \
{ "S2LPWhitening",    S2LPWhiteningAction, "u", "\r\nTo enable or disable whitening"}, \
{ "S2LPDirectRfSetTxMode",S2LPDirectRfSetTxModeAction, "u","\r\nSet the direct TX mode"}, \
{ "S2LPDirectRfSetRxMode",S2LPDirectRfSetRxModeAction, "u","\r\nSet the direct RX mode"}, \
{ "S2LPLinearFifoSetAlmostFullThresholdRx", S2LPLinearFifoSetAlmostFullThresholdRxAction, "u", "to set the Rx FIFO Thr"}, \
{ "S2LPLinearFifoSetAlmostEmptyThresholdTx", S2LPLinearFifoSetAlmostEmptyThresholdTxAction, "u", "to set the Tx FIFO Thr"}, \
{ "S2LPTimerSetWakeUpTimerUs", S2LPTimerSetWakeUpTimerUsAction, "w", "Wakeup timer timeout in us" }, \
{ "S2LPTimerLdcrMode",  S2LPTimerLdcrModeAction, "uu", "To set the LDC mode"}, \
{ "S2LPCalibrateRco",  S2LPCalibrateRcoAction, "", "Offline calibration of the RCO"}, \
{ "S2LPGetNBytesReportAll", S2LPGetNBytesRepAllAction, "u", "Decides if the getNBytes has to read addresses, seq num etc."}, \
{ "S2LPGetNBytes", S2LPGetNBytesHandler, "v", "this handler is to get N bytes in reception"}, \
{ "S2LPGetNBytesBatch", S2LPGetNBytesBatchHandler, "ww", "this handler is to get N bytes in reception"}, \
{ "S2LPSendNBytes", S2LPSendNBytesHandler, "b", "this handler is to sent a specified bytes sequence"}, \
{ "S2LPSendNBytesBatch", S2LPSendNBytesBatchHandler, "wwb", "this handler is to sent a specified bytes sequence every a specified num of ms"}, \
{ "S2LPGetRssiRunBatch", S2LPGetRssiRunBatchAction, "w", "RSSI run"}, \
{ "SdkEvalIrqHandler",     IrqHandlerAction, "u", "1 get IRQ in blocking mode" }, \
{ "S2LPGetLibVersion",   S2LPGetLibVersionAction, "", "" }, \
{ "S2LPSendBatchLP",   S2LPSendBatchLPAction, "u", "Enables the low power transmission in batch" }, \
{ "S2LPGetBatchLP",   S2LPGetBatchLPAction, "u", "Enables the low power reception in batch" }, \
{ "S2LPGetRcoFrequency",  S2LPGetRcoFrequencyAction, "", "Returns the RCO frequency in Hz"}

#define CMD_STROBE_SLEEP()      S2LPCmdStrobeSleep()
#define CMD_STROBE_TX()         {FEM_Operation(FEM_TX); S2LPCmdStrobeCommand(CMD_TX);}
#define CMD_STROBE_RX()         {FEM_Operation(FEM_RX); S2LPCmdStrobeCommand(CMD_RX);}
#define CMD_STROBE_SRES()       S2LPCmdStrobeSres()
#define CMD_STROBE_SABORT()     S2LPCmdStrobeSabort()
#define CMD_STROBE_STANDBY()    S2LPCmdStrobeStandby()
#define CMD_STROBE_READY()      S2LPCmdStrobeReady()

/*S2LP_PROCS */
void S2LPGetRcoFrequencyAction(void);
float S2LPGetMsUntilSync(void);
void S2LPPktBasicInitAction(void);
void S2LPPktBasicGetInfoAction(void);
void S2LPPktStackGetInfoAction(void);
void S2LPPktStackInitAction(void);
void S2LPPktBasicSetPayloadLengthAction(void);
void S2LPPktBasicGetPayloadLengthAction(void);
void S2LPTimerSetRxTimeoutUsAction(void);
void S2LPTimerGetRxTimeoutAction(void);
void S2LPGetPktFrmtAction(void);
void S2LPPktMbusGetPayloadLengthAction(void);
void S2LPPktMbusSetPayloadLengthAction(void);
void S2LPPktStackSetPayloadLengthAction(void);
void S2LPPktStackGetPayloadLengthAction(void);
void S2LPPktMbusGetInfoAction(void);
void S2LPPktMbusInitAction(void);
void S2LPRadioInitAction(void);
void S2LPRadioGetInfoAction(void);
void S2LPRadioGetXtalFrequencyAction(void);
void S2LPRadioSetFrequencyBaseAction(void);
void S2LPRadioGetFrequencyBaseAction(void);
void S2LPRadioSetModulationAction(void);
void S2LPRadioGetModulationAction(void);
void S2LPQiGetRssidBmAction(void);
void S2LPRadioSetPALevelNdBmAction(void);
void S2LPRadioSetPALevelMaxIndAction(void);
void S2LPRadioSetPALeveldBmAction(void);
void S2LPRadioGetPALeveldBmAction(void);
void S2LPWhiteningAction(void);
void S2LPDirectRfSetRxModeAction(void);
void S2LPDirectRfSetTxModeAction(void);
void S2LPGpioInitAction(void);
void S2LPIrqAction(void);
void S2LPIrqGetStatusAction(void);
void S2LPLinearFifoSetAlmostFullThresholdRxAction(void);
void S2LPLinearFifoSetAlmostEmptyThresholdTxAction(void);
void S2LPGetVersionAction(void);
void S2LPGetLibVersionAction(void);
void S2LPTimerSetWakeUpTimerUsAction(void);
void S2LPTimerLdcrModeAction(void);
void S2LPCalibrateRcoAction(void);
/*UC_PROCS */
void S2LPGetNBytesRepAllAction(void);
void S2LPSendBatchLPAction(void);
void S2LPGetBatchLPAction(void);
void S2LPGetNBytesHandler(void);
void IrqHandlerAction(void);
void S2LPGetNBytesBatchHandler(void);
void S2LPSendPatternHandler(void);
void S2LPSendNBytesHandler(void);
void S2LPSendNBytesBatchHandler(void);
void S2LPGetRssiRunBatchAction(void);

uint8_t checkStop(void);

#endif /*__S2LP_CLI_COMMANDS_H__*/

/**
  ******************************************************************************
  * @file    SDK_CLI_Commands.h
  * @author  STMicroelectronics
  * @date    March 2022
  * @brief   SDK CLI Commands header file
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
#ifndef __SDK_CLI_COMMANDS_H__
#define __SDK_CLI_COMMANDS_H__

#include <stdint.h>

#define SDK_EVAL_COMMAND_TABLE \
{ "SdkEvalSpiCommandStrobes",  SdkEvalSpiCommandStrobesAction,    "u",  "\r\nCommand code" }, \
{ "SdkEvalSpiReadRegisters",   SdkEvalSpiReadRegistersAction,     "uu", "\r\nRegister address\r\nNo of registers to read" }, \
{ "SdkEvalSpiWriteRegisters",  SdkEvalSpiWriteRegistersAction,    "ub", "\r\nRegister address\r\nRegisters values i.e. /x00/x01" }, \
{ "SdkEvalSpiReadFifo",  SdkEvalSpiReadFifoAction,              "u",  "\r\nNo of byte to read from RX FIFO" }, \
{ "SdkEvalSpiWriteFifo", SdkEvalSpiWriteFifoAction,             "b",  "\r\nData to write in TX FIFO"}, \
{ "S2MGpioIrqConfiguration",  S2MGpioIrqConfigurationAction,    "uu", "\r\nGPIO (0,1,2,3)\r\nenable/disable (1,0)"}, \
{ "SdkEvalSdn",           SdkEvalSdnAction,             "u",   "\r\n0 exit shutdown\r\n1 enter shutdown"}, \
{ "SdkEvalRfboardIdentification",           SdkEvalRfboardIdentificationAction,             "w",   "\r\nxtal frequency in MHz or 0 if auto detection"}, \
{ "FEMSetBypass",              FEMSetBypassAction,                "u",   "\r\nBypass mode\r\n1: YES, 0: NO"}, \
{ "SdkEvalLedHandler",       SdkEvalLedHandlerAction,         "uu",   "\r\nLed number\r\n1: ON, 0: OFF"}, \
{ "EepromReadPage",          EepromReadPageAction,             "vuu",  "\r\nPage number from 0 to 255.\r\nAddress offset from 0 to 31.\r\nNumber of bytes to read."}, \
{ "SdkEvalGetVersion", SdkEvalGetVersionAction, "", "to know the motherboard version"}

void SdkEvalSpiCommandStrobesAction(void);
void SdkEvalSpiReadRegistersAction(void);
void SdkEvalSpiWriteRegistersAction(void);
void SdkEvalSdnAction(void);
void SdkEvalRfboardIdentificationAction(void);
void SdkEvalSpiReadFifoAction(void);
void SdkEvalSpiWriteFifoAction(void);
void SdkEvalLedHandlerAction(void);
void S2MGpioIrqConfigurationAction(void);
void CliResetTimerAction(void);
void EepromReadPageAction(void);
void SdkEvalGetVersionAction(void);
uint32_t SdkEvalGetTimerValue(void);
void SdkEvalUpdateTimerValue(void);
void TimeMeasure(uint8_t gpio, uint32_t time_stamp);
void FEMSetBypassAction(void);

#endif /*__SDK_CLI_COMMANDS_H__*/

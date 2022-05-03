
/**
  ******************************************************************************
  * @file    SDK_CLI_Commands.c
  * @author  STMicroelectronics
  * @date    March 2022
  * @brief   SDK CLI Commands source file
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
#include "SDK_CLI_commands.h"
#include "utils.h"
#include "S2LP_CLI_commands.h"
#include "command-interpreter2.h"
#include "crc_lib.h"

/*Load S2LP Middleware layer*/
#include "SDK_UTILS_Timers.h"
#include "SDK_UTILS_Time_Measure_Service.h"
#include "serial_utils.h"

#include "s2lp_management.h"
#include "s2lp_sdkapi_mapping.h"
#include "stm32l0xx_nucleo.h"
#include "s2868a1.h"
#include "S2LP_Nucleo_STM32_AUTO.h"

extern uint8_t responsePrintf(const char * formatString, ...);
extern uint8_t gpio_timer_mask;
static volatile uint8_t stopCmdFlag=0;

#ifndef M2S_GPIO_EXTI_PREEMPTION_PRIORITY
	#define M2S_GPIO_EXTI_PREEMPTION_PRIORITY 0x01
#endif

#if (defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
    #define S2LP_Middleware_GpioInterruptCmd(pA, pB, pC, pD) S2LP_Middleware_GpioInterruptCmd(pA, pB, pC, pD, M2S_GPIO_EDGE_EVENT)
#endif

#define SdkDelayMs(...) HAL_Delay(__VA_ARGS__)

/**
 * @addtogroup CLI
 * @{
 */

/**
 * @defgroup SDK_EVAL_CLI_Functions                   SDK-EVAL Routines
 * @brief  This module contains a set of routines related to the motherboard.
 * @{
 */

/**
 * @brief  Send a command to the device.
 *         <p><b>HowToCall</b>: S2LPSpiCommandStrobes u</p>
 * @param  command_code
 *         For S2-LP:
 * <ul><li> 0x60 : CMD_TX - Start to transmit; valid only from READY</li>
 *     <li> 0x61: CMD_RX - Start to receive; valid only from READY</li>
 *     <li> 0x62: CMD_READY - Go to READY; valid only from STANDBY or SLEEP or LOCK</li>
 *     <li> 0x63: CMD_STANDBY - Go to STANDBY; valid only from READY</li>
 *     <li> 0x64: CMD_SLEEP - Go to SLEEP; valid only from READY</li>
 *     <li> 0x65: CMD_LOCKRX - Go to LOCK state by using the RX configuration of the synth; valid only from READY</li>
 *     <li> 0x66: CMD_LOCKTX - Go to LOCK state by using the TX configuration of the synth; valid only from READY</li>
 *     <li> 0x67: CMD_SABORT - Force exit form TX or RX states and go to READY state; valid only from TX or RX</li>
 *     <li> 0x68: CMD_LDC_RELOAD - LDC Mode: Reload the LDC timer with the value stored in the  LDC_PRESCALER / COUNTER  registers; valid from all states</li>
 *     <li> 0x69: CMD_RCO_CALIB - Start (or re-start) the RCO calibration</li>
 *     <li> 0x70: CMD_SRES - Reset of all digital part, except SPI registers</li>
 *     <li> 0x71: CMD_FLUSHRXFIFO - Clean the RX FIFO; valid from all states</li>
 *     <li> 0x72: CMD_FLUSHTXFIFO - Clean the TX FIFO; valid from all states</li>
 *     <li> 0x73: CMD_SEQUENCE_UPDATE - Autoretransmission: Reload the Packet sequence counter with the value stored in the PROTOCOL[2] register valid from all states</li>
 * </ul>
 * @retval timer CLI_timer
 */
void SdkEvalSpiCommandStrobesAction(void)
{
  uint8_t  cCommandCode = unsignedCommandArgument(0);

  switch (cCommandCode)
  {
  case CMD_TX:
    {
	CMD_STROBE_TX();
	break;
    }
  case CMD_RX:
    {
	CMD_STROBE_RX();
	break;
    }
  case CMD_SRES:
    {
	CMD_STROBE_SRES();
	break;
    }
  case CMD_SABORT:
    {
	CMD_STROBE_SABORT();
	break;
    }
  default:
    {
	S2LPSpiCommandStrobes(cCommandCode);
    }
  }

  SdkEvalUpdateTimerValue();

  responsePrintf("{&N API call...&t4x}\r\n",
		     "SdkEvalSpiCommandStrobes",
		     "timer",SdkEvalGetTimerValue());
}

/**
 * @brief  Get the motherboard version.
 *         <p><b>HowToCall</b>:  SdkEvalGetVersion</p>
 * @details This value can be:
 *      <ul>
 *      <li> 0x00: IKR001 motherboard</li>
 *      <li> 0x01: IKR002 motherboard</li>
 *      <li> 0x02: IDS001 USB dongle</li>
 *      <li> 0x10: BlueNRG-1 Motherboard</li>
 *      <li> 0x20: BlueNRG-2 Motherboard</li>
 *      <li> 0x21: BlueNRG-2 Monarch Reference Design Motherboard</li>
 *      <li> 0x80: STM32L0-NUCLEO</li>
 *      <li> 0x80: STM32L1-NUCLEO</li>
 *      </ul>
 * @param  None
 * @retval version.
 */
void SdkEvalGetVersionAction(void)
{
  uint8_t cVer=BOARD_VERSION;

  responsePrintf("{&N API call... &tx}\r\n",
		 "SdkEvalGetVersion", "version",cVer);
}

/**
 * @brief  Read register(s) from the device.
 *         <p><b>HowToCall</b>: S2LPSpiReadRegisters u u</p>
 * @param  address register address.
 * @param  n_regs number of registers to read.
 * @retval values_list list of the registers values.
 * @retval timer CLI_timer
 */
void SdkEvalSpiReadRegistersAction(void)
{
  uint8_t buffer[255];

  uint8_t  cRegAddress = unsignedCommandArgument(0);
  uint8_t cNbBytes = unsignedCommandArgument(1);

  S2LPSpiReadRegisters(cRegAddress, cNbBytes, buffer);

  SdkEvalUpdateTimerValue();

  responsePrintf("{&N API callback...\r\n", "SdkEvalSpiReadRegisters");
  responsePrintf("{%t: ","regs_list");

  for(uint8_t i = 0; i < cNbBytes-1; i++)
  {
    responsePrintf("0x%x,0x%x,",cRegAddress, buffer[i]);
    cRegAddress++;
  }
  responsePrintf("0x%x,0x%x",cRegAddress, buffer[cNbBytes-1]);

  responsePrintf("}\r\n");
  responsePrintf("&t4x\r\n",
                 "timer",SdkEvalGetTimerValue());
  responsePrintf("}\r\n");

}

/**
 * @brief  Write register(s) to the device.
 *         <p><b>HowToCall</b>: S2LPSpiReadRegisters u b</p>
 * @param  address register address.
 * @param  values values to write as a block.
 * @retval timer CLI_timer
 */
void SdkEvalSpiWriteRegistersAction(void)
{
  uint8_t buffer[255];
  uint8_t  cRegAddress = unsignedCommandArgument(0);

  uint8_t cNbBytes = copyStringArgument(1, buffer, 255, 0);

  S2LPSpiWriteRegisters(cRegAddress, cNbBytes, buffer);

  SdkEvalUpdateTimerValue();
  responsePrintf("{&N API call...&t4x}\r\n",
		 "SdkEvalSpiWriteRegisters",
                 "timer",SdkEvalGetTimerValue());
}

/**
 * @brief  Read the RX FIFO.
 *         <p><b>HowToCall</b>: SdkEvalSpiReadFifoAction u</p>
 * @param  n_bytes number of bytes to read from the RX FIFO.
 * @retval values read values
 * @retval timer CLI_timer
 */
void SdkEvalSpiReadFifoAction(void)
{
  uint8_t buffer[255];
  uint8_t cNbBytes = unsignedCommandArgument(0);

  S2LPSpiReadFifo(cNbBytes, buffer);

  SdkEvalUpdateTimerValue();

  responsePrintf("{&N API call...\r\n", "SdkEvalSpiReadFifo");
  responsePrintf("{%t: ","bytes");

  for(uint8_t i = 0; i < cNbBytes-1; i++)
  {
    responsePrintf("0x%x,", buffer[i]);
  }
  responsePrintf("0x%x", buffer[cNbBytes-1]);

  responsePrintf("}\r\n");
  responsePrintf("&t4x\r\n",
                 "timer",SdkEvalGetTimerValue());
  responsePrintf("}\r\n");
}

/**
 * @brief  Write the TX FIFO.
 *         <p><b>HowToCall</b>: SdkEvalSpiWriteFifoAction b</p>
 * @param  values values to write to FIFO as a block.
 * @retval timer CLI_timer
 */
void SdkEvalSpiWriteFifoAction(void)
{
  uint8_t buffer[255];
  uint8_t cNbBytes = copyStringArgument(0, buffer, 255, 0);

  S2LPSpiWriteFifo(cNbBytes, buffer);

  SdkEvalUpdateTimerValue();
  responsePrintf("{&N API call...&t4x}\r\n",
		 "SdkEvalSpiWriteFifo",
                 "timer",SdkEvalGetTimerValue());

}

/**
 * @brief  Drive the shutdown pin.
 *         <p><b>HowToCall</b>: SdkEvalSdn u</p>
 * @param  value put in SDN (1, pin high), exit from SDN (0, pin low)
 * @retval None
 */
void SdkEvalSdnAction(void)
{
  uint8_t value = unsignedCommandArgument(0);

  if(value==1) {
    S2LPShutdownInit();
  }
  else{
    S2LPShutdownExit();
  }

  responsePrintf("{&N API call...&t4x}\r\n", "SdkEvalSdn", "timer", SdkEvalGetTimerValue());
}

/**
 * @brief  Identify the RF board getting some info from the EEPROM and configuring the library.
 *              If EEPROM is not present, this function will measure the XTAL frequency using a uC timer.
 *         <p><b>HowToCall</b>: SdkEvalRfboardIdentification u</p>
 * @param  xtal if this value is 0, the XTAL will be automatically detected, else it will be set to this value.
 * @retval None
 */
void SdkEvalRfboardIdentificationAction(void)
{
  uint32_t xtal = unsignedCommandArgument(0);

  S2LPManagementIdentificationRFBoard();
  if(xtal!=0)
  {
    if(xtal<100) xtal=xtal*1000000;
    S2LPRadioSetXtalFrequency(xtal);
  }

  /* Microcontroller EXTI configuration */
  S2LP_Middleware_GpioInit(M2S_GPIO_3, M2S_MODE_GPIO_IN);
  S2LP_Middleware_GpioInit(M2S_GPIO_2, M2S_MODE_GPIO_IN);
  S2LP_Middleware_GpioInit(M2S_GPIO_1, M2S_MODE_GPIO_IN);
  S2LP_Middleware_GpioInit(M2S_GPIO_0, M2S_MODE_GPIO_IN);

#if  (defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
  SdkEvalComIOConfig(SdkEvalComIOProcessInputData);
#endif

  responsePrintf("{&N API call...}\r\n", "SdkEvalRfboardIdentification");
}

/**
 * @brief  Set on or off a led.
 *         <p><b>HowToCall</b>: SdkEvalLedHandler u u</p>
 * @param  led This value can be:
 *       <ul>
 *          <li>0: LED1</li><li>1: LED2</li>
 *          <li>2: LED3</li><li>3: LED4</li><li>4: LED5</li>
 *       </ul>
 * @param  val This value can be 1 or 0
 * @retval timer CLI_timer
 */
void SdkEvalLedHandlerAction(void)
{
  uint8_t cLedNumber = unsignedCommandArgument(0)-1;
  uint8_t cLedStatus = unsignedCommandArgument(1);
  UNUSED(cLedNumber);

  BSP_LED_Init(LED2);

  if(cLedStatus == 1) {
	BSP_LED_On(LED2);
  }
  else if(cLedStatus == 0) {
    BSP_LED_Off(LED2);
  }

  SdkEvalUpdateTimerValue();
  responsePrintf("{&N API call...&t4x}\r\n", "SdkEvalLedHandler", "timer",SdkEvalGetTimerValue());
}

/**
 * @brief  Configure a GPIO of the uC in EXTI (external interrupt) mode.
 *         <p><b>HowToCall</b>: S2MGpioIrqConfiguration u u</p>
 * @param  gpio This value can be:
 *       <ul>
 *          <li>0: GPIO0</li><li>1: GPIO1</li>
 *          <li>2: GPIO2</li><li>3: GPIO3</li>
 *       </ul>
 * @param  en enable (1), disable (0).
 * @retval None.
 */
void S2MGpioIrqConfigurationAction(void)
{
  M2SGpioPin xM2SGpio = (M2SGpioPin)(unsignedCommandArgument(0));
  FunctionalState xState = (FunctionalState)(unsignedCommandArgument(1));

  if(xState == ENABLE)
  {
    S2LP_Middleware_GpioInit(xM2SGpio, M2S_MODE_EXTI_IN);
    S2LP_Middleware_GpioInterruptCmd(xM2SGpio, M2S_GPIO_EXTI_PREEMPTION_PRIORITY, 0, xState);
  }
  else
  {
    S2LP_Middleware_GpioInit(xM2SGpio, M2S_MODE_GPIO_IN);
    S2LP_Middleware_GpioInterruptCmd(xM2SGpio, M2S_GPIO_EXTI_PREEMPTION_PRIORITY, 0, xState);
  }

  responsePrintf("{&N API call...}\r\n", "S2MGpioIrqConfiguration");
}

/**
 * @brief  Read a value or a list of values from the manufactory EEPROM.
 *         <p><b>HowToCall</b>: EepromReadPage v u u</p>
 * @param  page page number
 * @param  offset offset to start to read from inside the page.
 * @param  n_bytes number of bytes to read.
 * @retval list of the read values.
 */
void EepromReadPageAction(void)
{
  if(EepromIdentification()) {
    uint8_t buffer[32];
    uint16_t cPageNum = unsignedCommandArgument(0);
    uint8_t cAddressOffset = unsignedCommandArgument(1);
    uint8_t cNbBytes = unsignedCommandArgument(2);

    uint16_t  cAddressLocation = cPageNum*32 + cAddressOffset;

    if((cAddressOffset+cNbBytes)>32)
    {
      cNbBytes = 32-cAddressOffset;
    }

    if((S2LPManagementGetCut()==S2LP_CUT_2_0) || (S2LPManagementGetCut()==S2LP_CUT_2_1))
    {
      S2LPShutdownInit();
      SdkDelayMs(10);
    }

    S2868A1_EEPROM_ReadPage(EEPROM_INSTANCE, cAddressLocation, cNbBytes, buffer);
    if((S2LPManagementGetCut()==S2LP_CUT_2_0) || (S2LPManagementGetCut()==S2LP_CUT_2_1))
    {
      S2LPShutdownExit();
    }

#if  (defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
    SdkEvalComIOConfig(SdkEvalComIOProcessInputData);
#endif

    responsePrintf("{&N API callback...\r\n", "EepromReadPage");
    responsePrintf("{%t: ","Data");

    for(uint8_t i = 0; i < cNbBytes-1; i++)
    {
      responsePrintf("0x%x,",buffer[i]);
    }
    responsePrintf("0x%x",buffer[cNbBytes-1]);

    responsePrintf("}}\r\n");
  }
  else {
    responsePrintf("{&N API callback...\r\n", "EepromReadPage");
    responsePrintf("{%t: 0x00}}\r\n","Data");
  }
}

/**
 * @brief  Set the FEM in Bypass mode
 *         <p><b>HowToCall</b>:  FEMSetBypass u</p>
 * @param  bypass
 *        <ul>
 *          <li>0: Bypass NO</li>
 *          <li>1: Bypass YES</li>
 *       </ul>
 * @retval None.
 */
void FEMSetBypassAction(void)
{
  uint8_t bypassMode = unsignedCommandArgument(0);
  UNUSED(bypassMode);

  responsePrintf("{&N API call... }\r\n","FEMSetBypass");
}
/**
 *@}
 */

/**
 *@}
 */

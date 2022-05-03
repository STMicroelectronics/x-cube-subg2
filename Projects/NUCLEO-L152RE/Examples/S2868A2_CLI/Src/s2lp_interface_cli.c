
/**
  ******************************************************************************
  * @file    s2lp_interface_cli.c
  * @author  SRA Application Team
  * @brief   Source file for S2LP radio configuration/driver
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

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "s2lp_interface_cli.h"
#include "s2lp_management.h"
#include "stm32l1xx_nucleo.h"
#include "s2868a2.h"
#include "s2lp.h"
#include "subg2_bsp_ip_conf.h"

#include "SDK_UTILS_Time_Measure_Service.h"
#include "SDK_UTILS_Timers.h"
#include "utils.h"
#include "S2LP_CLI_commands.h"

/** @defgroup S2LP_Nucleo
  * @{
  */

/** @defgroup Main
  * @{
  */
 /* Extern variables-----------------------------------------------------------*/
extern uint8_t gpio_timer_mask;
extern IrqQueue s_xIrqQueue;
extern volatile FlagStatus xIRQRaised;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GpioIrqHandler *GpioIrq[] = {

#if (USE_S2868A2_RADIO_GPIO_0 == 1)
  Radio_interrupt_callback_0,
#endif
#if (USE_S2868A2_RADIO_GPIO_1 == 1)
  Radio_interrupt_callback_1,
#endif
#if (USE_S2868A2_RADIO_GPIO_2 == 1)
  Radio_interrupt_callback_2,
#endif
#if (USE_S2868A2_RADIO_GPIO_3 == 1)
  Radio_interrupt_callback_3,
#endif
};
/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
void S2LPInterfaceInit(void)
{
  /* Initialize the SDN pin micro side */
  S2868A2_RADIO_Init();

  if( S2LP_Init() != S2LP_OK)
   {
    /* Initialization Error */
     while(1){};
   }

  /* EepromSpiInitialization(); */
  S2868A2_EEPROM_Init(EEPROM_INSTANCE);

  /* S2LP ON */
  S2868A2_RADIO_EnterShutdown();
  S2868A2_RADIO_ExitShutdown();

  S2LP_ManagementIdentificationRFBoard();

  /* if needed this will set the range extender pins */
  S2LP_ManagementRangeExtInit();

  /* uC IRQ enable */
  S2868A2_RADIO_IoIrqEnable(GpioIrq);
}
/*----------------------------------------------------------------------------*/
void S2LP_Interface_IoIrqEnable(void)
{
  (void) S2868A2_RADIO_IoIrqEnable(GpioIrq);
}
/*----------------------------------------------------------------------------*/
void S2LP_Interface_IoIrqDisable(void)
{
  (void) S2868A2_RADIO_IoIrqDisable(GpioIrq);
}
/*----------------------------------------------------------------------------*/
void Radio_interrupt_callback_0(void)
{
	uint32_t timer = SdkEvalGetTimerValue();
	s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPush].timer = timer;

	if(gpio_timer_mask & 0x3)
	  TimeMeasure(S2LP_GPIO_0, timer);
	else
	  xIRQRaised = SET;
}
/*----------------------------------------------------------------------------*/
void Radio_interrupt_callback_1(void)
{
	uint32_t timer = SdkEvalGetTimerValue();
	s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPush].timer = timer;

	if(gpio_timer_mask & 0xC)
	  TimeMeasure(S2LP_GPIO_1, timer);
	else
	  xIRQRaised = SET;
}
/*----------------------------------------------------------------------------*/
void Radio_interrupt_callback_2(void)
{
	uint32_t timer = SdkEvalGetTimerValue();
	s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPush].timer = timer;

	if(gpio_timer_mask & 0x30)
	  TimeMeasure(S2LP_GPIO_2, timer);
	else
	  xIRQRaised = SET;
}
/*----------------------------------------------------------------------------*/
void Radio_interrupt_callback_3(void)
{
	uint32_t timer = SdkEvalGetTimerValue();
	s_xIrqQueue.xIrqQueue[s_xIrqQueue.cIndexPush].timer = timer;

    if(gpio_timer_mask & 0xC0)
      TimeMeasure(S2LP_GPIO_3, timer);
    else
      xIRQRaised = SET;
}
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif


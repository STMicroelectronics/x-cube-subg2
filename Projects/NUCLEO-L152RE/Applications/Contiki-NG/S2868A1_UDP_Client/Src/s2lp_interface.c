
/**
  ******************************************************************************
  * @file    s2lp_interface.c
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
#include "s2lp_interface.h"
#include "s2lp_management.h"
#include "radio-driver.h"
#include "stm32l1xx_nucleo.h"
#include "s2868a1.h"
#include "s2lp.h"
#include "subg2_bsp_ip_conf.h"
#include "contiki-platform.h"

#include "stm32l1xx_nucleo.h"

/** @defgroup S2LP_Nucleo
  * @{
  */

/** @defgroup Main
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static GpioIrqHandler *GpioIrq[] = {

#if (USE_S2868A1_RADIO_GPIO_0 == 1)
  Radio_interrupt_callback,
#endif
#if (USE_S2868A1_RADIO_GPIO_1 == 1)
  Radio_interrupt_callback,
#endif
#if (USE_S2868A1_RADIO_GPIO_2 == 1)
  Radio_interrupt_callback,
#endif
#if (USE_S2868A1_RADIO_GPIO_3 == 1)
  Radio_interrupt_callback,
#endif
};
/* Global variables ----------------------------------------------------------*/
extern SRadioInit xRadioInit;

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
  S2868A1_RADIO_Init();

  if( S2LP_Init() != S2LP_OK)
   {
    /* Initialization Error */
     while(1){};
   }

  /* EepromSpiInitialization(); */
  S2868A1_EEPROM_Init(EEPROM_INSTANCE);

  /* S2LP ON */
  S2868A1_RADIO_EnterShutdown();
  S2868A1_RADIO_ExitShutdown();

  S2LP_ManagementIdentificationRFBoard();

  /* if the board has eeprom, we can compensate the offset calling S2LP_ManagementGetOffset
  (if eeprom is not present this fcn will return 0) */
  xRadioInit.lFrequencyBase = (uint32_t) BASE_FREQUENCY + S2LP_ManagementGetOffset();

  /* if needed this will set the range extender pins */
  S2LP_ManagementRangeExtInit();

  /* uC IRQ enable */
  S2868A1_RADIO_IoIrqEnable(GpioIrq);
}
/*----------------------------------------------------------------------------*/
void S2LP_Interface_IoIrqEnable(void)
{
  (void) S2868A1_RADIO_IoIrqEnable(GpioIrq);
}
/*----------------------------------------------------------------------------*/
void S2LP_Interface_IoIrqDisable(void)
{
  (void) S2868A1_RADIO_IoIrqDisable(GpioIrq);
}
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif


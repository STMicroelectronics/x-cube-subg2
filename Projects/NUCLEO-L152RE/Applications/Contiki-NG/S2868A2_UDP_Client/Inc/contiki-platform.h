/**
  ******************************************************************************
  * @file    contiki-platform.h
  * @author  SRA Application Team
  * @brief   Contiki-NG main header file for ST platform
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

/**
 * @addtogroup Contiki-NG_STM32_Library
 * @{
 * @file Header file for the Contiki-NG main
 */

/*---------------------------------------------------------------------------*/
#ifndef CONTIKI_PLATFORM_H_
#define CONTIKI_PLATFORM_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

void MX_SUBG2_Contiki_NG_Init(void);
void MX_SUBG2_Contiki_NG_Process(void);
void Stack_6LoWPAN_Init(void);
void uart_set_input(uint8_t uart, int (* input)(unsigned char c));
void platform_idle(void);
void UART_Platform_Transmit(uint8_t * msg);
void UART_Platform_init(void);

extern int (* input_handler)(unsigned char c);
/*---------------------------------------------------------------------------*/
#endif /*CONTIKI_PLATFORM_H_*/
/*---------------------------------------------------------------------------*/
/** @} */


/**
  ******************************************************************************
  * @file    contiki-platform.h
  * @author  SRA Application Team
  * @brief   Contiki-NG main header file for ST platform
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

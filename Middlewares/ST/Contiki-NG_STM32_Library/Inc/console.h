/**
  ******************************************************************************
  * @file    console.h
  * @author  SRA Application Team
  * @brief   Header for console.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Software License Agreement
  * SLA0055, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0055
  *
  ******************************************************************************
  */
  
#ifndef CONSOLE_H_
#define CONSOLE_H_
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t uart_send_char(uint8_t ch);

uint8_t uart_receive_char(void);
#endif /*CONSOLE_H_*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

 /**
  ******************************************************************************
  * @file    console.c
  * @author  SRA Application Team
  * @brief   This file provides implementation of standard input/output
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include "main.h"
#include "subg2_bsp_ip_conf.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/** @brief Sends a character to serial port
 * @param ch Character to send
 * @retval Character sent
 */
uint8_t uart_send_char(uint8_t ch)
{
  (void) HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
/*---------------------------------------------------------------------------*/
/** @brief Receives a character from serial port
 * @retval Character received
 */
uint8_t uart_receive_char(void)
{
  uint8_t ch;
  (void) HAL_UART_Receive(&UartHandle, &ch, 1, HAL_MAX_DELAY);
	
  /* Echo character back to console */
  (void) HAL_UART_Transmit(&UartHandle, &ch, 1, HAL_MAX_DELAY);

  /* And cope with Windows */
  if(ch == (uint8_t) '\r'){
    uint8_t ret = (uint8_t) '\n';
    (void) HAL_UART_Transmit(&UartHandle, &ret, 1, HAL_MAX_DELAY);
  }

  return ch;
}
/*---------------------------------------------------------------------------*/
#if defined (__IAR_SYSTEMS_ICC__)
size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize);
size_t __read(int Handle, unsigned char *Buf, size_t Bufsize);

/** @brief IAR specific low level standard input
 * @param handle IAR internal handle
 * @param buf Buffer where to store characters read from stdin
 * @param bufsize Number of characters to read
 * @retval Number of characters read
 */
/* MISRA C-2012 Dir-4.6_a violation for purpose */
size_t __read(int handle, unsigned char *buf, size_t bufsize)
{
  uint32_t i;
  size_t retval = (size_t) -1;
  
  if (handle == 0){
    retval = bufsize;
    for(i=0; i<bufsize; i++) {
      buf[i] = uart_receive_char();
    }
  }
  
  return retval;
}

/** @brief IAR specific low level standard output
 * @param handle IAR internal handle
 * @param buf Buffer containing characters to be written to stdout
 * @param bufsize Number of characters to write
 * @retval Number of characters read
 */
/* MISRA C-2012 Dir-4.6_a violation for purpose */
size_t __write(int handle, const unsigned char * buf, size_t bufsize)
{
  uint32_t i;
  size_t retval = (size_t) -1;
  
  if ((handle == 1) || (handle == 2)) {
    retval = bufsize;
    for(i=0; i< bufsize; i++) {
      (void) uart_send_char(buf[i]);
    }
  }  
  return retval;
}
/*---------------------------------------------------------------------------*/
#elif defined (__CC_ARM)
#if (USE_COM_LOG == 0)
/**
 * @brief fputc call for standard output implementation
 * @param ch Character to print
 * @param f File pointer
 * @retval Character printed
 */
int fputc(int ch, FILE *f)
{
  return uart_send_char(ch);
}

/** @brief fgetc call for standard input implementation
 * @param f File pointer
 * @retval Character acquired from standard input
 */
int fgetc(FILE *f)
{
  return uart_receive_char();
}
#endif /*(USE_COM_LOG == 0)*/
/*---------------------------------------------------------------------------*/
#elif defined (__GNUC__)
#if (USE_COM_LOG == 0)
/** @brief putchar call for standard output implementation
 * @param ch Character to print
 * @retval Character printed
 */
int __io_putchar(int ch)
{
  return uart_send_char(ch);
}

/** @brief getchar call for standard input implementation
 * @param None
 * @retval Character acquired from standard input
 */
int __io_getchar(void)
{
  return uart_receive_char();
}
#endif /*(USE_COM_LOG == 0)*/
/*---------------------------------------------------------------------------*/
#else /*!__IAR_SYSTEMS_ICC__ && !__CC_ARM && !__GNUC__*/
#error "Toolchain not supported"
#endif /*__IAR_SYSTEMS_ICC__ || __CC_ARM || __GNUC__*/
/*---------------------------------------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


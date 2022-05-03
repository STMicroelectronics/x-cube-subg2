/**
* @file    SDK_EVAL_Com_DMA.c
* @author  VMA division - AMS
* @version 3.2.1
* @date    February, 2020
* @brief   This file provides all the low level API to manage SDK UART.
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
* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
*/

/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_Com.h"
#include "subg2_bsp_ip_conf.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "Platform_Configuration_CLI.h"

#ifdef __ICCARM__
	#include <yfuns.h>
	#include <stdint.h>
#else
	#include <stdio.h>
	/* Keil debug port defines */
	#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
	#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
	#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
	#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
	#define TRCENA          0x01000000

#if defined(__CC_ARM) /*ARM Compiler 5*/
	struct __FILE { int handle; /* Add whatever needed */ };
#endif
#if !defined(__CC_ARM) && !defined(__ARMCC_VERSION)
	//GCC compiler
	FILE __stdout;
#endif
	FILE __stdin;
#endif

/** @addtogroup SDK_EVAL_NUCLEO
* @{
*/

/** @addtogroup SDK_EVAL_Com
* @{
*/

/** @defgroup SDK_EVAL_Com_Private_Variables                    SDK EVAL Com Private Variables
* @{
*/

#define RECEIVE_QUEUE_SIZE                      NUCLEO_UARTx_RX_QUEUE_SIZE
#define TRANSMIT_QUEUE_SIZE                     NUCLEO_UARTx_TX_QUEUE_SIZE

#if !defined(NUCLEO_UARTx_TX_DMA_CHANNEL_IRQn) || !defined(NUCLEO_UARTx_RX_DMA_CHANNEL_IRQn)
#error "Please enable DMA for USART TX and RX"
#endif /*DMA is not set for USART*/

#define UART_ENTER_CRITICAL()          {NVIC_DisableIRQ(NUCLEO_UARTx_IRQn); HAL_NVIC_DisableIRQ(NUCLEO_UARTx_TX_DMA_CHANNEL_IRQn);}
#define UART_EXIT_CRITICAL()           {NVIC_EnableIRQ(NUCLEO_UARTx_IRQn); HAL_NVIC_EnableIRQ(NUCLEO_UARTx_TX_DMA_CHANNEL_IRQn);}

uint8_t rxQ[RECEIVE_QUEUE_SIZE];
uint16_t rxHead = 0;
uint16_t rxTail = 0;
volatile uint16_t rxUsed = 0;

uint8_t txQ[TRANSMIT_QUEUE_SIZE];
uint16_t txHead = 0;
uint16_t txTail = 0;
volatile uint16_t txUsed = 0,txLastRequest=0;
uint8_t dmaTransmitting = 0;

/**
* @}
*/

/** @defgroup SDK_EVAL_Com_Private_Functions                            SDK EVAL Com Private Functions
* @{
*/

static void prepareDmaTx(void);

/**
* @brief  Configures UART port in DMA mode for both RX and TX.
* @param  None.
* @retval None.
*/
void SdkEvalComInit(void)
{
  UartHandle.pRxBuffPtr = (uint8_t *)rxQ; /* MISRA C-2012 rule 11.8 violation for purpose */
  UartHandle.RxXferSize = RECEIVE_QUEUE_SIZE;
  UartHandle.ErrorCode = (uint32_t)HAL_UART_ERROR_NONE;

  /* Enable the DMA transfer for the receiver request by setting the DMAR bit
     in the UART CR3 register */
  /* MISRA C-2012 rule 11.8 violation for purpose */
  (void) HAL_UART_Receive_DMA(&UartHandle,rxQ,RECEIVE_QUEUE_SIZE);
}

void SdkEvalComTriggerTx(void)
{
  prepareDmaTx();
}

void updatetxQ(void)
{
  UART_ENTER_CRITICAL();
  uint16_t  _temp;
  uint16_t dmaResidual=__HAL_DMA_GET_COUNTER(UartHandle.hdmatx);

  txTail=(txTail+(txLastRequest-dmaResidual))%TRANSMIT_QUEUE_SIZE;
  _temp = txLastRequest-dmaResidual;
  txUsed-=_temp;
  txLastRequest=dmaResidual;
  UART_EXIT_CRITICAL();
}

void enqueueTxChars(const unsigned char * buffer, uint16_t size)
{
  while ( size > 0 ) {

    while(txUsed>TRANSMIT_QUEUE_SIZE-size)
    {
      updatetxQ();
    }

    txQ[txHead] = *buffer++;
    UART_ENTER_CRITICAL();
    txUsed++;
    UART_EXIT_CRITICAL();
    txHead = (txHead+1) % TRANSMIT_QUEUE_SIZE;
    size--;
  }
  prepareDmaTx();
}

void SdkEvalComBaudrate(uint32_t baudrate)
{
  UartHandle.Init.BaudRate=baudrate;
  __HAL_UART_DISABLE(&UartHandle);
  HAL_UART_Init(&UartHandle);
  __HAL_UART_ENABLE(&UartHandle);
}

static void prepareDmaTx(void)
{
  if(!dmaTransmitting && txUsed!=0)
  {
    UART_ENTER_CRITICAL();
    dmaTransmitting=1;

    if(txTail+txUsed<TRANSMIT_QUEUE_SIZE)
    {
      txLastRequest=txUsed;
    }
    else
    {
      txLastRequest=(TRANSMIT_QUEUE_SIZE-txTail);
    }

    if(HAL_UART_Transmit_DMA(&UartHandle,&txQ[txTail],txLastRequest)==HAL_OK)
    {

    }

    UART_EXIT_CRITICAL();
  }
}

#ifdef __ICCARM__
  // IAR Standard library hook for serial output
  size_t cli_write(int handle, const unsigned char * buffer, size_t size)
  {
    /* This template only writes to "standard out" and "standard err",
     * for all other file handles it returns failure. */
    if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR) {
      return _LLIO_ERROR;
    }
    enqueueTxChars(buffer,size);

    return size;
  }
#else /*!__ICCARM__*/

  uint8_t __io_getcharNonBlocking(uint8_t *data)
  {
    int c = fgetc(&__stdin);

    if (c == EOF)
      return 0;
    else {
      *data = (uint8_t)c;
      return 1;
    }
  }

  ErrorStatus Read_Buffer_Pop(uint8_t *byte)
  {
    int nChars = 0;

    if(UartHandle.hdmarx)
    {
		rxHead=(RECEIVE_QUEUE_SIZE - __HAL_DMA_GET_COUNTER(UartHandle.hdmarx))%RECEIVE_QUEUE_SIZE;
        if(rxHead>=rxTail)
	      rxUsed=rxHead-rxTail;
        else
	      rxUsed=RECEIVE_QUEUE_SIZE-rxTail+rxHead;
    }

    for(nChars = 0; (rxUsed>0) && (nChars < 1); nChars++) {
      *byte++ = rxQ[rxTail];
      rxTail = (rxTail+1) % RECEIVE_QUEUE_SIZE;
      rxUsed--;
    }

    return (ErrorStatus)nChars;
  }

  int fgetc (FILE *f) {
    int data = -1;
    Read_Buffer_Pop((uint8_t *)&data);
    return data;
  }

 void __io_flush( void )
  {
    fputc(0, &__stdout);
  }

 int cli_fputc(int c, FILE *f)
  {
    updatetxQ();
    const unsigned char c1 = c;
    enqueueTxChars(&c1,1);

    return 1;
  }

#if defined (__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) /* For ARM Compiler 5 and 6*/
  void __io_putchar( char c )
  {
    cli_fputc(c, &__stdout);
  }
#else /*GCC based*/
  int _write(int file, char *ptr, int len)
  {
  	int DataIdx;

  	for (DataIdx = 0; DataIdx < len; DataIdx++)
  	{
  		cli_fputc(*ptr++, &__stdout);
  	}
  	return len;
  }

#endif /*__CC_ARM*/
#endif /*__ICCARM__*/

/**
  * @brief  Tx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  uint16_t  _temp;

  if (huart->Instance == UartHandle.Instance)
  {
	  txTail=(txTail+txLastRequest)%TRANSMIT_QUEUE_SIZE;

	  _temp = txLastRequest;
	  txUsed-=_temp;

	  txLastRequest=0;
	  dmaTransmitting=0;

	  prepareDmaTx();
  }
}

#ifdef __ICCARM__
  // IAR Standard library hook for serial input
  size_t __read(int handle, unsigned char * buffer, size_t size)
  {
    int nChars = 0;

    /* This template only reads from "standard in", for all other file
    * handles it returns failure. */
    if (handle != _LLIO_STDIN)
    {
      return _LLIO_ERROR;
    }

    if(UartHandle.hdmarx)
    {
	  rxHead=(RECEIVE_QUEUE_SIZE- __HAL_DMA_GET_COUNTER(UartHandle.hdmarx))%RECEIVE_QUEUE_SIZE;

      if(rxHead>=rxTail)
	    rxUsed=rxHead-rxTail;
      else
	    rxUsed=RECEIVE_QUEUE_SIZE-rxTail+rxHead;
    }

    for(nChars = 0; (rxUsed>0) && (nChars < size); nChars++) {
      *buffer++ = rxQ[rxTail];
      rxTail = (rxTail+1) % RECEIVE_QUEUE_SIZE;
      rxUsed--;
    }

    return nChars;
  }

  uint8_t __io_getcharNonBlocking(uint8_t *data)
  {
    if (__read(_LLIO_STDIN,data,1))
      return 1;
    else
      return 0;
  }

  void __io_putchar( char c )
  {
    cli_write(_LLIO_STDOUT, (unsigned char *)&c, 1);
  }

  int __io_getchar()
  {
    unsigned char c;
    __read(_LLIO_STDIN, &c, 1);
    return (int)(c);
  }

  void __io_flush( void )
  {
    __write(_LLIO_STDOUT, NULL, 0);
  }
#endif /*__ICCARM__*/

void enqueueRxChars(unsigned char * buffer, uint16_t size)
{
  while (( size > 0 ) && (rxUsed < (RECEIVE_QUEUE_SIZE-1))) {
      rxQ[rxHead] = *buffer++;
      rxHead = (rxHead+1) % RECEIVE_QUEUE_SIZE;
      rxUsed++;
      size--;
  }
}

uint8_t append_to_buf(void)
{
  uint8_t c;

  HAL_UART_Receive(&UartHandle, &c, 1, 100);
  rxQ[rxHead] = c;
  rxHead=(rxHead+1)%RECEIVE_QUEUE_SIZE;
  rxUsed++;

  if(c=='\n' || c=='\r')
    return 1;

  return 0;
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

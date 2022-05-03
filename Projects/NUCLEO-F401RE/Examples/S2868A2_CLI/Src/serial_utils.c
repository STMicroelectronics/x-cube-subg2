/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : serial_utils.c
* Author             : MCD Application Team
* Version            : V1.0
* Date               : September 2009
* Description        : Driver for serial_utilities  management on STM32W108 MB851 board
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>

#include "serial_utils.h"

/* Private variables ---------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : writeHex
* Description    : write hex
* Input          : charBuffer,value,charCount
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t *writeHex(uint8_t *charBuffer, uint16_t value, uint8_t charCount)
{
  uint8_t c = charCount;
  charBuffer += charCount;
  for (; c; c--) {
    uint8_t n = value & 0x0F;
    value = value >> 4;
    *(--charBuffer) = n + (n < 10
                           ? '0'
                           : 'A' - 10);
  }
  return charBuffer + charCount;
}/* end writeHex() */

/*******************************************************************************
* Function Name  : werialWriteString
* Description    : Write a string
* Input          : string
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t serialWriteString(const char * string)
{
   while (*string != '\0')
   {
      #if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
        __io_putchar(*string);
      #else
        putchar(*string);
      #endif
     string++;
   }
   return TRUE;
}/* end serialWriteString() */

/*******************************************************************************
* Function Name  : serialWaitSend
* Description    : Wait on serial
* Input          : string
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t serialWaitSend(void)
{
  __io_flush();
  return TRUE;
}/* end serialWaitSend() */

/*******************************************************************************
* Function Name  : serialReadPartialLine
* Description    : Read a serial input line
* Input          : max, index
* Output         : data
* Return         : status
*******************************************************************************/
uint8_t serialReadPartialLine(char *data, uint16_t max, uint16_t * index)
{
  uint8_t err;
  uint8_t ch;

  if (((*index) == 0) || ((*index) >= max))
    data[0] = '\0';

  for (;;) {
    err = serialReadByte(&ch);
    // no new serial char?, keep looping
    if (!err) return err;

    // handle bogus characters
    if ( ch > 0x7F ) continue;

    // handle leading newline - fogBUGZ # 584
    if (((*index) == 0) &&
        ((ch == '\n') || (ch == 0))) continue;

    // Drop the CR, or NULL that is part of EOL sequence.
    if ((*index) >= max) {
      *index = 0;
      if ((ch == '\r') || (ch == 0)) continue;
    }

    // handle backspace
    if ( ch == 0x8 ) {
      if ( (*index) > 0 ) {
        // delete the last character from our string
        (*index)--;
        data[*index] = '\0';
        // echo backspace
        serialWriteString("\010 \010");
      }
      // don't add or process this character
      continue;
    }

    //if the string is about to overflow, fake in a CR
    if ( (*index) + 2 > max ) {
      ch = '\r';
    }

    serialWriteByte(ch); // term char echo

    //upcase that char
    if ( ch>='a' && ch<='z') ch = ch - ('a'-'A');

    // build a string until we press enter
    if ( ( ch == '\r' ) || ( ch == '\n' ) ) {
      data[*index] = '\0';

      if (ch == '\r') {
        serialWriteByte('\n'); // "append" LF
        *index = 0;                       // Reset for next line; \n next
      } else {
        serialWriteByte('\r'); // "append" CR
        *index = max;          // Reset for next line; \r,\0 next
      }

      return TRUE;
    }

    data[(*index)++] = ch;
  }
}/* end serialReadPartialLine() */

/*******************************************************************************
* Function Name  : serialWriteData
* Description    : Write serial data
* Input          : data, length
* Output         : None
* Return         : status
*******************************************************************************/
uint8_t serialWriteData(uint8_t *data, uint8_t length)
{
  uint8_t i=0;
  for (i = 0; i < length; i++)
  {
      #if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
        __io_putchar(data[i]);
      #else
        putchar(data[i]);
      #endif
  }
  return TRUE;
}/* end serialWriteData() */

/*******************************************************************************
* Function Name  : serialWriteData
* Description    : Write a byte
* Input          : dataByte
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t serialWriteByte(uint8_t dataByte)
{
  return serialWriteData(&dataByte, 1);
}/* end serialWriteByte() */
/*******************************************************************************
* Function Name  : serialReadByte
* Description    : Non blocking read
* Input          : none
* Output         : dataByte: buffer containing the read byte if any
* Return         : TRUE if there is a data, FALSE otherwise
*******************************************************************************/
uint8_t serialReadByte(uint8_t *dataByte)
{
  return __io_getcharNonBlocking(dataByte);
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/


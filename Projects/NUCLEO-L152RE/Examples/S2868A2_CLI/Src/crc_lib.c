/**
 * @file    crc_lib.c
 * @author  LowPower RF BU - AMG
 * @version 1.0.0
 * @date    April 13, 2017
 * @brief   CRC library.
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
 * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
 */
#include <stdint.h>
#include "crc_lib.h"

static struct {
  uint32_t poly;
  uint8_t size;
  uint32_t seed;
  uint32_t upper_bit_mask;
  uint32_t mask;
  uint8_t reverse;
} crc_config;

static uint32_t crc;

static uint32_t reverse(register uint32_t x)
{
  x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
  x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
  x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
  x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
  return((x >> 16) | (x << 16));
}

void crc_reset_seed(void)
{
  if(crc_config.reverse)
  {
    crc=((uint32_t)reverse(crc_config.seed)>>(8*(4-crc_config.size)));
  }
  else
  {
    crc=crc_config.seed;
  }
}

void crc_init(uint32_t poly, uint8_t size, uint32_t seed, uint8_t reverse)
{
  crc_config.poly=poly;
  crc_config.size=size;
  crc_config.seed=seed;
  crc_config.upper_bit_mask=0x80<<(8*(size-1));
  crc_config.mask=0;
  for(uint8_t i=0;i<size*8;i++)
    crc_config.mask |=(((uint32_t)1)<<i);
  crc_config.reverse=reverse;
  crc_reset_seed();
}

void crc_update(uint8_t data)
{
  uint8_t i;

  if(crc_config.reverse)
  {
    data=((uint32_t)reverse(data)>>24);
  }

  for (i = 0; i < 8; i++)
  {
    if ((((crc & crc_config.upper_bit_mask) >> (8*(crc_config.size-1))) ^ (data & 0x80)) & crc_config.mask)
      crc = (crc << 1)  ^ crc_config.poly;
    else
      crc = (crc << 1);

    data <<= 1;
  }
  crc &= crc_config.mask;
}

uint32_t crc_get(void)
{
  if(crc_config.reverse)
    crc=reverse(crc)>>(8*(4-crc_config.size));

  return crc & crc_config.mask;
}

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

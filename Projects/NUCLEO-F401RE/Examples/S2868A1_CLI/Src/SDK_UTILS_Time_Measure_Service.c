/**
 * @file    SDK_UTILS_Time_Measure_Service.c
 * @author  LowPower RF BU - AMG
 * @version 1.0.0
 * @date    May, 2019
 * @brief   Timer measurmente service
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
 * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
 */

#include "SDK_UTILS_Time_Measure_Service.h"

#if (defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
    #define S2LP_Middleware_GpioInterruptCmd(pA, pB, pC, pD) S2LP_Middleware_GpioInterruptCmd(pA, pB, pC, pD, M2S_GPIO_EDGE_EVENT)
#endif

static time_measure_t time_meas[4][32];
static uint16_t time_meas_idx[4]={0,0,0,0};

uint8_t gpio_timer_mask=0;

void TimeMeasureServiceSetup(M2SGpioPin gpio, uint8_t enable)
{
  uint8_t gpio_bitn = gpio << 1;

  if(enable)
  {
    S2LP_Middleware_GpioInterruptCmd(gpio, 1, 0x0A, 0);

    gpio_timer_mask|=(enable<<gpio_bitn);

    S2LP_Middleware_GpioInit(gpio, M2S_MODE_EXTI_IN);

    if ((enable == 1) || (enable == 3)) {
	S2LP_Middleware_GpioTriggerRising(gpio,ENABLE);
	S2LP_Middleware_GpioTriggerFalling(gpio,DISABLE);
    } else {
	S2LP_Middleware_GpioTriggerRising(gpio,DISABLE);
	S2LP_Middleware_GpioTriggerFalling(gpio,ENABLE);
    }

    time_meas_idx[gpio]=0;
    S2LP_Middleware_GpioInterruptCmd(gpio, 1, 0x0A, 1);
  }
  else
  {
    gpio_timer_mask&=~(0x3<<gpio_bitn);

    S2LP_Middleware_GpioInit(gpio, M2S_MODE_GPIO_IN);
    S2LP_Middleware_GpioInterruptCmd(gpio, 1, 0x0A, 0);
  }
}

time_measure_t *TimeMeasureServiceGet(uint8_t gpio, uint8_t *entries)
{
  *entries = time_meas_idx[(uint8_t)gpio];
  return time_meas[gpio];
}

void TimeMeasureServiceReset(uint8_t gpio)
{
  time_meas_idx[gpio]=0;
}

void TimeMeasure(uint8_t gpio, uint32_t time_stamp)
{
  uint32_t actual_time=time_stamp;

  if(time_meas_idx[(uint8_t)gpio]>=(sizeof(time_meas[0])/(sizeof(time_meas[0][0])))) {
    /* Just disable interrupts since the buffer is full */
    S2LP_Middleware_GpioInterruptCmd((M2SGpioPin)gpio, 0, 0, 0);
    return;
  }

  time_meas[gpio][time_meas_idx[gpio]].timestamp=actual_time;

  if(S2LP_Middleware_GpioGetTriggerFalling((M2SGpioPin)gpio))
  {
    time_meas[gpio][time_meas_idx[gpio]].direction = 0;

    if (((gpio_timer_mask >> (2 * gpio)) & 0x3) == 1)
    {
	S2LP_Middleware_GpioTriggerRising((M2SGpioPin)gpio,ENABLE);
	S2LP_Middleware_GpioTriggerFalling((M2SGpioPin)gpio,DISABLE);
    }
  }
  else
  {
    time_meas[gpio][time_meas_idx[gpio]].direction = 1;

    if (((gpio_timer_mask >> (2 * gpio)) & 0x3) == 1)
    {
	S2LP_Middleware_GpioTriggerRising((M2SGpioPin)gpio,DISABLE);
	S2LP_Middleware_GpioTriggerFalling((M2SGpioPin)gpio,ENABLE);
    }
  }

  time_meas_idx[gpio]++;
}

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

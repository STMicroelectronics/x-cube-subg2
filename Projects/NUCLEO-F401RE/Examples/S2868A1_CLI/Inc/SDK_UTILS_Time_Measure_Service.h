/**
 * @file    SDK_UTILS_Time_Measure_Service.h
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
#ifndef __SDK_UTILS_TIME_MEASURE_SERVICE_H__
#define __SDK_UTILS_TIME_MEASURE_SERVICE_H__

#include "S2LP_CORE_GPIO.h"
#include "hal_types.h"

typedef struct time_measure_s {
  uint32_t timestamp;
  uint8_t direction; /* 1 rising, 0 falling */
} time_measure_t;

void TimeMeasureServiceSetup(M2SGpioPin gpio, uint8_t enable);
time_measure_t *TimeMeasureServiceGet(uint8_t gpio, uint8_t *entries);
void TimeMeasureServiceReset(uint8_t gpio);
void TimeMeasure(uint8_t gpio, uint32_t time_stamp);

#endif /*__SDK_UTILS_TIME_MEASURE_SERVICE_H__*/
/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/


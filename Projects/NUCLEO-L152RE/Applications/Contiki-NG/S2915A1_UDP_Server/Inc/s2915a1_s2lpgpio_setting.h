
/**
  ******************************************************************************
  * @file    s2lpgpio_setting.h
  * @author  SRA Application Team
  * @brief   This file contains definitions for the GPIO settings of S2LP
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __S2LPGPIO_SETTING_H__
#define __S2LPGPIO_SETTING_H__

#ifdef __cplusplus
extern "C" {
#endif

#define USE_S2915A1_RADIO_GPIO_0                        0
#define USE_S2915A1_RADIO_GPIO_1                        0
#define USE_S2915A1_RADIO_GPIO_2                        0
#define USE_S2915A1_RADIO_GPIO_3                        1

#define S2915A1_RADIO_GPIO                              S2915A1_RADIO_GPIO_3

#ifdef __cplusplus
}
#endif

#endif /* __S2LPGPIO_SETTING_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

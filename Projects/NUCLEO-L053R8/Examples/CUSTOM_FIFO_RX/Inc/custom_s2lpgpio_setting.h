
/**
 ******************************************************************************
 * @file    custom_s2lpgpio_setting.h
 * @author  SRA Application Team
 * @brief   This file contains definitions for the GPIO settings of S2LP
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CUSTOM_S2LPGPIO_SETTING_H__
#define __CUSTOM_S2LPGPIO_SETTING_H__

#ifdef __cplusplus
extern "C" {
#endif
/* USE_CUSTOM_S2LP_RADIO_GPIO_x is set if the relevant GPIO_x is used for
 * any purpose (Input, Output or EXTI).
 * The actual MODE will be reflected in the custom_s2lp_conf.h file in
 * CUSTOM_S2LP_RADIO_GPIO_x_MODE macros that assume these possible values:
 *
 * typedef enum
 * {
 *   RADIO_MODE_GPIO_IN  = 0x00,
 *   RADIO_MODE_EXTI_IN,
 *   RADIO_MODE_GPIO_OUT,
 * }CUSTOM_S2LP_RADIO_GPIO_Mode;
 *
 * like defined in custom_s2lp.h file.
 * Do not edit the following values by hand but use STM32CubeMX to change the
 * GPIO settings in order to re-generate proper configuration.
 */

#define USE_CUSTOM_S2LP_RADIO_GPIO_0                        0
#define USE_CUSTOM_S2LP_RADIO_GPIO_1                        1
#define USE_CUSTOM_S2LP_RADIO_GPIO_2                        0
#define USE_CUSTOM_S2LP_RADIO_GPIO_3                        1

/* GPIO Mode: Above macros define if a GPIO is used in any mode,
 * the specific MODE in defined the custom_s2lp_conf.h file
 */

#ifdef __cplusplus
}
#endif

#endif /* __CUSTOM_S2LPGPIO_SETTING_H__ */

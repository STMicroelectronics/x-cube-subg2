
/**
 ******************************************************************************
 * @file    s2lpgpio_setting.h
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
#ifndef __S2LPGPIO_SETTING_H__
#define __S2LPGPIO_SETTING_H__

#ifdef __cplusplus
extern "C" {
#endif

/* USE_S2868A1_RADIO_GPIO_x is set if the relevant GPIO_x is used for
 * any purpose (Input, Output or EXTI).
 * The actual MODE will be reflected in the s2868a1_conf.h file in
 * S2868A1_RADIO_GPIO_x_MODE macros that assume these possible values:
 *
 * typedef enum
 * {
 *   RADIO_MODE_GPIO_IN  = 0x00,
 *   RADIO_MODE_EXTI_IN,
 *   RADIO_MODE_GPIO_OUT,
 * }S2868A1_RADIO_GPIO_Mode;
 *
 * like defined in s2868a1.h file.
 * Do not edit the following values by hand but use STM32CubeMX to change the
 * GPIO settings in order to re-generate proper configuration.
 */

#define USE_S2868A1_RADIO_GPIO_0                        1
#define USE_S2868A1_RADIO_GPIO_1                        1
#define USE_S2868A1_RADIO_GPIO_2                        1
#define USE_S2868A1_RADIO_GPIO_3                        1

/* GPIO Mode: Above macros define if a GPIO is used in any mode,
 * the specific MODE in defined the s2868a1_conf.h file
 */

/* S2868A1_RADIO_GPIO define the "main" radio GPIO that must always
 * be selected ("Basic Settings" of STM32CubeMX parameters tab).
 * If more than one GPIO is used, the role of those additional GPIO is taken
 * care by the specific application.
*/
#define S2868A1_RADIO_GPIO                              S2868A1_RADIO_GPIO_3

#ifdef __cplusplus
}
#endif

#endif /* __S2LPGPIO_SETTING_H__ */

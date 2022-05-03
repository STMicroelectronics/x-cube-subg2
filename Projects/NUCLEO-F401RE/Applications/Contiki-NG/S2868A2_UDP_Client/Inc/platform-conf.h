/**
  ******************************************************************************
  * @file    platform-conf.h
  * @author  SRA Application Team
  * @brief   Configuration parameters of Contiki platform.
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
/*---------------------------------------------------------------------------*/
/**
 * @addtogroup configuration_files
 * @ingroup Contiki-NG_STM32_Library
 * @{
 * @file Platform specific configuration file for Contiki
 */

/*---------------------------------------------------------------------------*/
#ifndef PLATFORM_CONF_H__
#define PLATFORM_CONF_H__
/*---------------------------------------------------------------------------*/
#include <inttypes.h>
#include <string.h>
#include "main.h"
#include "radio-driver.h"
#include "RTE_Components.h"
/*---------------------------------------------------------------------------*/
/*This is needed if we want to stringize the result of expansion
 * of a macro argument (i.e. we need two levels) */
#define xstr(s) str(s)
/*---------------------------------------------------------------------------*/
#define str(x) #x
/*---------------------------------------------------------------------------*/
#define PRINT_PARAMETER(PARAM, format)  do{\
	printf(#PARAM ":\t" format "\n", PARAM);\
}while(0)
/*---------------------------------------------------------------------------*/
#define PRINT_PARAMETER_STR(PARAM)  do{\
	printf(#PARAM ":\t" str(PARAM) "\n");\
}while(0)
/*---------------------------------------------------------------------------*/
#ifndef PLATFORM_HAS_LEDS //In case it is defined in RTE_Components.h
#define PLATFORM_HAS_LEDS 0
#else /*PLATFORM_HAS_LEDS*/

/*LED_GREEN comes from BSP and is and index==0, LEDS_GREEN will be defined in
 Contiki-NG LED module starting from LEDS_CONF_GREEN and is a bitfield */
#define LEDS_CONF_GREEN  ((1) << LED_GREEN) /*Nucleo LED*/
#define LEDS_CONF_RED    (2) /*Radio LED if available*/
#define LEDS_CONF_BLUE   (4) /*Not used*/
#define LEDS_CONF_YELLOW (8) /*Not used*/

#endif /*!PLATFORM_HAS_LEDS*/

#ifndef PLATFORM_HAS_BUTTON //In case it is defined in RTE_Components.h
#define PLATFORM_HAS_BUTTON 0
#endif /*!PLATFORM_HAS_BUTTON*/

/*Radio must be present */
#define PLATFORM_HAS_RADIO 1
/*---------------------------------------------------------------------------*/

/*S2xxxAy Radio X-NUCLEO LED is mounted but not available by default, so
  currently we use Board LEDs only. Add +1 if the X-NUCLEO LED is available. */
#ifdef LEDn /*Coming from BSP API*/
#define LEDS_CONF_COUNT  LEDn
#else /*!LEDn*/
#define LEDS_CONF_COUNT  0
#endif /*LEDn*/

#ifndef LEDS_CONF_ALL
#define LEDS_CONF_ALL LEDS_GREEN //We are supporting now only S2-LP expansion board, whose LED is not enabled.
#endif /*LEDS_CONF_ALL*/

/*---------------------------------------------------------------------------*/
#define UART1_CONF_TX_WITH_INTERRUPT        0
#define UART_CONF_ENABLE                    1
#define NETSTACK_CONF_RADIO                 subGHz_radio_driver
/*---------------------------------------------------------------------------*/
extern uint32_t F_CPU;
#define CLOCK_CONF_SECOND             1000
/* One tick: 1 ms */

#if WITH_IP64
#include "ip64-conf.h"
#define WITH_SLIP 1
#ifndef UIP_FALLBACK_INTERFACE
#define UIP_FALLBACK_INTERFACE ip64_uip_fallback_interface
#endif
#endif /* WITH_IP64 */
/*---------------------------------------------------------------------------*/
typedef unsigned long clock_time_t;
/*---------------------------------------------------------------------------*/
#define CC_CONF_REGISTER_ARGS          0
#define CC_CONF_FUNCTION_POINTER_ARGS  1
#define CC_CONF_FASTCALL
#define CC_CONF_VA_ARGS                1
#define CC_CONF_INLINE                 inline

#define CCIF
#define CLIF
/*---------------------------------------------------------------------------*/
#ifndef __ARCHDEP__TYPES
#define __ARCHDEP__TYPES
typedef uint8_t          u8_t;
typedef uint16_t        u16_t;
typedef uint32_t        u32_t;
typedef int8_t           i8_t;
typedef int16_t         i16_t;
typedef int32_t         i32_t;
typedef i8_t             s8_t;
typedef i16_t           s16_t;
typedef i32_t           s32_t;
#endif /*__ARCHDEP__TYPES*/
/*---------------------------------------------------------------------------*/
typedef unsigned short  uip_stats_t;
/*---------------------------------------------------------------------------*/
extern volatile unsigned long seconds;
extern volatile clock_time_t ticks;
/*---------------------------------------------------------------------------*/
#endif /* PLATFORM_CONF_H__ */
/*---------------------------------------------------------------------------*/
/** @} */


 /*******************************************************************************
  * @file    custom_s2lp.h
  * @author  SRA Team
  * @brief   driver CUSTOM_S2LP header file
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
#ifndef __CUSTOM_S2LP_H__
#define __CUSTOM_S2LP_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "custom_s2lp_conf.h"
#include "s2lp.h"
#include "custom_s2lpgpio_setting.h"

/**
  * @brief Define for S2LP based custom board
  */

/* Exported types ------------------------------------------------------------*/

  /* MCU GPIO pin working mode for GPIO */
typedef enum
{
    RADIO_MODE_GPIO_IN  = 0x00,    /*!< Work as GPIO input */
    RADIO_MODE_EXTI_IN,           /*!< Work as EXTI */
    RADIO_MODE_GPIO_OUT,           /*!< Work as GPIO output */
}CUSTOM_S2LP_RADIO_GPIO_Mode;

//Below type is currently unused with new implementation
/* MCU GPIO pin Edge */
typedef enum
{
  FALLING  = 0x00,     /*!< Falling  Edge */
  RISING,              /*!< Rising  Edge */
  RISING_FALLING,
  DEFAULT
}CUSTOM_S2LP_RADIO_GPIO_EDGE_Mode;

typedef enum
{
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
  CUSTOM_S2LP_RADIO_GPIO_0     = 0x00, /*!< GPIO_0 selected */
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
  CUSTOM_S2LP_RADIO_GPIO_1     = USE_CUSTOM_S2LP_RADIO_GPIO_0,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
  CUSTOM_S2LP_RADIO_GPIO_2     = USE_CUSTOM_S2LP_RADIO_GPIO_0 + USE_CUSTOM_S2LP_RADIO_GPIO_1,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
  CUSTOM_S2LP_RADIO_GPIO_3     = USE_CUSTOM_S2LP_RADIO_GPIO_0 + USE_CUSTOM_S2LP_RADIO_GPIO_1 + USE_CUSTOM_S2LP_RADIO_GPIO_2,
#endif
  CUSTOM_S2LP_RADIO_GPIO_SDN   = USE_CUSTOM_S2LP_RADIO_GPIO_0 + USE_CUSTOM_S2LP_RADIO_GPIO_1 + USE_CUSTOM_S2LP_RADIO_GPIO_2+ USE_CUSTOM_S2LP_RADIO_GPIO_3,
}CUSTOM_S2LP_RADIO_GPIO_TypeDef;

/* EEPROM SPI commands */

/* EEPROM SPI status */

#define CUSTOM_S2LP_RADIO_SPI_NSS_PIN_LOW()           HAL_GPIO_WritePin(CUSTOM_S2LP_RADIO_SPI_NSS_PORT, CUSTOM_S2LP_RADIO_SPI_NSS_PIN, GPIO_PIN_RESET)
#define CUSTOM_S2LP_RADIO_SPI_NSS_PIN_HIGH()          HAL_GPIO_WritePin(CUSTOM_S2LP_RADIO_SPI_NSS_PORT, CUSTOM_S2LP_RADIO_SPI_NSS_PIN, GPIO_PIN_SET)

   /* Hardware IO IRQ callback function definition */
typedef void ( GpioIrqHandler )( void );

/* Exported constants --------------------------------------------------------*/
/* Common Error codes */
#define CUSTOM_S2LP_ERROR_NONE                         0
#define CUSTOM_S2LP_ERROR_PERIPH_FAILURE              -1
#define CUSTOM_S2LP_ERROR_COMPONENT_FAILURE           -2
#define CUSTOM_S2LP_ERROR_UNKNOWN_FAILURE             -3
#define CUSTOM_S2LP_ERROR_UNKNOWN_COMPONENT           -4
#define CUSTOM_S2LP_ERROR_BUS_FAILURE                 -5
#define CUSTOM_S2LP_ERROR_CLOCK_FAILURE               -6
#define CUSTOM_S2LP_ERROR_MSP_FAILURE                 -7
#define CUSTOM_S2LP_ERROR_FEATURE_NOT_SUPPORTED       -8
/* Exported variables------------------------------------------------------- */

/* Exported functions ------------------------------------------------------- */

/*!
 * @brief Initializes the radio Board
 * @retval none
 */
int32_t CUSTOM_S2LP_RADIO_Init( void );
/*!
 * @brief DeInitializes the radio Board
 * @retval none
 */
int32_t CUSTOM_S2LP_RADIO_DeInit( void );

/*!
 * @brief Resets the CUSTOM_S2LP
 * @param  none
 * @retval none
 */

int32_t CUSTOM_S2LP_RADIO_Reset(void);

/*!
 * @brief Initializes the radio I/Os Irq
 * @param [IN] GpioIrqHandler: GpioIrqHandler struct nb
 * @retval none
 */
int32_t CUSTOM_S2LP_RADIO_IoIrqEnable( GpioIrqHandler ** );

/*!
 * @brief Set XO state on the board
 * @param  [IN] state: dummy with this HW
 * @retval none
 */
int32_t CUSTOM_S2LP_RADIO_BoardSetXO ( uint8_t );

/*!
 * @brief Get Board Wake Up time
 * @param  none
 * @retval Board Wakeup time
 */
int32_t CUSTOM_S2LP_RADIO_GetWakeUpTime( void );

/*!
 * @brief Sets the radio output power.
 * @param [IN] power: Sets the RF output power
 * @param [IN] channel: used channel
 * @retval none
 */
int32_t CUSTOM_S2LP_RADIO_SetRfTxPower( int8_t , uint32_t );

/*!
 * @brief
 * @param [IN] status: 1 when coming from opMode == RF_OPMODE_SLEEP, 0 otherwise
 * @retval none
 */
int32_t CUSTOM_S2LP_RADIO_SetAntSwLowPower( bool );

/*!
 * @brief Initializes the radio I/Os Irq
 * @param [IN] GpioIrqHandler: GpioIrqHandler struct nb
 * @retval none
 */
int32_t CUSTOM_S2LP_RADIO_IoIrqEnable(GpioIrqHandler **);

/*!
 * @brief Deinitializes the radio I/Os Irq
 * @param [IN] GpioIrqHandler: GpioIrqHandler struct nb
 * @retval none
 */
int32_t CUSTOM_S2LP_RADIO_IoIrqDisable(GpioIrqHandler **);

/*!
 * @brief Initializes the RF Switch I/Os pins interface
 * @param [IN] on_off 0: OFF, 1: ON
 * @retval none
 */

int32_t CUSTOM_S2LP_RADIO_EnterShutdown( void );

int32_t CUSTOM_S2LP_RADIO_ExitShutdown( void );

uint8_t CUSTOM_S2LP_RADIO_CheckShutdown( void ) ;

/*!
 * @brief Controls the antena switch if necessary
 * @param [IN] opMode: Current radio operating mode
 * @retval none
 */
int32_t CUSTOM_S2LP_RADIO_SetAntSw(uint8_t);

/*!
 * @brief Sets the SX1276 operating mode
 * @param [IN] opMode New operating mode
 * @retval none
 */
int32_t CUSTOM_S2LP_RADIO_SetOpMode( uint8_t opMode );

/*!
 * @brief Checks if the given RF frequency is supported by the hardware
 * @param [IN] frequency RF frequency to be checked
 * @retval isSupported [true: supported, false: unsupported]
 */
int32_t CUSTOM_S2LP_RADIO_CheckRfFrequency( uint32_t frequency);

/**
  * @brief  Enable TCXO.
  * @param  None
  * @retval Error Code
  */
int32_t CUSTOM_S2LP_RADIO_EnableTCXO(void);

/**
  * @brief  Legacy API to change settings at runtime
  *@param xGpio can be 4 different GPIO used in Radio of S2LP
  * @param  xGpioMode can be different Mode
  * @param  xGpioEdge The edge required for interrupt
  * @retval None
  */
void CUSTOM_S2LP_RADIO_GPIO_Init_Update( CUSTOM_S2LP_RADIO_GPIO_TypeDef xGpio, CUSTOM_S2LP_RADIO_GPIO_Mode xGpioMode , CUSTOM_S2LP_RADIO_GPIO_EDGE_Mode xGpioEdge);

__weak void FEM_Operation(FEM_OperationType operation);

__weak int32_t CUSTOM_S2LP_EEPROM_ReadPage(uint32_t Instance, uint16_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer);
#ifdef __cplusplus
}
#endif

#endif /* __CUSTOM_S2LP_H__*/

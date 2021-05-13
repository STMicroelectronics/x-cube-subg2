
 /*******************************************************************************
  * @file    s2868A2.h
  * @author  SRA Team
  * @brief   driver S2868A2 header file
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
#ifndef __S2868A2_H__
#define __S2868A2_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "s2868a2_conf.h"
#include "s2lp.h"

   
/** 
  * @brief Define for SX1276MB1XAS board  
  */ 
   
/* Exported types ------------------------------------------------------------*/
 
typedef enum 
{
  S2868A2_LED = 0,
} Led_t;

  /* MCU GPIO pin working mode for GPIO */
typedef enum                                                                                          
{
    RADIO_MODE_GPIO_IN  = 0x00,   /*!< Work as GPIO input */
    RADIO_MODE_EXTI_IN,           /*!< Work as EXTI */
    RADIO_MODE_GPIO_OUT,          /*!< Work as GPIO output */
}S2868A2_RADIO_GPIO_Mode;  
/* MCU GPIO pin Edge */
typedef enum                                                                                          
{
  FALLING  = 0x00,     /*!< Falling  Edge */
  RISING,              /*!< Rising  Edge */
  RISING_FALLING,
  DEFAULT  
}S2868A2_RADIO_GPIO_EDGE_Mode;  


typedef enum 
{  
#if (USE_S2868A2_RADIO_GPIO_0 == 1)    
  S2868A2_RADIO_GPIO_0     = 0x00, /*!< GPIO_0 selected */
#endif 
#if (USE_S2868A2_RADIO_GPIO_1 == 1)    
  S2868A2_RADIO_GPIO_1     = USE_S2868A2_RADIO_GPIO_0,
#endif   
#if (USE_S2868A2_RADIO_GPIO_2 == 1)    
  S2868A2_RADIO_GPIO_2     = USE_S2868A2_RADIO_GPIO_0 + USE_S2868A2_RADIO_GPIO_1,
#endif   
#if (USE_S2868A2_RADIO_GPIO_3 == 1)     
  S2868A2_RADIO_GPIO_3     = USE_S2868A2_RADIO_GPIO_0 + USE_S2868A2_RADIO_GPIO_1 + USE_S2868A2_RADIO_GPIO_2,
#endif     
  S2868A2_RADIO_GPIO_SDN   = USE_S2868A2_RADIO_GPIO_0 + USE_S2868A2_RADIO_GPIO_1 + USE_S2868A2_RADIO_GPIO_2+ USE_S2868A2_RADIO_GPIO_3,
}S2868A2_RADIO_GPIO_TypeDef;


/* EEPROM SPI commands */
#define EEPROM_CMD_WREN    0x06    // Write Enable
#define EEPROM_CMD_WRDI    0x04    // Write Disable
#define EEPROM_CMD_RDSR    0x05    // Read Status Register
#define EEPROM_CMD_WRSR    0x01    // Write Status Register
#define EEPROM_CMD_READ    0x03    // Read from Memory Array
#define EEPROM_CMD_WRITE   0x02    // Write to Memory Array   

/* EEPROM SPI status */
#define EEPROM_STATUS_SRWD    0x80       // Status Register Write Disable
#define EEPROM_STATUS_BP      0x0C       // Block Protect
#define EEPROM_STATUS_WEL     0x02       // Write Enable   
#define EEPROM_STATUS_WIP     0x01       // Write in Progress

#define S2868A2_RADIO_SPI_NSS_PIN_LOW()           HAL_GPIO_WritePin(S2868A2_RADIO_SPI_NSS_PORT, S2868A2_RADIO_SPI_NSS_PIN, GPIO_PIN_RESET)
#define S2868A2_RADIO_SPI_NSS_PIN_HIGH()          HAL_GPIO_WritePin(S2868A2_RADIO_SPI_NSS_PORT, S2868A2_RADIO_SPI_NSS_PIN, GPIO_PIN_SET)
#define EEPROM_NSS_PIN_LOW()          HAL_GPIO_WritePin(S2868A2_EEPROM_SPI_CS_PORT, S2868A2_EEPROM_SPI_CS_PIN, GPIO_PIN_RESET)
#define EEPROM_NSS_PIN_HIGH()         HAL_GPIO_WritePin(S2868A2_EEPROM_SPI_CS_PORT, S2868A2_EEPROM_SPI_CS_PIN, GPIO_PIN_SET)
#define EEPROM_INSTANCE               (0)
   /* Hardware IO IRQ callback function definition */
typedef void ( GpioIrqHandler )( void );

/* Exported constants --------------------------------------------------------*/
/* Common Error codes */
#define S2868A2_ERROR_NONE                         0
#define S2868A2_ERROR_PERIPH_FAILURE              -1
#define S2868A2_ERROR_COMPONENT_FAILURE           -2
#define S2868A2_ERROR_UNKNOWN_FAILURE             -3
#define S2868A2_ERROR_UNKNOWN_COMPONENT           -4
#define S2868A2_ERROR_BUS_FAILURE                 -5
#define S2868A2_ERROR_CLOCK_FAILURE               -6
#define S2868A2_ERROR_MSP_FAILURE                 -7
#define S2868A2_ERROR_FEATURE_NOT_SUPPORTED       -8
/* Exported variables------------------------------------------------------- */ 

/* Exported functions ------------------------------------------------------- */ 

/*!
 * @brief Initializes the radio Board
 * @retval none
 */
int32_t S2868A2_RADIO_Init( void );
/*!
 * @brief DeInitializes the radio Board
 * @retval none
 */
int32_t S2868A2_RADIO_DeInit( void );

/*!
 * @brief Resets the S2868A2
 * @param  none
 * @retval none
 */

int32_t S2868A2_RADIO_Reset(void);

/*!
 * @brief Initializes the radio I/Os Irq
 * @param [IN] GpioIrqHandler: GpioIrqHandler struct nb
 * @retval none
 */
int32_t S2868A2_RADIO_IoIrqEnable( GpioIrqHandler ** );

/*!
 * @brief Set XO state on the board
 * @param  [IN] state: dummy with this HW
 * @retval none
 */
int32_t S2868A2_RADIO_BoardSetXO ( uint8_t );

/*!
 * @brief Get Board Wake Up time
 * @param  none
 * @retval Board Wakeup time
 */
int32_t S2868A2_RADIO_GetWakeUpTime( void );

/*!
 * @brief Sets the radio output power.
 * @param [IN] power: Sets the RF output power
 * @param [IN] channel: used channel
 * @retval none
 */
int32_t S2868A2_RADIO_SetRfTxPower( int8_t , uint32_t );

/*!
 * @brief 
 * @param [IN] status: 1 when coming from opMode == RF_OPMODE_SLEEP, 0 otherwise
 * @retval none
 */
int32_t S2868A2_RADIO_SetAntSwLowPower( bool );


/*!
 * @brief Initializes the radio I/Os Irq
 * @param [IN] GpioIrqHandler: GpioIrqHandler struct nb
 * @retval none
 */
int32_t S2868A2_RADIO_IoIrqEnable(GpioIrqHandler **);

/*!
 * @brief Deinitializes the radio I/Os Irq
 * @param [IN] GpioIrqHandler: GpioIrqHandler struct nb
 * @retval none
 */
int32_t S2868A2_RADIO_IoIrqDisable(GpioIrqHandler **);

/*!
 * @brief Initializes the RF Switch I/Os pins interface
 * @param [IN] on_off 0: OFF, 1: ON
 * @retval none
 */

int32_t S2868A2_RADIO_EnterShutdown( void );

int32_t S2868A2_RADIO_ExitShutdown( void );

uint8_t S2868A2_RADIO_CheckShutdown( void );

/*!
 * @brief Controls the antena switch if necessary
 * @param [IN] opMode: Current radio operating mode
 * @retval none
 */
int32_t S2868A2_RADIO_SetAntSw(uint8_t);

/*!
 * @brief Sets the SX1276 operating mode
 * @param [IN] opMode New operating mode
 * @retval none
 */
int32_t S2868A2_RADIO_SetOpMode( uint8_t opMode );

/*!
 * @brief Checks if the given RF frequency is supported by the hardware
 * @param [IN] frequency RF frequency to be checked
 * @retval isSupported [true: supported, false: unsupported]
 */
int32_t S2868A2_RADIO_CheckRfFrequency( uint32_t frequency);

/**
  * @brief  Enable TCXO.
  * @param  None
  * @retval Error Code 
  */
int32_t S2868A2_RADIO_EnableTCXO(void);

/**
  * @brief  LED GPIO Init
  * @param  enum
  * @retval Error code
  */
int32_t S2868A2_LED_Init(Led_t Led);   /* Mount R17 resistor to use led  */

/**
  * @brief  LED On API
  * @param  enum
  * @retval Error Code
*/
int32_t S2868A2_LED_On(Led_t Led);

/**
  * @brief  LED Off API.
  * @param  enum
  * @retval Error Code
*/
int32_t S2868A2_LED_Off(Led_t Led);


/**
  * @brief  LED Toggle.
  * @param  LED number
  * @retval Error Code
  */
int32_t S2868A2_LED_Toggle(Led_t Led); 

/**
  * @brief  Read LED State
  * @param  Led Number
  * @retval Error Code
  */
int32_t S2868A2_LED_GetState(Led_t Led);

/**
  * @brief  EEPROM Initialization
  * @param  Instance
  * @retval Error Code
  */
int32_t S2868A2_EEPROM_Init(uint32_t Instance);

/**
  * @brief  EEPROM DeInit.
  * @param  Instance
  * @retval Error Code
  */
int32_t S2868A2_EEPROM_DeInit(uint32_t Instance);

uint8_t BSP_EEPROM_SetSrwd(uint32_t Instance);

uint8_t BSP_EEPROM_ResetSrwd(uint32_t Instance);

int32_t BSP_EEPROM_WriteEnable(uint32_t Instance);

/**
  * @brief  EEPROM Write Page.
  * @param  Instance, Address, Buffer, Number of Bytes to be written
  * @retval Error Code
  */
int32_t S2868A2_EEPROM_WritePage(uint32_t Instance, uint16_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer);

/**
  * @brief  EEPROM Read Page.
  * @param  Instance, Address, Buffer, Number of Bytes to be written
  * @retval Error Code
  */
int32_t S2868A2_EEPROM_ReadPage(uint32_t Instance, uint16_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer);


/**
  * @brief  to verify EERPOM is Ready.
  * @param  Instance of EEPROM
  * @retval Error Code
  */
int32_t S2868A2_EEPROM_IsReady(uint32_t Instance);

/**
  * @brief  FunctionDescription
  * @param xGpio can be 4 different GPIO used in Radio of S2LP
  * @param  xGpioMode can be different Mode 
  * @param  xGpioEdge the edge for interrupt
  * @retval None
  */
void S2868A2_RADIO_GPIO_Init( S2868A2_RADIO_GPIO_TypeDef xGpio, S2868A2_RADIO_GPIO_Mode xGpioMode, S2868A2_RADIO_GPIO_EDGE_Mode xGpioEdge);

#ifdef __cplusplus
}
#endif

#endif /* __S2868A2_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

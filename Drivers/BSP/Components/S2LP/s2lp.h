
 /*******************************************************************************
  * @file    s2lp.h
  * @brief   Header for driver s2lp.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#ifndef __S2LP_H__
#define __S2LP_H__

#include <stdint.h>
#include <stdbool.h>
#include "s2lp_regs.h"
#include "s2lp_pktbasic.h"
#include "s2lp_commands.h"
#include "s2lp_csma.h"
#include "s2lp_gpio.h"
#include "s2lp_timer.h"
#include "s2lp_fifo.h"
#include "s2lp_general.h"
#include "s2lp_packethandler.h"
#include "s2lp_pktwmbus.h"
#include "s2lp_pktstack.h"
#include "s2lp_radio.h"
#include "s2lp_qi.h"
#include "s2lp_types.h"
#include "s2lp_radio.h"


/** @addtogroup S2LP_Libraries
 * @{
 */

/** @defgroup S2LP_SPI_Driver         SPI Driver
 * @brief Header file for low level S2LP SPI driver.
 * @details See the file <i>@ref MCU_Interface.h</i> for more details.
 * @{
 */

/** @defgroup SPI_Exported_Types        SPI Exported Types
 * @{
 */

typedef S2LPStatus StatusBytes;

/**
 * @}
 */


/** @defgroup SPI_Exported_Constants    SPI Exported Constants
 * @{
 */

/**
 * @}
 */


/** @defgroup SPI_Exported_Functions    SPI Exported Functions
 * @{
 */

/*!
 * Generic SX126X error code
 */
#define S2LP_OK      0
#define S2LP_ERROR  -1


#define HEADER_WRITE_MASK     0x00 /*!< Write mask for header byte*/
#define HEADER_READ_MASK      0x01 /*!< Read mask for header byte*/
#define HEADER_ADDRESS_MASK   0x00 /*!< Address mask for header byte*/
#define HEADER_COMMAND_MASK   0x80 /*!< Command mask for header byte*/

#define LINEAR_FIFO_ADDRESS 0xFF  /*!< Linear FIFO address*/

#define S2LP_DIG_DOMAIN_XTAL_THRESH  30000000        /*!< Digital domain logic threshold for XTAL in MHz */

#define XTAL_FREQUENCY          50000000U 

/** XTAL frequency offset compensation value in Hertz
  * Please, take into account that if nominal frequency is 50 MHz and
  * measured XTAL frequency is (for example) 50000157, then XTAL_FREQUENCY_OFFSET must be
  * set to -157, If not avaialble set it to 0 */
#define XTAL_FREQUENCY_OFFSET                   0

/* This is getting the base frequency from the band defined in the board.
 * For user board, define the desired frequency in Hz (e.g: 868000000) */
#define BOARD_BASE_FREQUENCY                    (S2LPGetFrequencyBand())
#define BOARD_FREQUENCY_BAND                    3 /* 868MHz */



#define S2LP_RX_FIFO_SIZE   128
#define S2LP_TX_FIFO_SIZE   128
#define S2LP_CMD_SIZE   2
#define S2LP_BUF_SIZE   S2LP_TX_FIFO_SIZE


#define BUILT_HEADER(add_comm, w_r) (add_comm | w_r)  /*!< macro to build the header byte*/
#define WRITE_HEADER    BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_WRITE_MASK) /*!< macro to build the write header byte*/
#define READ_HEADER     BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_READ_MASK)  /*!< macro to build the read header byte*/
#define COMMAND_HEADER  BUILT_HEADER(HEADER_COMMAND_MASK, HEADER_WRITE_MASK) /*!< macro to build the command header byte*/


#define S2LP_MCU_CLOCK  0x88U
#define S2LP_MODE_DIGITAL_OUTPUT_LP 0x02U
/* Exported types ------------------------------------------------------------*/

/*!
 * @brief Initializes the SPI object 
 * @param  none
 * @retval Error code: 0 ok, negative KO
 */
typedef int32_t     (*S2LPBus_Init_Func)(void);   
typedef int32_t     (*S2LPBus_DeInit_Func)(void); 
typedef int32_t     (*S2LPBus_WriteBuf_Func)( uint8_t *, uint8_t *, uint16_t );
typedef void        (*S2LPBus_Delay)(uint32_t );   

typedef struct
{
  S2LPBus_Init_Func            Init;
  S2LPBus_DeInit_Func          DeInit;
  S2LPBus_WriteBuf_Func        WriteBuffer;
  S2LPBus_Delay                Delay;
} S2LP_IO_t;

typedef enum
{
  RANGE_EXT_NONE = 0,
  RANGE_EXT_SKY_66100_11 = 1,
  RANGE_EXT_SKYWORKS_SE2435L = 2,
  RANGE_EXT_SKYWORKS_SKY66420 = 3,
  RANGE_EXT_SKYWORKS_868 = 4
} RangeExtType;

typedef enum
{
  S2LP_CUT_2_1 = 0x91,
  S2LP_CUT_2_0 = 0x81,
  S2LP_CUT_3_0 = 0xC1,
} S2LPCutType;

typedef enum
{
  PA_SHUTDOWN     = 0x00,
  PA_TX_BYPASS    = 0x01,
  PA_TX     	  = 0x02,
  PA_RX     	  = 0x03,
} PA_OperationType;



/* Exported constants --------------------------------------------------------*/    

#define S2LPGeneralLibraryVersion() "S2LP_Libraries_v.1.3.0"

/**
 * @defgroup Types_Exported_Variables   Types Exported Variables
 * @{
 */

extern volatile S2LPStatus g_xStatus;

/**
 * @}
 */

/* Exported macros --------------------------------------------------------*/    


/*!
 * ============================================================================
 * Public functions prototypes
 * ============================================================================
 */

 /**
  * @brief  Function
  * @param  pointer to IO functions
  * @retval error status
  */
int32_t S2LP_RegisterBusIO (S2LP_IO_t *pIO);
        
/*!
 * @brief Initialises the bus for S2LP driver communication
 * @param none
 * @retval forward error code from the bsp BUS Init
 */
int32_t S2LP_Init( void );


StatusBytes S2LP_WriteRegister(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer );

StatusBytes S2LP_ReadRegister(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer );

StatusBytes S2LP_SendCommand(uint8_t cCommandCode);

StatusBytes S2LP_WriteFIFO(uint8_t cNbBytes, uint8_t* pcBuffer);

StatusBytes S2LP_ReadFIFO(uint8_t cNbBytes, uint8_t* pcBuffer);


void S2LP_RefreshStatus(void);
void S2LP_StrobeCommand(S2LP_CMD_ xCommandCode);
int32_t S2LP_RcoCalibration(void);
void S2LP_EnableRangeExtMode(void);
void S2LP_TCXOInit(void);
/** @defgroup SPI_Exported_Macros       SPI Exported Macros
 * @{
 */

/**
 * @}
 */


/**
 * @}
 */


/**
 * @}
 */




#endif /* __S2LP_H__ */

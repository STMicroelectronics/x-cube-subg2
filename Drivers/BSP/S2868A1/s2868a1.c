
 /*******************************************************************************
  * @file    S2868A1.c
  * @author  SRA Team
  * @brief   driver s2868A1 board
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
  
/* Includes ------------------------------------------------------------------*/
#include "s2868a1.h"
#include "s2lp.h"
    
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BOARD_WAKEUP_TIME  0
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

GPIO_TypeDef*  aLED_GPIO_PORT[S2868A1_LEDn] = {S2868A1_LED_GPIO_PORT};
const uint16_t aLED_GPIO_PIN[S2868A1_LEDn] = {S2868A1_LED_GPIO_PIN};

EXTI_HandleTypeDef S2868A1_RADIO_GPIO_hexti[S2868A1_RADIO_GPIOn - 1] = {
#if (USE_S2868A1_RADIO_GPIO_0 == 1) 
                                    {.Line = S2868A1_RADIO_GPIO_0_EXTI_LINE},
#endif                               
#if (USE_S2868A1_RADIO_GPIO_1 == 1)                                    
                                    {.Line = S2868A1_RADIO_GPIO_1_EXTI_LINE},
#endif                               
#if (USE_S2868A1_RADIO_GPIO_2 == 1)       
                                     {.Line = S2868A1_RADIO_GPIO_2_EXTI_LINE},
#endif
#if (USE_S2868A1_RADIO_GPIO_3 == 1)  
                                     {.Line = S2868A1_RADIO_GPIO_3_EXTI_LINE}
#endif                                                          
};

/* Private function prototypes -----------------------------------------------*/


static const uint32_t S2868A1_RADIO_GPIO_EXTI_LINE[S2868A1_RADIO_GPIOn - 1] = {
#if (USE_S2868A1_RADIO_GPIO_0 == 1) 
    S2868A1_RADIO_GPIO_0_EXTI_LINE,
#endif
#if (USE_S2868A1_RADIO_GPIO_1 == 1) 
    S2868A1_RADIO_GPIO_1_EXTI_LINE,
#endif
#if (USE_S2868A1_RADIO_GPIO_2 == 1) 
    S2868A1_RADIO_GPIO_2_EXTI_LINE,
#endif
#if (USE_S2868A1_RADIO_GPIO_3 == 1) 
    S2868A1_RADIO_GPIO_3_EXTI_LINE,  
#endif
 };
  
static const uint32_t S2868A1_RADIO_GPIO_IT_PRIO [S2868A1_RADIO_GPIOn - 1] = {
#if (USE_S2868A1_RADIO_GPIO_0 == 1) 
    S2868A1_RADIO_GPIO_0_IT_PRIO,
#endif
#if (USE_S2868A1_RADIO_GPIO_1 == 1) 
    S2868A1_RADIO_GPIO_1_IT_PRIO,
#endif
#if (USE_S2868A1_RADIO_GPIO_2 == 1)    
    S2868A1_RADIO_GPIO_2_IT_PRIO,
#endif
#if (USE_S2868A1_RADIO_GPIO_3 == 1) 
    S2868A1_RADIO_GPIO_3_IT_PRIO, 
#endif
 };
  
static const IRQn_Type S2868A1_RADIO_GPIO_IRQn [S2868A1_RADIO_GPIOn - 1]   = {
#if (USE_S2868A1_RADIO_GPIO_0 == 1) 
    S2868A1_RADIO_GPIO_0_IRQn,
#endif
#if (USE_S2868A1_RADIO_GPIO_1 == 1) 
    S2868A1_RADIO_GPIO_1_IRQn,
#endif
#if (USE_S2868A1_RADIO_GPIO_2 == 1) 
    S2868A1_RADIO_GPIO_2_IRQn,
#endif
#if (USE_S2868A1_RADIO_GPIO_3 == 1) 
    S2868A1_RADIO_GPIO_3_IRQn  
#endif
 }; 
static GPIO_TypeDef* S2868A1_RADIO_GPIO_PORT[S2868A1_RADIO_GPIOn] = {
#if (USE_S2868A1_RADIO_GPIO_0 == 1) 
  S2868A1_RADIO_GPIO_0_PORT,
#endif
#if (USE_S2868A1_RADIO_GPIO_1 == 1) 
  S2868A1_RADIO_GPIO_1_PORT,
#endif
#if (USE_S2868A1_RADIO_GPIO_2 == 1) 
  S2868A1_RADIO_GPIO_2_PORT,
#endif
#if (USE_S2868A1_RADIO_GPIO_3 == 1) 
  S2868A1_RADIO_GPIO_3_PORT,
#endif 
  S2868A1_RADIO_GPIO_SDN_PORT
};

static const uint16_t S2868A1_RADIO_GPIO_PIN[S2868A1_RADIO_GPIOn] = {
#if (USE_S2868A1_RADIO_GPIO_0 == 1) 
  S2868A1_RADIO_GPIO_0_PIN,
#endif
#if (USE_S2868A1_RADIO_GPIO_1 == 1) 
  S2868A1_RADIO_GPIO_1_PIN,
#endif
#if (USE_S2868A1_RADIO_GPIO_2 == 1) 
  S2868A1_RADIO_GPIO_2_PIN,
#endif
#if (USE_S2868A1_RADIO_GPIO_3 == 1) 
  S2868A1_RADIO_GPIO_3_PIN,
#endif
  S2868A1_RADIO_GPIO_SDN_PIN
};
/* Private function prototypes -----------------------------------------------*/
static void S2868A1_SPI_CS_Init(void);
static void GPIO_DeInit(void);
void S2868A1_RADIO_GPIO_Init( S2868A1_RADIO_GPIO_TypeDef xGpio, S2868A1_RADIO_GPIO_Mode xGpioMode , S2868A1_RADIO_GPIO_EDGE_Mode xGpioEdge); 
static int32_t S2868A1_SPI_SendRecvWrapper(uint8_t *pHeader, uint8_t *pBuff, uint16_t Length);
static int32_t EEPROM_WaitEndWriteOperation(uint32_t Instance);

/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  FunctionDescription
  *@param xGpio can be 4 different GPIO used in Radio of S2LP
  * @param  xGpioMode can be different Mode 
  * @param  xGpioEdge The edge required for interrupt
  * @retval None
  */
void S2868A1_RADIO_GPIO_Init( S2868A1_RADIO_GPIO_TypeDef xGpio, S2868A1_RADIO_GPIO_Mode xGpioMode , S2868A1_RADIO_GPIO_EDGE_Mode xGpioEdge)  
{
  GPIO_InitTypeDef GPIO_InitStructure, EXTI_InitStructure;
  
  /* Enable Radio GPIO clock */
  switch(xGpio)
  {
#if (USE_S2868A1_RADIO_GPIO_0 == 1) 
  case S2868A1_RADIO_GPIO_0:
    S2868A1_RADIO_GPIO_0_GPIO_CLK_ENABLE();
    break;
#endif    
#if (USE_S2868A1_RADIO_GPIO_1 == 1)  
  case S2868A1_RADIO_GPIO_1:
    S2868A1_RADIO_GPIO_1_GPIO_CLK_ENABLE();
    break;
#endif 
#if (USE_S2868A1_RADIO_GPIO_2 == 1)  
  case S2868A1_RADIO_GPIO_2:
    S2868A1_RADIO_GPIO_2_GPIO_CLK_ENABLE();
    break;
#endif
#if (USE_S2868A1_RADIO_GPIO_3 == 1) 
  case S2868A1_RADIO_GPIO_3:
    S2868A1_RADIO_GPIO_3_GPIO_CLK_ENABLE();
    break;
#endif
  case S2868A1_RADIO_GPIO_SDN:
    S2868A1_RADIO_GPIO_SDN_CLOCK_ENABLE();
    break;
  }
  
  /* GPIO Init */
  /* Configures MCU GPIO */
  if(xGpioMode == RADIO_MODE_GPIO_OUT)
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  else
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pin = S2868A1_RADIO_GPIO_PIN[xGpio];
  HAL_GPIO_Init(S2868A1_RADIO_GPIO_PORT[xGpio], &GPIO_InitStructure);
  
  /* Configures MCU GPIO EXTI line */
  EXTI_InitStructure.Pull = GPIO_NOPULL;
  
  if(xGpioEdge == FALLING)
    EXTI_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  else if(xGpioEdge == RISING)
    EXTI_InitStructure.Mode = GPIO_MODE_IT_RISING;
  else if (xGpioEdge == RISING_FALLING)
    EXTI_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  else if (xGpioEdge == DEFAULT)
    EXTI_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  
  EXTI_InitStructure.Pin = S2868A1_RADIO_GPIO_PIN[xGpio];
  EXTI_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  
  if (xGpioMode == RADIO_MODE_EXTI_IN) {
    HAL_GPIO_Init(S2868A1_RADIO_GPIO_PORT[xGpio], &EXTI_InitStructure);   
  }  
}

/**
  * @brief  DeInit GPIO.
  * @param  None
  * @retval None
  */
static void GPIO_DeInit(void)
{
  {
  GPIO_InitTypeDef initStruct={0};
/*  SPI_HandleTypeDef dummy_hspi; */

  /* GPIO DeInit */
  
  initStruct.Mode = GPIO_MODE_IT_RISING ;
  initStruct.Pull = GPIO_PULLDOWN;

  for (uint32_t i = 0; i < S2868A1_RADIO_GPIOn ; i++)
  {
    initStruct.Pin = S2868A1_RADIO_GPIO_PIN[i];  
    HAL_GPIO_DeInit( S2868A1_RADIO_GPIO_PORT[i], initStruct.Pin );
  }
}
}

/**
  * @brief  Initializes SPI MSP.
  * @param  hspi  SPI handler
  * @retval None
  */
static void S2868A1_SPI_CS_Init()
{
  GPIO_InitTypeDef GPIO_InitStruct;
  S2868A1_RADIO_SPI_NSS_CLK_ENABLE();
  
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  
  GPIO_InitStruct.Pin = S2868A1_RADIO_SPI_NSS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(S2868A1_RADIO_SPI_NSS_PORT, &GPIO_InitStruct);
  
  S2868A1_RADIO_SPI_NSS_PIN_HIGH();
}


/**
  * @brief  Wrapper Function for S2868A1 SPI.
  * @param  pHeader is the header to send/receive
  * @param  pBuff is the buffer to write/read
  * @param  Length of data to be read
  * @retval TBD
  */
int32_t S2868A1_SPI_SendRecvWrapper(uint8_t *pHeader, uint8_t *pBuff, uint16_t Length)
{
  int32_t status;
  S2868A1_ENTER_CRITICAL();
  S2868A1_RADIO_SPI_NSS_PIN_LOW();
  status = S2868A1_SPI_SendRecv( pHeader, pHeader, 2 );
  if (!status)
  {
    if(Length)
    {
      status = S2868A1_SPI_SendRecv( pBuff, pBuff, Length );
    }
  }
  S2868A1_RADIO_SPI_NSS_PIN_HIGH();
  S2868A1_EXIT_CRITICAL();
  return status;  
}

/**
* @brief  Wait polling the SPI until the internal WIP flag is RESET.
*         The flag is SET when a write operation is running.
* @param  Instance
* @retval None
*/
static int32_t EEPROM_WaitEndWriteOperation(uint32_t Instance)
{
  
  UNUSED(Instance);    
  uint8_t cmd = EEPROM_CMD_RDSR;
  uint8_t dummy = 0xFF;
  uint8_t status;
  
  /* Put the SPI chip select low to start the transaction */
  EEPROM_NSS_PIN_LOW();
  
  /* Send command */
  EEPROM_SPI_SendRecv(&cmd, &status, 1);
  
  /* Polling on status register */
  do{
    EEPROM_SPI_SendRecv(&dummy, &status, 1);
  }while(status&EEPROM_STATUS_WIP);
  
  /* Put the SPI chip select high to end the transaction */
  EEPROM_NSS_PIN_HIGH();
  
  return S2868A1_ERROR_NONE;    
}

/******************************* LEds and Buttons Services ********************/

/**
  * @brief  This function initialize the GPIO to manage the Leds through GPIO
  * @param  Led: LED to be initialized. 
  * @return Status
  */
int32_t S2868A1_LED_Init(Led_t Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
  S2868A1_LED_GPIO_CLK_ENABLE();

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = aLED_GPIO_PIN[Led];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init(aLED_GPIO_PORT[Led], &GPIO_InitStruct);
  
  return S2868A1_ERROR_NONE;
}

/**
* @brief  Turns selected LED On.
* @param  Led: Specifies the Led to be set on. 
* @retval None
*/
int32_t S2868A1_LED_On(Led_t Led)
{
  HAL_GPIO_WritePin(aLED_GPIO_PORT[Led], aLED_GPIO_PIN[Led], GPIO_PIN_SET); 
  return S2868A1_ERROR_NONE;  
}

/**
* @brief  Turns selected LED Off. 
* @param  Led: Specifies the Led to be set off. 
* @retval None
*/
int32_t S2868A1_LED_Off(Led_t Led)
{
  HAL_GPIO_WritePin(aLED_GPIO_PORT[Led], aLED_GPIO_PIN[Led], GPIO_PIN_RESET); 
  return S2868A1_ERROR_NONE;
}

/**
* @brief  Toggles the selected LED.
* @param  Led: Specifies the Led to be toggled. 
* @retval None
*/
int32_t S2868A1_LED_Toggle(Led_t Led)
{
  HAL_GPIO_TogglePin(aLED_GPIO_PORT[Led], aLED_GPIO_PIN[Led]);
  return S2868A1_ERROR_NONE;
}

/**
  * @brief  Get the status of the selected LED.
  * @param  Led Specifies the Led to get its state.
  *         This parameter can be one of following parameters:
  *           @arg  LED2
  * @retval LED status
  */
int32_t S2868A1_LED_GetState(Led_t Led)
{
  return (int32_t)HAL_GPIO_ReadPin(aLED_GPIO_PORT[Led], aLED_GPIO_PIN[Led]);
}
/******************************* S2LP Radio Low level Services *****************/

/**
  * @brief  Radio Initialization 
  * @retval None
  */
int32_t S2868A1_RADIO_Init( void )
{
 S2LP_IO_t  IOCtx;
   
   /*register SPI bus function to the component */
   IOCtx.Init = S2868A1_SPI_Init;
   IOCtx.DeInit = S2868A1_SPI_DeInit;
   IOCtx.WriteBuffer = S2868A1_SPI_SendRecvWrapper;
   IOCtx.Delay = S2868A1_Delay;
   
   S2LP_RegisterBusIO(&IOCtx);
   S2868A1_RADIO_GPIO_Init(S2868A1_RADIO_GPIO_SDN, RADIO_MODE_GPIO_OUT, DEFAULT);
   S2868A1_SPI_CS_Init();
   S2868A1_RADIO_GPIO_Init(S2868A1_RADIO_GPIO, RADIO_MODE_EXTI_IN, FALLING);
  
  return S2868A1_ERROR_NONE;
}

/**
  * @brief  FunctionDescription
  * @retval Error Code
  */
int32_t S2868A1_RADIO_DeInit( void )
{
  GPIO_DeInit();
  return S2868A1_ERROR_NONE;    
}
/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2868A1_RADIO_Reset(void)
{
  return S2868A1_ERROR_FEATURE_NOT_SUPPORTED;  
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2868A1_RADIO_BoardSetXO( uint8_t state )
{
  UNUSED (state);
  return S2868A1_ERROR_FEATURE_NOT_SUPPORTED;  
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2868A1_RADIO_GetWakeUpTime( void )
{
  return  BOARD_WAKEUP_TIME;
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2868A1_RADIO_EnterShutdown( void )     
{
  HAL_GPIO_WritePin( S2868A1_RADIO_GPIO_SDN_PORT, S2868A1_RADIO_GPIO_SDN_PIN, GPIO_PIN_SET);
  return S2868A1_ERROR_NONE;       
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2868A1_RADIO_ExitShutdown( void )     /* Equivalent to RadioEnterShutdown and RadioExitShutdown*/
{
  HAL_GPIO_WritePin( S2868A1_RADIO_GPIO_SDN_PORT, S2868A1_RADIO_GPIO_SDN_PIN, GPIO_PIN_RESET);
  /* Delay to allow the circuit POR */
  HAL_Delay(1);
  return S2868A1_ERROR_NONE;       
}

/**
  * @brief  FunctionDescription                                                                                        
  * @retval None
  */
uint8_t S2868A1_RADIO_CheckShutdown( void )     
{
  uint8_t pinState;
  pinState = (uint8_t)HAL_GPIO_ReadPin( S2868A1_RADIO_GPIO_SDN_PORT, S2868A1_RADIO_GPIO_SDN_PIN);
  return pinState;       
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2868A1_RADIO_CheckRfFrequency( uint32_t frequency )
{
  return S2868A1_ERROR_NONE; 
}

/**
* @brief  Enables the interrupt on GPIO .
* @param  xGpio Specifies the GPIO whose priority shall be changed.
*         This parameter can be one of following parameters:
*         @arg M2S_GPIO_0: GPIO_0
*         @arg M2S_GPIO_1: GPIO_1
*         @arg M2S_GPIO_2: GPIO_2
*         @arg M2S_GPIO_3: GPIO_3
* @param  nPreemption Specifies Preemption Priority.
* @param  nSubpriority Specifies Subgroup Priority.
* @param  xNewState Specifies the State.
*         This parameter can be one of following parameters:
*         @arg ENABLE: Interrupt is enabled
*         @arg DISABLE: Interrupt is disabled
* @retval None.
*/
int32_t S2868A1_RADIO_IoIrqEnable(GpioIrqHandler **irqHanlder) 
{
  for (uint32_t GPIO = 0; GPIO < (S2868A1_RADIO_GPIOn - 1); GPIO++)
  {
    HAL_EXTI_GetHandle(&S2868A1_RADIO_GPIO_hexti[GPIO], S2868A1_RADIO_GPIO_EXTI_LINE[GPIO]);  
    HAL_EXTI_RegisterCallback(&S2868A1_RADIO_GPIO_hexti[GPIO],  HAL_EXTI_COMMON_CB_ID, irqHanlder[GPIO]);
    HAL_NVIC_SetPriority(S2868A1_RADIO_GPIO_IRQn[GPIO], S2868A1_RADIO_GPIO_IT_PRIO[GPIO], 0x00);
    HAL_NVIC_EnableIRQ( S2868A1_RADIO_GPIO_IRQn[GPIO]);    
  }
  return S2868A1_ERROR_NONE;     
} 

/**
* @brief  Disables the interrupt on GPIO .
* ...
*/
int32_t S2868A1_RADIO_IoIrqDisable(GpioIrqHandler **irqHanlder)
{
  for (uint32_t GPIO = 0; GPIO < (S2868A1_RADIO_GPIOn - 1); GPIO++)
  {
    HAL_NVIC_DisableIRQ( S2868A1_RADIO_GPIO_IRQn[GPIO]);    
  }
  return S2868A1_ERROR_NONE;     
} 

/**
* @brief  Initializes the SPI for the EEPROM.
* @param  Instance of EEPROM
* @retval Error Code
*/
int32_t S2868A1_EEPROM_Init(uint32_t Instance)
{
  UNUSED(Instance);
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure SPI pin: CS */
  GPIO_InitStructure.Pin = S2868A1_EEPROM_SPI_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(S2868A1_EEPROM_SPI_CS_PORT, &GPIO_InitStructure);
  
  /* Enable CS GPIO clock */
  S2868A1_EEPROM_SPI_CS_RCC();
  
  /* Put the SPI chip select high to end the transaction */
  EEPROM_NSS_PIN_HIGH();
  
  return S2868A1_ERROR_NONE;   
}

/**
  * @brief  DeInit the chip select pin of EEPROM.
  * @param  Instance of EEPROM
  * @retval Error Code
  */
int32_t  S2868A1_EEPROM_DeInit(uint32_t Instance)
{
   UNUSED(Instance);
   GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure SPI pin: CS */
  GPIO_InitStructure.Pin = S2868A1_EEPROM_SPI_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_DeInit(S2868A1_EEPROM_SPI_CS_PORT, GPIO_InitStructure.Pin);
   
  return S2868A1_ERROR_NONE;    
}

/**
* @brief  Set the ERSR status bit.
* @param  None
* @retval Status
*/
uint8_t BSP_EEPROM_SetSrwd(uint32_t Instance)
{
  uint8_t status[2];
  uint8_t cmd[] = {EEPROM_CMD_WRSR, EEPROM_STATUS_SRWD};
  
  /* Put the SPI chip select low to start the transaction */
  EEPROM_NSS_PIN_LOW();
  
  /* Send command */
  EEPROM_SPI_SendRecv(cmd, status, 2);
  
  /* Put the SPI chip select high to end the transaction */
  EEPROM_NSS_PIN_HIGH();
  
  return status[1];
}

/**
* @brief  Reset the ERSR status bit.
* @param  None
* @retval Status
*/
uint8_t BSP_EEPROM_ResetSrwd(uint32_t Instance)
{
  uint8_t status[2];
  uint8_t cmd[] = {EEPROM_CMD_WRSR, 0};
  
  BSP_EEPROM_WriteEnable(Instance);
  EEPROM_WaitEndWriteOperation(Instance);
  /* Put the SPI chip select low to start the transaction */
  EEPROM_NSS_PIN_LOW();
  
  /* Send command */
  EEPROM_SPI_SendRecv(cmd, status, 2);
  
  /* Put the SPI chip select high to end the transaction */
  EEPROM_NSS_PIN_HIGH();
  EEPROM_WaitEndWriteOperation(Instance);
  
  return status[1];
}

/**
* @brief  Set the internal WEL flag to allow write operation.
* @param  None
* @retval None
*/
int32_t BSP_EEPROM_WriteEnable(uint32_t Instance)
{
  UNUSED(Instance); 
  uint8_t cmd = EEPROM_CMD_WREN;
  uint8_t status;
  
  /* Put the SPI chip select low to start the transaction */
  EEPROM_NSS_PIN_LOW();
  
   /* Send command */
  EEPROM_SPI_SendRecv(&cmd, &status, 1);
  
  /* Put the SPI chip select high to end the transaction */
  EEPROM_NSS_PIN_HIGH();
  
  return S2868A1_ERROR_NONE; 
  
}

/**
* @brief  Write a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
*         It is allowed to write only a page for each operation. If the bytes
*         exceed the single page location, the other bytes are written at the 
*         beginning.
* @param  None
* @retval None
*/
int32_t S2868A1_EEPROM_WritePage(uint32_t Instance, uint16_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t cmd = EEPROM_CMD_WRITE, tmp[255];
  uint8_t address[2];
  UNUSED(Instance);
  
  /* Wait the end of a previous write operation */
  EEPROM_WaitEndWriteOperation(Instance);
  
  /* SET the WREN flag */
  BSP_EEPROM_WriteEnable(Instance);
  
   for(uint8_t k=0; k<2; k++) {
    address[k] = (uint8_t)(nAddress>>((1-k)*8));
  }
  
  
  /* Put the SPI chip select low to start the transaction */
  EEPROM_NSS_PIN_LOW();
  
  /* Write the header bytes and read the SPIRIT status bytes */
  EEPROM_SPI_SendRecv(&cmd,tmp, 1);
  EEPROM_SPI_SendRecv(address, tmp,2);
  EEPROM_SPI_SendRecv(pcBuffer, tmp, cNbBytes);
  
  /* Put the SPI chip select high to end the transaction */
  EEPROM_NSS_PIN_HIGH();
  return S2868A1_ERROR_NONE;
}

/**
* @brief  Read a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
* @param  None
* @retval None
*/
int32_t S2868A1_EEPROM_ReadPage(uint32_t Instance, uint16_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t cmd[3];
  uint8_t dummy[255];
  cmd[0] = EEPROM_CMD_READ;
  UNUSED(Instance);
  
  for(uint8_t k=0; k<2; k++) {
    cmd[k+1] = (uint8_t)(nAddress>>((1-k)*8));
  }  
  
  /* Wait the end of a previous write operation */
  EEPROM_WaitEndWriteOperation(Instance);
  
  /* Put the SPI chip select low to start the transaction */
  EEPROM_NSS_PIN_LOW();
  
  /* Write the header bytes and read status bytes */
  EEPROM_SPI_SendRecv(cmd, dummy, 3);
  
  /* Read the registers according to the number of bytes */
  EEPROM_SPI_SendRecv(dummy, pcBuffer, cNbBytes);
  
  /* Put the SPI chip select high to end the transaction */
  EEPROM_NSS_PIN_HIGH();
  return S2868A1_ERROR_NONE;
  
}
/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
int32_t S2868A1_EEPROM_IsReady(uint32_t Instance)
{
  int32_t errno = 0;
  
  uint8_t status[2];
  uint8_t cmd[] = {EEPROM_CMD_RDSR, 0xFF};
  
  /* Put the SPI chip select low to start the transaction */
  EEPROM_NSS_PIN_LOW();
  
  /* Send command */
  EEPROM_SPI_SendRecv(cmd, status, 2);
  
  /* Put the SPI chip select high to end the transaction */
  EEPROM_NSS_PIN_HIGH();
  
  if((status[1] & 0xF0) == EEPROM_STATUS_SRWD) {
    /* if it is EEPROM_STATUS_SRWD, ok the EEPROM is present and ready to work */
    errno = 1;
  }
  return errno;
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2868A1_RADIO_EnableTCXO(void)
{
  GPIO_InitTypeDef initStruct={0};
  
  S2868A1_RADIO_TCXO_CTL_GPIO_CLK_ENABLE();
    
  initStruct.Pull = GPIO_NOPULL;
  initStruct.Speed = GPIO_SPEED_FREQ_HIGH;  
  initStruct.Pin = S2868A1_RADIO_TCXO_CTL_PIN;
  initStruct.Mode = GPIO_MODE_OUTPUT_PP;
  
  HAL_GPIO_Init(S2868A1_RADIO_TCXO_CTL_PORT, &initStruct);
  HAL_GPIO_WritePin(S2868A1_RADIO_TCXO_CTL_PORT, S2868A1_RADIO_TCXO_CTL_PIN, GPIO_PIN_SET);
  
  return S2868A1_ERROR_NONE;   
}


  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

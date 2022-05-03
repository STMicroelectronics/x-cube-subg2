
 /*******************************************************************************
  * @file    S2915A1.c
  * @author  SRA Team
  * @brief   driver s2915A1 board
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */
  
/* Includes ------------------------------------------------------------------*/
#include "s2915a1.h"
#include "s2lp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BOARD_WAKEUP_TIME  0
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile int irq_disable_cnt = 0;

#if (S2915A1_LEDn > 0)
GPIO_TypeDef*  aLED_GPIO_PORT[S2915A1_LEDn] = {S2915A1_LED_GPIO_PORT};
const uint16_t aLED_GPIO_PIN[S2915A1_LEDn] = {S2915A1_LED_GPIO_PIN};
const uint32_t aLED_GPIO_PULL_MODE[S2915A1_LEDn] = 	{S2915A1_LED_GPIO_PULL_MODE};
const uint32_t aLED_GPIO_SPEED[S2915A1_LEDn] = 	{S2915A1_LED_GPIO_SPEED};
#endif /*S2915A1_LEDn > 0*/

EXTI_HandleTypeDef S2915A1_RADIO_GPIO_hexti[S2915A1_RADIO_GPIOn - 1] = {
#if (USE_S2915A1_RADIO_GPIO_0 == 1) 
                                    {.Line = S2915A1_RADIO_GPIO_0_EXTI_LINE},
#endif                               
#if (USE_S2915A1_RADIO_GPIO_1 == 1)                                    
                                    {.Line = S2915A1_RADIO_GPIO_1_EXTI_LINE},
#endif                               
#if (USE_S2915A1_RADIO_GPIO_2 == 1)       
                                     {.Line = S2915A1_RADIO_GPIO_2_EXTI_LINE},
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1)  
                                     {.Line = S2915A1_RADIO_GPIO_3_EXTI_LINE}
#endif                                                          
};

/* Private function prototypes -----------------------------------------------*/
static const S2915A1_RADIO_GPIO_Mode S2915A1_RADIO_GPIO_MODE[S2915A1_RADIO_GPIOn - 1] = {
#if (USE_S2915A1_RADIO_GPIO_0 == 1)
    S2915A1_RADIO_GPIO_0_MODE,
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1)
    S2915A1_RADIO_GPIO_1_MODE,
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1)
    S2915A1_RADIO_GPIO_2_MODE,
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1)
    S2915A1_RADIO_GPIO_3_MODE,
#endif
 };

static const uint32_t S2915A1_RADIO_GPIO_EDGE_MODE[S2915A1_RADIO_GPIOn - 1] = {
#if (USE_S2915A1_RADIO_GPIO_0 == 1)
    S2915A1_RADIO_GPIO_0_EXTI_MODE,
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1)
    S2915A1_RADIO_GPIO_1_EXTI_MODE,
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1)
    S2915A1_RADIO_GPIO_2_EXTI_MODE,
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1)
    S2915A1_RADIO_GPIO_3_EXTI_MODE,
#endif
 };

static const uint32_t S2915A1_RADIO_GPIO_EXTI_LINE[S2915A1_RADIO_GPIOn - 1] = {
#if (USE_S2915A1_RADIO_GPIO_0 == 1) 
    S2915A1_RADIO_GPIO_0_EXTI_LINE,
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1) 
    S2915A1_RADIO_GPIO_1_EXTI_LINE,
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1) 
    S2915A1_RADIO_GPIO_2_EXTI_LINE,
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1) 
    S2915A1_RADIO_GPIO_3_EXTI_LINE,  
#endif
 };
  
static const uint32_t S2915A1_RADIO_GPIO_IT_PRIO [S2915A1_RADIO_GPIOn - 1] = {
#if (USE_S2915A1_RADIO_GPIO_0 == 1) 
    S2915A1_RADIO_GPIO_0_IT_PRIO,
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1) 
    S2915A1_RADIO_GPIO_1_IT_PRIO,
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1)    
    S2915A1_RADIO_GPIO_2_IT_PRIO,
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1) 
    S2915A1_RADIO_GPIO_3_IT_PRIO, 
#endif
 };

static const uint32_t S2915A1_RADIO_GPIO_IT_SUBPRIO [S2915A1_RADIO_GPIOn - 1] = {
#if (USE_S2915A1_RADIO_GPIO_0 == 1)
    S2915A1_RADIO_GPIO_0_IT_SUBPRIO,
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1)
    S2915A1_RADIO_GPIO_1_IT_SUBPRIO,
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1)
    S2915A1_RADIO_GPIO_2_IT_SUBPRIO,
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1)
    S2915A1_RADIO_GPIO_3_IT_SUBPRIO, 
#endif
 };
  
static const IRQn_Type S2915A1_RADIO_GPIO_IRQn [S2915A1_RADIO_GPIOn - 1]   = {
#if (USE_S2915A1_RADIO_GPIO_0 == 1) 
    S2915A1_RADIO_GPIO_0_IRQn,
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1) 
    S2915A1_RADIO_GPIO_1_IRQn,
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1) 
    S2915A1_RADIO_GPIO_2_IRQn,
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1) 
    S2915A1_RADIO_GPIO_3_IRQn  
#endif
 }; 
 
static const uint32_t S2915A1_RADIO_GPIO_SPEED[S2915A1_RADIO_GPIOn] = {
#if (USE_S2915A1_RADIO_GPIO_0 == 1)
    S2915A1_RADIO_GPIO_0_SPEED,
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1)
    S2915A1_RADIO_GPIO_1_SPEED,
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1)
    S2915A1_RADIO_GPIO_2_SPEED,
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1)
    S2915A1_RADIO_GPIO_3_SPEED,  
#endif
    S2915A1_RADIO_GPIO_SDN_SPEED
 };

static const uint32_t S2915A1_RADIO_GPIO_PULL_MODE[S2915A1_RADIO_GPIOn] = {
#if (USE_S2915A1_RADIO_GPIO_0 == 1)
    S2915A1_RADIO_GPIO_0_PULL_MODE,
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1)
    S2915A1_RADIO_GPIO_1_PULL_MODE,
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1)
    S2915A1_RADIO_GPIO_2_PULL_MODE,
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1)
    S2915A1_RADIO_GPIO_3_PULL_MODE,  
#endif
   S2915A1_RADIO_GPIO_SDN_PULL_MODE
 };

static GPIO_TypeDef* S2915A1_RADIO_GPIO_PORT[S2915A1_RADIO_GPIOn] = {
#if (USE_S2915A1_RADIO_GPIO_0 == 1) 
  S2915A1_RADIO_GPIO_0_PORT,
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1) 
  S2915A1_RADIO_GPIO_1_PORT,
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1) 
  S2915A1_RADIO_GPIO_2_PORT,
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1) 
  S2915A1_RADIO_GPIO_3_PORT,
#endif 
  S2915A1_RADIO_GPIO_SDN_PORT
};

static const uint16_t S2915A1_RADIO_GPIO_PIN[S2915A1_RADIO_GPIOn] = {
#if (USE_S2915A1_RADIO_GPIO_0 == 1) 
  S2915A1_RADIO_GPIO_0_PIN,
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1) 
  S2915A1_RADIO_GPIO_1_PIN,
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1) 
  S2915A1_RADIO_GPIO_2_PIN,
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1) 
  S2915A1_RADIO_GPIO_3_PIN,
#endif
  S2915A1_RADIO_GPIO_SDN_PIN
};
/* Private function prototypes -----------------------------------------------*/
static void S2915A1_SPI_CS_Init(void);
static void GPIO_DeInit(void);
int32_t S2915A1_SPI_SendRecvWrapper(uint8_t *pTxData, uint8_t *pRxData, uint16_t Length);
static int32_t EEPROM_WaitEndWriteOperation(uint32_t Instance);
static uint8_t _isBypassEnabled = 0;
static void S2915A1_RADIO_GPIO_Init( S2915A1_RADIO_GPIO_TypeDef xGpio, S2915A1_RADIO_GPIO_Mode xGpioMode); 

/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  Legacy API to change settings at runtime
  * @param  xGpio the GPIO 
  * @param  xGpioMode the gpio mode
  * @param  xGpioEdge the gpio edge
  * @retval None
  */
void S2915A1_RADIO_GPIO_Init_Update( S2915A1_RADIO_GPIO_TypeDef xGpio, S2915A1_RADIO_GPIO_Mode xGpioMode , S2915A1_RADIO_GPIO_EDGE_Mode xGpioEdge)  
{
  GPIO_InitTypeDef GPIO_InitStructure, EXTI_InitStructure;
  
  /* Enable Radio GPIO clock */
  switch(xGpio)
  {
#if (USE_S2915A1_RADIO_GPIO_0 == 1) 
  case S2915A1_RADIO_GPIO_0:
    S2915A1_RADIO_GPIO_0_GPIO_CLK_ENABLE();
    break;
#endif    
#if (USE_S2915A1_RADIO_GPIO_1 == 1)  
  case S2915A1_RADIO_GPIO_1:
    S2915A1_RADIO_GPIO_1_GPIO_CLK_ENABLE();
    break;
#endif 
#if (USE_S2915A1_RADIO_GPIO_2 == 1)  
  case S2915A1_RADIO_GPIO_2:
    S2915A1_RADIO_GPIO_2_GPIO_CLK_ENABLE();
    break;
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1) 
  case S2915A1_RADIO_GPIO_3:
    S2915A1_RADIO_GPIO_3_GPIO_CLK_ENABLE();
    break;
#endif
  case S2915A1_RADIO_GPIO_SDN:
    S2915A1_RADIO_GPIO_SDN_CLOCK_ENABLE();
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
  GPIO_InitStructure.Pin = S2915A1_RADIO_GPIO_PIN[xGpio];
  HAL_GPIO_Init(S2915A1_RADIO_GPIO_PORT[xGpio], &GPIO_InitStructure);
  
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
  
  EXTI_InitStructure.Pin = S2915A1_RADIO_GPIO_PIN[xGpio];
  EXTI_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  
  if (xGpioMode == RADIO_MODE_EXTI_IN) {
    HAL_GPIO_Init(S2915A1_RADIO_GPIO_PORT[xGpio], &EXTI_InitStructure);   
  }  
}

/**
  * @brief  Function for Radio IO initialization
  * @param  xGpio the GPIO 
  * @param  xGpioMode the gpio mode
  * @retval None
  */
void S2915A1_RADIO_GPIO_Init( S2915A1_RADIO_GPIO_TypeDef xGpio, S2915A1_RADIO_GPIO_Mode xGpioMode)
{
   GPIO_InitTypeDef GPIO_InitStructure, EXTI_InitStructure;
  
  /* Enable Radio GPIO clock */
  switch(xGpio)
  {
#if (USE_S2915A1_RADIO_GPIO_0 == 1) 
  case S2915A1_RADIO_GPIO_0:
    S2915A1_RADIO_GPIO_0_GPIO_CLK_ENABLE();
    break;
#endif    
#if (USE_S2915A1_RADIO_GPIO_1 == 1)  
  case S2915A1_RADIO_GPIO_1:
    S2915A1_RADIO_GPIO_1_GPIO_CLK_ENABLE();
    break;
#endif 
#if (USE_S2915A1_RADIO_GPIO_2 == 1)  
  case S2915A1_RADIO_GPIO_2:
    S2915A1_RADIO_GPIO_2_GPIO_CLK_ENABLE();
    break;
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1) 
  case S2915A1_RADIO_GPIO_3:
    S2915A1_RADIO_GPIO_3_GPIO_CLK_ENABLE();
    break;
#endif
  case S2915A1_RADIO_GPIO_SDN:
    S2915A1_RADIO_GPIO_SDN_CLOCK_ENABLE();
    break;
  }
  
  /* GPIO Init */
  if (xGpioMode == RADIO_MODE_EXTI_IN) {
	/* Configures MCU GPIO EXTI line */
	EXTI_InitStructure.Pin = S2915A1_RADIO_GPIO_PIN[xGpio];
	EXTI_InitStructure.Mode = S2915A1_RADIO_GPIO_EDGE_MODE[xGpio];
    EXTI_InitStructure.Pull = S2915A1_RADIO_GPIO_PULL_MODE[xGpio];
    HAL_GPIO_Init(S2915A1_RADIO_GPIO_PORT[xGpio], &EXTI_InitStructure);   
  }  else {
	  /* Configures MCU GPIO */
	  if(xGpioMode == RADIO_MODE_GPIO_OUT){
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	    GPIO_InitStructure.Speed = S2915A1_RADIO_GPIO_SPEED[xGpio];
	  } else {
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	  }
	  GPIO_InitStructure.Pull = S2915A1_RADIO_GPIO_PULL_MODE[xGpio];
	  GPIO_InitStructure.Pin = S2915A1_RADIO_GPIO_PIN[xGpio];
	  HAL_GPIO_Init(S2915A1_RADIO_GPIO_PORT[xGpio], &GPIO_InitStructure);
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
    
    for (uint32_t i = 0; i < S2915A1_RADIO_GPIOn ; i++)
    {
      initStruct.Pin = S2915A1_RADIO_GPIO_PIN[i];  
      HAL_GPIO_DeInit( S2915A1_RADIO_GPIO_PORT[i], initStruct.Pin );
    }
  }
}

/**
  * @brief  Initializes SPI MSP.
  * @param  hspi  SPI handler
  * @retval None
  */
static void S2915A1_SPI_CS_Init()
{
  GPIO_InitTypeDef GPIO_InitStruct;
  S2915A1_RADIO_SPI_NSS_CLK_ENABLE();
  
  GPIO_InitStruct.Pull = S2915A1_RADIO_SPI_NSS_PULL_MODE;
  GPIO_InitStruct.Speed = S2915A1_RADIO_SPI_NSS_SPEED;
  GPIO_InitStruct.Pin = S2915A1_RADIO_SPI_NSS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(S2915A1_RADIO_SPI_NSS_PORT, &GPIO_InitStruct);
  
  S2915A1_RADIO_SPI_NSS_PIN_HIGH();
}


/**
  * @brief  Wrapper Function for S2915A1 SPI.
  * @param  pHeader is the header to send/receive
  * @param  pBuff is the buffer to write/read
  * @param  Length of data to be read
  * @retval TBD
  */
int32_t S2915A1_SPI_SendRecvWrapper(uint8_t *pHeader, uint8_t *pBuff, uint16_t Length)
{
  int32_t status;
  S2915A1_ENTER_CRITICAL();
  S2915A1_RADIO_SPI_NSS_PIN_LOW();
  status = S2915A1_SPI_SendRecv( pHeader, pHeader, 2 );
  if (!status)
  {
    if(Length)
    {
      status = S2915A1_SPI_SendRecv( pBuff, pBuff, Length );
    }
  }
  S2915A1_RADIO_SPI_NSS_PIN_HIGH();
  S2915A1_EXIT_CRITICAL();
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
  
  return S2915A1_ERROR_NONE;    
}

/******************************* LEds and Buttons Services ********************/

/**
  * @brief  This function initialize the GPIO to manage the Leds through GPIO
  * @param  Led: LED to be initialized. 
  * @return Status
  */
int32_t S2915A1_LED_Init(Led_t Led)
{
#if (S2915A1_LEDn > 0)
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
  S2915A1_LED_GPIO_CLK_ENABLE();

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = aLED_GPIO_PIN[Led];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = aLED_GPIO_PULL_MODE[Led];
  GPIO_InitStruct.Speed = aLED_GPIO_SPEED[Led];
  
  HAL_GPIO_Init(aLED_GPIO_PORT[Led], &GPIO_InitStruct);
#endif /*(S2915A1_LEDn > 0)*/  

  return S2915A1_ERROR_NONE;
}

/**
* @brief  Turns selected LED On.
* @param  Led: Specifies the Led to be set on. 
* @retval None
*/
int32_t S2915A1_LED_On(Led_t Led)
{
#if (S2915A1_LEDn > 0)
  HAL_GPIO_WritePin(aLED_GPIO_PORT[Led], aLED_GPIO_PIN[Led], GPIO_PIN_SET); 
#endif /* (S2915A1_LEDn > 0) */

  return S2915A1_ERROR_NONE;  
}

/**
* @brief  Turns selected LED Off. 
* @param  Led: Specifies the Led to be set off. 
* @retval None
*/
int32_t S2915A1_LED_Off(Led_t Led)
{
#if (S2915A1_LEDn > 0)
  HAL_GPIO_WritePin(aLED_GPIO_PORT[Led], aLED_GPIO_PIN[Led], GPIO_PIN_RESET); 
#endif /*(S2915A1_LEDn > 0)*/ 

  return S2915A1_ERROR_NONE;
}

/**
* @brief  Toggles the selected LED.
* @param  Led: Specifies the Led to be toggled. 
* @retval None
*/
int32_t S2915A1_LED_Toggle(Led_t Led)
{
#if (S2915A1_LEDn > 0)
  HAL_GPIO_TogglePin(aLED_GPIO_PORT[Led], aLED_GPIO_PIN[Led]);
#endif /*(S2915A1_LEDn > 0)*/ 
  return S2915A1_ERROR_NONE;
}

/**
  * @brief  Get the status of the selected LED.
  * @param  Led Specifies the Led to get its state.
  *         This parameter can be one of following parameters:
  *           @arg  LED2
  * @retval LED status
  */
int32_t S2915A1_LED_GetState(Led_t Led)
{
#if (S2915A1_LEDn > 0)
  return (int32_t)HAL_GPIO_ReadPin(aLED_GPIO_PORT[Led], aLED_GPIO_PIN[Led]);
#else
  return 0;
#endif /*(S2915A1_LEDn > 0)*/
}
/******************************* S2LP Radio Low level Services *****************/

/**
  * @brief  Radio Initialization 
  * @retval None
  */
int32_t S2915A1_RADIO_Init( void )
{
   S2LP_IO_t  IOCtx;
   
   /*register SPI bus function to the component */
   IOCtx.Init = S2915A1_SPI_Init;
   IOCtx.DeInit = S2915A1_SPI_DeInit;
   IOCtx.WriteBuffer = S2915A1_SPI_SendRecvWrapper;
   IOCtx.Delay = S2915A1_Delay;
   
   S2LP_RegisterBusIO(&IOCtx);
   S2915A1_RADIO_GPIO_Init(S2915A1_RADIO_GPIO_SDN, RADIO_MODE_GPIO_OUT);
   S2915A1_SPI_CS_Init();

   for (uint32_t GPIO = 0; GPIO < (S2915A1_RADIO_GPIOn - 1); GPIO++)
   {
		S2915A1_RADIO_GPIO_Init((S2915A1_RADIO_GPIO_TypeDef) GPIO, S2915A1_RADIO_GPIO_MODE[GPIO]);
   }
  
  return S2915A1_ERROR_NONE;
}

/**
  * @brief  FunctionDescription
  * @retval Error Code
  */
int32_t S2915A1_RADIO_DeInit( void )
{
  GPIO_DeInit();
  return S2915A1_ERROR_NONE;    
}
/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2915A1_RADIO_Reset(void)
{
  return S2915A1_ERROR_FEATURE_NOT_SUPPORTED;  
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2915A1_RADIO_BoardSetXO( uint8_t state )
{
  UNUSED (state);
  return S2915A1_ERROR_FEATURE_NOT_SUPPORTED;  
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2915A1_RADIO_GetWakeUpTime( void )
{
  return  BOARD_WAKEUP_TIME;
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2915A1_RADIO_EnterShutdown( void )     
{
  HAL_GPIO_WritePin( S2915A1_RADIO_GPIO_SDN_PORT, S2915A1_RADIO_GPIO_SDN_PIN, GPIO_PIN_SET);
  return S2915A1_ERROR_NONE;       
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2915A1_RADIO_ExitShutdown( void )     /* Equivalent to RadioEnterShutdown and RadioExitShutdown*/
{
  HAL_GPIO_WritePin( S2915A1_RADIO_GPIO_SDN_PORT, S2915A1_RADIO_GPIO_SDN_PIN, GPIO_PIN_RESET);
  /* Delay to allow the circuit POR */
  HAL_Delay(1);
  return S2915A1_ERROR_NONE;       
}


/**
  * @brief  FunctionDescription                                                                                        
  * @retval None
  */
uint8_t S2915A1_RADIO_CheckShutdown( void )     
{
  uint8_t pinState;
  pinState = (uint8_t)HAL_GPIO_ReadPin( S2915A1_RADIO_GPIO_SDN_PORT, S2915A1_RADIO_GPIO_SDN_PIN);
  return pinState;       
}


/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t S2915A1_RADIO_CheckRfFrequency( uint32_t frequency )
{
  return S2915A1_ERROR_NONE; 
}

/**
* @brief  Enables or disables the interrupt on GPIO .
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
int32_t S2915A1_RADIO_IoIrqEnable(GpioIrqHandler **irqHanlder) 
{
  for (uint32_t GPIO = 0; GPIO < (S2915A1_RADIO_GPIOn - 1); GPIO++)
  {
	  if (S2915A1_RADIO_GPIO_MODE[GPIO] == RADIO_MODE_EXTI_IN)
	  {
		/* For S2915A1 board the above check could be skipped, but we keep it to be in line with the 
		 * coding of S2868Ax boards. */
		HAL_EXTI_GetHandle(&S2915A1_RADIO_GPIO_hexti[GPIO], S2915A1_RADIO_GPIO_EXTI_LINE[GPIO]);  
		HAL_EXTI_RegisterCallback(&S2915A1_RADIO_GPIO_hexti[GPIO],  HAL_EXTI_COMMON_CB_ID, irqHanlder[GPIO]);

		HAL_NVIC_SetPriority(S2915A1_RADIO_GPIO_IRQn[GPIO], S2915A1_RADIO_GPIO_IT_PRIO[GPIO], S2915A1_RADIO_GPIO_IT_SUBPRIO[GPIO]);
		
		HAL_NVIC_EnableIRQ( S2915A1_RADIO_GPIO_IRQn[GPIO]);    
	  }
  }
  return S2915A1_ERROR_NONE;     
} 

/**
* @brief  Disables the interrupt on GPIO .
* ...
*/
int32_t S2915A1_RADIO_IoIrqDisable(GpioIrqHandler **irqHanlder)
{
  for (uint32_t GPIO = 0; GPIO < (S2915A1_RADIO_GPIOn - 1); GPIO++)
  {
	if (S2915A1_RADIO_GPIO_MODE[GPIO] == RADIO_MODE_EXTI_IN)
	{
		/* For S2915A1 board the above check could be skipped, but we keep it to be in line with the 
		 * coding of S2868Ax boards. */
		HAL_NVIC_DisableIRQ( S2915A1_RADIO_GPIO_IRQn[GPIO]);    
	}
  }
  return S2915A1_ERROR_NONE;     
} 

/**
* @brief  Initializes the SPI for the EEPROM.
* @param  Instance of EEPROM
* @retval Error Code
*/
int32_t S2915A1_EEPROM_Init(uint32_t Instance)
{
  UNUSED(Instance);
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure SPI pin: CS */
  GPIO_InitStructure.Pin = S2915A1_EEPROM_SPI_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = S2915A1_EEPROM_SPI_CS_PULL_MODE;
  GPIO_InitStructure.Speed = S2915A1_EEPROM_SPI_CS_SPEED;
  HAL_GPIO_Init(S2915A1_EEPROM_SPI_CS_PORT, &GPIO_InitStructure);
  
  /* Enable CS GPIO clock */
  S2915A1_EEPROM_SPI_CS_RCC();
  
  /* Put the SPI chip select high to end the transaction */
  EEPROM_NSS_PIN_HIGH();
  
  return S2915A1_ERROR_NONE;   
}

/**
  * @brief  DeInit the chip select pin of EEPROM.
  * @param  Instance of EEPROM
  * @retval Error Code
  */
int32_t  S2915A1_EEPROM_DeInit(uint32_t Instance)
{
   UNUSED(Instance);
   GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure SPI pin: CS */
  GPIO_InitStructure.Pin = S2915A1_EEPROM_SPI_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = S2915A1_EEPROM_SPI_CS_PULL_MODE;
  GPIO_InitStructure.Speed = S2915A1_EEPROM_SPI_CS_SPEED;
  HAL_GPIO_DeInit(S2915A1_EEPROM_SPI_CS_PORT, GPIO_InitStructure.Pin);
   
  return S2915A1_ERROR_NONE;    
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
  
  return S2915A1_ERROR_NONE; 
  
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
int32_t S2915A1_EEPROM_WritePage(uint32_t Instance, uint16_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer)
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
  return S2915A1_ERROR_NONE;
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
int32_t S2915A1_EEPROM_ReadPage(uint32_t Instance, uint16_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer)
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
  return S2915A1_ERROR_NONE;
  
}
/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
int32_t S2915A1_EEPROM_IsReady(uint32_t Instance)
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
int32_t S2915A1_RADIO_EnableTCXO(void)
{
  GPIO_InitTypeDef initStruct={0};
  
  S2915A1_RADIO_TCXO_CTL_GPIO_CLK_ENABLE();
    
  initStruct.Pull = GPIO_NOPULL;
  initStruct.Speed = GPIO_SPEED_FREQ_HIGH;  
  initStruct.Pin = S2915A1_RADIO_TCXO_CTL_PIN;
  initStruct.Mode = GPIO_MODE_OUTPUT_PP;
  
  HAL_GPIO_Init(S2915A1_RADIO_TCXO_CTL_PORT, &initStruct);
  HAL_GPIO_WritePin(S2915A1_RADIO_TCXO_CTL_PORT, S2915A1_RADIO_TCXO_CTL_PIN, GPIO_PIN_SET);
  
  return S2915A1_ERROR_NONE;   
}

/**
* @brief  Configure Range ext (PA).
* @param  None.
* @retval None.
*/
/*  gpio0 :  PA0 : CSD
    gpio1 :  PA4 : CPS
    gpio2 :  PB0 : CTX */
void S2915A1_RADIO_RangeExtGpioInit(void)
{
    /* Configures MCU GPIOs to drive external PA */
    GPIO_InitTypeDef GPIO_InitStructure;

    /*CSD*/
    S2915A1_PA_CSD_GPIO_CLK();
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = S2915A1_PA_CSD_PULL_MODE;
    GPIO_InitStructure.Speed = S2915A1_PA_CSD_SPEED;
    GPIO_InitStructure.Pin = S2915A1_PA_CSD_PIN;
    HAL_GPIO_Init(S2915A1_PA_CSD_PORT, &GPIO_InitStructure);

    /*CPS*/
    S2915A1_PA_CPS_GPIO_CLK();
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = S2915A1_PA_CPS_PULL_MODE;
    GPIO_InitStructure.Speed = S2915A1_PA_CPS_SPEED;
    GPIO_InitStructure.Pin = S2915A1_PA_CPS_PIN;
    HAL_GPIO_Init(S2915A1_PA_CPS_PORT, &GPIO_InitStructure);

    /*CTX*/
    S2915A1_PA_CTX_GPIO_CLK();
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = S2915A1_PA_CTX_PULL_MODE;
    GPIO_InitStructure.Speed = S2915A1_PA_CTX_SPEED;
    GPIO_InitStructure.Pin = S2915A1_PA_CTX_PIN;
    HAL_GPIO_Init(S2915A1_PA_CTX_PORT, &GPIO_InitStructure);
}


/**
* @brief  Operate Range ext (PA).
* @param  None.
* @retval None.
*/
/*  gpio0 :  PA0 : CSD
    gpio1 :  PA4 : CPS
    gpio2 :  PB0 : CTX */
void S2915A1_RADIO_RangeExtOperate(FEM_OperationType operation)
{ 
  
  S2915A1_RADIO_RangeExtGpioInit();
  
  switch (operation)
  {
  case FEM_SHUTDOWN:
    {
      /* Puts CSD high to turn on PA */
      HAL_GPIO_WritePin(S2915A1_PA_CSD_PORT, S2915A1_PA_CSD_PIN, GPIO_PIN_RESET);
      
      /* Puts CTX high to go in TX state DON'T CARE */
      HAL_GPIO_WritePin(S2915A1_PA_CTX_PORT, S2915A1_PA_CTX_PIN, GPIO_PIN_SET);
      
      /*No Bypass mode select DON'T CARE  */
      HAL_GPIO_WritePin(S2915A1_PA_CPS_PORT, S2915A1_PA_CPS_PIN, GPIO_PIN_SET);
      break;
    }
  case FEM_TX_BYPASS:
    {
      /* Puts CSD high to turn on PA */
      HAL_GPIO_WritePin(S2915A1_PA_CSD_PORT, S2915A1_PA_CSD_PIN, GPIO_PIN_SET);
      
      /* Puts CTX high to go in TX state */
      HAL_GPIO_WritePin(S2915A1_PA_CTX_PORT, S2915A1_PA_CTX_PIN, GPIO_PIN_SET);
      
      /*Bypass mode select  */
      HAL_GPIO_WritePin(S2915A1_PA_CPS_PORT, S2915A1_PA_CPS_PIN, GPIO_PIN_RESET);
      break;
    }
  case FEM_TX:
    {
      /* Puts CSD high to turn on PA */
      HAL_GPIO_WritePin(S2915A1_PA_CSD_PORT, S2915A1_PA_CSD_PIN, GPIO_PIN_SET);
      
      /* Puts CTX high to go in TX state */
      HAL_GPIO_WritePin(S2915A1_PA_CTX_PORT, S2915A1_PA_CTX_PIN, GPIO_PIN_SET);
      
      /* Check Bypass mode */
      if (S2915A1_FEM_GetBypass())
        HAL_GPIO_WritePin(S2915A1_PA_CPS_PORT, S2915A1_PA_CPS_PIN, GPIO_PIN_RESET);
      else
        HAL_GPIO_WritePin(S2915A1_PA_CPS_PORT, S2915A1_PA_CPS_PIN, GPIO_PIN_SET);
      break;
    }
  case FEM_RX:
    {
      /* Puts CSD high to turn on PA */
      HAL_GPIO_WritePin(S2915A1_PA_CSD_PORT, S2915A1_PA_CSD_PIN, GPIO_PIN_SET);
      
      /* Puts CTX low */
      HAL_GPIO_WritePin(S2915A1_PA_CTX_PORT, S2915A1_PA_CTX_PIN, GPIO_PIN_RESET);
      
      /*No Bypass mode select DON'T CARE  */
      HAL_GPIO_WritePin(S2915A1_PA_CPS_PORT, S2915A1_PA_CPS_PIN, GPIO_PIN_SET);
      break;
    }
  default:
    /* Error */
    break;
  }
}

/**
  * @brief  Function to bypass the FEM
  * @param  bypass_mode Bypass mode
  * @retval None
  */
void S2915A1_FEM_SetBypass(uint8_t bypass_mode)
{
  _isBypassEnabled = bypass_mode;
}

/**
  * @brief  Function to get the bypass status
  * @retval The FEM status
  */
uint8_t S2915A1_FEM_GetBypass(void)
{
  return _isBypassEnabled;
}

__weak void FEM_Operation(FEM_OperationType operation)
{
	
}

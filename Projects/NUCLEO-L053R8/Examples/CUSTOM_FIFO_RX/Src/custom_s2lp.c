
/*******************************************************************************
 * @file    custom_s2lp.c
 * @author  SRA Team
 * @brief   driver custom_s2lp board
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

/* Includes ------------------------------------------------------------------*/
#include "s2lp.h"
#include "custom_s2lp.h"
#include "custom_s2lp_conf.h"
#include "custom_s2lpgpio_setting.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BOARD_WAKEUP_TIME  0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

EXTI_HandleTypeDef CUSTOM_S2LP_RADIO_GPIO_hexti[CUSTOM_S2LP_RADIO_GPIOn - 1] = {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
                                    {.Line = CUSTOM_S2LP_RADIO_GPIO_0_EXTI_LINE},
#endif

#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
                                    {.Line = CUSTOM_S2LP_RADIO_GPIO_1_EXTI_LINE},
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
                                     {.Line = CUSTOM_S2LP_RADIO_GPIO_2_EXTI_LINE},
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
                                     {.Line = CUSTOM_S2LP_RADIO_GPIO_3_EXTI_LINE}
#endif
};

static const CUSTOM_S2LP_RADIO_GPIO_Mode CUSTOM_S2LP_RADIO_GPIO_MODE[CUSTOM_S2LP_RADIO_GPIOn - 1] = {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
    CUSTOM_S2LP_RADIO_GPIO_0_MODE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
    CUSTOM_S2LP_RADIO_GPIO_1_MODE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
    CUSTOM_S2LP_RADIO_GPIO_2_MODE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
    CUSTOM_S2LP_RADIO_GPIO_3_MODE,
#endif
 };

static const uint32_t CUSTOM_S2LP_RADIO_GPIO_EDGE_MODE[CUSTOM_S2LP_RADIO_GPIOn - 1] = {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
    CUSTOM_S2LP_RADIO_GPIO_0_EXTI_MODE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
    CUSTOM_S2LP_RADIO_GPIO_1_EXTI_MODE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
    CUSTOM_S2LP_RADIO_GPIO_2_EXTI_MODE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
    CUSTOM_S2LP_RADIO_GPIO_3_EXTI_MODE,
#endif
 };

static const uint32_t CUSTOM_S2LP_RADIO_GPIO_EXTI_LINE[CUSTOM_S2LP_RADIO_GPIOn - 1] = {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
    CUSTOM_S2LP_RADIO_GPIO_0_EXTI_LINE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
    CUSTOM_S2LP_RADIO_GPIO_1_EXTI_LINE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
    CUSTOM_S2LP_RADIO_GPIO_2_EXTI_LINE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
    CUSTOM_S2LP_RADIO_GPIO_3_EXTI_LINE,
#endif
 };

static const uint32_t CUSTOM_S2LP_RADIO_GPIO_IT_PRIO [CUSTOM_S2LP_RADIO_GPIOn - 1] = {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
    CUSTOM_S2LP_RADIO_GPIO_0_IT_PRIO,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
    CUSTOM_S2LP_RADIO_GPIO_1_IT_PRIO,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
    CUSTOM_S2LP_RADIO_GPIO_2_IT_PRIO,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
    CUSTOM_S2LP_RADIO_GPIO_3_IT_PRIO,
#endif
 };

static const uint32_t CUSTOM_S2LP_RADIO_GPIO_IT_SUBPRIO [CUSTOM_S2LP_RADIO_GPIOn - 1] = {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
    CUSTOM_S2LP_RADIO_GPIO_0_IT_SUBPRIO,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
    CUSTOM_S2LP_RADIO_GPIO_1_IT_SUBPRIO,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
    CUSTOM_S2LP_RADIO_GPIO_2_IT_SUBPRIO,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
    CUSTOM_S2LP_RADIO_GPIO_3_IT_SUBPRIO,
#endif
 };

static const IRQn_Type CUSTOM_S2LP_RADIO_GPIO_IRQn [CUSTOM_S2LP_RADIO_GPIOn - 1]   = {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
    CUSTOM_S2LP_RADIO_GPIO_0_IRQn,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
    CUSTOM_S2LP_RADIO_GPIO_1_IRQn,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
    CUSTOM_S2LP_RADIO_GPIO_2_IRQn,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
    CUSTOM_S2LP_RADIO_GPIO_3_IRQn
#endif
 };

static const uint32_t CUSTOM_S2LP_RADIO_GPIO_SPEED[CUSTOM_S2LP_RADIO_GPIOn] = {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
    CUSTOM_S2LP_RADIO_GPIO_0_SPEED,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
    CUSTOM_S2LP_RADIO_GPIO_1_SPEED,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
    CUSTOM_S2LP_RADIO_GPIO_2_SPEED,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
    CUSTOM_S2LP_RADIO_GPIO_3_SPEED,
#endif
    CUSTOM_S2LP_RADIO_GPIO_SDN_SPEED
 };

static const uint32_t CUSTOM_S2LP_RADIO_GPIO_PULL_MODE[CUSTOM_S2LP_RADIO_GPIOn] = {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
    CUSTOM_S2LP_RADIO_GPIO_0_PULL_MODE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
    CUSTOM_S2LP_RADIO_GPIO_1_PULL_MODE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
    CUSTOM_S2LP_RADIO_GPIO_2_PULL_MODE,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
    CUSTOM_S2LP_RADIO_GPIO_3_PULL_MODE,
#endif
   CUSTOM_S2LP_RADIO_GPIO_SDN_PULL_MODE
 };

static GPIO_TypeDef* CUSTOM_S2LP_RADIO_GPIO_PORT[CUSTOM_S2LP_RADIO_GPIOn] = {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
  CUSTOM_S2LP_RADIO_GPIO_0_PORT,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
  CUSTOM_S2LP_RADIO_GPIO_1_PORT,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
  CUSTOM_S2LP_RADIO_GPIO_2_PORT,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
  CUSTOM_S2LP_RADIO_GPIO_3_PORT,
#endif
  CUSTOM_S2LP_RADIO_GPIO_SDN_PORT
};

static const uint16_t CUSTOM_S2LP_RADIO_GPIO_PIN[CUSTOM_S2LP_RADIO_GPIOn] = {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
  CUSTOM_S2LP_RADIO_GPIO_0_PIN,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
  CUSTOM_S2LP_RADIO_GPIO_1_PIN,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
  CUSTOM_S2LP_RADIO_GPIO_2_PIN,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
  CUSTOM_S2LP_RADIO_GPIO_3_PIN,
#endif
  CUSTOM_S2LP_RADIO_GPIO_SDN_PIN
};
/* Private function prototypes -----------------------------------------------*/
static void CUSTOM_S2LP_SPI_CS_Init(void);
static void GPIO_DeInit(void);
static void CUSTOM_S2LP_RADIO_GPIO_Init( CUSTOM_S2LP_RADIO_GPIO_TypeDef xGpio, CUSTOM_S2LP_RADIO_GPIO_Mode xGpioMode);
static int32_t CUSTOM_S2LP_SPI_SendRecvWrapper(uint8_t *pHeader, uint8_t *pBuff, uint16_t Length);
volatile int irq_disable_cnt = 0;

/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  Legacy API to change settings at runtime
  * @param xGpio can be 4 different GPIO used in Radio of S2LP
  * @param  xGpioMode can be different Mode
  * @param  xGpioEdge the edge for interrupt
  * @retval None
  */
void CUSTOM_S2LP_RADIO_GPIO_Init_Update( CUSTOM_S2LP_RADIO_GPIO_TypeDef xGpio, CUSTOM_S2LP_RADIO_GPIO_Mode xGpioMode , CUSTOM_S2LP_RADIO_GPIO_EDGE_Mode xGpioEdge)
{
  GPIO_InitTypeDef GPIO_InitStructure, EXTI_InitStructure;

  /* Enable Radio GPIO clock */
  switch(xGpio)
  {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
  case CUSTOM_S2LP_RADIO_GPIO_0:
    CUSTOM_S2LP_RADIO_GPIO_0_GPIO_CLK_ENABLE();
    break;
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
  case CUSTOM_S2LP_RADIO_GPIO_1:
    CUSTOM_S2LP_RADIO_GPIO_1_GPIO_CLK_ENABLE();
    break;
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
  case CUSTOM_S2LP_RADIO_GPIO_2:
    CUSTOM_S2LP_RADIO_GPIO_2_GPIO_CLK_ENABLE();
    break;
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
  case CUSTOM_S2LP_RADIO_GPIO_3:
    CUSTOM_S2LP_RADIO_GPIO_3_GPIO_CLK_ENABLE();
    break;
#endif
  case CUSTOM_S2LP_RADIO_GPIO_SDN:
    CUSTOM_S2LP_RADIO_GPIO_SDN_CLOCK_ENABLE();
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
  GPIO_InitStructure.Pin = CUSTOM_S2LP_RADIO_GPIO_PIN[xGpio];
  HAL_GPIO_Init(CUSTOM_S2LP_RADIO_GPIO_PORT[xGpio], &GPIO_InitStructure);

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

  EXTI_InitStructure.Pin = CUSTOM_S2LP_RADIO_GPIO_PIN[xGpio];
  EXTI_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

  if (xGpioMode == RADIO_MODE_EXTI_IN) {
    HAL_GPIO_Init(CUSTOM_S2LP_RADIO_GPIO_PORT[xGpio], &EXTI_InitStructure);
  }
}

/**
  * @brief  FunctionDescription
  *@param xGpio can be 4 different GPIO used in Radio of S2LP
  * @param  xGpioMode can be different Mode
  * @retval None
  */
static void CUSTOM_S2LP_RADIO_GPIO_Init( CUSTOM_S2LP_RADIO_GPIO_TypeDef xGpio, CUSTOM_S2LP_RADIO_GPIO_Mode xGpioMode)
{
  GPIO_InitTypeDef GPIO_InitStructure, EXTI_InitStructure;

  /* Enable Radio GPIO clock */
  switch(xGpio)
  {
#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
  case CUSTOM_S2LP_RADIO_GPIO_0:
    CUSTOM_S2LP_RADIO_GPIO_0_GPIO_CLK_ENABLE();
    break;
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
  case CUSTOM_S2LP_RADIO_GPIO_1:
    CUSTOM_S2LP_RADIO_GPIO_1_GPIO_CLK_ENABLE();
    break;
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
  case CUSTOM_S2LP_RADIO_GPIO_2:
    CUSTOM_S2LP_RADIO_GPIO_2_GPIO_CLK_ENABLE();
    break;
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
  case CUSTOM_S2LP_RADIO_GPIO_3:
    CUSTOM_S2LP_RADIO_GPIO_3_GPIO_CLK_ENABLE();
    break;
#endif
  case CUSTOM_S2LP_RADIO_GPIO_SDN:
    CUSTOM_S2LP_RADIO_GPIO_SDN_CLOCK_ENABLE();
    break;
  }

  /* GPIO Init */
  if (xGpioMode == RADIO_MODE_EXTI_IN) {
	/* Configures MCU GPIO EXTI line */
	EXTI_InitStructure.Pin = CUSTOM_S2LP_RADIO_GPIO_PIN[xGpio];
	EXTI_InitStructure.Mode = CUSTOM_S2LP_RADIO_GPIO_EDGE_MODE[xGpio];
    EXTI_InitStructure.Pull = CUSTOM_S2LP_RADIO_GPIO_PULL_MODE[xGpio];
    HAL_GPIO_Init(CUSTOM_S2LP_RADIO_GPIO_PORT[xGpio], &EXTI_InitStructure);
  }  else {
	  /* Configures MCU GPIO */
	  if(xGpioMode == RADIO_MODE_GPIO_OUT){
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	    GPIO_InitStructure.Speed = CUSTOM_S2LP_RADIO_GPIO_SPEED[xGpio];
	  } else {
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	  }
	  GPIO_InitStructure.Pull = CUSTOM_S2LP_RADIO_GPIO_PULL_MODE[xGpio];
	  GPIO_InitStructure.Pin = CUSTOM_S2LP_RADIO_GPIO_PIN[xGpio];
	  HAL_GPIO_Init(CUSTOM_S2LP_RADIO_GPIO_PORT[xGpio], &GPIO_InitStructure);
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

  for (uint32_t i = 0; i < CUSTOM_S2LP_RADIO_GPIOn ; i++)
  {
    initStruct.Pin = CUSTOM_S2LP_RADIO_GPIO_PIN[i];
    HAL_GPIO_DeInit( CUSTOM_S2LP_RADIO_GPIO_PORT[i], initStruct.Pin );
  }
}
}

/**
  * @brief  Initializes SPI MSP.
  * @param  hspi  SPI handler
  * @retval None
  */
static void CUSTOM_S2LP_SPI_CS_Init()
{
  GPIO_InitTypeDef GPIO_InitStruct;
  CUSTOM_S2LP_RADIO_SPI_NSS_CLK_ENABLE();

  GPIO_InitStruct.Pull = CUSTOM_S2LP_RADIO_SPI_NSS_PULL_MODE;
  GPIO_InitStruct.Speed = CUSTOM_S2LP_RADIO_SPI_NSS_SPEED;
  GPIO_InitStruct.Pin = CUSTOM_S2LP_RADIO_SPI_NSS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(CUSTOM_S2LP_RADIO_SPI_NSS_PORT, &GPIO_InitStruct);

  CUSTOM_S2LP_RADIO_SPI_NSS_PIN_HIGH();
}

/**
  * @brief  Wrapper Function for CUSTOM_S2LP SPI.
  * @param  pHeader is the header to send/receive
  * @param  pBuff is the buffer to write/read
  * @param  Length of data to be read
  * @retval TBD
  */
int32_t CUSTOM_S2LP_SPI_SendRecvWrapper(uint8_t *pHeader, uint8_t *pBuff, uint16_t Length)
{
  int32_t status;
  CUSTOM_S2LP_ENTER_CRITICAL();
  CUSTOM_S2LP_RADIO_SPI_NSS_PIN_LOW();
  status = CUSTOM_S2LP_SPI_SendRecv( pHeader, pHeader, 2 );
  if (!status)
  {
    if(Length)
    {
      status = CUSTOM_S2LP_SPI_SendRecv( pBuff, pBuff, Length );
    }
  }
  CUSTOM_S2LP_RADIO_SPI_NSS_PIN_HIGH();
  CUSTOM_S2LP_EXIT_CRITICAL();
  return status;
}

/******************************* LEds and Buttons Services ********************/

/******************************* S2LP Radio Low level Services *****************/

/**
  * @brief  Radio Initialization
  * @retval None
  */
int32_t CUSTOM_S2LP_RADIO_Init( void )
{
 S2LP_IO_t  IOCtx;

   /*register SPI bus function to the component */
   IOCtx.Init = CUSTOM_S2LP_SPI_Init;
   IOCtx.DeInit = CUSTOM_S2LP_SPI_DeInit;
   IOCtx.WriteBuffer = CUSTOM_S2LP_SPI_SendRecvWrapper;
   IOCtx.Delay = CUSTOM_S2LP_Delay;

   S2LP_RegisterBusIO(&IOCtx);
   CUSTOM_S2LP_RADIO_GPIO_Init(CUSTOM_S2LP_RADIO_GPIO_SDN, RADIO_MODE_GPIO_OUT);
   CUSTOM_S2LP_SPI_CS_Init();
   for (uint32_t GPIO = 0; GPIO < (CUSTOM_S2LP_RADIO_GPIOn - 1); GPIO++)
   {
		CUSTOM_S2LP_RADIO_GPIO_Init((CUSTOM_S2LP_RADIO_GPIO_TypeDef) GPIO, CUSTOM_S2LP_RADIO_GPIO_MODE[GPIO]);
   }

  return CUSTOM_S2LP_ERROR_NONE;
}

/**
  * @brief  FunctionDescription
  * @retval Error Code
  */
int32_t CUSTOM_S2LP_RADIO_DeInit( void )
{
  GPIO_DeInit();
  return CUSTOM_S2LP_ERROR_NONE;
}
/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t CUSTOM_S2LP_RADIO_Reset(void)
{
  return CUSTOM_S2LP_ERROR_FEATURE_NOT_SUPPORTED;
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t CUSTOM_S2LP_RADIO_BoardSetXO( uint8_t state )
{
  UNUSED (state);
  return CUSTOM_S2LP_ERROR_FEATURE_NOT_SUPPORTED;
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t CUSTOM_S2LP_RADIO_GetWakeUpTime( void )
{
  return  BOARD_WAKEUP_TIME;
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t CUSTOM_S2LP_RADIO_EnterShutdown( void )
{
  HAL_GPIO_WritePin( CUSTOM_S2LP_RADIO_GPIO_SDN_PORT, CUSTOM_S2LP_RADIO_GPIO_SDN_PIN, GPIO_PIN_SET);
  return CUSTOM_S2LP_ERROR_NONE;
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t CUSTOM_S2LP_RADIO_ExitShutdown( void )     /* Equivalent to RadioEnterShutdown and RadioExitShutdown*/
{
  HAL_GPIO_WritePin( CUSTOM_S2LP_RADIO_GPIO_SDN_PORT, CUSTOM_S2LP_RADIO_GPIO_SDN_PIN, GPIO_PIN_RESET);
  /* Delay to allow the circuit POR */
  HAL_Delay(1);
  return CUSTOM_S2LP_ERROR_NONE;
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
uint8_t CUSTOM_S2LP_RADIO_CheckShutdown( void )
{
  uint8_t pinState;
  pinState = (uint8_t)HAL_GPIO_ReadPin( CUSTOM_S2LP_RADIO_GPIO_SDN_PORT, CUSTOM_S2LP_RADIO_GPIO_SDN_PIN);
  return pinState;
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t CUSTOM_S2LP_RADIO_CheckRfFrequency( uint32_t frequency )
{
  return CUSTOM_S2LP_ERROR_NONE;
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
int32_t CUSTOM_S2LP_RADIO_IoIrqEnable(GpioIrqHandler **irqHanlder)
{
  for (uint32_t GPIO = 0; GPIO < (CUSTOM_S2LP_RADIO_GPIOn - 1); GPIO++)
  {
      if(CUSTOM_S2LP_RADIO_GPIO_MODE[GPIO] == RADIO_MODE_EXTI_IN)
      {
        HAL_EXTI_GetHandle(&CUSTOM_S2LP_RADIO_GPIO_hexti[GPIO], CUSTOM_S2LP_RADIO_GPIO_EXTI_LINE[GPIO]);
        HAL_EXTI_RegisterCallback(&CUSTOM_S2LP_RADIO_GPIO_hexti[GPIO],  HAL_EXTI_COMMON_CB_ID, irqHanlder[GPIO]);
        HAL_NVIC_SetPriority(CUSTOM_S2LP_RADIO_GPIO_IRQn[GPIO], CUSTOM_S2LP_RADIO_GPIO_IT_PRIO[GPIO], CUSTOM_S2LP_RADIO_GPIO_IT_SUBPRIO[GPIO]);
        HAL_NVIC_EnableIRQ( CUSTOM_S2LP_RADIO_GPIO_IRQn[GPIO]);
      }
  }
  return CUSTOM_S2LP_ERROR_NONE;
}

/**
* @brief  Disables the interrupt on GPIO .
* ...
*/
int32_t CUSTOM_S2LP_RADIO_IoIrqDisable(GpioIrqHandler **irqHanlder)
{
  for (uint32_t GPIO = 0; GPIO < (CUSTOM_S2LP_RADIO_GPIOn - 1); GPIO++)
  {
      if(CUSTOM_S2LP_RADIO_GPIO_MODE[GPIO] == RADIO_MODE_EXTI_IN)
      {
		HAL_NVIC_DisableIRQ( CUSTOM_S2LP_RADIO_GPIO_IRQn[GPIO]);
	  }
  }
  return CUSTOM_S2LP_ERROR_NONE;
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
int32_t CUSTOM_S2LP_RADIO_EnableTCXO(void)
{
  GPIO_InitTypeDef initStruct={0};

  CUSTOM_S2LP_RADIO_TCXO_CTL_GPIO_CLK_ENABLE();

  initStruct.Pull = GPIO_NOPULL;
  initStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  initStruct.Pin = CUSTOM_S2LP_RADIO_TCXO_CTL_PIN;
  initStruct.Mode = GPIO_MODE_OUTPUT_PP;

  HAL_GPIO_Init(CUSTOM_S2LP_RADIO_TCXO_CTL_PORT, &initStruct);
  HAL_GPIO_WritePin(CUSTOM_S2LP_RADIO_TCXO_CTL_PORT, CUSTOM_S2LP_RADIO_TCXO_CTL_PIN, GPIO_PIN_SET);

  return CUSTOM_S2LP_ERROR_NONE;
}

__weak int32_t CUSTOM_S2LP_EEPROM_ReadPage(uint32_t Instance, uint16_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer)
{
	/*EEPROM is currently not supported in custom case. Implement your own code here. */
	return 0;
}

__weak void FEM_Operation(FEM_OperationType operation)
{

}

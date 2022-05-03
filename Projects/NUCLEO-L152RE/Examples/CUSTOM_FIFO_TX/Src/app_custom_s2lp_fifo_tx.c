
/**
 * @file    app_custom_s2lp_fifo_tx.c
 * @author  STMicroelectronics
 * @version 1.3.0
 * @date    July, 2019
 * @brief   Example of transmission of S2-LP Basic packets with payload longer than 128 bytes.
 * @details
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
 */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "s2lp.h"
#include "S2LP_Gpio.h"
#include "custom_s2lp.h"
#include "custom_s2lp_conf.h"
#include "s2lp_radio_settings.h"
#include "custom_s2lpgpio_setting.h"

#include "S2LP_PktBasic.h"
#include "s2lp_sdkapi_mapping.h"

#include "custom_s2lp_management.h"
/* Private function prototypes -----------------------------------------------*/

void MX_S2LP_FIFO_TX_Init(void);
void MX_S2LP_FIFO_TX_Process(void);
void fitPayload(void);
void S2LPInterfaceInit(void);
RangeExtType S2LP_ManagementGetRangeExtender(void);
void S2LP_ManagementRangeExtInit(void);
void S2LP_ManagementIdentificationRFBoard(void);
int32_t S2LP_ManagementGetOffset(void);

__WEAK void FifoTxCallback_GPIO_0(void);
__WEAK void FifoTxCallback_GPIO_1(void);
__WEAK void FifoTxCallback_GPIO_2(void);
__WEAK void FifoTxCallback_GPIO_3(void);

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/* max payload size */
#define PAYLOAD_LENGTH_FIX 	300
#define DEV_FIFO_SIZE 		128

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * @brief Tx buffer index, residual and total bytes to be transmitted
 */
uint16_t nTxIndex, nResidualPcktLength, nPayloadLength=20;

/**
 * @brief Almost empty Tx FIFO threshold
 */
uint8_t cThreholdTxFifoAE = 48;

/**
 * @brief Tx buffer declaration: data to transmit
 */

uint8_t vectcTxBuff[PAYLOAD_LENGTH_FIX];

/**
 * @brief Declare the Tx done flag
 */
volatile FlagStatus xTxDoneFlag = RESET;

/**
 * @brief IRQ status struct declaration
 */
S2LPIrqs xIrqStatus;

/**
 * @defgroup Fifo_TX_A_Private_Variables  FIFO TX A Private Variables
 * @{
 */

/* Global variables ----------------------------------------------------------*/

/**
  * @brief Radio structure fitting
  */
SRadioInit xRadioInit = {
  BASE_FREQUENCY,
  MODULATION_SELECT,
  DATARATE,
  FREQ_DEVIATION,
  BANDWIDTH
};

PktBasicInit xBasicInit={
  PREAMBLE_LENGTH,
  SYNC_LENGTH,
  SYNC_WORD,
  VARIABLE_LENGTH,
  EXTENDED_LENGTH_FIELD,
  CRC_MODE,
  EN_ADDRESS,
  EN_FEC,
  EN_WHITENING
};

/**
* @brief GPIO structure fitting
*/
SGpioInit xGpioIRQ={
  S2LP_GPIO_3,
  RADIO_GPIO_MODE_DIGITAL_OUTPUT_LP,
  RADIO_GPIO_DIG_OUT_IRQ

};

static GpioIrqHandler *GpioIrq[] = {

#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
    FifoTxCallback_GPIO_0,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
    FifoTxCallback_GPIO_1,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
    FifoTxCallback_GPIO_2,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
    FifoTxCallback_GPIO_3,
#endif
};

/* Functions Definition ------------------------------------------------------*/

void MX_X_CUBE_SUBG2_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN S2LP_Library_Init_PreTreatment */

  /* USER CODE END S2LP_Library_Init_PreTreatment */

  /* Initialize the peripherals and the S2LP components */
  MX_S2LP_FIFO_TX_Init();

  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN S2LP_Library_Init_PostTreatment */

  /* USER CODE END S2LP_Library_Init_PostTreatment */
}

/**
 * @brief  Process function for FIFO TX Application
 * @param  None
 * @retval None
 */

void MX_S2LP_FIFO_TX_Process()
{

    /* make a copy of the payload length (to be used to have the number of the residual bytes to transmit) */
    nResidualPcktLength = nPayloadLength;

    /* index TX buffer */
    nTxIndex = 0;

    /* fit the packet payload with data */
    fitPayload();

    /* payload length config */
    S2LP_PCKT_BASIC_SetPayloadLength(nPayloadLength);

    printf("A data to transmit: \n\r");
    for(uint16_t i=0 ; i<nPayloadLength ; i++)
    {
    printf("%d ", vectcTxBuff[i]);
    }
    printf("\n\n\r");

    /* flush the Tx FIFO */
    S2LP_CMD_StrobeFlushTxFifo();

    /* check if the payload length is higher than the FIFO length */
    if(nPayloadLength>DEV_FIFO_SIZE)
    {
	/* ... if yes transmit data using an AE IRQ and a FIFO reloading mechanism */
	/* set the almost empty threshold */
	S2LP_FIFO_SetAlmostEmptyThresholdTx(cThreholdTxFifoAE);
	S2LP_GPIO_IrqConfig(TX_FIFO_ALMOST_EMPTY , S_ENABLE);

	/* write the linear fifo with the first 128 bytes of payload */
	S2LP_WriteFIFO(DEV_FIFO_SIZE, vectcTxBuff);

	/* store the number of transmitted bytes */
	nTxIndex = DEV_FIFO_SIZE;

	/* update the residual number of bytes to be transmitted */
	nResidualPcktLength -= DEV_FIFO_SIZE;
    }
    else
    {
	/* .. if not transmit data writing all of them to the Tx FIFO */
	S2LP_WriteFIFO(nPayloadLength, vectcTxBuff);
    }

    /* send the TX command */
    S2LP_CMD_StrobeTx();

    /* wait for TX done */
    while(!xTxDoneFlag);
    xTxDoneFlag = RESET;

    printf("\n\rAE Thr=%d\n\rPayload length=%d\n\n\r ", cThreholdTxFifoAE,nPayloadLength);

    /* pause between two transmissions */
    HAL_Delay(500);

    /* increment the payload length (overflowing it at PAYLOAD_LENGTH_FIX) */
    nPayloadLength = (nPayloadLength+1)%PAYLOAD_LENGTH_FIX;
}

/**
 * @brief  Init function for FIFO TX Application
 * @param  None
 * @retval None
 */

void MX_S2LP_FIFO_TX_Init()
{
	BSP_COM_Init(COM1);

    S2LPInterfaceInit();

    /* S2LP IRQ config */
    S2LP_GPIO_Init(&xGpioIRQ);

    /* S2LP Radio config */
    S2LP_RADIO_Init(&xRadioInit);

    /* S2LP Radio set power */
    S2LP_RADIO_SetMaxPALevel(S_DISABLE);

    if(!S2LP_ManagementGetRangeExtender())
    {
        /* If we haven't an external PA, use the library function */
        S2LP_RADIO_SetPALeveldBm(7,POWER_DBM);
    }
    else
    {
        /* in case we are using a board with external PA, the S2LPRadioSetPALeveldBm will be not functioning because
        the output power is affected by the amplification of this external component.
        Set the raw register. */
        uint8_t paLevelValue = 0x25; /* For example, this value will give about 19dBm on a STEVAL FKI-915V1 */
        S2LP_WriteRegister(PA_POWER8_ADDR, 1, &paLevelValue);
    }
    S2LP_RADIO_SetPALevelMaxIndex(7);

    /* S2LP Packet config */
    S2LP_PCKT_BASIC_Init(&xBasicInit);

    /* S2LP IRQs enable */
    S2LP_GPIO_IrqDeInit(NULL);
    S2LP_GPIO_IrqConfig(TX_DATA_SENT, S_ENABLE);

    /* IRQ registers blanking */
    S2LP_GPIO_IrqClearStatus();

    #if 0
    S2LP_Middleware_GpioInterruptCmd(S2LP_GPIO_3,IRQ_PREEMPTION_PRIORITY,0,ENABLE);
    #else
    //CUSTOM_S2LP_RADIO_IoIrqDisable(GpioIrq);
    #endif

    /* flush the Tx FIFO */
    S2LP_CMD_StrobeFlushTxFifo();

}

/*
 * LM background task
 */
void MX_X_CUBE_SUBG2_Process(void)
{
        MX_S2LP_FIFO_TX_Process();
    /* USER CODE BEGIN S2LP_Library_Process */

    /* USER CODE END S2LP_Library_Process */
}

/**
 * @brief  Fits the Tx buffer with the first nPayloadLength integer numbers starting from 0
 * @param  None
 * @retval None
 */
void fitPayload(void)
{
  for(uint16_t i=0; i<nPayloadLength ; i++)
    vectcTxBuff[i]=i;
}

/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
void S2LPInterfaceInit(void)
{
  /* Initialize the SDN pin micro side */
  CUSTOM_S2LP_RADIO_Init();

  if( S2LP_Init() != S2LP_OK)
   {
    /* Initialization Error */
     while(1){};
   }

  /* EepromSpiInitialization(); */
  /* for this application we don't use the EEPROM */
  /* CUSTOM_S2LP_EEPROM_Init(EEPROM_INSTANCE);  */

  /* S2LP ON */
  CUSTOM_S2LP_RADIO_EnterShutdown();
  CUSTOM_S2LP_RADIO_ExitShutdown();

  S2LP_ManagementIdentificationRFBoard();

  /* if the board has eeprom, we can compensate the offset calling S2LP_ManagementGetOffset
  (if eeprom is not present this fcn will return 0) */
  xRadioInit.lFrequencyBase = (uint32_t) BASE_FREQUENCY + S2LP_ManagementGetOffset();

  /* if needed this will set the range extender pins */
  S2LP_ManagementRangeExtInit();

  /* uC IRQ enable */
  CUSTOM_S2LP_RADIO_IoIrqEnable(GpioIrq);
}

/**
 * @brief  This function handles External interrupt request (associated with GPIO_3).
 * @param  None
 * @retval None
 */
void FifoTxCallback_GPIO_3(void)
{
    /* Get the IRQ status */
	S2LP_GPIO_IrqGetStatus(&xIrqStatus);

    /* Check the S2LP TX_DATA_SENT IRQ flag */
    if(xIrqStatus.IRQ_TX_DATA_SENT)
    {
	/* set the xTxDoneFlag to manage the event in the main() */
	xTxDoneFlag = SET;
    }
    else
    {
	/* Check the S2LP TX_FIFO_ALMOST_EMPTY IRQ flag */
	if(xIrqStatus.IRQ_TX_FIFO_ALMOST_EMPTY)
	{
	  /* read the number of elements in the Tx FIFO */
	  uint8_t cNElemTxFifo = S2LP_FIFO_ReadNumberBytesTxFifo();

	  /* check if the sum of the residual payload to be transmitted and the actual bytes in FIFO are higher than 128 */
	  if(nResidualPcktLength+cNElemTxFifo>DEV_FIFO_SIZE)
	  {
	    /* .. if yes another almost full IRQ has to be managed */

	    /* ..so fill the Tx FIFO */
	    S2LP_WriteFIFO(DEV_FIFO_SIZE-cNElemTxFifo-1, &vectcTxBuff[nTxIndex]);

	    /* update the number of bytes to be transmitted */
	    nResidualPcktLength -= DEV_FIFO_SIZE-cNElemTxFifo-1;

	    /* update the number of bytes transmitted until now */
	    nTxIndex += DEV_FIFO_SIZE-cNElemTxFifo-1;
	  }
	  else
	  {
	    /* .. if not all the nResidualPcktLength bytes remaining can be written to the Tx FIFO */
	    /* ..so disable the TX_FIFO_ALMOST_EMPTY IRQ */
	    S2LP_GPIO_IrqConfig(TX_FIFO_ALMOST_EMPTY , S_DISABLE);

	    /* unarm the AE threshold mechanism */
		S2LP_FIFO_SetAlmostEmptyThresholdTx(DEV_FIFO_SIZE);

	    /* fill the Tx fifo */
	    S2LP_WriteFIFO(nResidualPcktLength, &vectcTxBuff[nTxIndex]);

	    /* update the number of transmitted bytes */
	    nTxIndex += nResidualPcktLength;

	    /* update the number of bytes to be transmitted */
	    nResidualPcktLength = 0;
	  }

	  /* re-read the number of elements in the Tx FIFO */
	  cNElemTxFifo = S2LP_FIFO_ReadNumberBytesTxFifo();
	}
    }
}

__weak void FifoTxCallback_GPIO_0()
{
    /* Empty Function */
}

__weak void FifoTxCallback_GPIO_1()
{
    /* Empty Function */
}

__weak void FifoTxCallback_GPIO_2()
{
    /* Empty Function */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

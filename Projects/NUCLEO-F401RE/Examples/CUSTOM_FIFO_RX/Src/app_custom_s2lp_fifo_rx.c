
/**
 * @file    app_custom_s2lp_fifo_rx.c
 * @author  STMicroelectronics
 * @version 1.3.0
 * @date    July, 2019
 * @brief   Example of reception of S2-LP Basic packets with payload longer than 128 bytes.
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
#include "custom_s2lp.h"
#include "custom_s2lp_conf.h"
#include "s2lp_radio_settings.h"
#include "custom_s2lpgpio_setting.h"

#include "S2LP_PktBasic.h"
#include "s2lp_sdkapi_mapping.h"

#include "custom_s2lp_management.h"

#define USE_VCOM

/**
* @addtogroup SDK_Examples
* @{
*/

/**
* @addtogroup SDK_Fifo_Handler        SDK FIFO Handler
* @{
*/

void printDbgData(void);
void S2LPInterfaceInit(void);
void MX_S2LP_FIFO_RX_Init(void);
void MX_S2LP_FIFO_RX_Process(void);
void S2LP_ManagementRangeExtInit(void);
int32_t S2LP_ManagementGetOffset(void);
void S2LP_ManagementIdentificationRFBoard(void);
void S2LP_ManagementSetRangeExtender(RangeExtType value);
RangeExtType S2LP_ManagementGetRangeExtender(void);

__WEAK void FifoRx_S2LP_Callback_GPIO_0(void);
__WEAK void FifoRx_S2LP_Callback_GPIO_1(void);
__WEAK void FifoRx_S2LP_Callback_GPIO_2(void);
__WEAK void FifoRx_S2LP_Callback_GPIO_3(void);

/**
* @addtogroup SDK_Fifo_Handler_B              SDK FIFO Handler B
* @brief Device B configured as a receiver.
* @details This code explains how to configure a receiver for basic packets with payload longer than 128 bytes.
*
* The user can change the Basic packet configuration parameters editing the defines
* at the beginning of the file.
* @{
*/

/**
* @defgroup Fifo_Handler_B_Private_Variables FIFO Handler B Private Variables
* @{
*/

/* max payload size */
#define PAYLOAD_LENGTH_FIX	300

#undef EXTENDED_LENGTH_FIELD
#define EXTENDED_LENGTH_FIELD S_ENABLE

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

/**
* @brief Packet Basic structure fitting
*/
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
* @brief GPIO IRQ structure fitting
*/
SGpioInit xGpioIRQ={
  S2LP_GPIO_3,
  S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP,
  S2LP_GPIO_DIG_OUT_IRQ
};

/**
* @brief GPIO RX FIFO AF structure fitting
*/
SGpioInit xGpioAF={
  S2LP_GPIO_1,
  S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP,
  S2LP_GPIO_DIG_OUT_TXRX_FIFO_ALMOST_FULL
};

/**
* @brief Declare the Rx done flag
*/
volatile FlagStatus xRxDoneFlag = RESET;

/**
* @brief IRQ status struct declaration
*/
S2LPIrqs xIrqStatus;

/**
* @brief Rx buffer declaration: how to store the received data
*/
uint8_t vectcRxBuff[PAYLOAD_LENGTH_FIX];

/**
* @brief Rx buffer index, residual and total bytes to be received
*/
uint16_t nRxIndex, nPayloadLength = 20;

/**
* @brief Almost full Tx FIFO threshold  (measured from the tail)
*/
uint8_t cThreholdRxFifoAF = 78;

/**
* @brief Preemption priority IRQ
*/
#define IRQ_PREEMPTION_PRIORITY         0x03

/**
*@}
*/

static GpioIrqHandler *GpioIrq[] = {

#if (USE_CUSTOM_S2LP_RADIO_GPIO_0 == 1)
		FifoRx_S2LP_Callback_GPIO_0,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_1 == 1)
		FifoRx_S2LP_Callback_GPIO_1,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_2 == 1)
		FifoRx_S2LP_Callback_GPIO_2,
#endif
#if (USE_CUSTOM_S2LP_RADIO_GPIO_3 == 1)
		FifoRx_S2LP_Callback_GPIO_3,
#endif
};

void MX_X_CUBE_SUBG2_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN S2LP_Library_Init_PreTreatment */

  /* USER CODE END S2LP_Library_Init_PreTreatment */

  /* Initialize the peripherals and the S2LP components */
  MX_S2LP_FIFO_RX_Init();

  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN S2LP_Library_Init_PostTreatment */

  /* USER CODE END S2LP_Library_Init_PostTreatment */
}

/*
 * LM background task
 */
void MX_X_CUBE_SUBG2_Process(void)
{
    MX_S2LP_FIFO_RX_Process();
    /* USER CODE BEGIN S2LP_Library_Process */

    /* USER CODE END S2LP_Library_Process */
}

/**
* @defgroup Fifo_Handler_B_Private_Functions                                  FIFO Handler B Private Functions
* @{
*/

/**
* @brief  This function handles External interrupt request. In this application it is used
*         to manage the S2LP IRQ configured to be notified on the S2LP GPIO_3.
* @param  None
* @retval None
*/
 void FifoRx_S2LP_Callback_GPIO_1(void)
{
	uint8_t cRxDataLen;

	cRxDataLen = S2LP_FIFO_ReadNumberBytesRxFifo();

	if(cRxDataLen>2)
	{
		/* Read the RX FIFO */
		S2LP_ReadFIFO(cRxDataLen-2, &vectcRxBuff[nRxIndex]);
		nRxIndex+=cRxDataLen-2;
	}
}

void FifoRx_S2LP_Callback_GPIO_3(void)
{
	uint8_t cRxDataLen;

	/* Get the IRQ status */
	S2LP_GPIO_IrqGetStatus(&xIrqStatus);

	/* Check the RX_DATA_READY IRQ flag */
	if(xIrqStatus.IRQ_RX_DATA_READY)
	{
		cRxDataLen = S2LP_FIFO_ReadNumberBytesRxFifo();

		if(cRxDataLen!=0)
		{
			/* Read the RX FIFO */
			S2LP_ReadFIFO(cRxDataLen, &vectcRxBuff[nRxIndex]);
		}

		S2LP_CMD_StrobeFlushRxFifo();

		/* update the number of received bytes */
		nRxIndex += cRxDataLen;

		/* set the Rx done flag */
		xRxDoneFlag = SET;
	}

	/* Check the SPIRIT RX_DATA_DISC IRQ flag */
	if(xIrqStatus.IRQ_RX_DATA_DISC)
	{
		/* RX command - to ensure the device will be ready for the next reception */
		S2LP_CMD_StrobeRx();
	}
}

/**
* @brief  Debug printing function.
* @param  None
* @retval None
*/
void printDbgData(void)
{
  SBool correct=S_TRUE;

  /* print the received data */
  printf("B data received: \n\r");

  for(uint16_t i=0 ; i<nRxIndex ; i++)
  {
    printf("%d ", vectcRxBuff[i]);
    if(vectcRxBuff[i] != (uint8_t)i)
      correct=S_FALSE;
  }
  printf("\n\n\r");

  if(correct == S_FALSE){
    printf("Wrong data received.\n");
  }

  printf("\n\rAF Thr=%d\n\rPayload length=%d\n\n\r", cThreholdRxFifoAF,nRxIndex);
}

/**
* @brief  System main function.
* @param  None
* @retval None
*/
void MX_S2LP_FIFO_RX_Init(void)
{
    BSP_COM_Init(COM1);

	S2LPInterfaceInit();

  /* S2LP IRQ config */
  S2LP_GPIO_Init(&xGpioIRQ);
  /* Spirit AF config */
  S2LP_GPIO_Init(&xGpioAF);
  /* S2LP Radio config */
  S2LP_RADIO_Init(&xRadioInit);

  /* S2LP Packet config */
  S2LP_PCKT_BASIC_Init(&xBasicInit);
  S2LP_PCKT_BASIC_FilterOnCrc(S_DISABLE);

  /* S2LP IRQs enable */
  S2LP_GPIO_IrqDeInit(&xIrqStatus);
  S2LP_GPIO_IrqConfig(RX_DATA_DISC,S_ENABLE);
  S2LP_GPIO_IrqConfig(RX_DATA_READY,S_ENABLE);

  /* RX timeout config */
  SET_INFINITE_RX_TIMEOUT();

  /* IRQ registers blanking */
  S2LP_GPIO_IrqClearStatus();

  S2LP_FIFO_SetAlmostFullThresholdRx(128-cThreholdRxFifoAF);
}

void MX_S2LP_FIFO_RX_Process()
{

    /* reset the Rx buffer index (number of bytes received until now) */
    nRxIndex = 0;

    /* payload length config */
    S2LP_PCKT_BASIC_SetPayloadLength(nPayloadLength);
    S2LP_FIFO_MuxRxFifoIrqEnable(S_ENABLE);

    /* RX command */
    S2LP_CMD_StrobeRx();

    /* wait for RX done */
    while(!xRxDoneFlag);

    xRxDoneFlag = RESET;

#ifdef USE_VCOM
    printDbgData();
#endif

    /* increment the payload length (overflowing it at PAYLOAD_LENGTH_FIX) */
    nPayloadLength = (nPayloadLength+1)%PAYLOAD_LENGTH_FIX;
}

#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval : None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number */
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);

  /* Infinite loop */
  while (1){/* - */}
}
#endif

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
*@}
*/

/**
*@}
*/

/**
*@}
*/

/**
*@}
*/

/**
* End of C-code
*/

/**
* @brief  Weak function for IRQ Line
*/

__weak void FifoRx_S2LP_Callback_GPIO_0()
{
    /* Empty Function */
}

__weak void FifoRx_S2LP_Callback_GPIO_2()
{
    /* Empty Function */
}

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

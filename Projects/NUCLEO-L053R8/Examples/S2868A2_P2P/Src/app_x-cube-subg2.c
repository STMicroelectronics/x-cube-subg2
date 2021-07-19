
/**
  ******************************************************************************
  * @file    p2p_demo.c
  * @author  SRA Application Team
  * @brief   P2P demo applicative code.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under ODE Software License Agreement
  * SLA0094, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0094
  *
  ******************************************************************************
  */

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

#include "app_x-cube-subg2.h"
#include "stm32l0xx_nucleo.h"
#include "s2868a2.h"
#include "s2lp.h"
#include "p2p_demo_settings.h"

/** @defgroup S2LP_Nucleo
  * @{
  */

/** @defgroup Main
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TX_BUFFER_SIZE   20
#define RX_BUFFER_SIZE   96
/*Source address of one node should be destination for other node & vice-versa*/

#define MY_ADDRESS                  0x44  //0x44
#define DESTINATION_ADDRESS         0x44  //0x34

#if defined (USE_LOW_POWER_MODE)
//#define MCU_SLEEP_MODE
#define MCU_STOP_MODE
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t TxLength = TX_BUFFER_SIZE;
uint8_t RxLength = 0;
uint8_t aTransmitBuffer[TX_BUFFER_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,\
  16,17,18,19,20};
uint8_t aReceiveBuffer[RX_BUFFER_SIZE] = {0x00};
static S2LPCutType s_S2LPCut = S2LP_CUT_2_1;
static RangeExtType xRangeExtType = RANGE_EXT_NONE;
static int32_t s_RfModuleOffset=0;
static uint8_t s_RfModuleBand = 0, s_Tcxo=0;

extern SRadioInit xRadioInit;

/* Application defined push button, to detect long push */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static volatile FlagStatus s_xTIMChCompareModeRaised = RESET;
/**
* @brief  WMBus Link Layer State Enum.
*/
typedef enum {
   SM_STATE_START_RX=0,
   SM_STATE_WAIT_FOR_RX_DONE,
   SM_STATE_DATA_RECEIVED,
   SM_STATE_SEND_DATA,
   SM_STATE_WAIT_FOR_TX_DONE,
   SM_STATE_ACK_RECEIVED,
   SM_STATE_SEND_ACK,
   SM_STATE_TOGGLE_LED,
   SM_STATE_IDLE=0xFF
} SM_State_t;

typedef struct sRadioDriver
{
    void ( *Init )( void );
    void ( *GpioIrq )( SGpioInit *pGpioIRQ );
    void ( *RadioInit )( SRadioInit *pRadioInit );
    void ( *SetRadioPower )( uint8_t cIndex, float fPowerdBm );
    void ( *PacketConfig )( void );
    void ( *SetPayloadLen )( uint8_t length);
    void ( *SetDestinationAddress )( uint8_t address);
    void ( *EnableTxIrq )( void );
    void ( *EnableRxIrq )( void );
    void ( *DisableIrq )(void);
    void ( *SetRxTimeout )( float cRxTimeout );
    void ( *EnableSQI )(void);
    void ( *SetRssiThreshold)(int dbmValue);
    void ( *ClearIrqStatus )(void);
    void ( *StartRx )( void );
    void ( *StartTx )( uint8_t *buffer, uint8_t size );
    void ( *GetRxPacket )( uint8_t *buffer, uint8_t *size );
}RadioDriver_t;

typedef struct sMCULowPowerMode
{
    void ( *McuStopMode )( void );
    void ( *McuStandbyMode )( void );
    void ( *McuSleepMode )( void );
}MCULowPowerMode_t;

typedef struct sRadioLowPowerMode
{
    void ( *RadioShutDown )( void );
    void ( *RadioStandBy )( void );
    void ( *RadioSleep ) ( void );
    void ( *RadioPowerON )( void );
}RadioLowPowerMode_t;

typedef struct
{
  uint8_t Cmdtag;
  uint8_t CmdType;
  uint8_t CmdLen;
  uint8_t Cmd;
  uint8_t DataLen;
  uint8_t* DataBuff;
}AppliFrame_t;

#ifdef CSMA_ENABLE
typedef struct
{
  int32_t RxRSSIThreshold;
  uint8_t CsmaEnabled;
  int32_t CsmaRSSIThreshold;/*!<RSSI Threshold for CSMA*/
  uint8_t CsmaOverrideFail;
  uint8_t CsmaCcaPeriod; /*!< RSSI meas cycle = (CsmaCcaPeriod+1)*64*TBIT*/
  uint8_t CsmaCcaLength; /*!<times of CsmaCcaPeriod for ch assesment */
  uint8_t CsmaMaxNb; /*!<Max N backoff */
  uint16_t CsmaBuSeed; /*!<seed for rand in (2^rand)* presc */
  uint8_t CsmaBuPrescaler; /*!<presc for (2^rand)*presc */
}CSMA_Attr_typedef;
#endif

/* Exported functions ------------------------------------------------------- */

void Enter_LP_mode(void);
void Exit_LP_mode(void);
void MCU_Enter_StopMode(void);
void MCU_Enter_StandbyMode(void);
void MCU_Enter_SleepMode(void);
void RadioPowerON(void);
void RadioPowerOFF(void);
void RadioStandBy(void);
void RadioSleep(void);
void AppliSendBuff(AppliFrame_t *xTxFrame, uint8_t cTxlen);
void AppliReceiveBuff(uint8_t *RxFrameBuff, uint8_t cRxlen);
void STackProtocolInit(void);
void BasicProtocolInit(void);
static void P2PInterruptHandler(void);
void Set_KeyStatus(FlagStatus val);
void S2LPInterfaceInit(void);
void S2LP_ManagementSetBand(uint8_t value);
uint8_t S2LP_ManagementGetBand(void);
void S2LP_ManagementSetOffset(int32_t value);
int32_t S2LP_ManagementGetOffset(void);
uint8_t S2LP_ManagementGetTcxo(void);
S2LPCutType S2LP_ManagementGetCut(void);
void S2LP_ManagementRangeExtInit(void);
void S2LP_ManagementSetRangeExtender(RangeExtType xRangeType);
RangeExtType S2LP_ManagementGetRangeExtender(void);
void S2LPManagementIdentificationRFBoard(void);
uint32_t S2LPManagementComputeXtalFrequency(void);
uint8_t EepromIdentification(void);
void S2LP_ManagementSetTcxo(uint8_t tcxo);
void S2LP_PacketConfig(void);
void S2LP_SetPayloadlength(uint8_t length);
void S2LP_SetDestinationAddress(uint8_t address);
void S2LP_EnableTxIrq(void);
void S2LP_EnableRxIrq(void);
void S2LP_SetRxTimeout(float cRxTimeOut);
void S2LP_EnableSQI(void);
float S2LP_GetRssiTH(void);
void S2LP_StartRx(void);
void S2LP_GetRxPacket(uint8_t *buffer, uint8_t *size );
void S2LP_StartTx(uint8_t *buffer, uint8_t size);
void HAL_SYSTICK_Callback(void);
void MX_SUBG2_P2P_Init(void);
void MX_SUBG2_P2P_Process(uint8_t *pTxBuff, uint8_t cTxlen, uint8_t* pRxBuff, uint8_t cRxlen);

static GpioIrqHandler *GpioIrq[] = {

#if (USE_S2868A2_RADIO_GPIO_0 == 1)
  P2PInterruptHandler,
#endif
#if (USE_S2868A2_RADIO_GPIO_1 == 1)
  P2PInterruptHandler,
#endif
#if (USE_S2868A2_RADIO_GPIO_2 == 1)
  P2PInterruptHandler,
#endif
#if (USE_S2868A2_RADIO_GPIO_3 == 1)
  P2PInterruptHandler,
#endif
};

/**
* @brief RadioDriver_t structure fitting
*/

RadioDriver_t radio_cb =
{
  .PacketConfig = RadioPacketConfig,
  .SetPayloadLen = RadioSetPayloadlength,
  .SetDestinationAddress = RadioSetDestinationAddress,
  .EnableTxIrq = RadioEnableTxIrq,
  .EnableRxIrq = RadioEnableRxIrq,
  .SetRxTimeout = RadioSetRxTimeout,
  .EnableSQI = RadioEnableSQI,
  .StartRx = RadioStartRx,
  .StartTx = RadioStartTx,
  .GetRxPacket = RadioGetRxPacket
};

/**
* @brief MCULowPowerMode_t structure fitting
*/
MCULowPowerMode_t MCU_LPM_cb =
{
  .McuStopMode = MCU_Enter_StopMode,
  .McuStandbyMode = MCU_Enter_StandbyMode,
  .McuSleepMode = MCU_Enter_SleepMode
};

/**
* @brief RadioLowPowerMode_t structure fitting
*/
RadioLowPowerMode_t Radio_LPM_cb =
{
  .RadioShutDown = RadioPowerOFF,
  .RadioStandBy = RadioStandBy,
  .RadioSleep = RadioSleep,
  .RadioPowerON = RadioPowerON
};

/**
* @brief GPIO structure fitting
*/
SGpioInit xGpioIRQ={
  S2LP_GPIO_3,
  RADIO_GPIO_MODE_DIGITAL_OUTPUT_LP,
  RADIO_GPIO_DIG_OUT_IRQ

};

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

#if defined(USE_STack_PROTOCOL)
/**
* @brief Packet Basic structure fitting
*/
PktStackInit xStackInit={
  PREAMBLE_LENGTH,
  SYNC_LENGTH,
  SYNC_WORD,
  LENGTH_WIDTH,
  CRC_MODE,
  CONTROL_LENGTH,
  EN_FEC,
  EN_WHITENING
};

/* LLP structure fitting */
PktStackLlpInit xStackLLPInit ={
  EN_AUTOACK,
  EN_PIGGYBACKING,
  MAX_RETRANSMISSIONS
};

/**
* @brief Address structure fitting
*/
PktStackAddressesInit xAddressInit={
  EN_FILT_MY_ADDRESS,
  MY_ADDRESS,
  EN_FILT_MULTICAST_ADDRESS,
  MULTICAST_ADDRESS,
  EN_FILT_BROADCAST_ADDRESS,
  BROADCAST_ADDRESS
};

#elif defined(USE_BASIC_PROTOCOL)

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
* @brief Address structure fitting
*/
PktBasicAddressesInit xAddressInit={
  EN_FILT_MY_ADDRESS,
  MY_ADDRESS,
  EN_FILT_MULTICAST_ADDRESS,
  MULTICAST_ADDRESS,
  EN_FILT_BROADCAST_ADDRESS,
  BROADCAST_ADDRESS
};
#endif

#ifdef CSMA_ENABLE
/**
* @brief CSMA structure fitting
*/
RadioCsmaInit xCsmaInit={

  PERSISTENT_MODE_EN,
  CS_PERIOD,
  CS_TIMEOUT,
  MAX_NB,
  BU_COUNTER_SEED,
  CU_PRESCALER
};
#endif

/* Private define ------------------------------------------------------------*/
#define TIME_UP                                         0x01

/* Private variables ---------------------------------------------------------*/
RadioDriver_t *pRadioDriver;
MCULowPowerMode_t *pMCU_LPM_Comm;
RadioLowPowerMode_t  *pRadio_LPM_Comm;
/*Flags declarations*/
volatile int MasterFlag = 0;
volatile FlagStatus xRxDoneFlag = RESET, xTxDoneFlag=RESET, cmdFlag=RESET;
volatile FlagStatus xStartRx=RESET, rx_timeout=RESET, exitTime=RESET;
volatile FlagStatus PushButtonStatusWakeup=RESET;
volatile FlagStatus PushButtonStatusData=RESET;
/*IRQ status struct declaration*/
RadioIrqs xIrqStatus;

__IO uint32_t KEYStatusData = 0x00;
static AppliFrame_t xTxFrame, xRxFrame;
uint8_t TxFrameBuff[MAX_BUFFER_LEN] = {0x00};
uint16_t exitCounter = 0;
uint16_t txCounter = 0;
uint16_t wakeupCounter = 0;
uint16_t dataSendCounter = 0x00;

SM_State_t SM_State = SM_STATE_START_RX;/* The actual state running */

void MX_X_CUBE_SUBG2_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN S2LP_Library_Init_PreTreatment */

  /* USER CODE END S2LP_Library_Init_PreTreatment */

  /* Initialize the peripherals and the SUBG2 components */
  MX_SUBG2_P2P_Init();

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
  /* USER CODE BEGIN S2LP_Library_Process */
  MX_SUBG2_P2P_Process(aTransmitBuffer, TxLength, aReceiveBuffer, RxLength);
  /* USER CODE END S2LP_Library_Process */
}

  /**
  * @brief  Initialize the EnergyHarvesting Example
  * @retval None
  */

void MX_SUBG2_P2P_Init(void)
{

  /* Initialize USER LED */
  BSP_LED_Init(LED2);

  /* Configure USER Key Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  S2LPInterfaceInit();

   pRadioDriver = &radio_cb;

  /* S2LP IRQ config */
  S2LP_GPIO_Init(&xGpioIRQ);

  /* S2LP Radio config */
  S2LP_RADIO_Init(&xRadioInit);

  /* S2LP Radio set power */
  S2LP_RADIO_SetMaxPALevel(S_DISABLE);

  if(!S2LP_ManagementGetRangeExtender())
  {
    /* if we haven't an external PA, use the library function */
    S2LP_RADIO_SetPALeveldBm(POWER_INDEX,POWER_DBM);
  }
  else
  {
    /* in case we are using the PA board, the S2LPRadioSetPALeveldBm will be not functioning because
    the output power is affected by the amplification of this external component.
    Set the raw register. */
    uint8_t paLevelValue=0x25; /* for example, this value will give 23dBm about */
    S2LP_WriteRegister(PA_POWER8_ADDR, 1, &paLevelValue);
  }
  S2LP_RADIO_SetPALevelMaxIndex(POWER_INDEX);

  /* S2LP Packet config */
  pRadioDriver->PacketConfig();

  pRadioDriver->EnableSQI();

  S2LP_RADIO_QI_SetRssiThreshdBm(RSSI_THRESHOLD);

}

/**
  * @brief  Process of the P2P application
  * @retval None
  */

void MX_SUBG2_P2P_Process(uint8_t *pTxBuff, uint8_t cTxlen, uint8_t* pRxBuff, uint8_t cRxlen)
{
  uint8_t xIndex = 0;
  uint8_t ledToggleCtr = 0;
  uint8_t  dest_addr;
  /*float rRSSIValue = 0;*/

  switch(SM_State)
  {
  case SM_STATE_START_RX:
    {
      AppliReceiveBuff(pRxBuff, cRxlen);
      /* wait for data received or timeout period occured */
      SM_State = SM_STATE_WAIT_FOR_RX_DONE;
    }
    break;

  case SM_STATE_WAIT_FOR_RX_DONE:
    if((RESET != xRxDoneFlag)||(RESET != rx_timeout)||(SET != exitTime))
    {
      if((rx_timeout==SET)||(exitTime==RESET))
      {
        rx_timeout = RESET;
        SM_State = SM_STATE_START_RX;
      }
      else if(xRxDoneFlag)
      {
        xRxDoneFlag=RESET;
        SM_State = SM_STATE_DATA_RECEIVED;
      }
    }
    break;

  case SM_STATE_DATA_RECEIVED:
    {
      pRadioDriver = &radio_cb;

      pRadioDriver->GetRxPacket(pRxBuff,&cRxlen);
      /*rRSSIValue = Spirit1GetRssiTH();*/
      /*rRSSIValue = S2LPGetRssiTH();*/
      xRxFrame.Cmd = pRxBuff[0];
      xRxFrame.CmdLen = pRxBuff[1];
      xRxFrame.Cmdtag = pRxBuff[2];
      xRxFrame.CmdType = pRxBuff[3];
      xRxFrame.DataLen = pRxBuff[4];

      for (xIndex = 5; xIndex < cRxlen; xIndex++)
      {
        xTxFrame.DataBuff[xIndex-5] = pRxBuff[xIndex];
      }
      if(xRxFrame.Cmd == LED_TOGGLE)
      {
         MasterFlag = 0;
        SM_State = SM_STATE_TOGGLE_LED;
      }
      if(xRxFrame.Cmd == ACK_OK)
      {
        SM_State = SM_STATE_ACK_RECEIVED;
      }
    }
    break;

  case SM_STATE_SEND_ACK:
    {
      xTxFrame.Cmd = ACK_OK;
      xTxFrame.CmdLen = 0x01;
      xTxFrame.Cmdtag = xRxFrame.Cmdtag;
      xTxFrame.CmdType = APPLI_CMD;
	  xTxFrame.DataBuff = &pRxBuff[5];
      xTxFrame.DataLen = cTxlen;

      HAL_Delay(DELAY_TX_LED_GLOW);
      AppliSendBuff(&xTxFrame, xTxFrame.DataLen);
      SM_State = SM_STATE_WAIT_FOR_TX_DONE;
    }
    break;

  case SM_STATE_SEND_DATA:
    {
      xTxFrame.Cmd = LED_TOGGLE;
      xTxFrame.CmdLen = 0x01;
      xTxFrame.Cmdtag = txCounter++;
      xTxFrame.CmdType = APPLI_CMD;
      xTxFrame.DataBuff = pTxBuff;
      xTxFrame.DataLen = cTxlen;
      AppliSendBuff(&xTxFrame, xTxFrame.DataLen);
      SM_State = SM_STATE_WAIT_FOR_TX_DONE;
    }
    break;

  case SM_STATE_WAIT_FOR_TX_DONE:
    /* wait for TX done */
    if(xTxDoneFlag)
    {
      xTxDoneFlag = RESET;

      if(xTxFrame.Cmd == LED_TOGGLE)
      {
        SM_State = SM_STATE_START_RX;
      }
      else if(xTxFrame.Cmd == ACK_OK)
      {
        SM_State = SM_STATE_IDLE;
      }
    }
    break;

  case SM_STATE_ACK_RECEIVED:
   if (MasterFlag == 1)
   {
    for(; ledToggleCtr<4; ledToggleCtr++)
    {
      BSP_LED_Toggle(LED2);

      HAL_Delay(DELAY_RX_LED_TOGGLE);
    }

   }
   SM_State = SM_STATE_IDLE;
    break;

  case SM_STATE_TOGGLE_LED:

    BSP_LED_On(LED2);
    dest_addr = RadioGetReceivedDestinationAddress();

    if ((dest_addr == MULTICAST_ADDRESS) || (dest_addr == BROADCAST_ADDRESS))
    {
      /* in that case do not send ACK to avoid RF collisions between several RF ACK messages */
      HAL_Delay(DELAY_TX_LED_GLOW);
      SM_State = SM_STATE_IDLE;
    }
    else
    {
      SM_State = SM_STATE_SEND_ACK;
    }
    break;

  case SM_STATE_IDLE:

    BSP_LED_Off(LED2);
    SM_State = SM_STATE_START_RX;

#if defined(USE_LOW_POWER_MODE)
    Enter_LP_mode();
#endif
    break;
}
}

/**
* @brief  This function handles the point-to-point packet transmission
* @param  AppliFrame_t *xTxFrame = Pointer to AppliFrame_t structure
*         uint8_t cTxlen = Length of aTransmitBuffer
* @retval None
*/
void AppliSendBuff(AppliFrame_t *xTxFrame, uint8_t cTxlen)
{
  uint8_t xIndex = 0;
  uint8_t trxLength = 0;
  pRadioDriver = &radio_cb;

#ifdef USE_STack_PROTOCOL

  PktStackAddressesInit xAddressInit=
  {
    .xFilterOnMyAddress = S_ENABLE,
    .cMyAddress = MY_ADDRESS,
    .xFilterOnMulticastAddress = S_DISABLE,
    .cMulticastAddress = MULTICAST_ADDRESS,
    .xFilterOnBroadcastAddress = S_ENABLE,
    .cBroadcastAddress = BROADCAST_ADDRESS
  };
  S2LP_PCKT_STACK_AddressesInit(&xAddressInit);

#ifdef USE_STack_LLP

  /* LLP structure fitting */
  PktStackLlpInit xStackLLPInit=
  {
    .xAutoAck = S_DISABLE,
    .xPiggybacking = S_DISABLE,
    .xNMaxRetx = PKT_N_RETX_2
  };

#else

  /* LLP structure fitting */
  PktStackLlpInit xStackLLPInit=
  {
    .xAutoAck = S_DISABLE,
    .xPiggybacking = S_DISABLE,
    .xNMaxRetx = PKT_DISABLE_RETX
  };
#endif

  Radio_PktStackLlpInit(&xStackLLPInit);

#endif

#ifdef USE_BASIC_PROTOCOL
  S2LP_PCKT_BASIC_AddressesInit(&xAddressInit);
#endif

  TxFrameBuff[0] = xTxFrame->Cmd;
  TxFrameBuff[1] = xTxFrame->CmdLen;
  TxFrameBuff[2] = xTxFrame->Cmdtag;
  TxFrameBuff[3] = xTxFrame->CmdType;
  TxFrameBuff[4] = xTxFrame->DataLen;

  for(; xIndex < xTxFrame->DataLen; xIndex++)
  {
    TxFrameBuff[xIndex+5] =  xTxFrame->DataBuff[xIndex];
  }

  trxLength = (xIndex+5);

  S2LP_GPIO_IrqDeInit(NULL);

  pRadioDriver->EnableTxIrq();
  /* payload length config */
  pRadioDriver->SetPayloadLen(trxLength);
  /* rx timeout config */
  pRadioDriver->SetRxTimeout(RECEIVE_TIMEOUT);
  /* IRQ registers blanking */

  S2LP_GPIO_IrqClearStatus();
  /* destination address */
  pRadioDriver->SetDestinationAddress(DESTINATION_ADDRESS);

  /* send the TX command */
  pRadioDriver->StartTx(TxFrameBuff, trxLength);
}

/**
* @brief  This function handles the point-to-point packet reception
* @param  uint8_t *RxFrameBuff = Pointer to ReceiveBuffer
*         uint8_t cRxlen = length of ReceiveBuffer
* @retval None
*/
void AppliReceiveBuff(uint8_t *RxFrameBuff, uint8_t cRxlen)
{
  /*float rRSSIValue = 0;*/
  exitTime = SET;
  exitCounter = TIME_TO_EXIT_RX;

#ifdef USE_STack_PROTOCOL

#ifdef USE_STack_LLP
  /* LLP structure fitting */
  PktStackLlpInit xStackLLPInit=
  {
    .xAutoAck = S_ENABLE,
    .xPiggybacking = S_ENABLE,
    .xNMaxRetx = PKT_DISABLE_RETX
  };
#else
  /* LLP structure fitting */
  PktStackLlpInit xStackLLPInit=
  {
    .xAutoAck = S_DISABLE,
    .xPiggybacking = S_DISABLE,
    .xNMaxRetx = PKT_DISABLE_RETX
  };
#endif

  Radio_PktStackLlpInit(&xStackLLPInit);

  PktStackAddressesInit xAddressInit=
  {
    .xFilterOnMyAddress = S_ENABLE,
    .cMyAddress = MY_ADDRESS,
    .xFilterOnMulticastAddress = S_DISABLE,
    .cMulticastAddress = MULTICAST_ADDRESS,
    .xFilterOnBroadcastAddress = S_ENABLE,
    .cBroadcastAddress = BROADCAST_ADDRESS
  };

  S2LP_PCKT_STACK_AddressesInit(&xAddressInit);

  if(EN_FILT_SOURCE_ADDRESS)
  {
    Radio_PktStackFilterOnSourceAddress(S_ENABLE);
    Radio_PktStackSetRxSourceMask(SOURCE_ADDR_MASK);
    Radio_PktStackSetSourceReferenceAddress (SOURCE_ADDR_REF);

  }
  else
  {
    RadioPktStackFilterOnSourceAddress(S_DISABLE);
  }
#endif

  //RadioPktBasicAddressesInit(&xAddressInit);
  S2LP_PCKT_BASIC_AddressesInit(&xAddressInit);

  pRadioDriver = &radio_cb;
  /* S2LP IRQs disable */
  S2LP_GPIO_IrqDeInit(NULL);
  /* S2LP IRQs enable */

  pRadioDriver->EnableRxIrq();
  /* payload length config */
  pRadioDriver->SetPayloadLen(PAYLOAD_LEN);
  /* rx timeout config */

  S2LP_TIM_SetRxTimerMs(700.0);
  SET_INFINITE_RX_TIMEOUT();

  pRadioDriver->SetDestinationAddress(DESTINATION_ADDRESS);
  /* IRQ registers blanking */
  S2LP_GPIO_IrqClearStatus();

  /* RX command */
  pRadioDriver->StartRx();
  /* destination address */
}

/**
* @brief  This function initializes the STack Packet handler of S2LP
* @param  None
* @retval None
*/
void STackProtocolInit(void)
{

  PktStackInit xStackInit=
  {
    .xPreambleLength = PREAMBLE_LENGTH,
    .xSyncLength = SYNC_LENGTH,
    .lSyncWords = SYNC_WORD,

    .xFixVarLength = S_ENABLE,
    .cExtendedPktLenField = S_DISABLE,

    .xCrcMode = CRC_MODE,
    .xFec = EN_FEC,
    .xDataWhitening = EN_WHITENING
  };
  /* Radio Packet config */
  S2LP_PCKT_STACK_Init(&xStackInit);
}

/*xDataWhitening;  *
* @brief  This function initializes the BASIC Packet handler of S2LP
* @param  None
* @retval None
*/
void BasicProtocolInit(void)
{
  /* RAdio Packet config */
 S2LP_PCKT_BASIC_Init(&xBasicInit);
}

/**
* @brief  This routine will put the radio and mcu in LPM
* @param  None
* @retval None
*/
void Enter_LP_mode(void)
{
  pMCU_LPM_Comm = &MCU_LPM_cb;
  pRadio_LPM_Comm = &Radio_LPM_cb;

#if defined(MCU_SLEEP_MODE)&&defined(RF_SHUTDOWN)
  {
    pRadio_LPM_Comm->RadioShutDown();
    pMCU_LPM_Comm->McuSleepMode();
  }
#elif defined(MCU_SLEEP_MODE)&&defined(RF_STANDBY)
  {
    pRadio_LPM_Comm->RadioStandBy();
    pMCU_LPM_Comm->McuSleepMode();
  }
#elif defined(MCU_SLEEP_MODE)&&defined(RF_SLEEP)
  {
    pRadio_LPM_Comm->RadioSleep();
    pMCU_LPM_Comm->McuSleepMode();
  }
#elif defined(MCU_STOP_MODE)&&defined(RF_SHUTDOWN)
  {
    pRadio_LPM_Comm->RadioShutDown();
    pMCU_LPM_Comm->McuStopMode();
  }
#elif defined(MCU_STOP_MODE)&&defined(RF_STANDBY)
  {
    pRadio_LPM_Comm->RadioStandBy();
    pMCU_LPM_Comm->McuStopMode();
  }
#elif defined(MCU_STOP_MODE)&&defined(RF_SLEEP)
  {
    pRadio_LPM_Comm->RadioSleep();
    pMCU_LPM_Comm->McuStopMode();
  }
#else
  pMCU_LPM_Comm->McuSleepMode();
#endif
}

/**
* @brief  This routine wake-up the mcu and radio from LPM
* @param  None
* @retval None
*/
void Exit_LP_mode(void)
{
  pRadio_LPM_Comm = &Radio_LPM_cb;
  pRadio_LPM_Comm->RadioPowerON();
}

/**
* @brief  This routine puts the MCU in stop mode
* @param  None
* @retval None
*/
void MCU_Enter_StopMode(void)
{

  /*## Enter Stop Mode #######################################################*/
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

}

/**
* @brief  This routine puts the MCU in standby mode
* @param  None
* @retval None
*/
void MCU_Enter_StandbyMode(void)
{
    /*## Enter Standby Mode ####################################################*/
  /* Request to enter STANDBY mode  */
  HAL_PWR_EnterSTANDBYMode();
}

/**
* @brief  This routine puts the MCU in sleep mode
* @param  None
* @retval None
*/
void MCU_Enter_SleepMode(void)
{
  /*Suspend Tick increment to prevent wakeup by Systick interrupt.
  Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/

    /* Configure Key Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Suspend Tick increment to prevent wakeup by Systick interrupt.
  Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base) */
  HAL_SuspendTick();

  /* Request to enter SLEEP mode */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

/**
* @brief  This function will turn on the radio and waits till it enters the Ready state.
* @param  Param:None.
* @retval None
*
*/
void RadioPowerON(void)
{

  S2LP_CMD_StrobeReady();

  do{
    /* Delay for state transition */
    for(volatile uint8_t i=0; i!=0xFF; i++);

    /* Reads the MC_STATUS register */

    S2LP_RefreshStatus();

  }
  while(g_xStatus.MC_STATE!=MC_STATE_READY);
}

/**
* @brief  This function will Shut Down the radio.
* @param  Param:None.
* @retval None
*
*/
void RadioPowerOFF(void)
{
  S2868A2_RADIO_EnterShutdown();
}

/**
* @brief  This function will put the radio in standby state.
* @param  None.
* @retval None
*
*/
void RadioStandBy(void)
{
  S2LP_CMD_StrobeStandby();
}

/**
* @brief  This function will put the radio in sleep state.
* @param  None.
* @retval None
*
*/
void RadioSleep(void)
{
  S2LP_CMD_StrobeSleep();
}

/**
* @brief  This routine updates the respective status for key press.
* @param  None
* @retval None
*/
void Set_KeyStatus(FlagStatus val)
{
  if(val==SET)
  {
    SM_State = SM_STATE_SEND_DATA;
    MasterFlag = 1;
  }
  else
    MasterFlag = 0;
}

/**
* @brief  This function handles External interrupt request. In this application it is used
*         to manage the S2LP IRQ configured to be notified on the S2LP GPIO_3.
* @param  None
* @retval None
*/
static void P2PInterruptHandler(void)
{
  S2LP_GPIO_IrqGetStatus(&xIrqStatus);

  /* Check the S2LP TX_DATA_SENT IRQ flag */
  if(
     (xIrqStatus.IRQ_TX_DATA_SENT)

#ifdef CSMA_ENABLE
       ||(xIrqStatus.IRQ_MAX_BO_CCA_REACH)
#endif
         )
  {
#ifdef CSMA_ENABLE
    S2LP_CSMA_Enable(S_DISABLE);
    S2LP_PCKT_HNDL_SetRxPersistentMode(S_ENABLE);
    S2LP_RADIO_QI_CsBlanking(S_ENABLE);

    if(xIrqStatus.IRQ_MAX_BO_CCA_REACH)
    {
      S2LP_CMD_StrobeSabort();
    }
    S2LP_RADIO_QI_SetRssiThreshdBm(RSSI_THRESHOLD);

#endif

    xTxDoneFlag = SET;
  }

  /* Check the S2LP RX_DATA_READY IRQ flag */
  if((xIrqStatus.IRQ_RX_DATA_READY))
  {
    xRxDoneFlag = SET;
  }

  /* Restart receive after receive timeout*/
  if (xIrqStatus.IRQ_RX_TIMEOUT)
  {
    rx_timeout = SET;
    S2LP_CMD_StrobeRx();
  }

  /* Check the S2LP RX_DATA_DISC IRQ flag */
  if(xIrqStatus.IRQ_RX_DATA_DISC)
  {
    /* RX command - to ensure the device will be ready for the next reception */
    S2LP_CMD_StrobeRx();
  }
}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
 if(Button==BUTTON_USER)
  {
   Set_KeyStatus(SET);
  }
  else
    Set_KeyStatus(RESET);
}

/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
void S2LPInterfaceInit(void)
{
  /* Initialize the SDN pin micro side */
  S2868A2_RADIO_Init();

  if( S2LP_Init() != S2LP_OK)
   {
    /* Initialization Error */
     while(1){};
   }

  /* EepromSpiInitialization(); */
  S2868A2_EEPROM_Init(EEPROM_INSTANCE);

  /* S2LP ON */
  S2868A2_RADIO_EnterShutdown();
  S2868A2_RADIO_ExitShutdown();

  S2LPManagementIdentificationRFBoard();

  /* if the board has eeprom, we can compensate the offset calling S2LPManagementGetOffset
  (if eeprom is not present this fcn will return 0) */
  xRadioInit.lFrequencyBase = xRadioInit.lFrequencyBase + S2LP_ManagementGetOffset();

  /* if needed this will set the range extender pins */
  S2LP_ManagementRangeExtInit();

#if 0
  /* if needed this will set the EXT_REF bit of the S2-LP */

  S2LP_TCXOInit();
#endif

  /* uC IRQ enable */
  S2868A2_RADIO_IoIrqEnable(GpioIrq);
}

/**
* @brief  Sets the S2LP frequency band
* @param  uint8_t value: RF FREQUENCY
* @retval None
*/
void S2LP_ManagementSetBand(uint8_t value)
{
  s_RfModuleBand = value;
}
/**
* @brief  returns the S2LP frequency band
* @param  None
* @retval uint8_t value: RF FREQUENCY
*/
uint8_t S2LP_ManagementGetBand(void)
{
  return s_RfModuleBand;
}

void S2LP_ManagementSetOffset(int32_t value)
{
  s_RfModuleOffset=value;
}

int32_t S2LP_ManagementGetOffset(void)
{
  return s_RfModuleOffset;
}

/**
* @brief  Gets the S2LP Tcxo
* @param  RangeExtType
* @retval None
*/
uint8_t S2LP_ManagementGetTcxo(void)
{
  return s_Tcxo;
}

/**
* @brief  Gets the S2LP cut
* @param  RangeExtType
* @retval None
*/
S2LPCutType S2LP_ManagementGetCut(void)
{
  return s_S2LPCut;
}

/**
* @brief Init the RANGE EXTENSION SDK S2LP Management Range Extender Functions
* @param  None
* @retval None
* @{
*/
void S2LP_ManagementRangeExtInit(void)
{
  RangeExtType range_type = S2LP_ManagementGetRangeExtender();
  if(range_type==RANGE_EXT_SKYWORKS_868) {
    /* CSD control */
    S2LP_GPIO_Init(&(SGpioInit){S2LP_GPIO_0, S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP, S2LP_GPIO_DIG_OUT_TX_RX_MODE});

    /* CTX/BYP control */
    S2LP_GPIO_Init(&(SGpioInit){S2LP_GPIO_1, S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP, S2LP_GPIO_DIG_OUT_RX_STATE});

    /* Vcont control */
    S2LP_GPIO_Init(&(SGpioInit){S2LP_GPIO_2, S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP, S2LP_GPIO_DIG_OUT_TX_STATE});
  }
}

/**
* @brief  Sets the S2LP range extender type
* @param  RangeExtType
* @retval None
*/
void S2LP_ManagementSetRangeExtender(RangeExtType xRangeType)
{
  xRangeExtType = xRangeType;
}

/**
* @brief  returns the S2LP range extender type
* @param  None
* @retval RangeExtType
*/
RangeExtType S2LP_ManagementGetRangeExtender(void)
{
  return xRangeExtType;
}

void S2LPManagementIdentificationRFBoard(void)
{
  uint8_t tmp;
  StatusBytes status;

  do{
    /* Delay for state transition */
    for(volatile uint8_t i=0; i!=0xFF; i++);

    /* Reads the MC_STATUS register */
    status = S2LP_ReadRegister(0x8E, 1, &tmp);
  }  while(status.MC_STATE!=MC_STATE_READY);

  S2LP_ReadRegister(0xF1, 1, &tmp);

  s_S2LPCut=(S2LPCutType)tmp;

  if((s_S2LPCut==S2LP_CUT_2_0) || (s_S2LPCut==S2LP_CUT_2_1))
  {
    S2868A2_RADIO_EnterShutdown();
  }

   if(!EepromIdentification())  /* EEPROM is not present */
  {
    S2868A2_RADIO_ExitShutdown();
    if(S2LPManagementComputeXtalFrequency()==0)
    {
      /* if it fails force it to 50MHz */
      S2LP_RADIO_SetXtalFrequency(XTAL_FREQUENCY);
    }
  }
  else  // EEPROM present
  {
    //read the memory and set the variable
    uint8_t tmpBuffer[32];
    /* EepromRead(0x0000, 32, tmpBuffer);*/
    S2868A2_EEPROM_ReadPage(EEPROM_INSTANCE, 0x0000, 32, tmpBuffer);
    uint32_t xtal;
    float foffset=0;
    if(tmpBuffer[0]==0 || tmpBuffer[0]==0xFF) {
      /* this one happens in production where the E2PROM is here but blank */
      S2868A2_RADIO_EnableTCXO();
      if((s_S2LPCut==S2LP_CUT_2_0) || (s_S2LPCut==S2LP_CUT_2_1))
      {
        S2868A2_RADIO_ExitShutdown();
      }
      S2LPManagementComputeXtalFrequency();
      return;
    }
    switch(tmpBuffer[1]) {
    case 0:
      xtal = 24000000;
      S2LP_RADIO_SetXtalFrequency(xtal);
      break;
    case 1:
      xtal = 25000000;
      S2LP_RADIO_SetXtalFrequency(xtal);
      break;
    case 2:
      xtal = 26000000;
      S2LP_RADIO_SetXtalFrequency(xtal);
      break;
    case 3:
      xtal = 48000000;
      S2LP_RADIO_SetXtalFrequency(xtal);
      break;
    case 4:
      xtal = 50000000;
      S2LP_RADIO_SetXtalFrequency(xtal);
      break;
    case 5:
      xtal = 52000000;
      S2LP_RADIO_SetXtalFrequency(xtal);
      break;
    case 0xff:
      /* XTAL freqeuncy is custom */
      for(uint8_t i=0;i<4;i++)
      {
        ((uint8_t*)&xtal)[i]=tmpBuffer[30-i];
      }
      S2LP_RADIO_SetXtalFrequency(xtal);
      break;
    default:
      S2LPManagementComputeXtalFrequency();
      break;
    }

    /* TCXO field */
    if(tmpBuffer[31]==1)
    {
      S2LP_ManagementSetTcxo(1);
    }

    S2LP_ManagementSetBand(tmpBuffer[3]);
    S2LP_ManagementSetRangeExtender((RangeExtType)tmpBuffer[5]);

  /*  EepromRead(0x0021,4,tmpBuffer); */

    S2868A2_EEPROM_ReadPage(EEPROM_INSTANCE, 0x0021,4,tmpBuffer);

    for(uint8_t i=0;i<4;i++)
    {
      ((uint8_t*)&foffset)[i]=tmpBuffer[3-i];
    }

    S2LP_ManagementSetOffset((int32_t)foffset);
  }

  if((s_S2LPCut==S2LP_CUT_2_0) || (s_S2LPCut==S2LP_CUT_2_1))
  {
    S2868A2_RADIO_ExitShutdown();
  }

}

/**
* @brief  This function can be used to automatically measure the XTAL frequency making use of the
*         S2LP clock output to pin and an STM32L timer in compare mode.
* @param  None.
* @retval None.
*/
uint32_t S2LPManagementComputeXtalFrequency(void)
{
  uint32_t lMeasuredXtalFrequency;

  lMeasuredXtalFrequency=50000000;

  S2LP_RADIO_SetXtalFrequency(lMeasuredXtalFrequency);

  return lMeasuredXtalFrequency;
}

/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
uint8_t EepromIdentification(void)
{
  uint32_t status;
  uint8_t eeprom_status;
  /* try to get the status of the EEPROM */
 /* status = EepromStatus(); */
  status = S2868A2_EEPROM_IsReady(EEPROM_INSTANCE);

  if(status == 1) {
    /* if it is EEPROM_STATUS_SRWD, ok the EEPROM is present and ready to work */
    eeprom_status = 1;
  }
  else
  {
    /* EepromWriteEnable(); */
    BSP_EEPROM_WriteEnable(EEPROM_INSTANCE);

    HAL_Delay(1);
    /* else the bit may be not set (first time we see this EEPROM), try to set it*/
    eeprom_status = BSP_EEPROM_SetSrwd(EEPROM_INSTANCE);

    HAL_Delay(1);

    /* status = EepromStatus(); */
    status = S2868A2_EEPROM_IsReady(EEPROM_INSTANCE);;

    if(status == 1) { // 0xF0 mask [SRWD 0 0 0]
      /* if it is EEPROM_STATUS_SRWD, ok the EEPROM is present and ready to work */
      eeprom_status = 1;
    }
    else
    {
      /* else no EEPROM is present */
      eeprom_status = 0;
    }
  }
  return eeprom_status;
}

/**
* @brief Set TCXO
* @param  tcxo
* @retval None
* @{
*/
void S2LP_ManagementSetTcxo(uint8_t tcxo)
{
  s_Tcxo = tcxo;
}

/**
* @brief  this function sets the packet configuration according to the protocol used
* @param  None
* @retval None
*/
void S2LP_PacketConfig(void)
{
#if defined(USE_STack_PROTOCOL)

  STackProtocolInit();

#elif defined(USE_BASIC_PROTOCOL)

  BasicProtocolInit();

#endif
}

/**
* @brief  this function sets the payload length
* @param  uint8_t length
* @retval None
*/
void S2LP_SetPayloadlength(uint8_t length)
{
#if defined(USE_STack_PROTOCOL)
  /* Payload length config */
  S2LP_PCKT_STACK_SetPayloadLength(length);

#elif defined(USE_BASIC_PROTOCOL)
  /* payload length config */
  S2LP_PCKT_BASIC_SetPayloadLength(length);
#endif
}

/**
* @brief  this function sets the destination address
* @param  uint8_t adress
* @retval None
*/
void S2LP_SetDestinationAddress(uint8_t address)
{
#if defined(USE_STack_PROTOCOL)
  /* Destination address */
  S2LP_PktStackSetDestinationAddress(address);
#elif defined(USE_BASIC_PROTOCOL)
  /* destination address */
  S2LP_PCKT_HNDL_SetRxSourceReferenceAddress(address);
#endif
}

/**
* @brief  this function enables the Tx IRQ
* @param  None
* @retval None
*/
void S2LP_EnableTxIrq(void)
{
  /* S2LP IRQs enable */
  S2LP_GPIO_IrqConfig(TX_DATA_SENT, S_ENABLE);
#if defined(USE_STack_LLP)
  S2LP_GPIO_IrqConfig(MAX_RE_TX_REACH, S_ENABLE);
#endif
}

/**
* @brief  this function enables the Rx IRQ
* @param  None
* @retval None
*/
void S2LP_EnableRxIrq(void)
{
  /* S2LP IRQs enable */
  S2LP_GPIO_IrqConfig(RX_DATA_READY, S_ENABLE);
  S2LP_GPIO_IrqConfig(RX_DATA_DISC, S_ENABLE);
  S2LP_GPIO_IrqConfig(RX_TIMEOUT, S_ENABLE);
}

/**
* @brief  this function set the receive timeout period
* @param  None
* @retval None
*/
void S2LP_SetRxTimeout(float cRxTimeOut)
{
  if(cRxTimeOut == 0)
  {
    /* rx timeout config */
    SET_INFINITE_RX_TIMEOUT();
    S2LP_TIM_SetRxTimerStopCondition(ANY_ABOVE_THRESHOLD);
  }
  else
  {
    /* RX timeout config */
    S2LP_TIM_SetRxTimerMs(cRxTimeOut);
    S2LP_EnableSQI();
    S2LP_TIM_SetRxTimerStopCondition(RSSI_AND_SQI_ABOVE_THRESHOLD);
  }
}

/**
* @brief  this function enables SQI check
* @param  None
* @retval None
*/
void S2LP_EnableSQI(void)
{
  /* enable SQI check */
  S2LP_RADIO_QI_SetSQIThreshold(0x00);
  S2LP_RADIO_QI_EnableSQI(S_ENABLE);
}
/**
* @brief  this function sets the RSSI threshold
* @param  int dbmValue
* @retval None
*/
float S2LP_GetRssiTH(void)
{
  float dbmValue=0;
  dbmValue = S2LP_RADIO_QI_GetRssidBm();
  return dbmValue;
}

/**
* @brief  this function starts the RX process
* @param  None
* @retval None
*/
void S2LP_StartRx(void)
{
  if(g_xStatus.MC_STATE==MC_STATE_RX)
  {
    S2LP_CMD_StrobeSabort();
  }
  /* RX command */
  S2LP_CMD_StrobeRx();
}

/**
* @brief  this function receives the data
* @param  None
* @retval None
*/
void S2LP_GetRxPacket(uint8_t *buffer, uint8_t *cRxData )
{
  uint8_t noofbytes = 0;
  /* when rx data ready read the number of received bytes */
  *cRxData=S2LP_FIFO_ReadNumberBytesRxFifo();
  noofbytes = *cRxData;
  /* read the RX FIFO */
  S2LP_ReadFIFO(noofbytes, buffer);

  S2LP_CMD_StrobeFlushRxFifo();
}

/**
* @brief  this function starts the TX process
* @param  None
* @retval None
*/
void S2LP_StartTx(uint8_t *buffer, uint8_t size )
{
  if(g_xStatus.MC_STATE==MC_STATE_RX)
  {
    S2LP_CMD_StrobeSabort();
  }

#ifdef CSMA_ENABLE

  /* Enable CSMA */
  S2LP_PCKT_HNDL_SetRxPersistentMode(S_DISABLE);
  S2LP_RADIO_QI_CsBlanking(S_DISABLE);

  S2LP_CSMA_Init(&xCsmaInit);
  S2LP_CSMA_Enable(S_ENABLE);
  S2LP_RADIO_QI_SetRssiThreshdBm(CSMA_RSSI_THRESHOLD);

#endif

  /* fit the TX FIFO */
  S2LP_CMD_StrobeFlushTxFifo();

  S2LP_WriteFIFO(size, buffer);

  /* send the TX command */
  S2LP_CMD_StrobeTx();
}

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

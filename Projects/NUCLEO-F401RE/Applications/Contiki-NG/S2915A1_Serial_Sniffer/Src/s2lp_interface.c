
/**
  ******************************************************************************
  * @file    s2lp_interface.c
  * @author  SRA Application Team
  * @brief   Source file for S2LP radio configuration/driver
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
#include "s2lp_interface.h"
#include "radio-driver.h"
#include "stm32f4xx_nucleo.h"
#include "s2915a1.h"
#include "s2lp.h"
#include "subg2_bsp_ip_conf.h"
#include "contiki-platform.h"

#include "stm32f4xx_nucleo.h"

/** @defgroup S2LP_Nucleo
  * @{
  */

/** @defgroup Main
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static S2LPCutType s_S2LPCut = S2LP_CUT_2_1;
static RangeExtType xRangeExtType = RANGE_EXT_NONE;
static uint8_t s_RfModuleBand = 0, s_Tcxo=0;
static int32_t s_RfModuleOffset=0;

static GpioIrqHandler *GpioIrq[] = {

#if (USE_S2915A1_RADIO_GPIO_0 == 1)
  Radio_interrupt_callback,
#endif
#if (USE_S2915A1_RADIO_GPIO_1 == 1)
  Radio_interrupt_callback,
#endif
#if (USE_S2915A1_RADIO_GPIO_2 == 1)
  Radio_interrupt_callback,
#endif
#if (USE_S2915A1_RADIO_GPIO_3 == 1)
  Radio_interrupt_callback,
#endif
};
/* Global variables ----------------------------------------------------------*/
extern volatile unsigned long seconds;
extern volatile clock_time_t ticks;
extern SRadioInit xRadioInit;

/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
void S2LPInterfaceInit(void)
{
  /* Initialize the SDN pin micro side */
  S2915A1_RADIO_Init();

  if( S2LP_Init() != S2LP_OK)
   {
    /* Initialization Error */
     while(1){};
   }

  /* EepromSpiInitialization(); */
  S2915A1_EEPROM_Init(EEPROM_INSTANCE);

  /* S2LP ON */
  S2915A1_RADIO_EnterShutdown();
  S2915A1_RADIO_ExitShutdown();

  S2LPManagementIdentificationRFBoard();

  /* if the board has eeprom, we can compensate the offset calling S2LPManagementGetOffset
  (if eeprom is not present this fcn will return 0) */
  xRadioInit.lFrequencyBase = (uint32_t) BASE_FREQUENCY + S2LP_ManagementGetOffset();

  /* if needed this will set the range extender pins */
  S2LP_ManagementRangeExtInit();

  /* uC IRQ enable */
  S2915A1_RADIO_IoIrqEnable(GpioIrq);
}
/*----------------------------------------------------------------------------*/
void S2LP_Interface_IoIrqEnable(void)
{
  (void) S2915A1_RADIO_IoIrqEnable(GpioIrq);
}
/*----------------------------------------------------------------------------*/
void S2LP_Interface_IoIrqDisable(void)
{
  (void) S2915A1_RADIO_IoIrqDisable(GpioIrq);
}
/*----------------------------------------------------------------------------*/
void S2LPManagementRcoCalibration(void)
{
  uint8_t tmp[2],tmp2;

  S2LP_ReadRegister(0x6D, 1, &tmp2);
  tmp2 |= 0x01;
  S2LP_WriteRegister(0x6D, 1, &tmp2);

  S2LP_CMD_StrobeCommand(CMD_STANDBY);
  HAL_Delay(100);
  S2LP_CMD_StrobeCommand(CMD_READY);

  do
  {
    S2LP_ReadRegister(0x8D, 1, tmp);
  }
  while((tmp[0]&0x10)==0);

  S2LP_ReadRegister(0x94, 2, tmp);
  S2LP_ReadRegister(0x6F, 1, &tmp2);
  tmp[1]=(tmp[1]&0x80)|(tmp2&0x7F);

  S2LP_WriteRegister(0x6E, 2, tmp);
  S2LP_ReadRegister(0x6D, 1, &tmp2);
  tmp2 &= 0xFE;

  S2LP_WriteRegister(0x6D, 1, &tmp2);
}
/*----------------------------------------------------------------------------*/
/**
* @brief  Sets the S2LP frequency band
* @param  uint8_t value: RF FREQUENCY
* @retval None
*/
void S2LP_ManagementSetBand(uint8_t value)
{
  s_RfModuleBand = value;
}
/*----------------------------------------------------------------------------*/
/**
* @brief  returns the S2LP frequency band
* @param  None
* @retval uint8_t value: RF FREQUENCY
*/
uint8_t S2LP_ManagementGetBand(void)
{
  return s_RfModuleBand;
}
/*----------------------------------------------------------------------------*/
void S2LP_ManagementSetOffset(int32_t value)
{
  s_RfModuleOffset=value;
}
/*----------------------------------------------------------------------------*/
int32_t S2LP_ManagementGetOffset(void)
{
  return s_RfModuleOffset;
}
/*----------------------------------------------------------------------------*/
/**
* @brief  Gets the S2LP Tcxo
* @param  RangeExtType
* @retval None
*/
uint8_t S2LP_ManagementGetTcxo(void)
{
  return s_Tcxo;
}
/*----------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------*/
/**
* @brief  Gets the S2LP cut
* @param  RangeExtType
* @retval None
*/
S2LPCutType S2LP_ManagementGetCut(void)
{
  return s_S2LPCut;
}
/*----------------------------------------------------------------------------*/
/**
* @brief Init the RANGE EXTENSION SDK S2LP Management Range Extender Functions
* @param  None
* @retval None
* @{
*/
void S2LP_ManagementRangeExtInit(void)
{
  RangeExtType range_type = S2LP_ManagementGetRangeExtender();
  if(range_type == RANGE_EXT_SKYWORKS_SKY66420) {
    S2915A1_RADIO_RangeExtGpioInit();
  }
}
/*----------------------------------------------------------------------------*/
/**
* @brief  Sets the S2LP range extender type
* @param  RangeExtType
* @retval None
*/
void S2LP_ManagementSetRangeExtender(RangeExtType xRangeType)
{
  xRangeExtType = xRangeType;
}
/*----------------------------------------------------------------------------*/
/**
* @brief  returns the S2LP range extender type
* @param  None
* @retval RangeExtType
*/
RangeExtType S2LP_ManagementGetRangeExtender(void)
{
  return xRangeExtType;
}
/*----------------------------------------------------------------------------*/
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
    S2915A1_RADIO_EnterShutdown();
  }

   if(!EepromIdentification())  /* EEPROM is not present */
  {
    S2915A1_RADIO_ExitShutdown();
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
    S2915A1_EEPROM_ReadPage(EEPROM_INSTANCE, 0x0000, 32, tmpBuffer);
    uint32_t xtal;
    float foffset=0;
    if(tmpBuffer[0]==0 || tmpBuffer[0]==0xFF) {
      /* this one happens in production where the E2PROM is here but blank */
      S2915A1_RADIO_EnableTCXO();
      if((s_S2LPCut==S2LP_CUT_2_0) || (s_S2LPCut==S2LP_CUT_2_1))
      {
        S2915A1_RADIO_ExitShutdown();
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

    S2915A1_EEPROM_ReadPage(EEPROM_INSTANCE, 0x0021,4,tmpBuffer);

    for(uint8_t i=0;i<4;i++)
    {
      ((uint8_t*)&foffset)[i]=tmpBuffer[3-i];
    }

    S2LP_ManagementSetOffset((int32_t)foffset);
  }

  if((s_S2LPCut==S2LP_CUT_2_0) || (s_S2LPCut==S2LP_CUT_2_1))
  {
    S2915A1_RADIO_ExitShutdown();
  }

}
/*----------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------*/
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
  status = S2915A1_EEPROM_IsReady(EEPROM_INSTANCE);

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
    status = S2915A1_EEPROM_IsReady(EEPROM_INSTANCE);;

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

/*----------------------------------------------------------------------------*/
/**
* @brief  Configure Mode of Range ext (PA).
* @param  operation   : Transmission mode type
* @param  RangeExtType: PA type
* @retval None.
*/
void S2LP_ConfigRangeExt(PA_OperationType operation)
{
	if (S2LP_ManagementGetRangeExtender() == RANGE_EXT_SKYWORKS_SKY66420)
	{
		S2915A1_RADIO_RangeExtOperate(operation);
	}
}
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

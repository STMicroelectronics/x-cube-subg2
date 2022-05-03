
/**
  ******************************************************************************
  * @file    s2lp_management.c
  * @author  SRA Application Team
  * @brief   Source file for S2LP radio management functions
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

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "s2lp_management.h"
#include "s2868a1.h"
#include "s2lp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static S2LPCutType s_S2LPCut = S2LP_CUT_2_1;
static RangeExtType xRangeExtType = RANGE_EXT_NONE;
static uint8_t s_RfModuleBand = 0, s_Tcxo=0;
static int32_t s_RfModuleOffset=0;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/**
* @brief  Sets the S2LP frequency band (id)
* @param  uint8_t value: RF FREQUENCY BAND ID
* @retval None
*/
void S2LP_ManagementSetBand(uint8_t value)
{
  s_RfModuleBand = value;
}
/*----------------------------------------------------------------------------*/
/**
* @brief  returns the S2LP frequency band (id)
* @param  None
* @retval uint8_t value: RF FREQUENCY BAND ID
*/
uint8_t S2LP_ManagementGetBand(void)
{
  return s_RfModuleBand;
}
/*----------------------------------------------------------------------------*/
/**
* @brief  returns the S2LP frequency band
* @param  None
* @retval uint32_t value: RF FREQUENCY
*/
uint32_t S2LP_GetFrequencyBand(void)
{
  uint32_t frequency = 0;
  const uint32_t band_frequencies[] = {
    169000000,
    315000000,
    433000000,
    868000000,
    915000000,
    450000000
  };

  if (s_RfModuleBand < (sizeof(band_frequencies)/sizeof(uint32_t))) {
    frequency = band_frequencies[s_RfModuleBand];
  }

  return frequency;
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
* @param  None
* @retval tcxo
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
* @param  None
* @retval S2LP cut
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
  if(range_type==RANGE_EXT_SKYWORKS_868) {
    /* CSD control */
    S2LP_GPIO_Init(&(SGpioInit){S2LP_GPIO_0, S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP, S2LP_GPIO_DIG_OUT_TX_RX_MODE});

    /* CTX/BYP control */
    S2LP_GPIO_Init(&(SGpioInit){S2LP_GPIO_1, S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP, S2LP_GPIO_DIG_OUT_RX_STATE});

    /* Vcont control */
    S2LP_GPIO_Init(&(SGpioInit){S2LP_GPIO_2, S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP, S2LP_GPIO_DIG_OUT_TX_STATE});
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
void S2LP_ManagementIdentificationRFBoard(void)
{
  uint8_t tmp;

  do{
    /* Delay for state transition */
    for(volatile uint8_t i=0; i!=0xFF; i++);

    /* Reads the MC_STATUS register */
    S2LP_ReadRegister(0x8E, 1, &tmp);
    S2LP_RefreshStatus();
  }  while(g_xStatus.MC_STATE!=MC_STATE_READY);

  S2LP_ReadRegister(0xF1, 1, &tmp);

  s_S2LPCut=(S2LPCutType)tmp;

  if((s_S2LPCut==S2LP_CUT_2_0) || (s_S2LPCut==S2LP_CUT_2_1))
  {
    S2868A1_RADIO_EnterShutdown();
  }

   if(!EepromIdentification())  /* EEPROM is not present */
  {
    S2868A1_RADIO_ExitShutdown();
    if(S2LP_ManagementComputeXtalFrequency()==0)
    {
      /* if it fails force it to 50MHz */
      S2LP_RADIO_SetXtalFrequency(XTAL_FREQUENCY);
    }
  }
  else  // EEPROM present
  {
    //read the memory and set the variable
    uint8_t tmpBuffer[32];
    uint32_t xtal;
    float foffset=0;

    S2868A1_EEPROM_ReadPage(EEPROM_INSTANCE, 0x0000, 32, tmpBuffer);
    if(tmpBuffer[0]==0 || tmpBuffer[0]==0xFF) {
      /* this one happens in production where the E2PROM is here but blank */
      S2868A1_RADIO_EnableTCXO();
      if((s_S2LPCut==S2LP_CUT_2_0) || (s_S2LPCut==S2LP_CUT_2_1))
      {
        S2868A1_RADIO_ExitShutdown();
      }
      S2LP_ManagementComputeXtalFrequency();
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
      S2LP_ManagementComputeXtalFrequency();
      break;
    }

    /* TCXO field */
    if(tmpBuffer[31]==1)
    {
      S2LP_ManagementSetTcxo(1);
    }

    S2LP_ManagementSetBand(tmpBuffer[3]);
    S2LP_ManagementSetRangeExtender((RangeExtType)tmpBuffer[5]);

    S2868A1_EEPROM_ReadPage(EEPROM_INSTANCE, 0x0021,4,tmpBuffer);

    for(uint8_t i=0;i<4;i++)
    {
      ((uint8_t*)&foffset)[i]=tmpBuffer[3-i];
    }

    S2LP_ManagementSetOffset((int32_t)foffset);
  }

  if((s_S2LPCut==S2LP_CUT_2_0) || (s_S2LPCut==S2LP_CUT_2_1))
  {
    S2868A1_RADIO_ExitShutdown();
  }

}
/*----------------------------------------------------------------------------*/
/**
* @brief  This function can be used to automatically measure the XTAL frequency making use of the
*         S2LP clock output to pin and an STM32L timer in compare mode.
* @param  None.
* @retval None.
*/
uint32_t S2LP_ManagementComputeXtalFrequency(void)
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
  status = S2868A1_EEPROM_IsReady(EEPROM_INSTANCE);

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

    status = S2868A1_EEPROM_IsReady(EEPROM_INSTANCE);;

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

#ifdef __cplusplus
}
#endif

/**
  ******************************************************************************
  * @file    custom_s2lp_management.c
  * @author  SRA Application Team
  * @brief   Source file for Custom S2LP radio management functions
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
#include "s2lp_radio_settings.h"
#include "custom_s2lpgpio_setting.h"
#include "custom_s2lp_management.h"
#include "s2lp_sdkapi_mapping.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define EEPROM_NO					0
#define EEPROM_YES					1

/**
* @brief Definitions for EEPROM: by default not enabled for custom case.
*  EEPROM is not (yet ?) supported for the custom case, since it may be really
*  system specific. If you want to enable it you'll also have to add your own
*  code in the functions reported here below.
*/
#define EEPROM_PRESENT                          EEPROM_NO

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static S2LPCutType s_S2LPCut = S2LP_CUT_2_1;
static int32_t s_RfModuleOffset=0;
static uint8_t s_RfModuleBand = 3; /*by default 868MHz*/
static volatile RangeExtType s_RfRangeExtender = RANGE_EXT_NONE;
static volatile uint32_t s_RfXtalFrequency = XTAL_FREQUENCY;

/* Private function prototypes -----------------------------------------------*/

/*These functions have to be implemented if you want to support EEPROM for
  custom case */

__weak uint32_t S2LP_ManagementComputeXtalFrequency(void)
{
  /* Add your own code to support EEPROM in custom case. */
  return s_RfXtalFrequency;
}

__weak uint8_t EepromIdentification(void)
{
  /* Add your own code to support EEPROM in custom case. */
  return 0;
}

/* Exported functions --------------------------------------------------------*/
/**
* @brief Init the RANGE EXTENSION SDK S2LP Management Range Extender Functions
* @param  None
* @retval None
* @{
*/
void S2LP_ManagementRangeExtInit(void)
{
  RangeExtType range_type = S2LP_ManagementGetRangeExtender();
  if(range_type!=RANGE_EXT_NONE) {
	  /*To be implemented by user according to its custom board. */
  }
}
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
int32_t S2LP_ManagementGetOffset(void)
{
  return s_RfModuleOffset;
}
/*----------------------------------------------------------------------------*/
void S2LP_ManagementSetOffset(int32_t value)
{
  s_RfModuleOffset=value;
}
/*----------------------------------------------------------------------------*/
S2LPCutType S2LP_ManagementGetCut(void)
{
  return s_S2LPCut;
}
/*----------------------------------------------------------------------------*/
void S2LP_ManagementIdentificationRFBoard(void)
{
  uint8_t tmp;

  /* Read Cut version from S2LP registers */
  S2LP_ReadRegister(0xF1, 1, &tmp);
  s_S2LPCut = (S2LPCutType)tmp;

#if EEPROM_PRESENT == EEPROM_YES
/*This part of code has been left here like in the X-NUCLEO case, but some
  function, if you want to enable EEPROM support, has to be implemented. */
  uint8_t was_in_sdn=1;
  int32_t xtal_comp_value;
  float foffset = 0;

  if(CUSTOM_S2LP_RADIO_EnterShutdown()==RESET)
  {
    /* if reset it was not in SDN */
	  CUSTOM_S2LP_RADIO_EnterShutdown();
    was_in_sdn=0;
  }

  /* Auto recognize FKI or X-NUCLEO daughter boards */
  EepromIdentification();

  /* Read the EEPROM */
  uint8_t tmpBuffer[32];
  CUSTOM_S2LP_EEPROM_ReadPage(0, 0x0000, 32, tmpBuffer);

  /* Data in EEPROM is not valid ... */
  if(tmpBuffer[0]==0 || tmpBuffer[0]==0xFF) {
    S2LP_ManagementComputeXtalFrequency();
    if(was_in_sdn==0) { CUSTOM_S2LP_RADIO_ExitShutdown(); }

    /* If EEPROM fails, set no EXT_PA by default */
    S2LP_ManagementSetRangeExtender(RANGE_EXT_NONE);
    return;
  }

  switch(tmpBuffer[1]) {
  case 0:
    s_RfXtalFrequency = 24000000;
    break;
  case 1:
    s_RfXtalFrequency = 25000000;
    break;
  case 2:
    s_RfXtalFrequency = 26000000;
    break;
  case 3:
    s_RfXtalFrequency = 48000000;
    break;
  case 4:
    s_RfXtalFrequency = 50000000;
    break;
  case 5:
    s_RfXtalFrequency = 52000000;
    break;
  default:
    s_RfXtalFrequency=S2LP_ManagementComputeXtalFrequency();

    break;
  }

  s_RfModuleBand = tmpBuffer[3];

  CUSTOM_S2LP_EEPROM_ReadPage(0, 0x0021,4,tmpBuffer);

  for(uint8_t i=0;i<4;i++)
  {
    ((uint8_t*)&foffset)[i]=tmpBuffer[3-i];
  }

  xtal_comp_value = 0;

  /* foffset is a value measured during manufacturing as follows:
  *
  * foffset = fnominal-fmeasured.
  * To compensate such value it should be reported to xtal freq
  * and then subtracted
  *
  */
  if (foffset != 0xFFFFFFFF) {
    uint32_t frequency = S2LP_GetFrequencyBand();

    if (frequency != 0)
    {
	uint32_t xtal_frequency = s_RfXtalFrequency;

	/* This is the value to be added to the xtal nominal value
	to compensate the xtal offset */
	xtal_comp_value = (int32_t) ((xtal_frequency*(-foffset))/frequency);
    }
  }

  S2LP_RADIO_SetXtalFrequency(s_RfXtalFrequency+xtal_comp_value);

  S2LP_ManagementSetRangeExtender((RangeExtType)tmpBuffer[5]);

  if(was_in_sdn==0)
  {
	  CUSTOM_S2LP_RADIO_ExitShutdown();
  }
#else
  S2LP_RADIO_SetXtalFrequency(s_RfXtalFrequency);
#endif /*EEPROM_PRESENT*/
}
/*----------------------------------------------------------------------------*/
/**
* @brief  Set the S2LP range extender type
* @param  RangeExtType
* @retval None
*/
void S2LP_ManagementSetRangeExtender(RangeExtType value)
{
  s_RfRangeExtender = value;
}
/*----------------------------------------------------------------------------*/
/**
* @brief  returns the S2LP range extender type
* @param  None
* @retval RangeExtType
*/
RangeExtType S2LP_ManagementGetRangeExtender(void)
{
  return s_RfRangeExtender;
}
/*----------------------------------------------------------------------------*/

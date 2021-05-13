/**
 * @file    S2LP_Qi.c
 * @author  LowPower RF BU - AMG - SRA for ODE release
 * @version 1.3.2
 * @date    13-October-2020
 * @brief   Configuration and management of S2-LP QI.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
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
 * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
 */


/* Includes ------------------------------------------------------------------*/
#include "s2lp_qi.h"
#include "s2lp.h"


/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @addtogroup S2LP_Qi
 * @{
 */


/**
 * @defgroup Qi_Private_TypesDefinitions        QI Private Types Definitions
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Qi_Private_Defines                 QI Private Defines
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Qi_Private_Macros                  QI Private Macros
 * @{
 */
#define RADIO_QI_RegToRssidBm(VAL)    (VAL - 146)
#define RADIO_QI_RssidBmToReg(VAL)    (uint8_t)(VAL+146)
/**
 *@}
 */


/**
 * @defgroup Qi_Private_Variables               QI Private Variables
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Qi_Private_FunctionPrototypes      QI Private Function Prototypes
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Qi_Private_Functions               QI Private Functions
 * @{
 */

/**
 * @brief  Returns the RSSI value. 
 * @param  None.
 * @retval int32_t RSSI value.
 */
int32_t S2LP_RADIO_QI_GetRssidBm(void)
{
  uint8_t tmp;
  g_xStatus = S2LP_ReadRegister(RSSI_LEVEL_ADDR, 1, &tmp);  
  return RADIO_QI_RegToRssidBm((int32_t)tmp);
}

/**
 * @brief  Returns the RSSI value from a current RX mode.
 * @param  None.
 * @retval uint8_t RSSI value.
 */
int32_t S2LP_RADIO_QI_GetRssidBmRun(void)
{
  uint8_t tmpBuffer[2];
  
  /*The first value is the previous one, so the first read operation clear this value, 
               than it is read the actual during RX. */
  g_xStatus = S2LP_ReadRegister(RSSI_LEVEL_RUN_ADDR, 2, tmpBuffer);  
  
  return RADIO_QI_RegToRssidBm((int32_t)tmpBuffer[1]);
}



/**
 * @brief  Set the RSSI threshold in dBm.
 * @param  rssiThrehsold in dBm.
 * @retval None.
 */
void S2LP_RADIO_QI_SetRssiThreshdBm(int32_t wRssiThrehsold)
{
  uint8_t tmp;
  
  tmp = RADIO_QI_RssidBmToReg(wRssiThrehsold);
  g_xStatus = S2LP_WriteRegister(RSSI_TH_ADDR, 1, &tmp);  
}



/**
* @brief  Initialize the RSSI measurement.
* @param  Pointer to the SRssiInit struct to use fot the initialization.
* @retval None.
*/
void S2LP_RADIO_QI_RssiInit(SRssiInit* xSRssiInit)
{
  uint8_t tmpBuffer[2];

  /* Reads the PCKT_FLT_OPTIONS rrgister */
  S2LP_ReadRegister(RSSI_FLT_ADDR, 1, &tmpBuffer[0]);
  
  /* Enables or disables filtering on my address */
  tmpBuffer[0] &= ~(RSSI_FLT_REGMASK | CS_MODE_REGMASK);
  tmpBuffer[0] |= (xSRssiInit->cRssiFlt<<4);
  tmpBuffer[0] |= (xSRssiInit->xRssiMode<<2);
  
  tmpBuffer[1] = RADIO_QI_RssidBmToReg(xSRssiInit->cRssiThreshdBm);
    
  g_xStatus = S2LP_WriteRegister(RSSI_FLT_ADDR, 2, tmpBuffer);
}


/**
* @brief  Return the RSSI measurements information to be filled with the information.
* @param  Pointer to the SRssiInit struct.
*               It will be filled with RSSI Mode, RSSI filter gain value and RSSI Threshold.
* @retval None.
*/
void S2LP_RADIO_QI_GetRssiInfo(SRssiInit* xSRssiInit)
{
  uint8_t tmpBuffer[2];

  S2LP_ReadRegister(RSSI_FLT_ADDR, 2, tmpBuffer);
  
  xSRssiInit->xRssiMode = (SRssiMode)((tmpBuffer[0]&CS_MODE_REGMASK)>>2);
  xSRssiInit->cRssiFlt = (tmpBuffer[0]&RSSI_FLT_REGMASK)>>4;
  xSRssiInit->cRssiThreshdBm = RADIO_QI_RegToRssidBm(tmpBuffer[1]);
  
}


/**
* @brief  Enable or disables the CS blanking.
* @param  xCsBlank enable or disable.
* @retval None.
*/
void S2LP_RADIO_QI_CsBlanking(SFunctionalState xCsBlank)
{
  uint8_t tmp;
  
  S2LP_ReadRegister(ANT_SELECT_CONF_ADDR, 1, &tmp);
  
  if(xCsBlank == S_ENABLE) {
    tmp |= CS_BLANKING_REGMASK;
  } else {
    tmp &= ~CS_BLANKING_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(ANT_SELECT_CONF_ADDR, 1, &tmp);
  
}


/**
* @brief  Enable or Disable the antenna switching.
* @param  xAntennaSwitch enable or disable.
* @retval None.
*/
void S2LP_RADIO_QI_AntennaSwitching(SFunctionalState xAntennaSwitch)
{
  uint8_t tmp;
  
  S2LP_ReadRegister(ANT_SELECT_CONF_ADDR, 1, &tmp);
  
  if(xAntennaSwitch == S_ENABLE) {
    tmp |= AS_ENABLE_REGMASK;
  } else {
    tmp &= ~AS_ENABLE_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(ANT_SELECT_CONF_ADDR, 1, &tmp);
  
}


/**
* @brief  Set the PQI threshold.
* @param  PQI_LEVEL.
* @retval None.
*/
void S2LP_RADIO_QI_SetPQIThreshold(uint8_t cPQIThreshold)
{
  uint8_t tmp;

  S2LP_ReadRegister(QI_ADDR, 1, &tmp);
  tmp &= ~PQI_TH_REGMASK;
  tmp |= (((uint8_t)cPQIThreshold)<<1);
  S2LP_WriteRegister(QI_ADDR, 1, &tmp);
  
}

/**
* @brief  Set the SQI threshold.
* @param  SQI_LEVEL.
* @retval None.
*/
void S2LP_RADIO_QI_SetSQIThreshold(uint8_t cSQIThreshold)
{
  uint8_t tmp;

  S2LP_ReadRegister(QI_ADDR, 1, &tmp);
  tmp &= ~SQI_TH_REGMASK;
  tmp |= (((uint8_t)cSQIThreshold)<<5);
  S2LP_WriteRegister(QI_ADDR, 1, &tmp);
}

/**
* @brief  Set the SQI enable.
* @param  SQI_ENABLE.
* @retval None.
*/
void S2LP_RADIO_QI_EnableSQI(SFunctionalState xSQIEnable)
{
  uint8_t tmp;

  S2LP_ReadRegister(QI_ADDR, 1, &tmp);
  tmp &= ~SQI_EN_REGMASK;
  tmp |= (((uint8_t)xSQIEnable));
  S2LP_WriteRegister(QI_ADDR, 1, &tmp);
}


/**
* @brief  Return the CS (carrier sense) indication.
* @param  None.
* @retval CS indication.
*/
SFlagStatus S2LP_RADIO_QI_GetCs(void)
{
  uint8_t tmp;

  S2LP_ReadRegister(LINK_QUALIF1_ADDR, 1, &tmp);
  tmp = (tmp&CS_REGMASK)>>7;

  return (SFlagStatus)tmp;
}

/**
 * @brief  Returns the LQI value.
 * @param  None.
 * @retval uint8_t LQI value.
 */
uint8_t S2LP_RADIO_QI_GetLqi(void)
{
  uint8_t tempRegValue;

  /* Reads the LINK_QUALIF0 register value */
  g_xStatus = S2LP_ReadRegister(LINK_QUALIF2_ADDR, 1, &tempRegValue);

  /* Rebuild and return the LQI value */
  return ((tempRegValue & 0xF0)>> 4);
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



/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/

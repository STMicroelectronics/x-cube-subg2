/**
 * @file    S2LP_Csma.c
 * @author  LowPower RF BU - AMG
 * @version 1.3.0
 * @date    10-July-2017
 * @brief   Configuration and management of S2-LP CSMA.
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
#include "s2lp_csma.h"
#include "s2lp.h"

/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @addtogroup S2LP_Csma
 * @{
 */


/**
 * @defgroup Csma_Private_TypesDefinitions      CSMA Private TypesDefinitions
 * @{
 */


/**
 *@}
 */


/**
 * @defgroup Csma_Private_Defines               CSMA Private Defines
 * @{
 */

/**
 *@}
 */

/**
 * @defgroup Csma_Private_Macros               CSMA Private Macros
 * @{
 */
#define IS_CCA_PERIOD(PERIOD)   (PERIOD == CSMA_PERIOD_64TBIT || \
				 PERIOD == CSMA_PERIOD_128TBIT || \
				 PERIOD == CSMA_PERIOD_256TBIT || \
				 PERIOD == CSMA_PERIOD_512TBIT)

#define IS_CSMA_LENGTH(LENGTH)	     (LENGTH < 16)

#define IS_BU_COUNTER_SEED(SEED)	   (SEED!=0)
#define IS_BU_PRESCALER(PRESCALER)	 (PRESCALER<64)
#define IS_CMAX_NB(NB)			         (NB<8)

/**
 *@}
 */


/**
 * @defgroup Csma_Private_Variables             CSMA Private Variables
 * @{
 */

/**
 *@}
 */



/**
 * @defgroup Csma_Private_FunctionPrototypes    CSMA Private FunctionPrototypes
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Csma_Private_Functions             CSMA Private Functions
 * @{
 */


/**
 * @brief  Initialize the S2LP CSMA according to the specified parameters in the SCsmaInit.
 * @param  pxCsmaInit Csma init structure.
 *         This parameter is a pointer to @ref SCsmaInit.
 * @retval None.
 */
void S2LP_CSMA_Init(SCsmaInit* pxCsmaInit)
{
  uint8_t tmpBuffer[4];

  /* CSMA BU counter seed (MSB) config */
  tmpBuffer[0] = (uint8_t)(pxCsmaInit->nBuCounterSeed >> 8);

  /* CSMA BU counter seed (LSB) config */
  tmpBuffer[1] = (uint8_t) pxCsmaInit->nBuCounterSeed;

  /* CSMA BU prescaler config and CCA period config */
  tmpBuffer[2] = (pxCsmaInit->cBuPrescaler << 2) | pxCsmaInit->xMultiplierTbit;

  /* CSMA CCA length config and max number of back-off */
  tmpBuffer[3] = (pxCsmaInit->xCcaLength<<4) | pxCsmaInit->cMaxNb;

  /* Reads the PROTOCOL1_BASE register value, to write the SEED_RELOAD and CSMA_PERS_ON fields */
  S2LP_WriteRegister(CSMA_CONF3_ADDR, 4, tmpBuffer);

  S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);
  
  /* Writes the new value for persistent mode */
  if(pxCsmaInit->xCsmaPersistentMode==S_ENABLE) {
    tmpBuffer[0] |= CSMA_PERS_ON_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~CSMA_PERS_ON_REGMASK;
  }

  /* Writes PROTOCOL1_BASE register */
  g_xStatus = S2LP_WriteRegister(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);


}


 /**
  * @brief  Return the fitted structure SCsmaInit starting from the registers values.
  * @param  pxCsmaInit Csma structure to be fitted.
  *         This parameter is a pointer to @ref SCsmaInit.
  * @retval None.
  */
void S2LP_CSMA_GetInfo(SCsmaInit* pxSCsmaInit)
{
   uint8_t tmpBuffer[4];

   /* Reads PROTOCOL1_BASE register */
   S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);

   /* Reads the persistent mode enable bit */
   pxSCsmaInit->xCsmaPersistentMode = (SFunctionalState)((tmpBuffer[0]&CSMA_PERS_ON_REGMASK) >> 1);

   /* Reads CSMA_CONFIGx_BASE registers */
   g_xStatus = S2LP_ReadRegister(CSMA_CONF3_ADDR, 4, tmpBuffer);

   /* Reads the bu counter seed */
   pxSCsmaInit->nBuCounterSeed = (((uint16_t)tmpBuffer[0]) << 8) | (uint16_t)tmpBuffer[1];

   /* Reads the bu prescaler */
   pxSCsmaInit->cBuPrescaler = tmpBuffer[2]>>2;

   /* Reads the Cca period */
   pxSCsmaInit->xMultiplierTbit = (SCsmaPeriod)(tmpBuffer[2] & CCA_PERIOD_REGMASK);

   /* Reads the Cca length */
   pxSCsmaInit->xCcaLength = (tmpBuffer[3]&CCA_LEN_REGMASK)>>4;

   /* Reads the max number of back off */
   pxSCsmaInit->cMaxNb = tmpBuffer[3] & NBACKOFF_MAX_REGMASK;

}


/**
 * @brief  Enable or Disables the CSMA.
 * @param  xNewState the state of the CSMA mode.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void S2LP_CSMA_Enable(SFunctionalState xNewState)
{
  uint8_t tmp, tmp2;
  
  S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    /* Set the CSMA enable bit */
    
    tmp |= CSMA_ON_REGMASK;
    
    S2LP_ReadRegister(PM_CONF0_ADDR, 1, &tmp2);
    tmp2 |= SLEEP_MODE_SEL_REGMASK;
    S2LP_WriteRegister(PM_CONF0_ADDR, 1, &tmp2);
  }
  else
  {
    S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);
    
    /* reSet the CSMA enable bit */
    tmp &= ~CSMA_ON_REGMASK;
  }
  
  
  g_xStatus = S2LP_WriteRegister(PROTOCOL1_ADDR, 1, &tmp);
}

/**
 * @brief  Gets the CSMA mode. Says if it is enabled or disabled.
 * @param  None.
 * @retval SFunctionalState: CSMA mode.
 */
SFunctionalState S2LP_CSMA_GetCsma(void)
{
  uint8_t tmp;

  /* Reads the PROTOCOL1 register value */
  g_xStatus = S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);

  /* Return if set or reset */
  if(tmp & CSMA_ON_REGMASK) {
    return S_ENABLE;
  }
  else {
    return S_DISABLE;
  }

}

/**
 * @brief  Enables or Disables the persistent CSMA mode.
 * @param  xNewState the state of the persistent CSMA mode.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void S2LP_CSMA_PersistentMode(SFunctionalState xNewState)
{
  uint8_t tmp;

  S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);

  /* Enables/disables the CSMA persistent mode */
  if(xNewState==S_ENABLE) {
    tmp |= CSMA_PERS_ON_REGMASK;
  }
  else {
    tmp &= ~CSMA_PERS_ON_REGMASK;
  }

  g_xStatus = S2LP_WriteRegister(PROTOCOL1_ADDR, 1, &tmp);

}


/**
 * @brief  Gets the persistent CSMA mode.
 * @param  None.
 * @retval SFunctionalState: CSMA persistent mode.
 */
SFunctionalState S2LP_CSMA_GetPersistentMode(void)
{
  uint8_t tmp;

  g_xStatus = S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);

  if(tmp & CSMA_PERS_ON_REGMASK) {
    return S_ENABLE;
  }
  else {
    return S_DISABLE;
  }

}


/**
 * @brief  Enables or Disables the seed reload mode (if enabled it reloads the back-off generator seed using the value written in the BU_COUNTER_SEED register).
 * @param  xNewState the state of the seed reload mode.
 *	   This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void S2LP_CSMA_SeedReloadMode(SFunctionalState xNewState)
{
  uint8_t tmp;

  S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);

  /* Enables/disables the seed reload mode */
  if(xNewState==S_ENABLE) {
    tmp |= SEED_RELOAD_REGMASK;
  }
  else {
    tmp &= ~SEED_RELOAD_REGMASK;
  }

  g_xStatus = S2LP_WriteRegister(PROTOCOL1_ADDR, 1, &tmp);

}


/**
 * @brief  Gets the seed reload mode.
 * @param  None.
 * @retval SFunctionalState: CSMA seed reload mode.
 */
SFunctionalState S2LP_CSMA_GetSeedReloadMode(void)
{
  uint8_t tmp;

  g_xStatus = S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);

  /* Return if set or reset */
  if(tmp & SEED_RELOAD_REGMASK) {
    return S_ENABLE;
  }
  else {
    return S_DISABLE;
  }
}


/**
 * @brief  Set the BU counter seed (BU_COUNTER_SEED register). The CSMA back off time is given by the formula: BO = rand(2^NB)*BU.
 * @param  nBuCounterSeed seed of the random number generator used to apply the BBE algorithm (binary exponential backoff) used to space out repeated retransmissions of the same block of data, often to avoid network congestion).
 *	   This parameter is an uint16_t.
 * @retval None.
 */
void S2LP_CSMA_SetBuCounterSeed(uint16_t nBuCounterSeed)
{
  uint8_t tmpBuffer[2];

  tmpBuffer[0] = (uint8_t)(nBuCounterSeed>>8);
  tmpBuffer[1] = (uint8_t)nBuCounterSeed;

  g_xStatus = S2LP_WriteRegister(CSMA_CONF3_ADDR, 2, tmpBuffer);

}

/**
 * @brief  Return the BU counter seed (BU_COUNTER_SEED register).
 * @param  None.
 * @retval uint16_t Seed of the random number generator used to apply the BBE algorithm.
 */
uint16_t S2LP_CSMA_GetBuCounterSeed(void)
{
  uint8_t tmpBuffer[2];

  g_xStatus = S2LP_ReadRegister(CSMA_CONF3_ADDR, 2, tmpBuffer);

  return ((((uint16_t)tmpBuffer[0])<<8) + (uint16_t)tmpBuffer[1]);
}


/**
 * @brief  Set the BU prescaler. The CSMA back off time is given by the formula: BO = rand(2^NB)*BU.
 * @param  cBuPrescaler used to program the back-off unit BU.
 * 	   This parameter is an uint8_t.
 * @retval None.
 */
void S2LP_CSMA_SetBuPrescaler(uint8_t cBuPrescaler)
{
  uint8_t tmp;

  S2LP_ReadRegister(CSMA_CONF1_ADDR, 1, &tmp);

  tmp &= ~BU_PRSC_REGMASK;
  tmp |= (cBuPrescaler<<2);

  g_xStatus = S2LP_WriteRegister(CSMA_CONF1_ADDR, 1, &tmp);
}


/**
 * @brief  Return the BU prescaler.
 * @param  None.
 * @retval uint8_t Value back-off unit (BU).
 */
uint8_t S2LP_CSMA_GetBuPrescaler(void)
{
  uint8_t tmp;

  g_xStatus = S2LP_ReadRegister(CSMA_CONF1_ADDR, 1, &tmp);

  return (tmp >> 2);
}


/**
 * @brief  Set the CCA period.
 * @param  xMultiplierTbit value of CCA period to store.
 * 	   This parameter can be a value of @ref CcaPeriod.
 * @retval None.
 */
void S2LP_CSMA_SetCcaPeriod(SCsmaPeriod xMultiplierTbit)
{
  uint8_t tmp;

  S2LP_ReadRegister(CSMA_CONF1_ADDR, 1, &tmp);

  tmp &= ~CCA_PERIOD_REGMASK;
  tmp |= xMultiplierTbit;

  g_xStatus = S2LP_WriteRegister(CSMA_CONF1_ADDR, 1, &tmp);

}


/**
 * @brief  Return the CCA period.
 * @param  None.
 * @retval CcaPeriod CCA period.
 */
uint8_t S2LP_CSMA_GetCcaPeriod(void)
{
  uint8_t tmp;

  g_xStatus = S2LP_ReadRegister(CSMA_CONF1_ADDR, 1, &tmp);

  return (SCsmaPeriod)(tmp & CCA_PERIOD_REGMASK);
}


/**
 * @brief  Set the CCA length.
 * @param  xCcaLength the CCA length (a value between 1 and 15 that multiplies the CCA period).
 *	   This parameter can be any value of @ref CsmaLength.
 * @retval None.
 */
void S2LP_CSMA_SetCcaLength(uint8_t xCcaLength)
{
  uint8_t tmp;

  S2LP_ReadRegister(CSMA_CONF0_ADDR, 1, &tmp);

  tmp &= ~CCA_LEN_REGMASK;
  tmp |= xCcaLength;

  g_xStatus = S2LP_WriteRegister(CSMA_CONF0_ADDR, 1, &tmp);

}


/**
 * @brief  Return the CCA length.
 * @param  None.
 * @retval uint8_t CCA length.
 */
uint8_t S2LP_CSMA_GetCcaLength(void)
{
  uint8_t tmp;

  g_xStatus = S2LP_ReadRegister(CSMA_CONF0_ADDR, 1, &tmp);

  return (tmp >> 4);
}


/**
 * @brief  Set the max number of back-off. If reached S2LP stops the transmission.
 * @param  cMaxNb the max number of back-off.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void S2LP_CSMA_SetMaxNumberBackoff(uint8_t cMaxNb)
{
  uint8_t tmp;

  S2LP_ReadRegister(CSMA_CONF0_ADDR, 1, &tmp);

  tmp &= ~NBACKOFF_MAX_REGMASK;
  tmp |= cMaxNb;

  g_xStatus = S2LP_WriteRegister(CSMA_CONF0_ADDR, 1, &tmp);
}

/**
 * @brief  Return the max number of back-off.
 * @param  None.
 * @retval uint8_t Max number of back-off.
 */
uint8_t S2LP_CSMA_GetMaxNumberBackoff(void)
{
  uint8_t tmp;

  g_xStatus = S2LP_ReadRegister(CSMA_CONF0_ADDR, 1, &tmp);

  return (tmp & NBACKOFF_MAX_REGMASK);

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

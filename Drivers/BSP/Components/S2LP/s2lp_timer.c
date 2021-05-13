/**
 * @file    S2LP_Timer.c
 * @author  LowPower RF BU - AMG
 * @version 1.3.0
 * @date    10-July-2017
 * @brief   Configuration and management of S2-LP timers.
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

#include "s2lp.h"

/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @addtogroup S2LP_Timer
 * @{
 */


/**
 * @defgroup Timer_Private_TypesDefinitions             Timer Private Types Definitions
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Timer_Private_Defines                      Timer Private Defines
 * @{
 */

#define LDC_FREQ_CLK            S2LP_TIM_GetRcoFrequency()


/**
 *@}
 */


/**
 * @defgroup Timer_Private_Macros                       Timer Private Macros
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Timer_Private_Variables                    Timer Private Variables
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup Timer_Private_FunctionPrototypes            Timer Private Function Prototypes
 * @{
 */

void S2LP_TIM_ComputeWakeupTimerValues(uint32_t* plWakeUpUsec , uint8_t cCounter , uint8_t cPrescaler, uint8_t cMulti);
void S2LP_TIM_ComputeRxTimerValues(uint32_t* plDesiredUsec, uint8_t pcCounter, uint8_t pcPrescaler);
void S2LP_TIM_ComputeRxTimerRegValues(uint32_t plDesiredUsec , uint8_t* pcCounter , uint8_t* pcPrescaler);
void S2LP_TIM_ComputeWakeupTimerRegValues(uint32_t plDesiredUsec , uint8_t* pcCounter , uint8_t* pcPrescaler, uint8_t* pcMulti);


/**
 *@}
 */


/**
 * @defgroup Timer_Private_Functions                    Timer Private Functions
 * @{
 */

/**
 * @brief  Computes the wake up timer.
 * @param  plWakeUpUsec: pointer to the variable where the wakeup timer (in microseconds) should be stored.
 * @param  cCounter: Counter
 * @param  cPrescaler: Prescaler.
 * @param  cMulti: Multiplier.
 * @retval None.
 */
void S2LP_TIM_ComputeWakeupTimerValues(uint32_t* plWakeUpUsec , uint8_t cCounter , uint8_t cPrescaler, uint8_t cMulti)
{
  (*plWakeUpUsec) = (uint32_t)((uint64_t)1000000*(cCounter+1)*(cPrescaler+1)/LDC_FREQ_CLK*cMulti);
}

/**
 * @brief  Computes the values of the wakeup timer counter and prescaler from the user time expressed in millisecond.
 *         The prescaler and the counter values are computed maintaining the prescaler value as
 *         small as possible in order to obtain the best resolution, and in the meantime minimizing the error.
 * @param  lDesiredUsec desired wakeup timeout in microseconds.
 *         Since the counter and prescaler are 8 bit registers the maximum
 *         reachable value is maxTime = fTclk x 256 x 256.
 * @param  pcCounter pointer to the variable in which the value for the wakeup timer counter has to be stored.
 *         This parameter must be a uint8_t*.
 * @param  pcPrescaler pointer to the variable in which the value for the wakeup timer prescaler has to be stored.
 *         This parameter must be an uint8_t*.
 * @retval None
 */
void S2LP_TIM_ComputeWakeupTimerRegValues(uint32_t lDesiredUsec, uint8_t* pcCounter , uint8_t* pcPrescaler, uint8_t* pcMulti)
{
  uint32_t n;
  uint8_t i;
  
  for(i=0;i<4;i++)
  {
    if(lDesiredUsec<(((uint32_t)((uint64_t)1000000*65536/LDC_FREQ_CLK))<<i))
      break;
  }
  (*pcMulti)=i;
  

  /* N cycles in the time base of the timer: 
     - clock of the timer is RCO frequency
     - divide times 1000000 more because we have an input in us
  */
  n = (uint32_t)((uint64_t)lDesiredUsec*(LDC_FREQ_CLK>>i)/1000000);
    
  /* check if it is possible to reach that target with prescaler and counter of S2LP */
  if(n/0xFF>0xFD) {
    /* if not return the maximum possible value */
    (*pcCounter) = 0xFF;
    (*pcPrescaler) = 0xFF;
    return;
  }
  
  /* prescaler is really 2 as min value */
  (*pcPrescaler) = (n/0xFF)+2;
  (*pcCounter) = n / (*pcPrescaler);
  
//  /* check if the error is minimum */
//  err = S_ABS((float)((*pcCounter)*(*pcPrescaler)/rco_freq)-fDesiredMsec);
//  
//  if((*pcCounter)<=254) {
//    if(S_ABS((float)((*pcCounter)+1)*(*pcPrescaler)/rco_freq-fDesiredMsec)<err)
//      (*pcCounter) = (*pcCounter)+1;
//  }
    
  /* decrement prescaler and counter according to the logic of this timer in S2LP */
  (*pcPrescaler)--;
  if((*pcCounter)>1)
    (*pcCounter)--;
  else
    (*pcCounter)=1;
}

/**
 * @brief  Computes the values of the rx_timeout timer counter and prescaler from the user time expressed in millisecond.
 *         The prescaler and the counter values are computed maintaining the prescaler value as
 *         small as possible in order to obtain the best resolution, and in the meantime minimizing the error.
 * @param  lDesiredUsec desired rx_timeout in microsecs.
 *         This parameter must be a float. Since the counter and prescaler are 8 bit registers the maximum
 *         reachable value is maxTime = fTclk x 255 x 255.
 * @param  pcCounter pointer to the variable in which the value for the rx_timeout counter has to be stored.
 *         This parameter must be a uint8_t*.
 * @param  pcPrescaler pointer to the variable in which the value for the rx_timeout prescaler has to be stored.
 *         This parameter must be an uint8_t*.
 * @retval None
 */
void S2LP_TIM_ComputeRxTimerRegValues(uint32_t lDesiredUsec , uint8_t* pcCounter , uint8_t* pcPrescaler)
{
  uint32_t f_dig = S2LP_RADIO_GetXtalFrequency();
  uint32_t n;
  uint64_t tgt,tgt1,tgt2;
  
  /* if xtal is doubled divide it by 2 */
  if(f_dig > S2LP_DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }
  
  /* N cycles in the time base of the timer: 
     - clock of the timer is f_dig/1210
     - divide times 1000000 more because we have an input in us
  */
  tgt=(uint64_t)lDesiredUsec*f_dig;
  n=(uint32_t)(tgt/1210000000);
  tgt1=(uint64_t)1210000000*n;
  tgt2=(uint64_t)1210000000*(n+1);
  
  n=((tgt2-tgt)<(tgt-tgt1))?(n+1):(n);
  
  /* check if it is possible to reach that target with prescaler and counter of S2LP */
  if(n/0xFF>0xFD) {
    /* if not return the maximum possible value */
    (*pcCounter) = 0xFF;
    (*pcPrescaler) = 0xFF;
    return;
  }
  
  /* prescaler is really 2 as min value */
  (*pcPrescaler)=(n/0xFF)+2;
  (*pcCounter) = n / (*pcPrescaler);
  
  
  /* decrement prescaler and counter according to the logic of this timer in S2LP */ 
  (*pcPrescaler)--;
  
  if((*pcCounter)==0)
    (*pcCounter)=1;
}

/**
 * @brief  Computes the values of the rx_timeout given the timer counter and prescaler.
 * @param  pulDesiredUsec pointer to rx_timeout in microsecs.
 *         Since the counter and prescaler are 8 bit registers the maximum
 *         reachable value is maxTime = fTclk x 255 x 255.
 * @param  cCounter counter for the rx_timeout.
 *         This parameter must be a uint8_t.
 * @param  cPrescaler prescaler for the rx_timeout.
 *         This parameter must be an uint8_t.
 * @retval None
 */
void S2LP_TIM_ComputeRxTimerValues(uint32_t* pulDesiredUsec, uint8_t cCounter, uint8_t cPrescaler)
{
  uint32_t f_dig = S2LP_RADIO_GetXtalFrequency();
  
  /* if xtal is doubled divide it by 2 */
  if(f_dig>S2LP_DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }
  
  (*pulDesiredUsec) = (uint32_t)((uint64_t)1000000*(cPrescaler+1)*cCounter*1210/f_dig);
  
}

/**
 *@}
 */


/**
 * @defgroup Timer_Public_Functions                    Timer Public Functions
 * @{
 */


/**
 * @brief  Enables or Disables the LDCR mode.
 * @param  xNewState new state for LDCR mode.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void S2LP_TIM_LdcrMode(SFunctionalState xNewState)
{
  uint8_t tmp;
  
  S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);
  if(xNewState==S_ENABLE) {
    tmp |= LDC_MODE_REGMASK;
  }
  else {
    tmp &= ~LDC_MODE_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(PROTOCOL1_ADDR, 1, &tmp);
}


/**
 * @brief  Enables or Disables the LDCR timer reloading with the value stored in the LDCR_RELOAD registers.
 * @param  xNewState new state for LDCR reloading.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void S2LP_TIM_LdcrAutoReload(SFunctionalState xNewState)
{
  uint8_t tmp;

  S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);
  if(xNewState==S_ENABLE) {
    tmp |= LDC_RELOAD_ON_SYNC_REGMASK;
  }
  else {
    tmp &= ~LDC_RELOAD_ON_SYNC_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(PROTOCOL1_ADDR, 1, &tmp);
}


/**
 * @brief  Return the LDCR timer reload bit.
 * @param  None.
 * @retval SFunctionalState: value of the reload bit.
 */
SFunctionalState S2LP_TIM_LdcrGetAutoReload(void)
{
  uint8_t tmp;
  g_xStatus = S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);
  return (SFunctionalState)((tmp & LDC_RELOAD_ON_SYNC_REGMASK)!=0);
}

/**
 * @brief  Set the RX timeout timer initialization registers with the values of COUNTER and PRESCALER according to the formula: Trx=PRESCALER*COUNTER*Tck.
 *         Remember that it is possible to have infinite RX_Timeout writing 0 in the RX_Timeout_Counter and/or RX_Timeout_Prescaler registers.
 * @param  cCounter value for the timer counter.
 *         This parameter must be an uint8_t.
 * @param  cPrescaler value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void S2LP_TIM_SetRxTimer(uint8_t cCounter , uint8_t cPrescaler)
{
  uint8_t tmpBuffer[2] = {cCounter, cPrescaler};
  g_xStatus = S2LP_WriteRegister(TIMERS5_ADDR, 2, tmpBuffer);
}


/**
 * @brief  Set the RX timeout timer counter and prescaler from the desired value in ms. it is possible to fix the RX_Timeout to
 *         a minimum value of 50.417us to a maximum value of about 3.28 s.
 * @param  lDesiredUsec desired timer value.
 *         This parameter must be a uint32_t.
 * @retval None
 */
void S2LP_TIM_SetRxTimerUs(uint32_t lDesiredUsec)
{
  uint8_t tmpBuffer[2];
  S2LP_TIM_ComputeRxTimerRegValues(lDesiredUsec , &tmpBuffer[0] , &tmpBuffer[1]);
  g_xStatus = S2LP_WriteRegister(TIMERS5_ADDR, 2, tmpBuffer);
}


/**
 * @brief  Set the RX timeout timer counter. If it is equal to 0 the timeout is infinite.
 * @param  cCounter value for the timer counter.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void S2LP_TIM_SetRxTimerCounter(uint8_t cCounter)
{
  g_xStatus = S2LP_WriteRegister(TIMERS5_ADDR, 1, &cCounter);
}


/**
 * @brief  Set the RX timeout timer prescaler. If it is equal to 0 the timeout is infinite.
 * @param  cPrescaler value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None
 */
void S2LP_TIM_SetRxTimerPrescaler(uint8_t cPrescaler)
{
  g_xStatus = S2LP_WriteRegister(TIMERS4_ADDR, 1, &cPrescaler);
}


/**
 * @brief  Return the RX timeout timer.
 * @param  plTimeoutUsec pointer to the variable in which the timeout expressed in microseconds has to be stored.
 *         If the returned value is 0, it means that the RX_Timeout is infinite.
 *         This parameter must be a uint32_t*.
 * @param  pcCounter pointer to the variable in which the timer counter has to be stored.
 *         This parameter must be an uint8_t*.
 * @param  pcPrescaler pointer to the variable in which the timer prescaler has to be stored.
 *         This parameter must be an uint8_t*.
 * @retval None.
 */
void S2LP_TIM_GetRxTimerUs(uint32_t* plTimeoutUsec, uint8_t* pcCounter , uint8_t* pcPrescaler)
{
  uint8_t tmpBuffer[2];
  
  g_xStatus = S2LP_ReadRegister(TIMERS5_ADDR, 2, tmpBuffer);
  
  (*pcCounter) = tmpBuffer[0];
  (*pcPrescaler) = tmpBuffer[1];
  S2LP_TIM_ComputeRxTimerValues(plTimeoutUsec, tmpBuffer[0], tmpBuffer[1]);
}

/**
 * @brief  Set the LDCR wake up timer initialization registers with the values of
 *         COUNTER and PRESCALER according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where
 *         Tck = 28.818 us. The minimum vale of the wakeup timeout is 28.818us (PRESCALER and
 *         COUNTER equals to 0) and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals
 *         to 255).
 * @param  cCounter value for the timer counter.
 *         This parameter must be an uint8_t.
 * @param  cPrescaler value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void S2LP_TIM_SetWakeUpTimer(uint8_t cCounter , uint8_t cPrescaler)
{
  uint8_t tmpBuffer[2] = {cPrescaler, cCounter};
  g_xStatus = S2LP_WriteRegister(TIMERS3_ADDR, 2, tmpBuffer);
}


/**
 * @brief  Set the LDCR wake up timer counter and prescaler from the desired value in ms,
 *         according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 *         The minimum vale of the wakeup timeout is 28.818us (PRESCALER and COUNTER equals to 0)
 *         and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals to 255).
 * @param  fDesiredMsec desired timer value.
 *         This parameter must be a float.
 * @retval None.
 */
void S2LP_TIM_SetWakeUpTimerUs(uint32_t lDesiredUsec)
{
  uint8_t tmpBuffer[2], multi, tmp;

  /* Computes counter and prescaler */
  S2LP_TIM_ComputeWakeupTimerRegValues(lDesiredUsec , &tmpBuffer[1] , &tmpBuffer[0], &multi);

  S2LP_ReadRegister(PROTOCOL2_ADDR, 1, &tmp);
  tmp &= ~LDC_TIMER_MULT_REGMASK;
  tmp |= multi;
  S2LP_WriteRegister(PROTOCOL2_ADDR, 1, &tmp);

  g_xStatus = S2LP_WriteRegister(TIMERS3_ADDR, 2, tmpBuffer);

}


/**
 * @brief  Set the LDCR wake up timer counter. Remember that this value is incresead by one in the Twu calculation.
 *         Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 * @param  cCounter value for the timer counter.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void S2LP_TIM_SetWakeUpTimerCounter(uint8_t cCounter)
{
  g_xStatus = S2LP_WriteRegister(TIMERS2_ADDR, 1, &cCounter);
}


/**
 * @brief  Set the LDCR wake up timer prescaler. Remember that this value is incresead by one in the Twu calculation.
 *         Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 * @param  cPrescaler value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void S2LP_TIM_SetWakeUpTimerPrescaler(uint8_t cPrescaler)
{
  g_xStatus = S2LP_WriteRegister(TIMERS3_ADDR, 1, &cPrescaler);
}


/**
 * @brief  Return the LDCR wake up timer, according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 * @param  plWakeUpUsec pointer to the variable in which the wake-up time expressed in milliseconds has to be stored.
 *         This parameter must be a uint32_t*.
 * @param  pcCounter pointer to the variable in which the timer counter has to be stored.
 *         This parameter must be an uint8_t*.
 * @param  pcPrescaler pointer to the variable in which the timer prescaler has to be stored.
 *         This parameter must be an uint8_t*.
 * @retval None.
 */
void S2LP_TIM_GetWakeUpTimerUs(uint32_t* plWakeUpUsec, uint8_t* pcCounter, uint8_t* pcPrescaler, uint8_t* pcMulti)
{
  uint8_t tmpBuffer[2], tmp;
  
  S2LP_ReadRegister(PROTOCOL2_ADDR, 1, &tmp);
  tmp &= LDC_TIMER_MULT_REGMASK;
  *pcMulti = tmp;

  g_xStatus = S2LP_ReadRegister(TIMERS3_ADDR, 2, tmpBuffer);
  *pcCounter = tmpBuffer[1];
  *pcPrescaler = tmpBuffer[0];

  S2LP_TIM_ComputeWakeupTimerValues(plWakeUpUsec, tmpBuffer[0], tmpBuffer[1], tmp);
}


/**
 * @brief  Set the LDCR wake up timer reloading registers with the values of
 *         COUNTER and PRESCALER according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where
 *         Tck = 28.818 us. The minimum vale of the wakeup timeout is 28.818us (PRESCALER and
 *         COUNTER equals to 0) and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals
 *         to 255).
 * @param  cCounter reload value for the timer counter.
 *         This parameter must be an uint8_t.
 * @param  cPrescaler reload value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None.
 */
void S2LP_TIM_SetWakeUpTimerReload(uint8_t cCounter , uint8_t cPrescaler, uint8_t cMulti)
{
  uint8_t tmpBuffer[2] = {cPrescaler, cCounter}, tmp;

  S2LP_ReadRegister(PROTOCOL2_ADDR, 1, &tmp);
  tmp &= (~LDC_TIMER_MULT_REGMASK);
  tmp |= cMulti;
  S2LP_WriteRegister(PROTOCOL2_ADDR, 1, &tmp);

  g_xStatus = S2LP_WriteRegister(TIMERS1_ADDR, 2, tmpBuffer);
}


/**
 * @brief  Set the LDCR wake up reload timer counter and prescaler from the desired value in ms,
 *         according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 *         The minimum vale of the wakeup timeout is 28.818us (PRESCALER and COUNTER equals to 0)
 *         and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals to 255).
 * @param  lDesiredUsec desired timer value in us.
 * @retval None.
 */
void S2LP_TIM_SetWakeUpTimerReloadUs(uint32_t lDesiredUsec)
{
  uint8_t tmpBuffer[2], multi, tmp;

  /* Computes counter and prescaler */
  S2LP_TIM_ComputeWakeupTimerRegValues(lDesiredUsec , &tmpBuffer[1] , &tmpBuffer[0], &multi);

  S2LP_ReadRegister(PROTOCOL2_ADDR, 1, &tmp);
  tmp &= ~LDC_TIMER_MULT_REGMASK;
  tmp |= multi;
  S2LP_WriteRegister(PROTOCOL2_ADDR, 1, &tmp);

  g_xStatus = S2LP_WriteRegister(TIMERS1_ADDR, 2, tmpBuffer);
}


/**
 * @brief  Set the LDCR wake up timer reload counter. Remember that this value is incresead by one in the Twu calculation.
 *         Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 * @param  cCounter value for the timer counter.
 *         This parameter must be an uint8_t.
 * @retval None
 */
void S2LP_TIM_SetWakeUpTimerReloadCounter(uint8_t cCounter)
{
  g_xStatus = S2LP_WriteRegister(TIMERS0_ADDR, 1, &cCounter);
}


/**
 * @brief  Set the LDCR wake up timer reload prescaler. Remember that this value is incresead by one in the Twu calculation.
 *         Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 * @param  cPrescaler value for the timer prescaler.
 *         This parameter must be an uint8_t.
 * @retval None
 */
void S2LP_TIM_SetWakeUpTimerReloadPrescaler(uint8_t cPrescaler)
{
  g_xStatus = S2LP_WriteRegister(TIMERS1_ADDR, 1, &cPrescaler);
}


/**
 * @brief  Return the LDCR wake up reload timer, according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 * @param  plWakeUpReloadUsec pointer to the variable in which the wake-up reload time expressed in milliseconds has to be stored.
 *         This parameter must be a uint32_t*.
 * @param  pcCounter pointer to the variable in which the timer counter has to be stored.
 *         This parameter must be an uint8_t*.
 * @param  pcPrescaler pointer to the variable in which the timer prescaler has to be stored.
 *         This parameter must be an uint8_t*.
 * @retval None.
 */
void S2LP_TIM_GetWakeUpTimerReloadUs(uint32_t* plWakeUpReloadUsec, uint8_t* pcCounter, uint8_t* pcPrescaler, uint8_t* pcMulti)
{
  uint8_t tmpBuffer[2], tmp;
  
  S2LP_ReadRegister(PROTOCOL2_ADDR, 1, &tmp);
  tmp &= LDC_TIMER_MULT_REGMASK;
  *pcMulti = tmp;

  g_xStatus = S2LP_ReadRegister(TIMERS3_ADDR, 2, tmpBuffer);
  *pcCounter = tmpBuffer[1];
  *pcPrescaler = tmpBuffer[0];

  S2LP_TIM_ComputeWakeupTimerValues(plWakeUpReloadUsec, tmpBuffer[1], tmpBuffer[0], tmp);
}


/**
 * @brief  Set the RX timeout stop conditions.
 * @param  xStopCondition new stop condition.
 *         This parameter can be any value of @ref RxTimeoutStopCondition.
 * @retval None
 */
void S2LP_TIM_SetRxTimerStopCondition(RxTimeoutStopCondition xStopCondition)
{
  uint8_t tmp;
 
  S2LP_ReadRegister(PROTOCOL2_ADDR, 1, &tmp);
  tmp &= ~(CS_TIMEOUT_MASK_REGMASK | SQI_TIMEOUT_MASK_REGMASK | PQI_TIMEOUT_MASK_REGMASK);
  tmp |= (((uint8_t)xStopCondition) << 5);
  S2LP_WriteRegister(PROTOCOL2_ADDR, 1, &tmp);

  S2LP_ReadRegister(PCKT_FLT_OPTIONS_ADDR, 1, &tmp);
  tmp &= ~RX_TIMEOUT_AND_OR_SEL_REGMASK;
  tmp |= (((uint8_t)xStopCondition) >> 1);
  g_xStatus = S2LP_WriteRegister(PCKT_FLT_OPTIONS_ADDR, 1, &tmp);
}


/**
 * @brief  Computes and Return the RCO frequency. 
 *         This frequency depends on the xtal frequency and the XTAL bit in register 0x01.
 * @retval RCO frequency in Hz as an uint16_t.
 */
uint16_t S2LP_TIM_GetRcoFrequency(void)
{
  uint32_t xtal=S2LP_RADIO_GetXtalFrequency();
  
  switch(xtal)
  {
  case 24000000:
  case 48000000:
    return 32000;
  case 25000000:
  case 50000000:
    return 33300;
  }
  return 34700;
}


/**
 * @brief  Enables the Fast RX termination timer.
 * @param  None
 * @retval None
 */
void S2LP_TIM_FastRxTermTimer(SFunctionalState xNewState)
{
  uint8_t tmp;

  S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= FAST_CS_TERM_EN_REGMASK;
  }
  else {
    tmp &= ~FAST_CS_TERM_EN_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(PROTOCOL1_ADDR, 1, &tmp);  
}


/**
 * @brief  Set the Fast RX termination timer word. When the timer counter will reach this word,
 *              the timer expires.
 *             The timer counter is clocked at frequency: fCLK/24/2^CHFLT_E  
 * @param  fast_rx_word : the FAST_RX_TIMER word (register 0x54 value).
 * @retval None
 */
void S2LP_TIM_SetFastRxTermTimer(uint8_t fast_rx_word)
{
  g_xStatus = S2LP_WriteRegister(FAST_RX_TIMER_ADDR, 1, &fast_rx_word);  
}


/**
 * @brief  Set the Fast RX termination timer word starting from a us value.
 *             The timer counter is clocked at frequency: fCLK/24/2^CHFLT_E.
 * @param  fast_rx_us : fast rx termination target value in us.
 * @retval None
 * @note: the user should care about the max value that can be set unless asserts are used.
 */
void S2LP_TIM_SetFastRxTermTimerUs(uint32_t fast_rx_us)
{
  uint8_t tmp,fast_rx_word;
  
  S2LP_ReadRegister(CHFLT_ADDR, 1, &tmp);
    
  uint32_t f_dig=S2LP_RADIO_GetXtalFrequency();
  if(f_dig > S2LP_DIG_DOMAIN_XTAL_THRESH) {
    f_dig = f_dig/2;
  }
       
  fast_rx_word=((f_dig/24/(1<<(tmp&0x0F)))*fast_rx_us)/1000000;
  g_xStatus = S2LP_WriteRegister(FAST_RX_TIMER_ADDR, 1, &fast_rx_word);  
}


/**
 * @brief  Enables the RCO autocalibration when the device make the transition READY -> SLEEP.
 * @param  en:
 *              if it is S_ENABLE: RCO calibration is enabled.
 *              if it is S_DISABLE: RCO calibration is disabled.
 * @retval None
 */
void S2LP_TIM_CalibrationRco(SFunctionalState xCalibration)
{
  uint8_t tmp;
  
  S2LP_ReadRegister(XO_RCO_CONF0_ADDR, 1, &tmp);
  
  if(xCalibration == S_ENABLE) {
    tmp |= RCO_CALIBRATION_REGMASK;
  } else {
    tmp &= ~RCO_CALIBRATION_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(XO_RCO_CONF0_ADDR, 1, &tmp);  
}

/**
 * @brief  Enable the SLEEP_B mode. SLEEP_A and SLEEP_B are mutually exclusive.
 * @param  en:
 *              if it is S_ENABLE: SLEEP_B will be set
 *              if it is S_DISABLE: SLEEP_A will be set
 * @retval None
 */
void S2LP_TIM_SleepB(SFunctionalState en)
{
  uint8_t tmp;
  
  
  S2LP_ReadRegister(PM_CONF0_ADDR, 1, &tmp);
  
  if(en == S_ENABLE) {
    tmp |= SLEEP_MODE_SEL_REGMASK;
  } else {
    tmp &= ~SLEEP_MODE_SEL_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(PM_CONF0_ADDR, 1, &tmp); 
}

/**
 * @brief  TBD
 * @param  TBD
 * @retval TBD
 */
void S2LP_TIM_LdcIrqWa(SFunctionalState en)
{
  uint8_t tmp[2]={0x00,0x60};

  if(en)
  {
    tmp[0]=0x01; tmp[1]=0x64;
    
    do
    {
      S2LP_RefreshStatus();
    }
    while(g_xStatus.MC_STATE!=0x7C);
  }

  g_xStatus = S2LP_WriteRegister(0x7B, 1, &tmp[1]); 
  g_xStatus = S2LP_WriteRegister(0x7A, 1, &tmp[0]); 
}

/**
 * @brief  TBD
 * @param  TBD
 * @retval TBD
 */
void S2LP_TIM_SetRxTimerMs(float fDesiredMsec)
{
  S2LP_TIM_SetRxTimerUs((uint32_t)(fDesiredMsec*1000));
}

/**
 * @brief  TBD
 * @param  TBD
 * @retval TBD
 */
void S2LP_TIM_GetRxTimer(float* pfTimeoutMsec, uint8_t* pcCounter , uint8_t* pcPrescaler)
{
  uint32_t timeoutUsec;
  
  S2LP_TIM_GetRxTimerUs(&timeoutUsec, pcCounter , pcPrescaler);
    
  (*pfTimeoutMsec)=((float)timeoutUsec)/1000;
}

/**
 * @brief  TBD
 * @param  TBD
 * @retval TBD
 */
void S2LP_TIM_SetWakeUpTimerMs(float fDesiredMsec)
{
  S2LP_TIM_SetWakeUpTimerUs((uint32_t)(fDesiredMsec*1000));
}

/**
 * @brief  TBD
 * @param  TBD
 * @retval TBD
 */
void S2LP_TIM_SetWakeUpTimerReloadMs(float fDesiredMsec)
{
  S2LP_TIM_SetWakeUpTimerReloadUs((uint32_t)(fDesiredMsec*1000));
}

/**
 * @brief  TBD
 * @param  TBD
 * @retval TBD
 */
void S2LP_TIM_GetWakeUpTimer(float* pfWakeUpMsec, uint8_t* pcCounter , uint8_t* pcPrescaler, uint8_t* pcMulti)
{
  uint32_t timeoutUsec;
  
  S2LP_TIM_GetWakeUpTimerUs(&timeoutUsec, pcCounter , pcPrescaler, pcMulti);
    
  (*pfWakeUpMsec)=((float)timeoutUsec)/1000;
}

/**
 * @brief  TBD
 * @param  TBD
 * @retval TBD
 */
void S2LP_TIM_GetWakeUpTimerReload(float* pfWakeUpReloadMsec, uint8_t* pcCounter, uint8_t* pcPrescaler, uint8_t* pcMulti)
{
  uint32_t timeoutUsec;
  
  S2LP_TIM_GetWakeUpTimerReloadUs(&timeoutUsec, pcCounter , pcPrescaler, pcMulti);
    
  (*pfWakeUpReloadMsec)=((float)timeoutUsec)/1000;
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

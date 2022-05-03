/**
* @file    SDK_UTILS_Timers.c
* @author  LowPower RF BU - AMG
* @version 4.1.0
* @date    September, 2019
* @brief   SDK UTILS timers configuration.
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
#include "SDK_UTILS_Timers.h"
#include "Platform_Configuration_CLI.h"

uint32_t SdkGetCurrentSysTick(void);

/**
* @addtogroup SDK_EVAL_NUCLEO
* @{
*/

/**
* @addtogroup SDK_EVAL_Timers
* @{
*/

/**
* @defgroup SDK_EVAL_Timers_Private_TypesDefinitions           SDK EVAL Timers Private Types Definitions
* @{
*/

/**
*@}
*/

/**
* @defgroup SDK_EVAL_Timers_Private_Defines                    SDK EVAL Timers Private Defines
* @{
*/

/**
* @brief  Absolute value macro.
* @param  x: Value on which apply the abs function.
* @retval None
*/
#define ABS(x)  (x>0?x:-x)

/**
*@}
*/

/**
* @defgroup SDK_EVAL_Timers_Private_Macros                     SDK EVAL Timers Private Macros
* @{
*/

/**
*@}
*/

/**
* @defgroup SDK_EVAL_Timers_Private_Variables                  SDK EVAL Timers Private Variables
* @{
*/
volatile uint32_t lSystickCounter = 0;
volatile uint8_t xApplTimerIRQRaised = 0;
static TIMER_InfoType TimerAppliInfo;

/**
*@}
*/

/**
* @defgroup SDK_EVAL_Timers_Private_FunctionPrototypes         SDK EVAL Timers Private Function Prototypes
* @{
*/

/**
*@}
*/

/**
* @defgroup SDK_EVAL_Timers_Private_Functions                  SDK EVAL Timers Private Functions
* @{
*/
static void appliTimerCallback(uint32_t time)
{
  xApplTimerIRQRaised=1;
}

/**
* @brief  Computes two integer value prescaler and period such that Cycles = prescaler * period.
* @param  lCycles the specified cycles for the desired timer value.
* @param  pnPrescaler prescaler factor.
* @param  pnCounter period factor.
* @retval None.
*/
void SdkEvalTimersFindFactors(uint32_t lCycles, uint16_t *pnPrescaler, uint16_t *pnCounter)
{
  uint16_t b0;
  uint16_t a0;
  long err, err_min=lCycles;

  *pnPrescaler = a0 = ((lCycles-1)/0xffff) + 1;
  *pnCounter = b0 = lCycles / *pnPrescaler;

  for (; *pnPrescaler < 0xffff-1; (*pnPrescaler)++) {
    *pnCounter = lCycles / *pnPrescaler;
    err = (long)*pnPrescaler * (long)*pnCounter - (long)lCycles;
    if (ABS(err) > (*pnPrescaler / 2)) {
      (*pnCounter)++;
      err = (long)*pnPrescaler * (long)*pnCounter - (long)lCycles;
    }
    if (ABS(err) < ABS(err_min)) {
      err_min = err;
      a0 = *pnPrescaler;
      b0 = *pnCounter;
      if (err == 0) break;
    }
  }

  *pnPrescaler = a0;
  *pnCounter = b0;
}

/**
 * @brief  Enables or Disables a specific Timer with its IRQ.
 * @param  timerID: timer to be set.
 *          This parameter can be a pointer to @ref TIM_TypeDef
 * @param  enable: specifies if a timer has to be enabled or disabled.
 *
 * @retval The timeout value
 */
/*Original prototype has been modified in order to avoid to enable TIM (not used by GUI) via STM32CubeMX */
/* uint32_t SdkEvalTimersState(TimerID_Type timerID, TIM_HandleTypeDef *tim, uint8_t enable) */
uint32_t SdkEvalTimersState(TimerID_Type timerID, void *legacy_tim, uint8_t enable)
{
  uint32_t nRet = 0;
  UNUSED(legacy_tim);
  switch(timerID)
  {
  case APPL_TIMER_ID:
    {
	if (enable) {
	  TimerAppliInfo.enabled = 0;
	  TimerAppliInfo.timeout = SdkGetCurrentSysTick() + TimerAppliInfo.delay;
	  nRet = TimerAppliInfo.timeout;
	  TimerAppliInfo.callback = appliTimerCallback;
	  TimerAppliInfo.enabled = 1;
	}
	else {
	  TimerAppliInfo.enabled = 0;
	}

	break;
    }
/*  case GP_TIMER_ID:
    {
	((uint8_t)enable)?(HAL_TIM_Base_Start_IT(tim)):(HAL_TIM_Base_Stop_IT(tim));
	break;
    }*/
  default:
	break;
  }

  return nRet;
}

/**
 * @brief  Resets the counter of a specific timer.
 * @param  timerID: timer to be reset.
 *          This parameter can be a pointer to @ref TimerID_Type
 * @retval None
 */
void SdkEvalTimersResetCounter(TimerID_Type timerID) { }

void SdkEvalTimerTimestampReset(void)
{
  lSystickCounter = 0;
  SysTick->VAL = 0;
}

void SdkEvalTimersTimConfig_ms(TimerID_Type timerID, uint16_t ms)
{
  TimerAppliInfo.enabled = 0;
  TimerAppliInfo.delay = ms;
}

void SdkEvalUpdateTimerValue(void) { }

uint32_t SdkEvalGetTimerValue(void)
{
  SysTick->CTRL;
  uint32_t reload, ticks;

  do {
    reload = lSystickCounter;
    ticks = SysTick->VAL;
  } while (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk);

  return (reload*1000+(SysTick->LOAD-ticks)/CLOCK_FREQUENCY_MHZ);
}

__weak void SysTickUserAction(uint32_t counter) { }

/**
* @brief  This function handles SysTick Handler.
* @param  None
* @retval None
*/
void HAL_IncTick(void)
{
  uwTick += uwTickFreq;

  lSystickCounter++;
  if (TimerAppliInfo.enabled && (TimerAppliInfo.timeout == lSystickCounter)) {
    TimerAppliInfo.callback(lSystickCounter);
    TimerAppliInfo.timeout = lSystickCounter + TimerAppliInfo.delay;
  }
  SysTickUserAction(lSystickCounter);
}
/**
* @brief  This function implements return the current
*         systick with a step of 1 ms.
* @param  lTimeMs desired delay expressed in ms.
* @retval None
*/
uint32_t SdkGetCurrentSysTick(void)
{
  return lSystickCounter;
}

void SdkStartSysTick(void)
{
  SysTick_Config(CLOCK_FREQUENCY);
  NVIC_SetPriority (SysTick_IRQn, 0); /* set Priority for Systick Interrupt */
  lSystickCounter = 0;
}

/**
* @brief  This function implements a delay using the microcontroller
*         Systick with a step of 1 ms.
* @param  lTimeMs desired delay expressed in ms.
* @retval None
*/
void SdkDelayMs(volatile uint32_t lTimeMs)
{
  uint32_t nWaitPeriod = ~lSystickCounter;

  if(nWaitPeriod<lTimeMs) {
    while(lSystickCounter != 0xFFFFFFFF);
    nWaitPeriod = lTimeMs-nWaitPeriod;
  }
  else
    nWaitPeriod = lTimeMs+(~nWaitPeriod);

  while( lSystickCounter != nWaitPeriod );
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

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

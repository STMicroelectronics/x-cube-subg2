/**
 * @file    S2LP_Csma.h
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
*
* <b>Example:</b>
* @code
*
* CsmaInit csmaInit={
*   S_DISABLE,         // persistent mode
*   TBIT_TIME_64,      // Tbit time
*   TCCA_TIME_3,       // Tcca time
*   5,                 // max number of backoffs
*   0xFA21,            // BU counter seed
*   32                 // CU prescaler
* };
*
* ...
*
* S2LP_CSMA_Init(&csmaInit);
* S2LP_CSMA_(S_ENABLE);
*
*
* @endcode
*
* @note The CS status depends of the RSSI threshold set. Please see the S2LP_Qi
* module for details.
* <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __S2LP_CSMA_H
#define __S2LP_CSMA_H


/* Includes ------------------------------------------------------------------*/
#include "s2lp_types.h"
#include "s2lp_regs.h"


#ifdef __cplusplus
 extern "C" {
#endif


/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @defgroup S2LP_Csma        CSMA
 * @brief Configuration and management of S2LP CSMA.
 * @details See the file <i>@ref S2LP_Csma.h</i> for more details.
 * @{
 */

/**
 * @defgroup Csma_Exported_Types        CSMA Exported Types
 * @{
 */


/**
 * @brief  Multiplier for Tcca time enumeration (Tcca = Multiplier*Tbit).
 */
   
typedef enum {
  CSMA_PERIOD_64TBIT,      /*!< CSMA/CA: Sets CCA period to 64*TBIT */
  CSMA_PERIOD_128TBIT,    /*!< CSMA/CA: Sets CCA period to 128*TBIT */
  CSMA_PERIOD_256TBIT,    /*!< CSMA/CA: Sets CCA period to 256*TBIT */
  CSMA_PERIOD_512TBIT    /*!< CSMA/CA: Sets CCA period to 512*TBIT */
}SCsmaPeriod;


/**
  * @brief  S2LP CSMA Init structure definition
  */
typedef struct {
  SFunctionalState  xCsmaPersistentMode;    /*!< Enable the CSMA persistent mode */
  SCsmaPeriod       xMultiplierTbit;        /*!< Set the Tbit multiplier to obtain the Tcca. @ref CcaPeriod */
  uint8_t           xCcaLength;             /*!< Set the Tcca multiplier to determinate the Tlisten. From 0 to 15. */
  uint8_t           cMaxNb;                 /*!< Specifies the max number of backoff cycles. From 0 to 7. */
  uint16_t          nBuCounterSeed;         /*!< Specifies the BU counter seed. */
  uint8_t           cBuPrescaler;           /*!< Specifies the BU prescaler. From 0 to 63. */
} SCsmaInit;


/**
 *@}
 */


/**
 * @defgroup Csma_Exported_Constants    CSMA Exported Constants
 * @{
 */

/**
 * @defgroup Csma_Parameters            CSMA Parameters
 * @{
 */


/**
 *@}
 */

/**
 *@}
 */


/**
 * @defgroup Csma_Exported_Macros       CSMA Exported Macros
 * @{
 */


/**
 *@}
 */


/**
 * @defgroup Csma_Exported_Functions    CSMA Exported Functions
 * @{
 */

void S2LP_CSMA_Init(SCsmaInit* pxSCsmaInit);
void S2LP_CSMA_GetInfo(SCsmaInit* pxSCsmaInit);
void S2LP_CSMA_Enable(SFunctionalState xNewState);
SFunctionalState S2LP_CSMA_GetCsma(void);
void S2LP_CSMA_PersistentMode(SFunctionalState xNewState);
SFunctionalState S2LP_CSMA_GetPersistentMode(void);
void S2LP_CSMA_SeedReloadMode(SFunctionalState xNewState);
SFunctionalState S2LP_CSMA_GetSeedReloadMode(void);
void S2LP_CSMA_SetBuCounterSeed(uint16_t nBuCounterSeed);
uint16_t S2LP_CSMA_GetBuCounterSeed(void);
void S2LP_CSMA_SetBuPrescaler(uint8_t cBuPrescaler);
uint8_t S2LP_CSMA_GetBuPrescaler(void);
void S2LP_CSMA_SetCcaPeriod(SCsmaPeriod xMultiplierTbit);
uint8_t S2LP_CSMA_GetCcaPeriod(void);
void S2LP_CSMA_SetCcaLength(uint8_t xCcaLength);
uint8_t S2LP_CSMA_GetCcaLength(void);
void S2LP_CSMA_SetMaxNumberBackoff(uint8_t cMaxNb);
uint8_t S2LP_CSMA_GetMaxNumberBackoff(void);

/**
 *@}
 */

/**
 *@}
 */


/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/

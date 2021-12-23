 /*******************************************************************************
  * @file    s2LP.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    12-July-2019
  * @brief   low level driver s2lp
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
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "s2lp.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------*/
volatile S2LPStatus g_xStatus;
/*!
 * @brief IO function pointer structure
 */
static S2LP_IO_t IO_func;
/* -------- Static functions prototypes --------------------------------------------- */
/* Exported functions ---------------------------------------------------------*/
int32_t S2LP_RegisterBusIO(S2LP_IO_t *pIO)
{
  
  IO_func.Init            = pIO->Init;
  IO_func.DeInit          = pIO->DeInit;
  IO_func.WriteBuffer     = pIO->WriteBuffer;
  IO_func.Delay           = pIO->Delay;
  
  if(!IO_func.Init)
  {
    return S2LP_ERROR;
  }
  return S2LP_OK;
}

int32_t S2LP_Init( void )
{
  if (IO_func.Init()<0)
  {
    return S2LP_ERROR;
  }
  return S2LP_OK;
}


/**
* @brief  Write single or multiple registers.
* @param  cRegAddress: base register's address to be write
* @param  cNbBytes: number of registers and bytes to be write
* @param  pcBuffer: pointer to the buffer of values have to be written into registers
* @retval Device status
*/ 
StatusBytes S2LP_WriteRegister(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer )
{
	uint8_t header[S2LP_CMD_SIZE]={WRITE_HEADER,cRegAddress};
    StatusBytes status;
  
    IO_func.WriteBuffer( header, pcBuffer, cNbBytes );

    ((uint8_t*)&status)[1]=header[0];
    ((uint8_t*)&status)[0]=header[1]; 
  
    return status;
}

/**
* @brief  Read single or multiple registers.
* @param  cRegAddress: base register's address to be read
* @param  cNbBytes: number of registers and bytes to be read
* @param  pcBuffer: pointer to the buffer of registers' values read
* @retval Device status
*/
StatusBytes S2LP_ReadRegister(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer )
{
    uint8_t header[S2LP_CMD_SIZE]={READ_HEADER,cRegAddress};
    StatusBytes status;

    IO_func.WriteBuffer( header, pcBuffer, cNbBytes );

    ((uint8_t*)&status)[1]=header[0];
    ((uint8_t*)&status)[0]=header[1]; 
  
    return status;
}

/**
* @brief  Send a command
* @param  cCommandCode: command code to be sent
* @retval Device status
*/
StatusBytes S2LP_SendCommand(uint8_t cCommandCode)
{
  uint8_t header[S2LP_CMD_SIZE]={COMMAND_HEADER,cCommandCode};
  StatusBytes status;

  IO_func.WriteBuffer( header, NULL, 0 );
  
  ((uint8_t*)&status)[1]=header[0];
  ((uint8_t*)&status)[0]=header[1];
  
  return status;
}

/**
* @brief  Write data into TX FIFO.
* @param  cNbBytes: number of bytes to be written into TX FIFO
* @param  pcBuffer: pointer to data to write
* @retval Device status
*/
StatusBytes S2LP_WriteFIFO(uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t header[S2LP_CMD_SIZE]={WRITE_HEADER,LINEAR_FIFO_ADDRESS};
  StatusBytes status;

  IO_func.WriteBuffer( header, pcBuffer, cNbBytes );
  
  ((uint8_t*)&status)[1]=header[0];
  ((uint8_t*)&status)[0]=header[1];
  
  return status;
}


/**
* @brief  Read data from RX FIFO.
* @param  cNbBytes: number of bytes to read from RX FIFO
* @param  pcBuffer: pointer to data read from RX FIFO
* @retval Device status
*/
StatusBytes S2LP_ReadFIFO(uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t header[S2LP_CMD_SIZE]={READ_HEADER,LINEAR_FIFO_ADDRESS};
  StatusBytes status;

  IO_func.WriteBuffer( header, pcBuffer, cNbBytes );
  
  ((uint8_t*)&status)[1]=header[0];
  ((uint8_t*)&status)[0]=header[1];
  
  return status;
}

/**
 * @brief  Set External Reference.
 * @param  xExtMode new state for the external reference.
 *         This parameter can be: MODE_EXT_XO or MODE_EXT_XIN.
 * @retval None.
 */
void S2LP_SetExtRef(ModeExtRef xExtMode)
{
  uint8_t tmp;
  S2LP_ReadRegister(XO_RCO_CONF0_ADDR, 1, &tmp);
  if(xExtMode == MODE_EXT_XO) {
    tmp &= ~EXT_REF_REGMASK;
  }
  else {
    tmp |= EXT_REF_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(XO_RCO_CONF0_ADDR, 1, &tmp);

}


/**
 * @brief  Return External Reference.
 * @param  None.
 * @retval ModeExtRef Settled external reference.
 *         This parameter can be: MODE_EXT_XO or MODE_EXT_XIN.
 */
ModeExtRef S2LP_GetExtRef(void)
{
  uint8_t tmp;
  g_xStatus = S2LP_ReadRegister(XO_RCO_CONF0_ADDR, 1, &tmp);
  return (ModeExtRef)(tmp & EXT_REF_REGMASK);
}


/**
 * @brief  Return device part number.
 * @param  None.
 * @retval Device part number.
 */
uint8_t S2LP_GetDevicePN(void)
{
  uint8_t tmp;
  g_xStatus = S2LP_ReadRegister(DEVICE_INFO1_ADDR, 1, &tmp);
  return tmp;
}

/**
 * @brief  Return S2LP version.
 * @param  None.
 * @retval S2LP version.
 */
uint8_t S2LP_GetVersion(void)
{
  uint8_t tmp;
  S2LP_ReadRegister(DEVICE_INFO0_ADDR, 1, &tmp);
  return tmp;
}


/**
* @brief  Disable or enable the internal SMPS.
* @param  xNewState if this value is S_DISABLE the external SMPS is enabled and a vlotage must be provided from outside.
*               In this case the internal SMPS will be disabled.
* @retval None.
*/
void S2LP_SetExternalSmpsMode(SFunctionalState xNewState)
{
  uint8_t tmp;
  
  S2LP_ReadRegister(PM_CONF4_ADDR, 1, &tmp);
  
  if(xNewState == S_ENABLE) {
    tmp |= EXT_SMPS_REGMASK;
  } else {
    tmp &= ~EXT_SMPS_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(PM_CONF4_ADDR, 1, &tmp);
}

/**
 * @brief  Updates the gState (the global variable used to maintain memory of S2LP Status)
 *         reading the MC_STATE register of S2LP.
 * @param  None
 * @retval None
 */

void S2LP_RefreshStatus(void)
{
  uint8_t tempRegValue;
  do
  {
    /* Reads the MC_STATE register to update the g_xStatus */
    g_xStatus = S2LP_ReadRegister(MC_STATE0_ADDR, 1, &tempRegValue);
  }  while((tempRegValue>>1)!=g_xStatus.MC_STATE);  
}
/**
  * @brief  FunctionDescription
  * @retval Error code: S2LP_OK on success, S2LP_ERROR on error during calibration of RCO.
  */
int32_t S2LP_RcoCalibration(void)
{
  uint8_t tmp[2],tmp2;

  S2LP_ReadRegister(XO_RCO_CONF0_ADDR, 1, &tmp2);
  tmp2 |= 0x01;
  S2LP_WriteRegister(XO_RCO_CONF0_ADDR, 1, &tmp2);  /* Enable the RCO CALIB setting bit to 1 */

  S2LP_CMD_StrobeStandby();
  IO_func.Delay(50);
  S2LP_CMD_StrobeReady();

  do
  {
    S2LP_RefreshStatus();
    if(g_xStatus.ERROR_LOCK) {
      return S2LP_ERROR;
    }
  } while(g_xStatus.RCCAL_OK == 0);   /* Wait until RC_CAL_OK becomes 1 */

  S2LP_ReadRegister(RCO_CALIBR_OUT4_ADDR, 2, tmp);
  S2LP_ReadRegister(RCO_CALIBR_CONF2_ADDR, 1, &tmp2);

  tmp[1] = (tmp[1] & TEMP_SENSOR_EN_REGMASK) | (tmp2 & 0x7F);

  S2LP_WriteRegister(RCO_CALIBR_CONF3_ADDR, 2, tmp);

  S2LP_ReadRegister(XO_RCO_CONF0_ADDR, 1, &tmp2);
  tmp2 &= 0xFE;
  S2LP_WriteRegister(XO_RCO_CONF0_ADDR, 1, &tmp2);
  return S2LP_OK;
}

/**
  * @brief  FunctionDescription
  * @retval None
  */
void S2LP_TCXOInit(void)
{
  uint8_t tmp;
  S2LP_ReadRegister(XO_RCO_CONF0_ADDR, 1, &tmp);
  tmp|=EXT_REF_REGMASK;
  S2LP_WriteRegister(XO_RCO_CONF0_ADDR, 1, &tmp);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

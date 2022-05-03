/**
 * @file    s2lp.c
 * @author  ST Microelectronics
 * @brief   Wrapper of S2LP Library for BUS IO
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
 * <h2><center>&copy; COPYRIGHT 2021 STMicroelectronics</center></h2>
 */

  
/* Includes ------------------------------------------------------------------*/
#include "s2lp.h"
#include "s2lp_sdkapi_mapping.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define MAX_RCO_ERR 3

/* Private variables -------------------------------------------------------------*/
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
uint16_t S2LP_WriteRegister(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer )
{
    uint8_t header[S2LP_CMD_SIZE]={WRITE_HEADER,cRegAddress};
    uint16_t status;
  
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
uint16_t S2LP_ReadRegister(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer )
{
    uint8_t header[S2LP_CMD_SIZE]={READ_HEADER,cRegAddress};
    uint16_t status;

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
uint16_t S2LP_SendCommand(uint8_t cCommandCode)
{
  uint8_t header[S2LP_CMD_SIZE]={COMMAND_HEADER,cCommandCode};
  uint16_t status;

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
  * @brief  FunctionDescription
  * @retval Error code:  S2LP_OK on success, S2LP_ERROR on error during calibration of RCO.
  */
int32_t S2LP_RcoCalibration(void)
{
  uint8_t tmp[2],tmp2;
  int32_t nRet = S2LP_OK;
  uint8_t nErr = 0;

  S2LP_ReadRegister(XO_RCO_CONF0_ADDR, 1, &tmp2);
  tmp2 |= RCO_CALIBRATION_REGMASK;
  S2LP_WriteRegister(XO_RCO_CONF0_ADDR, 1, &tmp2);  /* Enable the RCO CALIB setting bit to 1 */

  S2LP_CMD_StrobeStandby();
  IO_func.Delay(50);
  S2LP_CMD_StrobeReady();

  do
  {
    S2LPSpiReadRegisters(MC_STATE1_ADDR, 1, tmp);
    
    //Check RCO Calibration Error and retry MAX_RCO_ERR times
    if ((tmp[0]&ERROR_LOCK_REGMASK)==1) 
    {
      //Disable TimerCalibrationRco
      S2LPSpiReadRegisters(XO_RCO_CONF0_ADDR, 1, &tmp2);
      tmp2 &= ~RCO_CALIBRATION_REGMASK;
      S2LPSpiWriteRegisters(XO_RCO_CONF0_ADDR, 1, &tmp2);
      
      //Enable TimerCalibrationRco
      S2LPSpiReadRegisters(XO_RCO_CONF0_ADDR, 1, &tmp2);
      tmp2 |= RCO_CALIBRATION_REGMASK;
      S2LPSpiWriteRegisters(XO_RCO_CONF0_ADDR, 1, &tmp2);
      nErr++;
    }    
  }
  while(((tmp[0]&RCO_CAL_OK_REGMASK)==0) && nErr <= MAX_RCO_ERR);

  if (nErr < MAX_RCO_ERR)
  {
    S2LPSpiReadRegisters(RCO_CALIBR_OUT4_ADDR, 2, tmp);
    S2LPSpiReadRegisters(RCO_CALIBR_CONF2_ADDR, 1, &tmp2);
    
    tmp[1]=(tmp[1]& RFB_IN_0_REGMASK)|(tmp2&(~RFB_IN_0_REGMASK)); 
    
    S2LPSpiWriteRegisters(RCO_CALIBR_CONF3_ADDR, 2, tmp);
    
    S2LPSpiReadRegisters(XO_RCO_CONF0_ADDR, 1, &tmp2);
    tmp2 &= ~RCO_CALIBRATION_REGMASK;
    S2LPSpiWriteRegisters(XO_RCO_CONF0_ADDR, 1, &tmp2);
  }
  else
  {
    nRet = S2LP_ERROR;
  }

  return nRet;

}

/**
  * @brief  S2LP_TCXOInit
  * @retval None
  */
void S2LP_TCXOInit(void)
{
  uint8_t tmp;
  S2LP_ReadRegister(XO_RCO_CONF0_ADDR, 1, &tmp);
  tmp|=EXT_REF_REGMASK;
  S2LP_WriteRegister(XO_RCO_CONF0_ADDR, 1, &tmp);
}

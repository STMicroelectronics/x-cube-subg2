/**
 * @file    crc_lib.h
 * @author  LowPower RF BU - AMG
 * @version 1.0.0
 * @date    April 13, 2017
 * @brief   CRC library header.
 * @details
 * @example
 *   // Init the CRC algorithm
 *   crc_init(0x1021,2,0xffff,0);
 *
 *   for(uint32_t i=0;i<n_bytes;i++)
 *    {  // update the CRC value
 *       crc_update(buffer[i]);
 *    }
 *
 *   // get the CRC value
 *   CRC_VAL=crc_get();
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
 * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
 */
#ifndef __CRC_LIB_H__
#define __CRC_LIB_H__

/*!******************************************************************
 * \fn void crc_init(uint32_t poly, uint8_t size, uint32_t seed, uint8_t reverse)
 * \brief Initializes the parameters to calculate the CRC.
 * \param[in] uint32_t poly: The generator polynomial to use in calculating the CRC. The bits in this integer are the coefficients of the polynomial.
 *              The only polynomials allowed are those that generate 1, 2, 3 or 4 bytes CRCs.
 * \param[in] uint8_t size: CRC size in bytes.
 * \param[in] uint32_t seed: Initial value used to start the CRC calculation.
 *              This initial value should be the initial shift register value.
 * \param[in] uint8_t reverse: A flag that selects a bit reversed algorithm when 1.
 * \retval None.
 *******************************************************************/
void crc_init(uint32_t poly, uint8_t size, uint32_t seed, uint8_t reverse);

/*!******************************************************************
 * \fn void crc_update(uint8_t data)
 * \brief Updates the CRC value with the passed data byte.
* \param[in] data: the data to be used to update the CRC.
 * \retval None.
 *******************************************************************/
void crc_update(uint8_t data);

/*!******************************************************************
 * \fn void crc_reset_seed(void)
 * \brief Initializes the seed of the algortithm. It should be called before starting each CRC process.
 * \param[in] None.
 * \retval None.
 *******************************************************************/
void crc_reset_seed(void);

/*!******************************************************************
 * \fn uint32_t crc_get(void)
 * \brief Get the CRC value.
 * \param[in] None.
 * \retval CRC value.
 *******************************************************************/
uint32_t crc_get(void);
#endif /*__CRC_LIB_H__*/

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

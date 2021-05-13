/**
  @page Utilities

  @verbatim
  ******************************************************************************
  * @file    Utilities_Readme.txt
  * @author  SRA Application Team
  * @brief   Utilities Readme 
  ******************************************************************************
  *
  * Copyright (c) 2021 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Description

  "SerialSniffer" and "wpcapslip6" utilities come from the Contiki OS integration of former
  ST platform named "stm32nucleo-spirit1".
  
  Contiki-NG/serial-io is part of the same Contiki-NG v4.6 Middleware embedded in this
  software packege. It has to be used (instead of wpcapslip6) in case of Border Router
  under Linux environment, if the tunslip6 tool is not already available on the PC
  (through Contiki or Contiki-NG):
  cd Contiki-NG/serial-io
  make tunslip6
  sudo ./tunslip6 –s /dev/ttyACMx aaaa::1/64
  
  where ttyACMx is the device associated to the nucleo board running the Border Router.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */


## <b>Utilities</b>

Utilities README.md file.

## <b>Description</b>

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


### <b>Author</b>

SRA Application Team

### <b>License</b>

Copyright (c) 2021 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

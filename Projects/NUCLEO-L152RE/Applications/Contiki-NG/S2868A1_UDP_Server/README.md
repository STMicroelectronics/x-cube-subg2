
## <b>S2868A1_UDP_Server Application Description</b>

Application that receives messages via UPD from a UDP Client.

Example Description:

This sample application shows how to use S2-LP radio and Contiki-NG OS to enable
UDP communication over 6LoWPAN network.
This sample application configures a node to receive messages from a UDP client
and send back a response.

To test this application you need:
- one STM32 Nucleo board with its X-NUCLEO-S2868A1 or X-NUCLEO-S2868A2 or
  X-NUCLEO-S2915A1 expansion board.
- a host PC running Windows or Linux (optional to print output to a serial line
  monitor)

On the STM32 Nucleo board this sample application for Udp Server must be running.
Another STM32 Nucleo board with its X-NUCLEO-S2868A1 or X-NUCLEO-S2868A2 or
X-NUCLEO-S2915A1 STM32 expansion board (running the Udp Client sample application
included in this package) should be used to test the communication via UDP
over 6LoWPAN.

Known limitations:

- When starting the project from Example Selector in STM32CubeMX and regenerating it
  from ioc file, you may face a build issue because the Contiki-NG middleware is copied in a 
  different folder and all the files are duplicated. To solve it, please delete the project folder of the 
  IDE that you are using (EWARM, MDK-ARM or STM32CubeIDE).
  Other duplicate files that you should delete, if you started the project for the
  Nucleo-F401RE board, from Src and Inc folders, are: stm32f4xx_nucleo.c, stm32f4xx_nucleo.h
  and stm32f4xx_nucleo_errno.h.
  The same operations apply if you starts the project for another STM32 Nucleo board (e.g. for
  Nucleo-L053R8, the files to be removed are stm32l0xx_nucleo.c, stm32l0xx_nucleo.c, stm32l0xx_nucleo.h
  and stm32l0xx_nucleo_errno.h).

### <b>Keywords</b>

Contiki-NG, 6LoWPAN, SubGHz_Phy, SPI, SUBGHZ, UART, TIM

### <b>Directory contents</b>

 - app_contiki-ng.c       This file provides interface between the main.c and
                          Contiki-NG based applications.
 
 - main.c                 Main program body.
 
 - stm32**xx_hal_msp.c    This file provides code for the MSP Initialization
                          and de-Initialization.
						
 - stm32**xx_nucleo_bus.c Source file for the BSP BUS IO driver.
 
 - udp-server.c           Source file for the UDP Server Contiki-NG Thread.
 
 - contiki-platform.c     This file provides the initialization for Contiki-NG.
 
 - radio-driver.c         This file provides the binding between Contiki-NG
                          network API and S2-LP radio.
 
 - stm32**xx_it.c         Source code for interrupt Service Routines.
 
 - subg2_contiki_it.c     Source code for interrupt Service Routines managed
                          within Contiki-NG based application.
						  
 - csma-output.c          Slightly modified version of Contiki-NG CSMA MAC
                          output (transmission) code.
						  
 - s2lp_interface.c       Initialization for S2-LP radio.
 
 - s2lp_management.c      Management functions for S2-LP radio.
 
 - stm32**xx_nucleo.c     Source file for the BSP Common driver
 
 - system_stm32**xx.c     CMSIS Cortex-M4 Device Peripheral Access Layer
                          System Source File.



### <b>Hardware and Software environment</b>

  - This example runs on STM32 Nucleo devices with X-NUCLEO-S2868A1 or
    X-NUCLEO-S2868A2 or X-NUCLEO-S2915A1 STM32 expansion board
  - This example has been tested with STMicroelectronics:
    - NUCLEO-L152RE RevC board
    - NUCLEO-F401RE RevC board
    and can be easily tailored to any other supported device and development board.


ADDITIONAL_BOARD : X-NUCLEO-S2868A1 https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-connect-hw/x-nucleo-s2868a1.html
ADDITIONAL_COMP : S2-LP https://www.st.com/content/st_com/en/products/wireless-connectivity/long-range/proprietary-sub-1-ghz-products/s2-lp.html

### <b>How to use it?</b>

-   In order to make the program work, you must do the following:
    -   WARNING: before opening the project with any toolchain be sure your folder installation path is not too in-depth since the toolchain may report errors after building.
    -   The tested tool chain and environment is explained in the Release notes
    -   Open the suitable toolchain (STM32CubeIDE, IAR, Keil) and open the project for the required STM32 Nucleo board
    -   Rebuild all files and load your image into target memory.
    -   Run the example.
    -   Alternatively, you can download the pre-built binaries in "Binary" folder included in the distributed package.
   
### <b>Author</b>

SRA Application Team

### <b>License</b>

Copyright (c) 2021 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

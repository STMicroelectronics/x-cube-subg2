
## <b>S2868A1_P2P Example Description</b>

Point To Point application for S2-LP Expansion Board.

Example Description:
  
  This example is to demonstrate Point-to-Point communication using S2-LP
  driver and STM32Cube firmware. 

Known limitations:

- When starting the project from Example Selector in STM32CubeMX and regenerating it
  from ioc file, you may face a build issue. To solve it, if you started the project for the
  Nucleo-F401RE board, remove from the IDE project the file stm32f4xx_nucleo.c in the Application/User
  virtual folder and delete, from Src and Inc folders, the files: stm32f4xx_nucleo.c, stm32f4xx_nucleo.h
  and stm32f4xx_nucleo_errno.h.
  The same operations apply if you starts the project for another STM32 Nucleo board (e.g. for
  Nucleo-L053R8, the files to be removed are stm32l0xx_nucleo.c, stm32l0xx_nucleo.c, stm32l0xx_nucleo.h
  and stm32l0xx_nucleo_errno.h).

### <b>Keywords</b>

SubGHz_Phy, SPI, SUBGHZ

### <b>Directory contents</b>

 - app_x-cube-subg2.c     P2P demo initialization and applicative code.
 
 - s2lp_management.c      Management functions for S2-LP radio.
 
 - main.c                 Main program body.
 
 - stm32**xx_hal_msp.c    This file provides code for the MSP Initialization
                          and de-Initialization.
						
 - stm32**xx_nucleo_bus.c Source file for the BSP BUS IO driver.
 
 - stm32**xx_it.c         Source code for interrupt Service Routines.
 
 - stm32**xx_nucleo.c     Source file for the BSP Common driver
 
 - system_stm32**xx.c     CMSIS Cortex-M4 Device Peripheral Access Layer
                          System Source File.

 
### <b>Hardware and Software environment</b>

  - This example runs on STM32 Nucleo devices with X-NUCLEO-S2868A1 or
    X-NUCLEO-S2868A2 or X-NUCLEO-S2915A1 STM32 expansion board
  - This example has been tested with STMicroelectronics:
    - NUCLEO-L053R8 RevC board
    - NUCLEO-L152RE RevC board
    - NUCLEO-F401RE RevC board
    and can be easily tailored to any other supported device and development 
    board.
     

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

-   How to verify the firmware
    -   Power-up the two nodes, each comprising of X-NUCLEO-S2868A1 or X-NUCLEO-S2868A2 or X-NUCLEO-S2915A1 mounted on STM32 Nucleo board
    -   The LED LD2 on STM32 Nucleo board starts to blink
    -   User can press user button B1 (Blue Colour) on any of the STM32 Nucleo board. This causes a command packet to be sent from the node (Node1) to Node2
    -   The successful communication with the other node will cause the LED D2 to blink on the receiving node (Node2)
    -   The receiving node (Node2) will acknowledge the command by sending an RF packet and this can be seen by blink of LED D2 on the Node1 
 
    To summarise:
    1)  User presses B1 on Node1
    2)  Node1  ----------- RF Command packet ------>   Node2
    3)  Success = LED D2 on Node2 Blinks
    4)  Node1  <---------- Ack packet --------------   Node2                                                     
    5)  Success = LED D2 Blinks on Node1
 
### <b>Author</b>

SRA Application Team

### <b>License</b>

Copyright (c) 2021 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

/**
  @page S2-LP Expansion Board for STM32 Nucleo Boards Point-to-Point example

  @verbatim
  ******************************************************************************
  * @file    Point-to-Point Example/readme.txt
  * @author  SRA Application Team
  * @brief   This application is an example to show a Point-to-Point 
             communication between two nodes using STM32 Nucleo boards and 
             S2-LP expansion boards 
  ******************************************************************************
  *
  * Copyright (c) 2020 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0094, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0094
  *
  ******************************************************************************
   @endverbatim

@par Example Description

  This example is to demonstrate Point-to-Point communication using S2-LP
  driver and STM32Cube firmware. 

@par Directory contents

 - app_x-cube-subg2.c     P2P demo initialization and applicative code.
 
 - main.c                 Main program body.
 
 - stm32**xx_hal_msp.c    This file provides code for the MSP Initialization
                          and de-Initialization.
						
 - stm32**xx_nucleo_bus.c Source file for the BSP BUS IO driver.
 
 - stm32**xx_it.c         Source code for interrupt Service Routines.
 
 - stm32**xx_nucleo.c     Source file for the BSP Common driver
 
 - system_stm32**xx.c     CMSIS Cortex-M4 Device Peripheral Access Layer
                          System Source File.

 
@par Hardware and Software environment

  - This example runs on STM32 Nucleo devices with X-NUCLEO-S2868A1 or
    X-NUCLEO-S2868A2 or X-NUCLEO-S2915A1 STM32 expansion board
  - This example has been tested with STMicroelectronics:
    - NUCLEO-L053R8 RevC board
    - NUCLEO-L152RE RevC board
    - NUCLEO-F401RE RevC board
    and can be easily tailored to any other supported device and development 
    board.
     


@par How to use it ? 

In order to make the program work, you must do the following:
 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
 - The tested tool chain and environment is explained in the Release notes
 - Open the suitable toolchain (STM32CubeIDE, IAR, Keil) and open 
    the project for the required STM32 Nucleo board
 - Rebuild all files and load your image into target memory.
 - Run the example.
 - Alternatively, you can download the pre-built binaries in "Binary" 
   folder included in the distributed package.

 How to verify the firmware
 - Power-up the two nodes, each comprising of X-NUCLEO-S2868A1 or
    X-NUCLEO-S2868A2 or X-NUCLEO-S2915A1 mounted on STM32 Nucleo board
 - The LED LD2 on STM32 Nucleo board starts to blink
 - User can press user button B1 (Blue Colour) on any of the STM32 Nucleo board
    This causes a command packet to be sent from the node (Node1) to Node2
 - The successful communication with the other node will cause the LED D2 to blink
	on the receiving node (Node2)
 - The receiving node (Node2) will acknowledge the command by sending an RF 
    packet and this can be seen by blink of LED D2 on the node-1 
 
    To summarise:
    
    *1*  User presses B1 on Node1
    *2*  Node1  ----------- RF Command packet ------>   Node2
    *3*                                    Success = LED D2 on Node2 Blinks
    *4*  Node1  <---------- Ack packet --------------   Node2                                                     
    *5*  Success = LED D2 Blinks on Node1
 
 
 - IMPORTANT NOTE: To avoid issues with USB connection (mandatory if you have USB 3.0), it is   
   suggested to update the ST-Link/V2 firmware for STM32 Nucleo boards to the latest version.
   Please refer to the readme.txt file in the Applications directory for details.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

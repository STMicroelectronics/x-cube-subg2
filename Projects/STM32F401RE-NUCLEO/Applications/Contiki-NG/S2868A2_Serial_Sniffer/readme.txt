/**
  @page Serial Sniffer - Application to capture RF packets.

  @verbatim
  ******************************************************************************
  * @file    Serial Sniffer/readme.txt
  * @author  SRA Application Team
  * @brief   This application provides an example on how to capture RF packets
  *          and display them through Wireshark protocol analyzer application.
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

This sample application shows how to use S2-LP radio and Contiki-NG OS to
configure a node to capture RF packets and send them to a host PC where they 
can be displayed and optionally saved using the Wireshark protocol analyzer
application.

To test this application you need:
- one STM32 Nucleo board with its X-NUCLEO-S2868A1 or X-NUCLEO-S2868A2 or
  X-NUCLEO-S2915A1 expansion board.
- a host PC running Windows or Linux and the serial sniffer utilities 
  that enable the packets to be sent to Wireshark application.

On the STM32 Nucleo board this sample application for Serial Sniffer must be running.
Other STM32 Nucleo boards with X-NUCLEO-S2868A1 or X-NUCLEO-S2868A2 or
X-NUCLEO-S2915A1 STM32 expansion boards (running other Contiki-NG based 
firmware like the Udp Client and Udp Server sample application included in this package)
can be used to generate the packets that can be captured useing the Serial
Sniffer firmware.


@par Directory contents

 - app_contiki-ng.c       This file provides interface between the main.c and
                          Contiki-NG based applications.
 
 - main.c                 Main program body.
 
 - stm32**xx_hal_msp.c    This file provides code for the MSP Initialization
                          and de-Initialization.
						
 - stm32**xx_nucleo_bus.c Source file for the BSP BUS IO driver.
 
 - serial-sniffer.c       Source file for the Serial Sniffer Contiki-NG Thread.
 
 - sniffer.c              Code for the sniffer that outputs packet captures in
                          PCAP format for Wireshark.

 - contiki-platform.c     This file provides the initialization for Contiki-NG.
 
 - radio-driver.c         This file provides the binding between Contiki-NG
                          network API and S2-LP radio.
 
 - stm32**xx_it.c         Source code for interrupt Service Routines.
 
 - subg2_contiki_it.c     Source code for interrupt Service Routines managed
                          within Contiki-NG based application.
						  
 - s2lp_interface.c       Initialization for S2-LP radio.
 
 - stm32**xx_nucleo.c     Source file for the BSP Common driver
 
 - system_stm32**xx.c     CMSIS Cortex-M4 Device Peripheral Access Layer
                          System Source File.



@par Hardware and Software environment

  - This example runs on STM32 Nucleo devices with X-NUCLEO-S2868A1 or
    X-NUCLEO-S2868A2 or X-NUCLEO-S2915A1 STM32 expansion board
  - This example has been tested with STMicroelectronics:
    - NUCLEO-L152RE RevC board
    - NUCLEO-F401RE RevC board
    and can be easily tailored to any other supported device and development board.


@par How to use it?

In order to make the program work, you must do the following:
 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
 - Open STM32CubeIDE toolchain (see the release note for detailed information about the version).
   Alternatively you can use the Keil uVision toolchain (see the release note for detailed information about the version).
   Alternatively you can use the IAR (see the release note for detailed information about the version).
 - Rebuild all files and load your image into target memory.
 - Run the example.
 - Alternatively, you can download the pre-built binaries in "Binary" 
   folder included in the distributed package. 
   
 - IMPORTANT NOTE: To avoid issues with USB connection (mandatory if you have USB 3.0), it is   
   suggested to update the ST-Link/V2 firmware for STM32 Nucleo boards to the latest version.
   Please refer to the readme.txt file in the Applications directory for details.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

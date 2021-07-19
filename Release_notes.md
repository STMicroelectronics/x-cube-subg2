---
pagetitle: Release Notes for X-CUBE-SUBG2 Package
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>X-CUBE-SUBG2</mark>
Copyright &copy; [2021]{style="background-color: yellow;"} STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# License

This software package is
licensed by ST under SLA0077, the "License"; You may not use this component except in compliance
with the License. You may obtain a copy of the License at:


[[SLA0077: SOFTWARE LICENSE AGREEMENT]{style="background-color: yellow;"}
](http://www.st.com/content/st_com/en/search.html#q=SLA0077-t=keywords-page=1)


# Purpose

The X-CUBE-SUBG2 software package is an expansion for STM32Cube. This software provides drivers running on STM32 for STM's Radio S2-LP Low data rate and low power sub-1GHz transceiver devices. It is built on top of STM32Cube software technology that ease portability across different STM32 micro-controllers. This Expansion Firmware is built over the X-NUCLEO-S2868A1, X-NUCLEO-S2868A2 or X-NUCLEO-S2915A1 Expansion Board. This Expansion Board can be plugged on the Arduino UNO R3 connectors of any STM32 Nucleo board. The user can mount the ST Morpho connectors if required. The Expansion Board can be easily stacked in order to evaluate different devices with Sub 1GHz communication.

**X-CUBE-SUBG2 software features**:

- Point-to-Point (P2P) communication example to transfer data from one node to another

- 6LoWPAN communication using Contiki-NG middleware. Sample applications include:

  - UDP Client to send messages to a Server
  - UDP Server to receive messages from a Client and to reply back
  - Serial Sniffer to capture RF packets and send them to Wireshark
  - Border Router to connect a 6LoWPAN and a IPv6 networks

- Easy portability across different MCU families thanks to STM32Cube

- Free user-friendly license terms

- Example implementation available on the X-NUCLEO-S2868A1 (868 Mhz), X-NUCLEO-S2868A2 (868 Mhz) or X-NUCLEO-S2915A1 (915 Mhz) STM32 expansion board plugged on top of one NUCLEO-F401RE or NUCLEO-L152RE or NUCLEO-L053R8



[![The X-CUBE-SUBG2 package contents](_htmresc/SUBG2-arch.png)]()

Here is the list of references to user documents:


- [X-NUCLEO-S2868A1 SCHEMATIC](https://www.st.com/resource/en/schematic_pack/x-nucleo-s2868a1_schematic.pdf)
- [X-NUCLEO-S2868A2 SCHEMATIC](https://www.st.com/resource/en/schematic_pack/x-nucleo-s2868a2_schematic.pdf)
- [X-NUCLEO-S2915A1 SCHEMATIC](https://www.st.com/resource/en/schematic_pack/x-nucleo-s2915a1_schematic.pdf)
- [DS11896 Ultra-low power, high performance, sub-1GHz transceiver](https://www.st.com/resource/en/datasheet/s2-lp.pdf)
- [UM2405 Getting started with the X-NUCLEO-S2868A1 Sub-1 GHz 868 MHz RF expansion board based on S2-LP radio for STM32 Nucleo](https://www.st.com/resource/en/user_manual/dm00498153.pdf)
- [UM2638 Getting started with the X-NUCLEO-S2868A2 Sub-1 GHz 868 MHz RF expansion board based on S2-LP radio for STM32 Nucleo](https://www.st.com/resource/en/user_manual/dm00656126.pdf)
- [UM2641 Getting started with the X-NUCLEO-S2915A1 Sub-1 GHz 915 MHz RF expansion board based on S2-LP radio for STM32 Nucleo](https://www.st.com/resource/en/user_manual/dm00660355.pdf)

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section4" checked aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V4.2.0 / 23-June-2021</label>
<div>			

## Main Changes

### Product update

 - Moved irq_disable_cnt semaphore variable from applicative code to BSP
 - Renamed bsp_ip_conf.h in subg2_bsp_ip_conf.h
 - Add support to Example Selector in STM32CubeMX (so new Projects folders names)


## Contents

<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

Drivers

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V4.0.0 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2868A1\Release_Notes.html)
  **S2915A1**                                                 V4.0.0 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2915A1\Release_Notes.html)
  **S2868A2**                                                 V4.0.0 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2868A2\Release_Notes.html)
  **S2LP**                                                    V2.1.1                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\Components\S2LP\Release_Notes.html)
  **STM32F4xx CMSIS**                                         V2.6.6                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\CMSIS\Device\ST\STM32F4xx\Release_Notes.html)
  **STM32F4xx HAL**                                           V1.7.12 []{.icon-st-update}                       [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\STM32F4xx_HAL_Driver\Release_Notes.html)
  **BSP STM32F4xx_NUCLEO**                                    V1.2.7                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\STM32F4xx-Nucleo\Release_Notes.html)
  **STM32L0xx CMSIS**                                         V1.9.1                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\CMSIS\Device\ST\STM32L0xx\Release_Notes.html)
  **STM32L0xx HAL**                                           V1.10.4                                           [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\STM32L0xx_HAL_Driver\Release_Notes.html)
  **BSP STM32L0xx_NUCLEO**                                    V2.1.3                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\STM32L0xx_Nucleo\Release_Notes.html)
  **STM32L1xx CMSIS**                                         V2.3.2 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\CMSIS\Device\ST\STM32L1xx\Release_Notes.html)
  **STM32L1xx HAL**                                           V1.4.4 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\STM32L1xx_HAL_Driver\Release_Notes.html)
  **BSP STM32L1xx_NUCLEO**                                    V1.1.3                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\STM32L1xx_Nucleo\Release_Notes.html)

Middlewares

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **Contiki-NG_STM32_Library**                                V1.0.2 []{.icon-st-update}                        [SLA0055](http://www.st.com/SLA0055)                                                                          [release note](Middlewares\ST\Contiki-NG_STM32_Library\Release_Notes.html)
  **Contiki-NG**                                              V4.6.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [LICENSE](Middlewares\Third_Party\Contiki-NG\LICENSE.md)
  **TinyDTLS** (deliverd in Contiki-NG)                       V4.6.0                                            [EPL](http://www.eclipse.org/legal/epl-v10.html)  and [EDL](http://www.eclipse.org/org/documents/edl-v10.php) [LICENSE](Middlewares\Third_Party\Contiki-NG\os\net\security\tinydtls\LICENSE)

Utilities  

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  Serial Sniffer Tool                                         V1.0.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [readme](Utilities\PC_Software\SerialSniffer\serialsniffer.readme)
  wpcapslip6 Tool                                             V1.0.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [readme](Utilities\PC_Software\wpcapslip6\wpcapslip6.readme)


## Known Limitations


  Headline
  ----------------------------------------------------------
  When using STM32CubeMX SUBG2 pack, the Contiki-NG based projects for STM32CubeIDE must use "Standard C" library instead of the default "Reduced C" (Properties, C/C++ Build, Settings, MCU Settings, Runtime library => Standard C).
  When using STM32CubeMX SUBG2 pack, do not use "Generate under root" option for STM32CubeIDE for Contiki-NG based projects.
  On dual-core STM32 series this expansion software can be used on both cores but exclusively
  On some Windows PC the boards used to run the Border Router and Serial Sniffer applications may need to be connected to a Serial Terminal application (like Teraterm) before using the user space utilities, to work correctly.
  If using older IAR System Workbench version (8.32.*), "rtimer-arch.h" Contiki-NG file causes an error.
  When using STM32CubeMX SUBG2 pack, if a project has been generated for P2P, to modify it for a Contiki-NG based application that uses LED or Button, it is required to deselect any application and save the .ioc project file before choosing the new Application.
  
\

## Development Toolchains and Compilers

- IAR System Workbench v8.50.9
- ARM Keil v5.32
- STM32CubeIDE v1.6.1


## Supported Devices and Boards

- NUCLEO-F401RE MB1136 rev.C
- NUCLEO-L152RE MB1136 rev.C
- NUCLEO-L053R8 MB1136 rev.C
- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-S2915A1 rev. XS2915A1
- X-NUCLEO-S2868A2

## Backward Compatibility

N/A

## Dependencies

N/A


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" checked aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V4.1.0 / 3-May-2021</label>
<div>			

## Main Changes

### Product update

 - Fixed a issue of Contiki-NG based projects with STM32CubeIDE 1.6.0.
 - Fixed a bug in the BSP that prevented different GPIO EXTI Lines to be used for the radio through STM32CubeMX.
 - Fixed a problem of Contiki-NG based projects with regards to BUTTON and LED (applies to STM32CubeMX pack as well).
 - Refactoring of internal functions of S2-LP radio driver interface for Contiki-NG.
 - New versioning scheme for X-NUCLEO BSP Drivers (for better integration in STM32CubeMX)
 - New versioning for Contiki-NG that now uses official package version (TinyDTLS is embedded in Contiki-NG so uses same version number).


## Contents

<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

Drivers

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V3.0.1 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2868A1\Release_Notes.html)
  **S2915A1**                                                 V3.0.1 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2915A1\Release_Notes.html)
  **S2868A2**                                                 V3.0.1 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2868A2\Release_Notes.html)
  **S2LP**                                                    V2.1.1                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\Components\S2LP\Release_Notes.html)

Middlewares

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **Contiki-NG_STM32_Library**                                V1.0.1 []{.icon-st-update}                        [SLA0055](http://www.st.com/SLA0055)                                                                          [release note](Middlewares\ST\Contiki-NG_STM32_Library\Release_Notes.html)
  **Contiki-NG**                                              V4.6.0 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [LICENSE](Middlewares\Third_Party\Contiki-NG\LICENSE.md)
  **TinyDTLS** (deliverd in Contiki-NG)                       V4.6.0 []{.icon-st-update}                        [EPL](http://www.eclipse.org/legal/epl-v10.html)  and [EDL](http://www.eclipse.org/org/documents/edl-v10.php) [LICENSE](Middlewares\Third_Party\Contiki-NG\os\net\security\tinydtls\LICENSE)

Utilities  

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  Serial Sniffer Tool                                         V1.0.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [readme](Utilities\PC_Software\SerialSniffer\serialsniffer.readme)
  wpcapslip6 Tool                                             V1.0.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [readme](Utilities\PC_Software\wpcapslip6\wpcapslip6.readme)


## Known Limitations


  Headline
  ----------------------------------------------------------
  When using STM32CubeMX SUBG2 pack, the Contiki-NG based projects for STM32CubeIDE must use "Standard C" library instead of the default "Reduced C" (Properties, C/C++ Build, Settings, MCU Settings, Runtime library => Standard C).
  When using STM32CubeMX SUBG2 pack, do not use "Generate under root" option for STM32CubeIDE for Contiki-NG based projects.
  On some Windows PC the boards used to run the Border Router and Serial Sniffer applications may need to be connected to a Serial Terminal application (like Teraterm) before using the user space utilities, to work correctly.
  If using older IAR System Workbench version (8.32.*), "rtimer-arch.h" Contiki-NG file causes an error.
  When using STM32CubeMX SUBG2 pack, if a project has been generated for P2P, to modify it for a Contiki-NG based application that uses LED or Button, it is required to deselect any application and save the .ioc project file before choosing the new Application.
  When using STM32CubeMX SUBG2 pack, if you build a project with the BSP only (no application, no Contiki-NG), to successfully use the radio you must add, at your application level, the following declaration: volatile int irq_disable_cnt = 0;
  
\

## Development Toolchains and Compilers

- IAR System Workbench v8.50.9
- ARM Keil v5.32
- STM32CubeIDE v1.6.1


## Supported Devices and Boards

- NUCLEO-F401RE MB1136 rev.C
- NUCLEO-L152RE MB1136 rev.C
- NUCLEO-L053R8 MB1136 rev.C
- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-S2915A1 rev. XS2915A1
- X-NUCLEO-S2868A2

## Backward Compatibility

- compatbile with version 4.0.0
- ioc files generated with versions up to 3.0.1 (STM32CubeMX 6.1.x) are not compatible with this release.

## Dependencies

This software release is compatible with:

- STM32CubeHAL F4 V1.26.0
- STM32CubeHAL L0 V1.12.0
- STM32CubeHAL L1 V1.10.2


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" checked aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V4.0.0 / 22-February-2021</label>
<div>			

## Main Changes

### Product update

 - Aligned to STMicroelectronics.X-CUBE-SUBG2.4.0.0.pack for STM32CubeMX (minimum required version V6.2.0)
 - SPI DMA APIs are now generated by STM32CubeMX.


## Contents

<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

Drivers

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V2.2.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2868A1\Release_Notes.html)
  **S2915A1**                                                 V1.1.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2915A1\Release_Notes.html)
  **S2868A2**                                                 V1.1.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2868A2\Release_Notes.html)
  **S2LP**                                                    V2.1.1                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\Components\S2LP\Release_Notes.html)

Middlewares

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **Contiki-NG_STM32_Library**                                V1.0.0                                            [SLA0055](http://www.st.com/SLA0055)                                                                          [release note](Middlewares\ST\Contiki-NG_STM32_Library\Release_Notes.html)
  **Contiki-NG**                                              V1.0.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [LICENSE](Middlewares\Third_Party\Contiki-NG\LICENSE.md)
  **TinyDTLS** (deliverd in Contiki-NG)                       V1.0.0                                            [EPL](http://www.eclipse.org/legal/epl-v10.html)  and [EDL](http://www.eclipse.org/org/documents/edl-v10.php) [LICENSE](Middlewares\Third_Party\Contiki-NG\os\net\security\tinydtls\LICENSE)

Utilities  

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  Serial Sniffer Tool                                         V1.0.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [readme](Utilities\PC_Software\SerialSniffer\serialsniffer.readme)
  wpcapslip6 Tool                                             V1.0.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [readme](Utilities\PC_Software\wpcapslip6\wpcapslip6.readme)


## Known Limitations


  Headline
  ----------------------------------------------------------
  On some Windows PC the boards used to run the Border Router and Serial Sniffer applications may need to be connected to a Serial Terminal application (like Teraterm) before using the user space utilities, to work correctly.
  If using older IAR System Workbench version (8.32.*), "rtimer-arch.h" Contiki-NG file causes an error.

\

## Development Toolchains and Compilers

- IAR System Workbench v8.50.5
- ARM Keil v5.31
- STM32CubeIDE v1.5.1


## Supported Devices and Boards

- NUCLEO-F401RE MB1136 rev.C
- NUCLEO-L152RE MB1136 rev.C
- NUCLEO-L053R8 MB1136 rev.C
- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-S2915A1 rev. XS2915A1
- X-NUCLEO-S2868A2

## Backward Compatibility

- ioc files generated with versions up to 3.0.1 (STM32CubeMX 6.1.x) are not compatible with this release.

## Dependencies

This software release is compatible with:

- STM32CubeHAL F4 V1.26.0
- STM32CubeHAL L0 V1.12.0
- STM32CubeHAL L1 V1.10.2


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" checked aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V3.0.1 / 15-December-2020</label>
<div>			

## Main Changes

### Product update

 - Bugfixes in the STM32CubeMX pack (G4 and MP1 platforms).


## Contents

<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

Drivers

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V2.2.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2868A1\Release_Notes.html)
  **S2915A1**                                                 V1.1.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2915A1\Release_Notes.html)
  **S2868A2**                                                 V1.1.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2868A2\Release_Notes.html)
  **S2LP**                                                    V2.1.1                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\Components\S2LP\Release_Notes.html)

Middlewares

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **Contiki-NG_STM32_Library**                                V1.0.0                                            [SLA0055](http://www.st.com/SLA0055)                                                                          [release note](Middlewares\ST\Contiki-NG_STM32_Library\Release_Notes.html)
  **Contiki-NG**                                              V1.0.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [LICENSE](Middlewares\Third_Party\Contiki-NG\LICENSE.md)

Utilities  

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  Serial Sniffer Tool                                         V1.0.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [readme](Utilities\PC_Software\SerialSniffer\serialsniffer.readme)
  wpcapslip6 Tool                                             V1.0.0                                            [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [readme](Utilities\PC_Software\wpcapslip6\wpcapslip6.readme)


## Known Limitations


  Headline
  ----------------------------------------------------------
  On some Windows PC the boards used to run the Border Router and Serial Sniffer applications may need to be connected to a Serial Terminal application (like Teraterm) before using the user space utilities, to work correctly.
  If using older IAR System Workbench version (8.32.*), "rtimer-arch.h" Contiki-NG file causes an error.

\

## Development Toolchains and Compilers

- IAR System Workbench v8.50.5
- ARM Keil v5.31
- STM32CubeIDE v1.4.2


## Supported Devices and Boards

- NUCLEO-F401RE MB1136 rev.C
- NUCLEO-L152RE MB1136 rev.C
- NUCLEO-L053R8 MB1136 rev.C
- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-S2915A1 rev. XS2915A1
- X-NUCLEO-S2868A2

## Backward Compatibility

N/A

## Dependencies

This software release is compatible with:

- STM32CubeHAL F4 V1.25.2
- STM32CubeHAL L0 V1.12.0
- STM32CubeHAL L1 V1.10.2


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" checked aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V3.0.0 / 15-November-2020</label>
<div>			

## Main Changes

### Product update

 - Align to STMicroelectronics.X-CUBE-SUBG2.3.0.0.pack for STM32CubeMX (minimum required version V6.1.0)
 - Added Contiki-NG Middleware and related applications, for 6LoWPAN communication.


## Contents

<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

Drivers

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V2.2.0 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2868A1\Release_Notes.html)
  **S2915A1**                                                 V1.1.0 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2915A1\Release_Notes.html)
  **S2868A2**                                                 V1.1.0 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\S2868A2\Release_Notes.html)
  **S2LP**                                                    V2.1.1 []{.icon-st-update}                        [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [release note](Drivers\BSP\Components\S2LP\Release_Notes.html)

Middlewares

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **Contiki-NG_STM32_Library**                                V1.0.0 []{.icon-st-add}                           [SLA0055](http://www.st.com/SLA0055)                                                                          [release note](Middlewares\ST\Contiki-NG_STM32_Library\Release_Notes.html)
  **Contiki-NG**                                              V1.0.0 []{.icon-st-add}                           [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [LICENSE](Middlewares\Third_Party\Contiki-NG\LICENSE.md)

Utilities  

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  Serial Sniffer Tool                                         V1.0.0 []{.icon-st-add}                           [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [readme](Utilities\PC_Software\SerialSniffer\serialsniffer.readme)
  wpcapslip6 Tool                                             V1.0.0 []{.icon-st-add}                           [BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)                                                  [readme](Utilities\PC_Software\wpcapslip6\wpcapslip6.readme)


## Known Limitations


  Headline
  ----------------------------------------------------------
  On some Windows PC the boards used to run the Border Router and Serial Sniffer applications may need to be connected to a Serial Terminal application (like Teraterm) before using the user space utilities, to work correctly.
  If using older IAR System Workbench version (8.32.*), "rtimer-arch.h" Contiki-NG file causes an error.

\

## Development Toolchains and Compilers

- IAR System Workbench v8.50.5
- ARM Keil v5.31
- STM32CubeIDE v1.4.2


## Supported Devices and Boards

- NUCLEO-F401RE MB1136 rev.C
- NUCLEO-L152RE MB1136 rev.C
- NUCLEO-L053R8 MB1136 rev.C
- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-S2915A1 rev. XS2915A1
- X-NUCLEO-S2868A2

## Backward Compatibility

N/A

## Dependencies

This software release is compatible with:

- STM32CubeHAL F4 V1.25.2
- STM32CubeHAL L0 V1.12.0
- STM32CubeHAL L1 V1.10.2


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" checked aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V2.0.0 / 9-July-2020</label>
<div>			

## Main Changes

### Product update

Update to support STM32CubeMX 6.0.


  Headline
  ----------------------------------------------------------
  STM32CubeMX configuration files (ioc) are available for example projects
  BSP v2 compliance
  BSP RADIO APIs implementation for X-NUCLEO-S2868A1, X-NUCLEO-S2915A1 and X-NUCLEO-S2868A2
  

  : Additional features


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V2.1.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](Drivers\BSP\S2868A1\Release_Notes.html)
  **S2915A1**                                                 V1.0.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](Drivers\BSP\S2915A1\Release_Notes.html)
  **S2868A2**                                                 V1.0.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](Drivers\BSP\S2868A2\Release_Notes.html)
  **S2LP**                                                    V2.1.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](Drivers\BSP\Components\S2LP\Release_Notes.html)

  : Components

\

Note: in the table above, components **highlighted** have changed since previous release.

## Known Limitations


  Headline
  ----------------------------------------------------------
  ContikiOS and WM-Bus integration postponed to a next release
  Not compatible with case-sensitive file systems (Linux)

  : Requirements not met or planned in a forthcoming release

\

## Development Toolchains and Compilers

- IAR System Workbench v8.32.3
- ARM Keil v5.29
- STM32CubeIDE v1.3.0


## Supported Devices and Boards

- NUCLEO-F401RE MB1136 rev.C
- NUCLEO-L152RE MB1136 rev.C
- NUCLEO-L053R8 MB1136 rev.C
- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-S2915A1 rev. XS2915A1
- X-NUCLEO-S2868A2

## Backward Compatibility

N/A

## Dependencies

This software release is compatible with:

- STM32CubeHAL F4 V1.25.0
- STM32CubeHAL L0 V1.11.2
- STM32CubeHAL L1 V1.10.0


</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section1" checked aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V1.1.0 / 21-Feb-2020</label>
<div>			

## Main Changes

### Product update

This release add support for two new Expansion Boards. The [X-NUCLEO-S2915A1](https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-connect-hw/x-nucleo-s2915a1.html) is an Expansion Board for the 915MHz (US). The [X-NUCLEO-S2868A2](https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-connect-hw/x-nucleo-s2868a2.html) is an Expansion Board that will supersede the [X-NUCLEO-S2868A1](https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-connect-hw/x-nucleo-s2868a1.html). 

The boards are supported through STM32CubeMX. 


  Headline
  ----------------------------------------------------------
  STM32CubeMX configuration files (ioc) are available for example projects
  BSP v2 compliance
  BSP RADIO APIs implementation for X-NUCLEO-S2868A1, X-NUCLEO-S2915A1 and X-NUCLEO-S2868A2
  

  : Additional features


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V2.1.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](Drivers\BSP\S2868A1\Release_Notes.html)
  **S2915A1**                                                 V1.0.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](Drivers\BSP\S2915A1\Release_Notes.html)
  **S2868A2**                                                 V1.0.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](Drivers\BSP\S2868A2\Release_Notes.html)
  **S2LP**                                                    V2.1.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](Drivers\BSP\Components\S2LP\Release_Notes.html)

  : Components

\

Note: in the table above, components **highlighted** have changed since previous release.

## Known Limitations


  Headline
  ----------------------------------------------------------
  ContikiOS and WM-Bus integration postponed to a next release
  Not compatible with case-sensitive file systems (Linux)

  : Requirements not met or planned in a forthcoming release

\

## Development Toolchains and Compilers

- IAR System Workbench v8.32.3
- ARM Keil v5.27
- STM32CubeIDE v1.1.0


## Supported Devices and Boards

- NUCLEO-F401RE MB1136 rev.C
- NUCLEO-L152RE MB1136 rev.C
- NUCLEO-L053R8 MB1136 rev.C
- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-S2915A1 rev. XS2915A1
- X-NUCLEO-S2868A2

## Backward Compatibility

N/A

## Dependencies

This software release is compatible with:

- STM32CubeHAL F4 V1.24.1
- STM32CubeHAL L0 V1.11.2
- STM32CubeHAL L1 V1.9.0


</div>
:::



::: {.collapse}
<input type="checkbox" id="collapse-section0"  aria-hidden="true">
<label for="collapse-section0" aria-hidden="true">V1.0.0 / 6-Dec-2019</label>
<div>			

## Main Changes

### First release

This is the first release of the X-CUBE-SUBG2 package, a redesign of the X-CUBE-SUBG1 that is integrated with STM32CubeMX and compliant with BSP v2 specifications. 

  Headline
  ----------------------------------------------------------
  STM32CubeMX configuration files (ioc) are available for example projects
  BSP v2 compliance
  BSP RADIO APIs implementation
  

  : Additional features


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V2.0.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](Drivers\BSP\S2868A1\Release_Notes.html)
  **S2LP**                                                    V2.0.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](Drivers\BSP\Components\S2LP\Release_Notes.html)

  : Components

\

Note: in the table above, components **highlighted** have changed since previous release.

## Known Limitations


  Headline
  ----------------------------------------------------------
  ContikiOS and WM-Bus integration postponed to next release
  Not compatible with case-sensitive file systems (Linux)

  : Requirements not met or planned in a forthcoming release

\

## Development Toolchains and Compilers

- IAR System Workbench v8.32
- ARM Keil v5.27
- STM32CubeIDE v1.1.0


## Supported Devices and Boards

- NUCLEO-F401RE MB1136 rev.C
- NUCLEO-L152RE MB1136 rev.C
- NUCLEO-L053R8 MB1136 rev.C
- X-NUCLEO-S2868A1 rev. XS2868V1


## Backward Compatibility

N/A

## Dependencies

This software release is compatible with:

- STM32CubeHAL F4 V1.24.1
- STM32CubeHAL L0 V1.11.2
- STM32CubeHAL L1 V1.9.0


</div>
:::

:::
:::

<footer class="sticky">
::: {.columns}
::: {.column width="95%"}
For complete documentation on **X-CUBE-SUBG2** ,
visit: [[X-CUBE-SUBG2](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-expansion-packages/x-cube-subg2.html)]{style="background-color: yellow;"}
:::
::: {.column width="5%"}

:::
:::
</footer>

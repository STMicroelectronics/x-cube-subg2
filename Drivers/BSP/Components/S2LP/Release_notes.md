---
pagetitle: Release Notes for S2-LP Component Driver
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>S2-LP</mark>
Copyright &copy; [2020]{style="background-color: yellow;"} STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>


# License

This software component is
licensed by ST under BSD 3-Clause, the "License"; You may not use this component except in compliance
with the License. You may obtain a copy of the License at:

[[BSD 3-Clause]{style="background-color: yellow;"}](https://opensource.org/licenses/BSD-3-Clause)

# Purpose

The S2-LP Component Driver is a driver for the STM32Cube. This software implements a Component Driver running on STM32 for STM's Radio S2-LP Low data rate and low power sub-1GHz transceiver devices. It is built on top of STM32Cube software technology that ease portability across different STM32 micro-controllers. 



Here is the list of references to user documents:

- [DS11896 Ultra-low power, high performance, sub-1GHz transceiver](https://www.st.com/resource/en/datasheet/s2-lp.pdf)

:::

::: {.col-sm-12 .col-lg-8}
# Update History
::: {.collapse}
<input type="checkbox" id="collapse-section1" checked aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">__2.1.1 / Sep 9th 2020__</label>
<div>			

## Main Changes

### Maintenance release

This is a maintenance release of the S2-LP Component Driver for BSP v2.0. 

  Headline
  ----------------------------------------------------------
  BSP v2 compliance
  BSP RADIO APIs implementation
  

  : Additional features

: Fixed bugs list

  Headline
  ----------------------------------------------------------
  Fixed Radio Refresh Status implementation
  Fixed Packet Basic Address Init


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2-LP**                                                   V2.1.1                                            [[BSD 3-Clause]](https://opensource.org/licenses/BSD-3-Clause)                                                [release note](.\Release_Notes.html)

  : Components

\

Note: in the table above, components **highlighted** have changed since previous release.

## Known Limitations


  Headline
  ----------------------------------------------------------
  No known limitations
  
  : Requirements not met or planned in a forthcoming release

\

## Development Toolchains and Compilers

- IAR System Workbench v8.50.5
- ARM Keil v5.31
- STM32CubeIDE v1.4.2


## Supported Devices and Boards

- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-S2868A2 rev. XS2868V2
- X-NUCLEO-S2915A1 rev. XS2915V1


## Backward Compatibility

N/A

## Dependencies

This software release is compatible with:

- STM32CubeHAL F4 V1.25.0
- STM32CubeHAL L0 V1.11.3
- STM32CubeHAL L1 V1.10.0

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" checked aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">__2.1.0 / Feb 21st 2020__</label>
<div>			

## Main Changes

### Maintenance release

This is the first release of the rework of S2-LP Component Driver for BSP v2.0. 

  Headline
  ----------------------------------------------------------
  BSP v2 compliance
  BSP RADIO APIs implementation
  

  : Additional features

: Fixed bugs list

  Headline
  ----------------------------------------------------------
  Fixed dependency between the Component Driver and the BSP 
  Fixed STM32CubeIDE warning in Release mode
  Moved S2LP_ConfigRangeExt() at app level
  Modifications to support also 915MHz board


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2-LP**                                                   V2.1.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](.\Release_Notes.html)

  : Components

\

Note: in the table above, components **highlighted** have changed since previous release.

## Known Limitations


  Headline
  ----------------------------------------------------------
  No known limitations
  
  : Requirements not met or planned in a forthcoming release

\

## Development Toolchains and Compilers

- IAR System Workbench v8.32
- ARM Keil v5.27
- STM32CubeIDE v1.1.0


## Supported Devices and Boards

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



::: {.collapse}
<input type="checkbox" id="collapse-section0" aria-hidden="true">
<label for="collapse-section0" aria-hidden="true">__2.0.0 / Dec 6th 2019__</label>
<div>			

## Main Changes

### Component release

This is the first release of the rework of S2-LP Component Driver for BSP v2.0. 

  Headline
  ----------------------------------------------------------
  BSP v2 compliance
  BSP RADIO APIs implementation
  

  : Additional features


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2-LP**                                                   V2.0.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](.\Release_Notes.html)

  : Components

\

Note: in the table above, components **highlighted** have changed since previous release.

## Known Limitations


  Headline
  ----------------------------------------------------------
  No known limitations
  
  : Requirements not met or planned in a forthcoming release

\

## Development Toolchains and Compilers

- IAR System Workbench v8.32
- ARM Keil v5.27
- STM32CubeIDE v1.1.0


## Supported Devices and Boards

- X-NUCLEO-S2868A1 rev. XS2868V1


## Backward Compatibility

N/A

## Dependencies

This software release is compatible with:

- STM32CubeHAL F4 V1.25.0
- STM32CubeHAL L0 V1.11.2
- STM32CubeHAL L1 V1.9.0

</div>
:::

:::
:::

<footer class="sticky">
::: {.columns}
::: {.column width="95%"}
For complete documentation on **S2-LP** ,
visit: [[S2-LP](https://www.st.com/content/st_com/en/products/wireless-transceivers-mcus-and-modules/sub-1ghz-rf/s2-lp.html)]
:::
::: {.column width="5%"}

:::
:::
</footer>

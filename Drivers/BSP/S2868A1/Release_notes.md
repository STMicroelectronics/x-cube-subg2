---
pagetitle: Release Notes for S2868A1 BSP 
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>S2868A1 BSP</mark>
Copyright &copy; [2021]{style="background-color: yellow;"} STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>


# License

This software component is
licensed by ST under BSD 3-Clause, the "License"; You may not use this component except in compliance
with the License. You may obtain a copy of the License at:

[[BSD 3-Clause]{style="background-color: yellow;"}](https://opensource.org/licenses/BSD-3-Clause)

# Purpose

The S2868A1 BSP is a SW component for the STM32Cube. This software implements the BSP v2.0 specifications for the X-NUCLEO-S2868A1 Expansion Board on STM32 for STM's Radio S2-LP Low data rate and low power sub-1GHz transceiver devices. It is built on top of STM32Cube software technology that ease portability across different STM32 micro-controllers. 

Here is the list of references to user documents:

- [DS11896 Ultra-low power, high performance, sub-1GHz transceiver](https://www.st.com/resource/en/datasheet/s2-lp.pdf)
- [UM2405 Getting started with the X-NUCLEO-S2868A1 Sub-1 GHz 868 MHz RF expansion board based on S2-LP radio for STM32 Nucleo](https://www.st.com/resource/en/user_manual/dm00498153.pdf)
- [X-NUCLEO-S2868A1 SCHEMATIC](https://www.st.com/resource/en/schematic_pack/x-nucleo-s2868a1_schematic.pdf)

:::

::: {.col-sm-12 .col-lg-8}
# Update History


::: {.collapse}
<input type="checkbox" id="collapse-section4" checked aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">__3.0.1 / April 9th 2021__</label>
<div>			

## Main Changes

### Product update

 - Fixed a bug in s2868a1.c file that prevented different GPIO EXTI Lines to be used for the radio through STM32CubeMX.
 - Version number has been updated in order to simplify the versioning in STM32CubeMX.
  

## Contents

<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V3.0.1 []{.icon-st-update}                        [[BSD 3-Clause]](https://opensource.org/licenses/BSD-3-Clause)                                                [release note](.\Release_Notes.html)


## Known Limitations


  Headline
  ----------------------------------------------------------
  No known limitations

\

## Development Toolchains and Compilers

- IAR System Workbench v8.50.9
- ARM Keil v5.32
- STM32CubeIDE v1.6.0


## Supported Devices and Boards

- X-NUCLEO-S2868A1 rev. XS2868V1


## Backward Compatibility

N/A

## Dependencies

This software release is compatible with:

- S2LP Component Driver v2.1.1

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" checked aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">__2.2.0 / Sep 9th 2020__</label>
<div>			

## Main Changes

### Maintenance release

This is a maintenance release of the S2868A1 BSP Driver, compliant with BSP v2.0. 

  Headline
  ----------------------------------------------------------
  BSP v2 compliance
  BSP RADIO APIs implementation
  Added API needed for SigFox package
  

  : Additional features

  : Fixed bugs list
  


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  S2868A1                                                     V2.2.0                                            [[BSD 3-Clause]](https://opensource.org/licenses/BSD-3-Clause)                                                [release note](.\Release_Notes.html)

  : Components

\


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


## Backward Compatibility

N/A

## Dependencies

This software release is compatible with:

Compatible with S2LP Component Driver v2.1.1

</div>
:::
::: {.collapse}
<input type="checkbox" id="collapse-section6" checked aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">__2.1.0 / Feb 21st 2020__</label>
<div>			

## Main Changes

### Maintenance release

This is a maintenance release of the S2868A1 BSP Driver, compliant with BSP v2.0. 

  Headline
  ----------------------------------------------------------
  BSP v2 compliance
  BSP RADIO APIs implementation
  

  : Additional features

  : Fixed bugs list

 Headline
  ----------------------------------------------------------
  Fixed dependency between the Component Driver and the BSP 
  Fixed typos in the documentation



## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V2.1.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](.\Release_Notes.html)

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

Compatible with S2LP Component Driver v2.1.0

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section5"  aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">__2.0.0 / Dec 6th 2019__</label>
<div>			

## Main Changes

### Component release

This is the first release of the S2868A1 BSP Driver, compliant with BSP v2.0. 

  Headline
  ----------------------------------------------------------
  BSP v2 compliance
  BSP RADIO APIs implementation
  

  : Additional features


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V2.0.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](.\Release_Notes.html)

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

New release. No backward compatibility

## Dependencies

This software release is compatible with:

N/A

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4"  aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">__1.0.4 / Sept 08th 2017__</label>
<div>			

## Main Changes

### Maintenance release

Retrieve Public Network handling in the radio driver


  Headline
  ----------------------------------------------------------
  BSP v1 compliance
  

  : Additional features


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V1.0.4                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](.\Release_Notes.html)

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

- IAR System Workbench 
- ARM Keil 
- SystemWorkbench for STM32


## Supported Devices and Boards

- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-IDS04A1 
- X-NUCLEO-IDS05A1


## Backward Compatibility

Backward compatible with previous release

## Dependencies



</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3"  aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">__1.0.3 / June 1st 2017__</label>
<div>	

## Main Changes

### Maintenance release

SX1276 radio driver externalized as component


  Headline
  ----------------------------------------------------------
  BSP v1 compliance
  

  : Additional features


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V1.0.3                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](.\Release_Notes.html)

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

- IAR System Workbench 
- ARM Keil 
- SystemWorkbench for STM32


## Supported Devices and Boards

- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-IDS04A1 
- X-NUCLEO-IDS05A1


## Backward Compatibility

Backward compatible with previous release

## Dependencies



</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section2"  aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">__1.0.2 / Feb 27th 2017__</label>
<div>	

## Main Changes

### Maintenance release

Power settings now board dependent


  Headline
  ----------------------------------------------------------
  BSP v1 compliance
  

  : Additional features


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V1.0.2                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](.\Release_Notes.html)

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

- IAR System Workbench 
- ARM Keil 
- SystemWorkbench for STM32


## Supported Devices and Boards

- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-IDS04A1 
- X-NUCLEO-IDS05A1


## Backward Compatibility

Backward compatible with previous release

## Dependencies



</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section1"  aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">__1.0.1 / Sept 15th 2016__</label>
<div>	

## Main Changes

### Maintenance release

Interrupt handling



  Headline
  ----------------------------------------------------------
  BSP v1 compliance
  

  : Additional features


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V1.0.1                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](.\Release_Notes.html)

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

- IAR System Workbench 
- ARM Keil 
- SystemWorkbench for STM32


## Supported Devices and Boards

- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-IDS04A1 
- X-NUCLEO-IDS05A1


## Backward Compatibility

Backward compatible with previous release

## Dependencies



</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section0"  aria-hidden="true">
<label for="collapse-section0" aria-hidden="true">__1.0.0 / Jul 1st 2016__</label>
<div>	

## Main Changes

### First Release

First release


  Headline
  ----------------------------------------------------------
  BSP v1 compliance
  

  : Additional features


## Contents

  Name                                                        Version                                           License                                                                                                       Release note
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  **S2868A1**                                                 V1.0.0                                            [SLA0044](http://www.st.com/SLA0044)                                                                          [release note](.\Release_Notes.html)

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

- IAR System Workbench 
- ARM Keil 
- SystemWorkbench for STM32


## Supported Devices and Boards

- X-NUCLEO-S2868A1 rev. XS2868V1
- X-NUCLEO-IDS04A1 
- X-NUCLEO-IDS05A1


## Backward Compatibility

Backward compatible with previous release

## Dependencies



</div>
:::

:::
:::

<footer class="sticky">
::: {.columns}
::: {.column width="95%"}
For complete documentation on **X-NUCLEO-S868A1** ,
visit: [[S2868A1](https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-connect-hw/x-nucleo-s2868a1.html)]
:::
::: {.column width="5%"}

:::
:::
</footer>

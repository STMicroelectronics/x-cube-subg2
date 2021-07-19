---
pagetitle: Release Notes for X-CUBE-SUBG2 Projects
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="../_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>X-CUBE-SUBG2</mark> Projects
Copyright &copy; 2021 STMicroelectronics\
    
[![ST logo](../_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# License

This software package is
licensed by ST under ST license SLA0077, the "License"; You may not use this component except in compliance
with the License. You may obtain a copy of the License at:
[http://www.st.com/SLA0077](http://www.st.com/content/st_com/en/search.html#q=SLA0077-t=keywords-page=1)

# Purpose

The <mark>X-CUBE-SUBG2</mark> Projects provide examples and applications to aid understanding how to develop an application using simple Point to Point communication or more complex 6LoWPAN based connectivity, using Contiki-NG Middleware.
Projects include STM32CubeMX project file (.ioc) for the graphical visualization of the STM32 microcontroller pins, peripherals and middlewares configuration.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section18" checked aria-hidden="true">
<label for="collapse-section18" aria-hidden="true">V4.2.0 / 21-June-2021</label>
<div>			

## Main Changes

### Product update

  Headline
  ----------------------------------------------------------
  Update .ioc configuration files to STM32CubeMX V6.3.0
  Add support to Example Selector in STM32CubeMX

## Contents

Projects

  Board/Device                                                Category                                          Name                                                                                                          Description
  ----------------------------------------------------------- ------------------------------------------------- ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
  NUCLEO-F401RE                                               Application                                       [S2868A1_Border_Router](NUCLEO-F401RE\Applications\Contiki-NG\S2868A1_Border_Router)                          [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2868A1_Border_Router\readme.txt)
  NUCLEO-F401RE                                               Application                                       [S2868A1_Serial_Sniffer](NUCLEO-F401RE\Applications\Contiki-NG\S2868A1_Serial_Sniffer)                        [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2868A1_Serial_Sniffer\readme.txt)
  NUCLEO-F401RE                                               Application                                       [S2868A1_UDP_Client](NUCLEO-F401RE\Applications\Contiki-NG\S2868A1_UDP_Client)                                [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2868A1_UDP_Client\readme.txt)
  NUCLEO-F401RE                                               Application                                       [S2868A1_UDP_Server](NUCLEO-F401RE\Applications\Contiki-NG\S2868A1_UDP_Server)                                [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2868A1_UDP_Server\readme.txt)
  NUCLEO-F401RE                                               Application                                       [S2868A2_Border_Router](NUCLEO-F401RE\Applications\Contiki-NG\S2868A2_Border_Router)                          [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2868A2_Border_Router\readme.txt)
  NUCLEO-F401RE                                               Application                                       [S2868A2_Serial_Sniffer](NUCLEO-F401RE\Applications\Contiki-NG\S2868A2_Serial_Sniffer)                        [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2868A2_Serial_Sniffer\readme.txt)
  NUCLEO-F401RE                                               Application                                       [S2868A2_UDP_Client](NUCLEO-F401RE\Applications\Contiki-NG\S2868A2_UDP_Client)                                [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2868A2_UDP_Client\readme.txt)
  NUCLEO-F401RE                                               Application                                       [S2868A2_UDP_Server](NUCLEO-F401RE\Applications\Contiki-NG\S2868A2_UDP_Server)                                [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2868A2_UDP_Server\readme.txt)
  NUCLEO-F401RE                                               Application                                       [S2915A1_Border_Router](NUCLEO-F401RE\Applications\Contiki-NG\S2915A1_Border_Router)                          [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2915A1_Border_Router\readme.txt)
  NUCLEO-F401RE                                               Application                                       [S2915A1_Serial_Sniffer](NUCLEO-F401RE\Applications\Contiki-NG\S2915A1_Serial_Sniffer)                        [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2915A1_Serial_Sniffer\readme.txt)
  NUCLEO-F401RE                                               Application                                       [S2915A1_UDP_Client](NUCLEO-F401RE\Applications\Contiki-NG\S2915A1_UDP_Client)                                [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2915A1_UDP_Client\readme.txt)
  NUCLEO-F401RE                                               Application                                       [S2915A1_UDP_Server](NUCLEO-F401RE\Applications\Contiki-NG\S2915A1_UDP_Server)                                [readme](NUCLEO-F401RE\Applications\Contiki-NG\S2915A1_UDP_Server\readme.txt)
  NUCLEO-F401RE                                               Example                                           [S2868A1_P2P](NUCLEO-F401RE\Examples\S2868A1_P2P)                                                             [readme](NUCLEO-F401RE\Examples\S2868A1_P2P\readme.txt)
  NUCLEO-F401RE                                               Example                                           [S2868A2_P2P](NUCLEO-F401RE\Examples\S2868A2_P2P)                                                             [readme](NUCLEO-F401RE\Examples\S2868A2_P2P\readme.txt)
  NUCLEO-F401RE                                               Example                                           [S2915A1_P2P](NUCLEO-F401RE\Examples\S2915A1_P2P)                                                             [readme](NUCLEO-F401RE\Examples\S2915A1_P2P\readme.txt)
  NUCLEO-L053R8                                               Example                                           [S2868A1_P2P](NUCLEO-L053R8\Examples\S2868A1_P2P)                                                             [readme](NUCLEO-L053R8\Examples\S2868A1_P2P\readme.txt)
  NUCLEO-L053R8                                               Example                                           [S2868A2_P2P](NUCLEO-L053R8\Examples\S2868A2_P2P)                                                             [readme](NUCLEO-L053R8\Examples\S2868A2_P2P\readme.txt)
  NUCLEO-L053R8                                               Example                                           [S2915A1_P2P](NUCLEO-L053R8\Examples\S2915A1_P2P)                                                             [readme](NUCLEO-L053R8\Examples\S2915A1_P2P\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2868A1_Border_Router](NUCLEO-L152RE\Applications\Contiki-NG\S2868A1_Border_Router)                          [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2868A1_Border_Router\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2868A1_Serial_Sniffer](NUCLEO-L152RE\Applications\Contiki-NG\S2868A1_Serial_Sniffer)                        [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2868A1_Serial_Sniffer\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2868A1_UDP_Client](NUCLEO-L152RE\Applications\Contiki-NG\S2868A1_UDP_Client)                                [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2868A1_UDP_Client\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2868A1_UDP_Server](NUCLEO-L152RE\Applications\Contiki-NG\S2868A1_UDP_Server)                                [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2868A1_UDP_Server\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2868A2_Border_Router](NUCLEO-L152RE\Applications\Contiki-NG\S2868A2_Border_Router)                          [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2868A2_Border_Router\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2868A2_Serial_Sniffer](NUCLEO-L152RE\Applications\Contiki-NG\S2868A2_Serial_Sniffer)                        [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2868A2_Serial_Sniffer\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2868A2_UDP_Client](NUCLEO-L152RE\Applications\Contiki-NG\S2868A2_UDP_Client)                                [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2868A2_UDP_Client\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2868A2_UDP_Server](NUCLEO-L152RE\Applications\Contiki-NG\S2868A2_UDP_Server)                                [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2868A2_UDP_Server\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2915A1_Border_Router](NUCLEO-L152RE\Applications\Contiki-NG\S2915A1_Border_Router)                          [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2915A1_Border_Router\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2915A1_Serial_Sniffer](NUCLEO-L152RE\Applications\Contiki-NG\S2915A1_Serial_Sniffer)                        [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2915A1_Serial_Sniffer\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2915A1_UDP_Client](NUCLEO-L152RE\Applications\Contiki-NG\S2915A1_UDP_Client)                                [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2915A1_UDP_Client\readme.txt)
  NUCLEO-L152RE                                               Application                                       [S2915A1_UDP_Server](NUCLEO-L152RE\Applications\Contiki-NG\S2915A1_UDP_Server)                                [readme](NUCLEO-L152RE\Applications\Contiki-NG\S2915A1_UDP_Server\readme.txt)
  NUCLEO-L152RE                                               Example                                           [S2868A1_P2P](NUCLEO-L152RE\Examples\S2868A1_P2P)                                                             [readme](NUCLEO-L152RE\Examples\S2868A1_P2P\readme.txt)
  NUCLEO-L152RE                                               Example                                           [S2868A2_P2P](NUCLEO-L152RE\Examples\S2868A2_P2P)                                                             [readme](NUCLEO-L152RE\Examples\S2868A2_P2P\readme.txt)
  NUCLEO-L152RE                                               Example                                           [S2915A1_P2P](NUCLEO-L152RE\Examples\S2915A1_P2P)                                                             [readme](NUCLEO-L152RE\Examples\S2915A1_P2P\readme.txt)



## Development Toolchains and Compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V8.50.9
- RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.32.0
- STM32CubeIDE V1.6.1

## Supported Devices and Boards

- STM32F401RE devices and STM32F401RE-Nucleo board RevB
- STM32L053R8 devices and STM32L053R8-Nucleo board RevB
- STM32L152RE devices and STM32L152RE-Nucleo board RevB
- X-NUCLEO-S2868A1
- X-NUCLEO-S2868A2
- X-NUCLEO-S2915A1

</div>
:::

:::
:::

<footer class="sticky">
::: {.columns}
::: {.column width="95%"}
For complete documentation on **X-CUBE-SUBG2**,
visit: [www.st.com](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-expansion-packages/x-cube-subg2.html)
:::
::: {.column width="5%"}
<abbr title="Based on template cx566953 version 2.0">Info</abbr>
:::
:::
</footer>

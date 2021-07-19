/*
 * Copyright (c) 201, RISE SICS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

#include "contiki.h"
/**
* @mainpage
* @section Introduction
*	Contiki-NG_STM32_Library  is a middleware for STM32Cube and specifically
*   the X-CUBE-SUBG2 expansion software.
*	The key features of Contiki-NG_STM32_Library are:
*		- Middleware library with Contiki-NG OS and 6LoWPAN protocol stack (vers. 4.5)
*		- Support for mesh networking technology by the means of the standard RPL protocol
*		- Built-in support for STM32 L1 and F4 platforms.
*		- Easy portability across different STM32 MCU families thanks to STM32Cube / STM32CubeMX.
*		- Sample applications enabling communication via UDP over 6LoWPAN.
*       - Sample application to route packets out from the wireless 6LoWPAN network
*       - Sample application to capture RF packets and send them to Wireshark application.
*		- Free user-friendly license terms.
*		- Examples available for NUCLEO-F401RE and NUCLEO-L152RE boards.
*
*	This software provides a porting of the Contiki-NG OS for the STM32 platform, in the STM32Cube and STM32CubeMX
*	software environment, enabling the support for the S2-LP sub-1GHz radio transceiver.
*	The package also includes four sample applications that the developer can use to start
*	experimenting with the code. Two of these applications are supposed to be used together
*	since they implement the communication between two nodes. A sender node (@ref Udp_client)
*	periodically sends UDP packets to a receiver node (@ref Udp_server) configured as root of the RPL
*	6LoWPAN network. A third application implements a
*	6LoWPAN border router (@ref Border_router) functionality. The last application
*	(@ref Serial_sniffer) can be used together with WireShark to sniff and analyze on air packets.
*
*/

/** @defgroup Border_router
* @{
*/

/** @addtogroup Border_router
* @{
*/

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "RPL BR"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Declare and auto-start this file's process */
PROCESS(contiki_ng_br, "Contiki-NG Border Router");
AUTOSTART_PROCESSES(&contiki_ng_br);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(contiki_ng_br, ev, data)
{
  PROCESS_BEGIN();

#if BORDER_ROUTER_CONF_WEBSERVER
  PROCESS_NAME(webserver_nogui_process);
  process_start(&webserver_nogui_process, NULL);
#endif /* BORDER_ROUTER_CONF_WEBSERVER */

  LOG_INFO("Contiki-NG Border Router started\n");

  PROCESS_END();
}

/**
* @}
*/

/**
* @}
*/

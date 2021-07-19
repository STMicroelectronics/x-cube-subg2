/**
  ******************************************************************************
  * @file
  * @author  MCD Application Team
  * @version V2.0.0
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT(c) 2021 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  */
  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __RTE_COMPONENTS_H__
#define  __RTE_COMPONENTS_H__

/* Defines ------------------------------------------------------------------*/
/* STMicroelectronics.X-CUBE-SUBG2.4.2.0 */
#define CONTIKI_NG_STM32LIB_ENABLED
#define PLATFORM_HAS_LEDS 1
#define PLATFORM_HAS_BUTTON 1
#define S2868A2
#define ROUTING_CONF_RPL_LITE 1
#define ROUTING_CONF_RPL_CLASSIC 0
#define NETSTACK_CONF_NETWORK sicslowpan_driver
#define NETSTACK_CONF_WITH_IPV6 1
#define PROJECT_CONF_H 1
#define CONTIKI_NG_CORE_ENABLED
#define MAC_CONF_WITH_CSMA 1
#define MAC_CONF_WITH_TSCH 0
#define NETSTACK_CONF_MAC csma_driver
#define NETSTACK_CONF_FRAMER framer_802154
#define S2868A2_UDP_RX
#define USE_AUTOSTART_PROCESS
#define BUILD_WITH_SHELL 1

#endif /* __RTE_COMPONENTS_H__ */

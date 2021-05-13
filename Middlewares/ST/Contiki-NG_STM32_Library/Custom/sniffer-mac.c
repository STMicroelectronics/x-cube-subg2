/**
  ******************************************************************************
  * @file    sniffer-mac.c
  * @author  SRA Application Team
  * @brief   Almost void implementation of MAC layer just to send packets to sniffer.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Software License Agreement
  * SLA0055, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0055
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "net/mac/mac.h"
#include "net/netstack.h"
#include "sniffer.h"
#include "net/packetbuf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void
send(mac_callback_t sent, void *ptr)
{
  UNUSED(sent);
  UNUSED(ptr);
}
/*---------------------------------------------------------------------------*/
static void
input(void)
{
  sniffer_input((uint32_t) LINKTYPE_IEEE802_15_4_NOFCS, (void*) packetbuf_dataptr(), packetbuf_datalen());
}
/*---------------------------------------------------------------------------*/
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
static int
on(void)
{
  NETSTACK_RADIO.on();
  return 0;
}
/*---------------------------------------------------------------------------*/
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
static int
off(void)
{
  return 0;
}
/*---------------------------------------------------------------------------*/
static void
init(void)
{
  (void) on();
}
/*---------------------------------------------------------------------------*/
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
static int
max_payload(void)
{
  return 0;
}
/*--------------------------------------------------------------------------- */
/* Functions Definition ------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
const struct mac_driver sniffer_mac_driver = {
  "serial-sniffer-mac",
  init,
  send,
  input,
  on,
  off,
  max_payload,
};
/*---------------------------------------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

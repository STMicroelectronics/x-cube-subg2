/**
  ******************************************************************************
  * @file    radio-driver.c
  * @author  SRA Application Team
  * @brief   Source file for S2LP radio configuration/driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under ODE Software License Agreement
  * SLA0094, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0094
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "radio-driver.h"
#include "contiki.h"

#include "net/linkaddr.h"
#include "net/mac/framer/frame802154.h"
#include "sys/rtimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"

#include "net/netstack.h"
#include "net/packetbuf.h"
#include <stdio.h>
#include "main.h"
#include "s2lp_interface.h"
#include "s2lp.h"
#include "s2lp_types.h"

#include "s2lp_pktbasic.h"

#if RADIO_ADDRESS_FILTERING
#define ACK_LEN 3
#endif /*RADIO_ADDRESS_FILTERING*/

/**
* @defgroup ST_Radio
* @ingroup Contiki-NG_STM32_Library
* @{
*/

/**
* @addtogroup ST_Radio
* @ingroup Contiki-NG_STM32_Library
* @{
* @file Porting of the ST_Radio for the Contiki-NG OS
*/
/*---------------------------------------------------------------------------*/
/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "Radio"
#define LOG_LEVEL LOG_LEVEL_NONE
/*Change LOG_LEVEL for example to LOG_LEVEL_MAC to change it at runtime      */
/*---------------------------------------------------------------------------*/
#define RADIO_WAIT_TIMEOUT (100 * RTIMER_SECOND/1000)
#define BUSYWAIT_UNTIL(cond, max_time)                                 \
do {                                                                   \
  rtimer_clock_t t0;                                                   \
  t0 = RTIMER_NOW();                                                   \
  while(!(cond) && RTIMER_CLOCK_LT(RTIMER_NOW(), t0 + (max_time)));    \
} while(0)
/*---------------------------------------------------------------------------*/
/* The buffer which holds incoming data. */
static uint32_t rx_num_bytes =0;
static uint8_t radio_rxbuf[PACKETBUF_SIZE];
/*---------------------------------------------------------------------------*/
#define CLEAR_RXBUF()           (rx_num_bytes = 0)
#define IS_RXBUF_EMPTY()        (rx_num_bytes == 0)
#define IS_RXBUF_FULL()         (rx_num_bytes != 0)
/*---------------------------------------------------------------------------*/
/* transceiver state. */
#define ON     1
#define OFF    0
/*---------------------------------------------------------------------------*/
static volatile unsigned int radio_on = OFF;
static volatile uint8_t receiving_packet = 0;
static volatile uint8_t transmitting_packet = 0;
static volatile uint8_t pending_packet = 0;
static volatile uint8_t packet_is_prepared = 0;
static radio_value_t last_packet_rssi = 0 ;
static packetbuf_attr_t last_packet_lqi = 0 ;
static int interrupt_callback_in_progress = 0;
static int interrupt_callback_wants_poll = 0;
/*---------------------------------------------------------------------------*/
static volatile uint32_t last_packet_timestamp = 0;
/*---------------------------------------------------------------------------*/
static int csma_tx_threshold = RSSI_TX_THRESHOLD;
/* Poll mode disabled by default */
/*static*/ uint8_t polling_mode = 0;
/* (Software) frame filtering enabled by default */
#if RADIO_ADDRESS_FILTERING
static uint8_t auto_pkt_filter = 1;
#else /*!RADIO_ADDRESS_FILTERING*/
static uint8_t auto_pkt_filter = 0;
#endif /*RADIO_ADDRESS_FILTERING*/
/* (Software) autoack is enabled by default (CSMA MAC will send by default) */
static uint8_t radio_send_auto_ack = 1;
#if RADIO_HW_CSMA
static uint8_t csma_enabled = 1;
#else /*!RADIO_HW_CSMA*/
static uint8_t csma_enabled = 0;
#endif /*RADIO_HW_CSMA*/
static int conf_channel = CHANNEL_NUMBER;
static int conf_tx_power = (int)POWER_DBM;//@TODO: validate
/*---------------------------------------------------------------------------*/
/**
* @brief GPIO structure fitting
*/
SGpioInit xGpioIRQ={
  S2LP_GPIO_3,
  RADIO_GPIO_MODE_DIGITAL_OUTPUT_LP,
  RADIO_GPIO_DIG_OUT_IRQ
};
/*---------------------------------------------------------------------------*/
/**
* @brief Radio structure fitting
*/
SRadioInit xRadioInit = {
  BASE_FREQUENCY,
  MODULATION_SELECT,
  DATARATE,
  FREQ_DEVIATION,
  BANDWIDTH
};
/*---------------------------------------------------------------------------*/
/**
* @brief Packet Basic structure fitting
*/
PktBasicInit xBasicInit={
  PREAMBLE_LENGTH,
  SYNC_LENGTH,
  SYNC_WORD,
  VARIABLE_LENGTH,
  EXTENDED_LENGTH_FIELD,
  CRC_MODE,
  EN_ADDRESS,
  EN_FEC,
  EN_WHITENING
};

#if RADIO_ADDRESS_FILTERING
PktBasicAddressesInit xAddressInit={
  EN_FILT_MY_ADDRESS,
  0x00, //-> To be filled later on with latest LL address byte
  EN_FILT_MULTICAST_ADDRESS,
  MULTICAST_ADDRESS,
  EN_FILT_BROADCAST_ADDRESS,
  BROADCAST_ADDRESS
};
#endif /*RADIO_ADDRESS_FILTERING*/
/*---------------------------------------------------------------------------*/
/*Flags declarations*/
volatile FlagStatus xTxDoneFlag = RESET;
/*---------------------------------------------------------------------------*/
#if RADIO_HW_CSMA
#define PERSISTENT_MODE_EN              S_DISABLE
#define CS_PERIOD                       CSMA_PERIOD_64TBIT
#define CS_TIMEOUT                      3
#define MAX_NB                          5
#define BU_COUNTER_SEED                 0xFA21
#define CU_PRESCALER                    32

// refer to radio-driver.h for RSSI Thresholds

/* Radio CSMA config */
SCsmaInit  xCsmaInit={
  PERSISTENT_MODE_EN,
  CS_PERIOD,
  CS_TIMEOUT,
  MAX_NB,
  BU_COUNTER_SEED,
  CU_PRESCALER
};

SRssiInit xSRssiInit = {
  .cRssiFlt = 14,
  .xRssiMode = RSSI_STATIC_MODE,
  .cRssiThreshdBm = RSSI_TX_THRESHOLD
};
#endif /*RADIO_HW_CSMA*/
/*---------------------------------------------------------------------------*/
PROCESS(subGHz_radio_process, "subGHz radio driver");
/*---------------------------------------------------------------------------*/
/*Static functions that implement Contiki-NG Radio API                          */
static int Radio_init(void);
static int Radio_prepare(const void *payload, unsigned short payload_len);
static int Radio_transmit(unsigned short payload_len);
static int Radio_send(const void *data, unsigned short len);
static int Radio_read(void *buf, unsigned short bufsize);
static int Radio_channel_clear(void);
static int Radio_receiving_packet(void);
static int Radio_pending_packet(void);
static int Radio_on(void);
static int Radio_off(void);
static radio_result_t Radio_get_value(radio_param_t parameter, radio_value_t *ret_value);
static radio_result_t Radio_set_value(radio_param_t parameter, radio_value_t input_value);
static radio_result_t Radio_get_object(radio_param_t parameter, void *destination, size_t size);
static radio_result_t Radio_set_object(radio_param_t parameter, const void *source, size_t size);
/*---------------------------------------------------------------------------*/
static void radio_set_polling_mode(uint8_t enable);
static int Radio_read_from_fifo (uint8_t* buf, unsigned short bufsize);
static S2LPState radio_refresh_status(void);
/*---------------------------------------------------------------------------*/
/* Radio Driver Structure as per Contiki-NG definition                          */
const struct radio_driver subGHz_radio_driver =
{
  Radio_init,
  Radio_prepare,
  Radio_transmit,
  Radio_send,
  Radio_read,
  Radio_channel_clear,
  Radio_receiving_packet,
  Radio_pending_packet,
  Radio_on,
  Radio_off,
  Radio_get_value,
  Radio_set_value,
  Radio_get_object,
  Radio_set_object
};
/*---------------------------------------------------------------------------*/
/**
* @brief  radio_refresh_status
* 	refresh and returns S2-LP status
* @param  none
* @retval S2LPState S2-LP status
*/
static S2LPState radio_refresh_status(void)
{
  S2LP_RefreshStatus();
  return g_xStatus.MC_STATE;
}
/*---------------------------------------------------------------------------*/
/**
* @brief  radio_print_status
* 	prints to the UART the status of the radio
* @param  none
* @retval None
*/
static void
radio_print_status(void)
{
  S2LPState s = radio_refresh_status();
  if(s == MC_STATE_STANDBY)
  {
    printf("radio-driver: MC_STATE_STANDBY\n");
  }
  else if(s == MC_STATE_SLEEP)
  {
    printf("radio-driver: MC_STATE_SLEEP\n");
  }
  else if(s == MC_STATE_READY)
  {
    printf("radio-driver: MC_STATE_READY\n");
  }
  else if(s == MC_STATE_TX)
  {
    printf("radio-driver: MC_STATE_TX\n");
  }
  else if(s == MC_STATE_RX)
  {
    printf("radio-driver: MC_STATE_RX\n");
  }
  else if(s == MC_STATE_SLEEP_NOFIFO)
  {
    printf("radio-driver: MC_STATE_SLEEP_NOFIFO\n");
  }
  else if(s == MC_STATE_SYNTH_SETUP)
  {
    printf("radio-driver: MC_STATE_SYNTH_SETUP\n");
  }
  else
  {
    printf("radio-driver: status: %X\n", (uint8_t) s);
  }
}
/*---------------------------------------------------------------------------*/
/**
* @brief  radio_set_ready_state
* 	sets the state of the radio to READY
* @param  none
* @retval None
*/
void radio_set_ready_state(void)
{
  LOG_DBG("READY IN\n");

  RADIO_IRQ_DISABLE();
  S2LP_GPIO_IrqClearStatus();

#if RADIO_SNIFF_MODE
  S2LP_GPIO_IrqConfig(RX_DATA_READY,S_DISABLE);
  S2LP_TIM_LdcrMode(S_DISABLE);
  S2LP_TIM_FastRxTermTimer(S_DISABLE);
#endif /*RADIO_SNIFF_MODE*/

  if (radio_refresh_status()==MC_STATE_RX){
    S2LP_CMD_StrobeSabort();
  } else {
      S2LP_CMD_StrobeReady();
  }
  BUSYWAIT_UNTIL(radio_refresh_status()==MC_STATE_READY, RADIO_WAIT_TIMEOUT);

  S2LP_CMD_StrobeCommand(CMD_FLUSHRXFIFO);
  receiving_packet = 0;
  pending_packet=0;
  CLEAR_RXBUF();

  S2LP_GPIO_IrqClearStatus();
  RADIO_IRQ_ENABLE();

    //radio_print_status() ;
  LOG_DBG("READY OUT\n");
}
/*---------------------------------------------------------------------------*/
/**
* @brief  Radio_init
* 	Initializes the radio
* @param  none
* @retval int result(0 == success)
*/
static int
Radio_init(void)
{
  LOG_DBG("RADIO INIT IN\n");

  S2LPInterfaceInit();

  /* Configures the Radio library */
  S2LP_RADIO_SetXtalFrequency(XTAL_FREQUENCY);

  S2LP_CMD_StrobeSres();

  /* S2LP Radio config */
  S2LP_RADIO_Init(&xRadioInit);

  S2LP_RADIO_SetChannel(CHANNEL_NUMBER);
  S2LP_RADIO_SetChannelSpace(CHANNEL_SPACE);

  if(!S2LP_ManagementGetRangeExtender()) //Also check similar code in s2lp_interface.c
  {
    /* if we haven't an external PA, use the library function */
    S2LP_RADIO_SetPALeveldBm(POWER_INDEX,POWER_DBM);
  }
  else
  {
    /* in case we are using the PA board, the S2LP_RADIO_SetPALeveldBm will be
    not functioning because the output power is affected by the amplification
    of this external component. Set the raw register. */
    uint8_t paLevelValue=0x25; /* for example, this value will give 23dBm about */
    S2LP_WriteRegister(PA_POWER8_ADDR, 1, &paLevelValue);
  }
  S2LP_RADIO_SetPALevelMaxIndex(POWER_INDEX);

/* Configures the Radio packet handler part*/
  S2LP_PCKT_BASIC_Init(&xBasicInit);

#if RADIO_ADDRESS_FILTERING
  S2LP_PCKT_HNDL_SetAutoPcktFilter(S_ENABLE);
  S2LP_PCKT_HNDL_SelectSecondarySync(S_DISABLE);
  xAddressInit.cMyAddress = linkaddr_node_addr.u8[LINKADDR_SIZE-1];
  S2LP_PCKT_BASIC_AddressesInit(&xAddressInit);
  LOG_DBG("Node Source address %2X\n", xAddressInit.cMyAddress);
#endif /*RADIO_ADDRESS_FILTERING*/

#if RADIO_HW_CSMA
  S2LP_CSMA_Init(&xCsmaInit);
  S2LP_RADIO_QI_RssiInit(&xSRssiInit);
#endif /*RADIO_HW_CSMA*/

  /* Enable the following interrupt sources, routed to GPIO */
  S2LP_GPIO_IrqDeInit(NULL);
  S2LP_GPIO_IrqClearStatus();
  S2LP_GPIO_IrqConfig(TX_DATA_SENT, S_ENABLE);
  S2LP_GPIO_IrqConfig(RX_DATA_READY, S_ENABLE);

#if RADIO_SNIFF_MODE
  S2LP_GPIO_IrqConfig(VALID_SYNC,S_DISABLE);
  S2LP_GPIO_IrqConfig(RX_DATA_DISC, S_DISABLE);
  S2LP_GPIO_IrqConfig(RX_TIMEOUT, S_DISABLE);
#else /*!RADIO_SNIFF_MODE*/
  S2LP_GPIO_IrqConfig(VALID_SYNC, S_ENABLE);
  S2LP_GPIO_IrqConfig(RX_DATA_DISC, S_ENABLE);
#endif /*RADIO_SNIFF_MODE*/

#if RADIO_HW_CSMA
  S2LP_GPIO_IrqConfig(MAX_BO_CCA_REACH , S_ENABLE);
  S2LP_CSMA_Enable(S_DISABLE); //It will be enabled in TX
#else /*!RADIO_HW_CSMA*/
  S2LP_GPIO_IrqConfig(MAX_BO_CCA_REACH , S_DISABLE);
#endif /*RADIO_HW_CSMA*/

#if RADIO_SNIFF_MODE
  SRssiInit xSRssiInit = {
    .cRssiFlt = 14,
    .xRssiMode = RSSI_STATIC_MODE,
    .cRssiThreshdBm = RSSI_TX_THRESHOLD
  };
  S2LP_RADIO_QI_RssiInit(&xSRssiInit);

  S2LP_TIM_SetWakeUpTimerUs(1000*MIN_PERIOD_WAKEUP_MS); //12 ms
  /* set the rx timeout */
  S2LP_TIM_SetRxTimerUs(1000*RX_TIMEOUT_MS); //30 ms

  S2LP_TIM_SleepB(S_ENABLE);

  /* enable LDC mode, FAST RX TERM and start Rx */
  S2LP_TIM_LdcrMode(S_ENABLE);
  /* enable the fast rx timer */
  S2LP_TIM_FastRxTermTimer(S_ENABLE);
#else /*!RADIO_SNIFF_MODE*/

  S2LP_RADIO_QI_SetRssiThreshdBm(RSSI_RX_THRESHOLD);
  SET_INFINITE_RX_TIMEOUT();
  /* Configure Radio */
  S2LP_PCKT_HNDL_SetRxPersistentMode(S_ENABLE);
#endif /*RADIO_SNIFF_MODE*/

  CLEAR_RXBUF();

  /* Configure the radio to route the IRQ signal to its GPIO 3 */
  S2LP_GPIO_Init(&xGpioIRQ);

  radio_set_polling_mode(polling_mode);

  /* This is ok for normal or SNIFF (RX command triggers the LDC in fast RX termination mode) */
  S2LP_CMD_StrobeRx();
  radio_on = ON;

  process_start(&subGHz_radio_process, NULL);

  LOG_DBG("Radio init done\n");
  return 0;
}

/*---------------------------------------------------------------------------*/
/**
* @brief  Radio_prepare
* 	prepares the radio for transmission
* @param  none
* @retval int result(0 == success)
*/
static int
Radio_prepare(const void *payload, unsigned short payload_len)
{
  LOG_DBG("Radio: prepare %u\n", payload_len);
  uint8_t tmpbuff[PACKETBUF_SIZE]; //@TODO: see below
  packet_is_prepared = 0;

  /* Checks if the payload length is supported: actually this can't happen, by
   * system desigm, but it is safer to have this for sanity check.
   */
  if(payload_len > PACKETBUF_SIZE)
  {
    LOG_DBG("Payload len too big (> %d), error.\n", PACKETBUF_SIZE);
    return RADIO_TX_ERR;
  }

  /* Sets the length of the packet to send */
  RADIO_IRQ_DISABLE();

  radio_set_ready_state();
  if ( radio_refresh_status()!= MC_STATE_READY) {
    LOG_ERR("Set Ready State failed.\n");
    radio_print_status();
    S2LP_CMD_StrobeSabort();
#if RADIO_SNIFF_MODE
    S2LP_TIM_LdcrMode(S_ENABLE);
    S2LP_TIM_FastRxTermTimer(S_ENABLE);
    S2LP_GPIO_IrqConfig(RX_DATA_READY,S_ENABLE);
#endif /*RADIO_SNIFF_MODE*/
    S2LP_CMD_StrobeRx();

    RADIO_IRQ_ENABLE();
    return   RADIO_TX_ERR;
  }

#if RADIO_ADDRESS_FILTERING
  const linkaddr_t *addr;
  if(auto_pkt_filter){
    if (payload_len == ACK_LEN || packetbuf_holds_broadcast()) {
      LOG_DBG("Preparing to send to broadcast (%02X) address\n", BROADCAST_ADDRESS);
      S2LP_PCKT_HNDL_SetRxSourceReferenceAddress(BROADCAST_ADDRESS);
    } else {
      addr = packetbuf_addr(PACKETBUF_ADDR_RECEIVER);
      LOG_DBG("Preparing to send to %2X address\n", addr->u8[LINKADDR_SIZE-1]);
      S2LP_PCKT_HNDL_SetRxSourceReferenceAddress(addr->u8[LINKADDR_SIZE-1]);
    }
  }
#endif /*RADIO_ADDRESS_FILTERING*/

  S2LP_CMD_StrobeCommand(CMD_FLUSHTXFIFO);

  S2LP_PCKT_BASIC_SetPayloadLength(payload_len);
  //@TODO change IO implementation to avoid the copy here
  memcpy(tmpbuff, payload, payload_len);

  /* Currently does no happen since S2LP_RX_FIFO_SIZE == MAX_PACKET_LEN
   * also note that S2LP_RX_FIFO_SIZE == S2LP_TX_FIFO_SIZE
   */
  if(payload_len > S2LP_TX_FIFO_SIZE)
  {
    LOG_DBG("Payload bigger than FIFO size.'n");
  }
  else
  {
    S2LP_WriteFIFO(payload_len, (uint8_t *)tmpbuff);
//    S2LP_WriteFIFO(payload_len, (uint8_t *)payload);
    packet_is_prepared = 1;
  }

  RADIO_IRQ_ENABLE();

  LOG_DBG("PREPARE OUT\n");

  return RADIO_TX_OK;
}
/*---------------------------------------------------------------------------*/
/**
* @brief  Radio_transmit
* 	transimts a packet with the Sub GHz radio
* @param  unsigned short payload_len
* @retval int result(0 == success)
*/
static int
Radio_transmit(unsigned short payload_len)
{
  int retval = RADIO_TX_ERR;
  uint8_t radio_state = radio_on;

  /* This function blocks until the packet has been transmitted */
  LOG_DBG("TRANSMIT IN\n");
  if(!packet_is_prepared)
  {
    LOG_DBG("Radio TRANSMIT: ERROR, packet is NOT prepared.\n");
    return RADIO_TX_ERR;
  }

  if(radio_on == OFF) {
    Radio_on();
  }

  RADIO_IRQ_DISABLE();

  transmitting_packet = 1;
  S2LP_GPIO_IrqClearStatus();
  RADIO_IRQ_ENABLE();

#if RADIO_HW_CSMA
  if (csma_enabled) { //@TODO: add an API to enable/disable CSMA
	S2LP_CSMA_Enable(S_ENABLE);
	S2LP_RADIO_QI_SetRssiThreshdBm(RSSI_TX_THRESHOLD);
	retval = RADIO_TX_COLLISION;
  }
#endif  /*RADIO_HW_CSMA*/

  xTxDoneFlag = RESET;
  S2LP_CMD_StrobeTx();
   /* wait for TX done */
  if (polling_mode) { //@TODO: To be validated
    while(!xTxDoneFlag) {
      uint8_t tmp;
      BUSYWAIT_UNTIL(0, 2 * RTIMER_SECOND/1000);
      S2LP_ReadRegister(TX_FIFO_STATUS_ADDR, 1, &tmp);
      if ((tmp & NELEM_TXFIFO_REGMASK) == 0 ){
        xTxDoneFlag = SET;
        transmitting_packet = 0;
      }
    }
    BUSYWAIT_UNTIL(0, 2 * RTIMER_SECOND/1000); //@TODO: we need a delay here, validate.
  } else {
	/*To be on the safe side we put a timeout. */
    BUSYWAIT_UNTIL(xTxDoneFlag, 10* RADIO_WAIT_TIMEOUT);
  }
  if (transmitting_packet){
    S2LP_CMD_StrobeSabort();
    if (xTxDoneFlag==RESET) {
      LOG_ERR("Packet not transmitted: TIMEOUT\n\r");
    } else {
      LOG_ERR("Packet not transmitted: ERROR\n\r");
    }
    transmitting_packet = 0;
  } else {
    retval = RADIO_TX_OK;
  }
  xTxDoneFlag = RESET;

#if RADIO_HW_CSMA
  if (csma_enabled) {
    S2LP_CSMA_Enable(S_DISABLE);
#  if !RADIO_SNIFF_MODE
    S2LP_RADIO_QI_SetRssiThreshdBm(RSSI_RX_THRESHOLD);
#  endif /*!RADIO_SNIFF_MODE*/
  }
#endif /*RADIO_HW_CSMA*/

  if(!IS_RXBUF_EMPTY()){
    CLEAR_RXBUF();
  }

  RADIO_IRQ_DISABLE();

#if RADIO_SNIFF_MODE
  S2LP_TIM_LdcrMode(S_ENABLE);
  S2LP_TIM_FastRxTermTimer(S_ENABLE);
  S2LP_GPIO_IrqConfig(RX_DATA_READY,S_ENABLE);
#endif /*RADIO_SNIFF_MODE*/

  S2LP_CMD_StrobeRx();
  BUSYWAIT_UNTIL(radio_refresh_status() == MC_STATE_RX
#if RADIO_SNIFF_MODE
                 || radio_refresh_status() == MC_STATE_SLEEP_NOFIFO
#endif /*RADIO_SNIFF_MODE*/
                 , RADIO_WAIT_TIMEOUT);

  packet_is_prepared = 0;

  S2LP_CMD_StrobeCommand(CMD_FLUSHTXFIFO);

  S2LP_GPIO_IrqClearStatus();
  RADIO_IRQ_ENABLE();

  LOG_DBG("TRANSMIT OUT\n");

  if(radio_state==OFF){
    /*If the radio was OFF before transmitting the packet, we must turn it
    * OFF (legacy for ContikiMAC like upper layer) */
    Radio_off();
  }

  return retval;
}
/*---------------------------------------------------------------------------*/
/**
* @brief  Radio_send
* 	Calls Radio_prepare and Radio_Transmit in a single function
* @param  const void *payload
* @param  unsigned short payload_len
* @retval int result(0 == success)
*/
static int Radio_send(const void *payload, unsigned short payload_len)
{
  LOG_DBG("Radio Send\r\n");

  if(Radio_prepare(payload, payload_len) != RADIO_TX_OK)
  {
#if RADIO_SNIFF_MODE
    S2LP_TIM_LdcrMode(S_ENABLE);
    S2LP_TIM_FastRxTermTimer(S_ENABLE);
    S2LP_GPIO_IrqConfig(RX_DATA_READY,S_ENABLE);
#endif /*RADIO_SNIFF_MODE*/
    S2LP_CMD_StrobeRx();
    LOG_DBG("PREPARE FAILED\n");
    return RADIO_TX_ERR;
  }
  return Radio_transmit(payload_len);
}
/*---------------------------------------------------------------------------*/
/**
* @brief  Radio_read
* 	reads a packet received with the subGHz radio
* @param  void *buf, unsigned short bufsize
* @retval int bufsize
*/
static int Radio_read(void *buf, unsigned short bufsize)
{
    int retval = 0;
    S2LPIrqs x_irq_status;

    if (polling_mode) {
       S2LP_GPIO_IrqGetStatus(&x_irq_status);
       if (x_irq_status.IRQ_RX_DATA_READY)
         retval = Radio_read_from_fifo (buf, bufsize);
    } else if (pending_packet && (rx_num_bytes!=0)) {
      if (rx_num_bytes <= bufsize) {
        memcpy(buf, radio_rxbuf, rx_num_bytes);
        retval = rx_num_bytes;
      } else {
        LOG_DBG("Buf too small (%d bytes to hold %lu bytes)\n", bufsize,rx_num_bytes );
      }
      pending_packet = 0;
    }
    /* RX command - to ensure the device will be ready for the next reception */
#if RADIO_SNIFF_MODE
      S2LP_CMD_StrobeSleep();
#else /*!RADIO_SNIFF_MODE*/
      S2LP_CMD_StrobeRx();
#endif /*RADIO_SNIFF_MODE*/
    CLEAR_RXBUF();
    LOG_DBG("READ OUT: %d\n", retval);
    return retval;
}
/*---------------------------------------------------------------------------*/
/**
* @brief  Radio_channel_clear
* 	checks the channel
* @param  none
* @retval int result
*/
static int
Radio_channel_clear(void)
{
  float rssi_value;
  /* Local variable used to memorize the S2LP state */
  uint8_t radio_state = radio_on;

  LOG_DBG("CHANNEL CLEAR IN\n");

  if(radio_on == OFF){
    /* Wakes up the Radio */
    Radio_on();
  }
  rssi_value = S2LP_RADIO_QI_GetRssidBmRun();
  int ret = (rssi_value<RSSI_TX_THRESHOLD)?1:0;

  /* Puts the S2LP in its previous state */
  if(radio_state==OFF){
    Radio_off();
  }

  return ret;
}
/*---------------------------------------------------------------------------*/
/**
* @brief  Radio_receiving_packet
* 	checks for receiving packet
* @param  none
* @retval int result
*/
static int
Radio_receiving_packet(void)
{
  S2LPIrqs x_irq_status;

  if (polling_mode) {
       S2LP_GPIO_IrqGetStatus(&x_irq_status);
       receiving_packet = (x_irq_status.IRQ_VALID_SYNC) ;
  }

  return receiving_packet;
}
/*---------------------------------------------------------------------------*/
/**
* @brief  Radio_pending_packet
* 	checks for pending packet
* @param  none
* @retval int result
*/
static int
Radio_pending_packet(void)
{
  S2LPIrqs x_irq_status;
  //LOG_DBG("RADIO PENDING PACKET\n");
  if (polling_mode) {
       S2LP_GPIO_IrqGetStatus(&x_irq_status);
       pending_packet = (x_irq_status.IRQ_RX_DATA_READY) ;
  }

  return pending_packet;
}
/*---------------------------------------------------------------------------*/
/**
* @brief  Radio_off
* 	turns off the Sub-GHz radio
* @param  none
* @retval int result(0 == success)
*/
static int
Radio_off(void)
{
  LOG_DBG("Radio: ->off\n");

  if(radio_on == ON)
  {
    /* Disables the mcu to get IRQ from the RADIO */
    RADIO_IRQ_DISABLE();  //Mind that it will be enabled only in the ON

#if RADIO_SNIFF_MODE
    S2LP_TIM_LdcrMode(S_DISABLE);
    S2LP_TIM_FastRxTermTimer(S_DISABLE);
    S2LP_CMD_StrobeReady();
    S2LP_CMD_StrobeRx();
#endif /*RADIO_SNIFF_MODE*/

    /* first stop rx/tx */
    S2LP_CMD_StrobeSabort();

    /* Clear any pending irqs */
    S2LP_GPIO_IrqClearStatus();

#if RADIO_SNIFF_MODE
    S2LP_CMD_StrobeReady();
#endif /*RADIO_SNIFF_MODE*/
    BUSYWAIT_UNTIL(radio_refresh_status() == MC_STATE_READY, RADIO_WAIT_TIMEOUT);

    if(radio_refresh_status() != MC_STATE_READY)
    {
      LOG_ERR("Radio: failed off->ready\n");
      return 1;
    }
    /* Puts the Radio in STANDBY */
    S2LP_CMD_StrobeStandby();
    BUSYWAIT_UNTIL(radio_refresh_status() == MC_STATE_STANDBY,RADIO_WAIT_TIMEOUT);

    if(radio_refresh_status() != MC_STATE_STANDBY)
    {
      LOG_ERR("Radio: failed off->stdby\n");
      return 1;
    }

    radio_on = OFF;
    CLEAR_RXBUF();
  }

  LOG_DBG("Radio: off.\n");
  //radio_print_status();
  return 0;
}
/*---------------------------------------------------------------------------*/
/**
* @brief  Radio_on
* 	turns on the Sub-GHz radio
* @param  none
* @retval int result(0 == success)
*/
static int Radio_on(void)
{
  LOG_DBG("Radio: on\n");

  if(radio_on == OFF) {
#if RADIO_SNIFF_MODE
    S2LP_TIM_LdcrMode(S_ENABLE);
    S2LP_TIM_FastRxTermTimer(S_ENABLE);
#endif /*RADIO_SNIFF_MODE*/
    radio_set_ready_state();
    S2LP_FIFO_MuxRxFifoIrqEnable(S_ENABLE);
    S2LP_CMD_StrobeRx();
    radio_on = ON;
    RADIO_IRQ_ENABLE(); //--> Coming from OFF, IRQ ARE DISABLED.
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
/**
* @brief  main Radio Contiki-NG PROCESS
*/
PROCESS_THREAD(subGHz_radio_process, ev, data)
{
  PROCESS_BEGIN();
  LOG_DBG("Radio: process started\n");

  while(1) {
    int len;

    PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
    LOG_DBG("Radio: polled\n");

    packetbuf_clear();
    len = Radio_read(packetbuf_dataptr(), PACKETBUF_SIZE);

    if(len > 0) {
      packetbuf_set_datalen(len);

      LOG_DBG("Calling MAC.Input(%d)\n", len);
      NETSTACK_MAC.input();
    }

    if(!IS_RXBUF_EMPTY()) {
      LOG_DBG("After MAC.input, RX BUF is not empty.\r\n");
      process_poll(&subGHz_radio_process);
    }

    if(interrupt_callback_wants_poll) {
      LOG_DBG("interrupt_callback_wants_poll\n");
      /*This part of code seems not to happen, so it is not tested.*/
      Radio_interrupt_callback();
      if(radio_refresh_status() == MC_STATE_READY)
      {
        S2LP_FIFO_MuxRxFifoIrqEnable(S_ENABLE);
#if RADIO_SNIFF_MODE
        /*As per previous comment, this is not tested. */
    	S2LP_CMD_StrobeSleep();
#else /*!RADIO_SNIFF_MODE*/
    	S2LP_CMD_StrobeRx();
    	BUSYWAIT_UNTIL(radio_refresh_status() == MC_STATE_RX, 1 * RTIMER_SECOND/1000);
#endif /*RADIO_SNIFF_MODE*/
      }
    }
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
static int
radio_get_rssi(void)
{
	/*Using default GetRssidBm() API */
  return (int) S2LP_RADIO_QI_GetRssidBm();
}
/*---------------------------------------------------------------------------*/
static int
radio_get_channel(void)
{
	uint8_t register_channel;
  /*Next statement is mainly for debugging purpose, it can be commented out. */
  register_channel = S2LP_RADIO_GetChannel();
  if (register_channel != conf_channel) {
    LOG_WARN("Warning retrieved channel %d != saved channel %d\n", register_channel, conf_channel );
	conf_channel = register_channel;
  }

  return register_channel;
}
/*---------------------------------------------------------------------------*/
static void
radio_set_channel(int c)
{
	/*Channel value has been validated in the calling function. */
  LOG_DBG("SET CHANNEL %d.\r\n", c);

  conf_channel = c;
  S2LP_RADIO_SetChannel(conf_channel);
  S2LP_RADIO_SetChannelSpace(CHANNEL_SPACE);
}
/*---------------------------------------------------------------------------*/
static void
radio_set_txpower(int8_t power)
{
	/*Power value is validated in the calling function */
  conf_tx_power = power;

  S2LP_RADIO_SetPALeveldBm(POWER_INDEX,conf_tx_power);
}
/*--------------------------------------------------------------------------*/
static int
radio_get_txpower(void)
{
  int register_tx_power;
  register_tx_power = S2LP_RADIO_GetPALeveldBm(POWER_INDEX);
  if (register_tx_power != conf_tx_power) {
    LOG_WARN("Warning retrieved tx power %d != saved tx power %d\n", register_tx_power, conf_tx_power );
	conf_tx_power = register_tx_power;
  }
  return (int)register_tx_power;
}
/*---------------------------------------------------------------------------*/
static void
radio_set_auto_pkt_filter(uint8_t enable)
{
  LOG_DBG("Set Auto Packet Filtering %d\n", enable);
  auto_pkt_filter = enable;
  S2LP_PCKT_HNDL_SetAutoPcktFilter(enable?S_ENABLE:S_DISABLE);
}
/*---------------------------------------------------------------------------*/
static void
radio_set_auto_ack(uint8_t enable)
{
  /* Actually CSMA MAC will send anyway, TSCH (that needs them disabled) will
     not send in any case since the implementation for Packet Basic is done
	 in software */
  radio_send_auto_ack = enable;
}
/*---------------------------------------------------------------------------*/
static void
radio_set_polling_mode(uint8_t enable)
{
  /* Polling Mode  must be fully validated. */
  LOG_DBG("POLLING MODE is %s.\r\n", enable?"ENABLED":"DISABLED");
  polling_mode = enable;
  if(polling_mode) {
    /* Disable interrupts */
    S2LP_GPIO_IrqConfig(RX_DATA_READY,S_DISABLE);
    S2LP_GPIO_IrqConfig(TX_DATA_SENT,S_DISABLE);
    S2LP_GPIO_IrqConfig(VALID_SYNC,S_DISABLE);
  } else {
    /* Initialize and enable interrupts */
    S2LP_GPIO_IrqConfig(RX_DATA_READY,S_ENABLE);
    S2LP_GPIO_IrqConfig(TX_DATA_SENT,S_ENABLE);
    S2LP_GPIO_IrqConfig(VALID_SYNC,S_ENABLE);
  }
}
/*---------------------------------------------------------------------------*/
/* Enable or disable CSMA Feature (check also RADIO_HW_CSMA macro) */
static void
radio_set_csma(uint8_t enable)
{
  //@TODO: validate
  csma_enabled = enable;
}
/*---------------------------------------------------------------------------*/
static rtimer_clock_t
radio_get_packet_timestamp(void)
{
//@TODO: This is to be validated.
  LOG_DBG("radio_get_packet_timestamp: %d\r\n", last_packet_timestamp);

  return last_packet_timestamp;
}
/*---------------------------------------------------------------------------*/
static radio_result_t
Radio_get_value(radio_param_t parameter, radio_value_t *ret_value)
{
  radio_result_t get_value_result;
  get_value_result = RADIO_RESULT_NOT_SUPPORTED;

  if(ret_value == NULL) {
    return RADIO_RESULT_INVALID_VALUE;
  }

  if (parameter == RADIO_PARAM_POWER_MODE) {
	  if (radio_on == ON) {
		  *ret_value = RADIO_POWER_MODE_ON;
	  } else {
		  *ret_value = RADIO_POWER_MODE_OFF;
	  }
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_PARAM_CHANNEL) {
	  *ret_value = radio_get_channel();
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_PARAM_RX_MODE) {
	  *ret_value = 0x00;
      if(polling_mode) {
        *ret_value |= RADIO_RX_MODE_POLL_MODE;
      }
	  if(radio_send_auto_ack) {
        *ret_value |= RADIO_RX_MODE_AUTOACK;
      }
      if(auto_pkt_filter) {
        *ret_value |= RADIO_RX_MODE_ADDRESS_FILTER;
      }
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_PARAM_TX_MODE) {
	  *ret_value = 0x00;
      if(csma_enabled) {
        *ret_value |= RADIO_TX_MODE_SEND_ON_CCA;
      }
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_PARAM_TXPOWER) {
      *ret_value = radio_get_txpower();
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_PARAM_RSSI) {
      *ret_value = radio_get_rssi();
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_PARAM_LAST_RSSI) {
      *ret_value = last_packet_rssi;
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_PARAM_CCA_THRESHOLD) {
      *ret_value = csma_tx_threshold;
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_CONST_CHANNEL_MIN) {
      *ret_value = CHANNEL_NUMBER_MIN;
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_CONST_CHANNEL_MAX) {
      *ret_value = CHANNEL_NUMBER_MAX;
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_CONST_TXPOWER_MIN) {
      *ret_value = RADIO_POWER_DBM_MIN;
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_CONST_TXPOWER_MAX) {
      *ret_value = RADIO_POWER_DBM_MAX;
	  get_value_result = RADIO_RESULT_OK;
  } else if (parameter == RADIO_CONST_MAX_PAYLOAD_LEN) {
	  /*TODO: check if this value is correct.*/
      *ret_value = MAX_PACKET_LEN;
	  get_value_result = RADIO_RESULT_OK;
  }

  return get_value_result;
}

/*---------------------------------------------------------------------------*/
static radio_result_t
Radio_set_value(radio_param_t parameter, radio_value_t input_value)
{
  radio_result_t set_value_result;
  set_value_result = RADIO_RESULT_NOT_SUPPORTED;

  if (parameter == RADIO_PARAM_POWER_MODE){
	  switch(input_value) {
		  case RADIO_POWER_MODE_ON:
		  Radio_on();
		  set_value_result = RADIO_RESULT_OK;
		  break;
		  case RADIO_POWER_MODE_OFF:
		  Radio_off();
		  set_value_result = RADIO_RESULT_OK;
		  break;
		  default:
		  set_value_result = RADIO_RESULT_INVALID_VALUE;
		  break;
	  }
  } else if (parameter == RADIO_PARAM_CHANNEL){
      if((input_value >= CHANNEL_NUMBER_MIN) && (input_value <= CHANNEL_NUMBER_MAX)) {
        radio_set_channel(input_value);
		set_value_result = RADIO_RESULT_OK;
      } else {
        set_value_result = RADIO_RESULT_INVALID_VALUE;
	  }
  } else if (parameter == RADIO_PARAM_RX_MODE) {
	  radio_value_t valid = RADIO_RX_MODE_ADDRESS_FILTER | RADIO_RX_MODE_AUTOACK | RADIO_RX_MODE_POLL_MODE ;
	  if (input_value & (~valid)) {
		  set_value_result = RADIO_RESULT_INVALID_VALUE;
	  } else {
          radio_set_auto_pkt_filter((input_value & RADIO_RX_MODE_ADDRESS_FILTER) != 0);
          radio_set_auto_ack((input_value & RADIO_RX_MODE_AUTOACK) != 0);
          radio_set_polling_mode((input_value & RADIO_RX_MODE_POLL_MODE) != 0);
		  set_value_result = RADIO_RESULT_OK;
	  }
  } else if (parameter == RADIO_PARAM_TX_MODE) {
	  radio_value_t valid = RADIO_TX_MODE_SEND_ON_CCA;
	  if (input_value & (~valid)) {
		  set_value_result = RADIO_RESULT_INVALID_VALUE;
	  } else {
		  radio_set_csma((input_value & RADIO_TX_MODE_SEND_ON_CCA) != 0);
		  set_value_result = RADIO_RESULT_OK;
	  }
  } else if (parameter == RADIO_PARAM_TXPOWER) {
	  if((input_value >= RADIO_POWER_DBM_MIN) && (input_value <= RADIO_POWER_DBM_MAX)) {
		  radio_set_txpower(input_value);
		  set_value_result = RADIO_RESULT_OK;
      } else {
        set_value_result = RADIO_RESULT_INVALID_VALUE;
	  }
  } else if (parameter == RADIO_PARAM_CCA_THRESHOLD) {
	  //TODO: this value is not currently taken into account, only RSSI_TX_THRESHOLD macro is used
      csma_tx_threshold = input_value;
	  set_value_result = RADIO_RESULT_OK;
  }

  return set_value_result;
}
/*---------------------------------------------------------------------------*/
static radio_result_t
Radio_get_object(radio_param_t parameter, void *destination, size_t size)
{
	radio_result_t get_object_retval;
	get_object_retval = RADIO_RESULT_NOT_SUPPORTED;

  /*@TODO: add other parameters. */
  if(parameter == RADIO_PARAM_LAST_PACKET_TIMESTAMP) {
    if((size == sizeof(rtimer_clock_t)) && (destination != NULL) ) {
	  /*@TODO: this has to be validated*/
      *(rtimer_clock_t *)destination = radio_get_packet_timestamp();
      get_object_retval = RADIO_RESULT_OK;
    } else {
      get_object_retval = RADIO_RESULT_INVALID_VALUE;
	}
  }
  return get_object_retval;
}
/*---------------------------------------------------------------------------*/
static radio_result_t
Radio_set_object(radio_param_t parameter, const void *source, size_t size)
{
  UNUSED(parameter);
  UNUSED(source);
  UNUSED(size);
  /*@TODO: this API is currently not supported. */

  return RADIO_RESULT_NOT_SUPPORTED;
}
/*---------------------------------------------------------------------------*/
/* @brief Radio_read_from_fifo can be called from Radio_interrupt_callback or
 *        from Radio_read depending on the operating mode.
 */
static int Radio_read_from_fifo (uint8_t* buf, unsigned short bufsize)
{
  uint8_t rx_bytes, retval = 0;

  rx_bytes = S2LP_FIFO_ReadNumberBytesRxFifo();

  if (rx_bytes <= bufsize) {
    S2LP_ReadFIFO(rx_bytes, (uint8_t*)buf);
    retval = rx_bytes;
    last_packet_timestamp = RTIMER_NOW() ; //@TODO: validate
    last_packet_rssi = (radio_value_t) S2LP_RADIO_QI_GetRssidBm();
    last_packet_lqi  = (packetbuf_attr_t) S2LP_RADIO_QI_GetLqi();
    packetbuf_set_attr(PACKETBUF_ATTR_RSSI, (packetbuf_attr_t) last_packet_rssi);
    packetbuf_set_attr(PACKETBUF_ATTR_LINK_QUALITY, last_packet_lqi);
  } else {
    LOG_DBG("Buf too small (%d bytes to hold %d bytes)\n", bufsize, rx_bytes);
  }
  if (polling_mode) {
    S2LP_CMD_StrobeFlushRxFifo();
  }

  return retval;
}
/*---------------------------------------------------------------------------*/
/**
* @brief  Radio_interrupt_callback
* 	callback when an interrupt is received
* @param  none
* @retval none
*/
void
Radio_interrupt_callback(void)
{
#define INTPRINTF(...) // printf
  S2LPIrqs x_irq_status;
  if (radio_spi_busy() || interrupt_callback_in_progress)
  {
    process_poll(&subGHz_radio_process);
    interrupt_callback_wants_poll = 1;
    return;
  }

  interrupt_callback_wants_poll = 0;
  interrupt_callback_in_progress = 1;

  /* get interrupt source from radio */
  S2LP_GPIO_IrqGetStatus(&x_irq_status);

  /* The IRQ_TX_DATA_SENT notifies the packet transmission.
  * Then puts the Radio in RX/Sleep according to the selected mode */
  if(x_irq_status.IRQ_TX_DATA_SENT && transmitting_packet)
  {
    INTPRINTF("IRQ_TX_DATA_SENT\n");
    transmitting_packet = 0;
    xTxDoneFlag = SET;
    interrupt_callback_in_progress = 0;
    return;
  }

#if !RADIO_SNIFF_MODE
  /* The IRQ_VALID_SYNC is used to notify a new packet is coming */
  if(x_irq_status.IRQ_VALID_SYNC  && !transmitting_packet )
  {
    interrupt_callback_in_progress = 0;
    INTPRINTF("IRQ_VALID_SYNC\n");
    receiving_packet = 1;
    S2LP_CMD_StrobeRx();
  }
#endif /*RADIO_SNIFF_MODE*/

#if RADIO_HW_CSMA
  if(x_irq_status.IRQ_MAX_BO_CCA_REACH)
  {
    interrupt_callback_in_progress = 0;
    /* Send a Tx command: i.e. keep on trying */
    S2LP_CMD_StrobeTx();
    return;
  }
#endif /*RADIO_HW_CSMA*/

  /* The IRQ_RX_DATA_READY notifies a new packet arrived */
  if(x_irq_status.IRQ_RX_DATA_READY && !(transmitting_packet))
  {
    receiving_packet = 0;

    (void) Radio_read_from_fifo(radio_rxbuf, sizeof(radio_rxbuf));
    rx_num_bytes = S2LP_PCKT_BASIC_GetReceivedPktLength();

    S2LP_CMD_StrobeFlushRxFifo();
    pending_packet = 1;
    interrupt_callback_in_progress = 0;
    process_poll(&subGHz_radio_process);
    return;
  }

#if !RADIO_SNIFF_MODE
  if(x_irq_status.IRQ_RX_DATA_DISC && !transmitting_packet)
  {
    INTPRINTF("IRQ_RX_DATA_DISC\r\n");
    /* RX command - to ensure the device will be ready for the next reception */
    if(x_irq_status.IRQ_RX_TIMEOUT)
    {
      S2LP_CMD_StrobeFlushRxFifo();
      S2LP_CMD_StrobeRx();
    }
  }
#endif /*!RADIO_SNIFF_MODE*/

  interrupt_callback_in_progress = 0;
}
/*---------------------------------------------------------------------------*/
/** @} */
/** @} */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

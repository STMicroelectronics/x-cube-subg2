
/**
* @file    app_cli.c
* @author  LowPower RF BU - AMG
* @version V1.3.0
* @date    May, 2019
* @brief   Command Line Interface
* @details
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*
* <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
*/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>

/* Application Includes */
#include "serial_utils.h"
#include "command-interpreter2.h"
#include "S2LP_CLI_commands.h"
#include "SDK_CLI_commands.h"
#include "utils.h"
#include "stm32l0xx_nucleo.h"

#include "SDK_UTILS_Timers.h"
#include "s2lp_sdkapi_mapping.h"
#include "s2lp_interface_cli.h"
#include "SDK_EVAL_Com.h"

#define FW_VERSION      "2.3.4"

/* Functions------------------------------------------------------------------*/
int ST_CLI_Init(void);

extern uint8_t responsePrintf(const char * formatString, ...);
extern uint8_t responseHeaderPrintf(const char * formatString, ...);

/* Commands callbacks */
void helpAction(void);
void interactiveAction(void);
void fwVersionAction(void);
void rebootAction(void);
void dfuAction(void);
void Appli_Exti_CB(uint32_t GPIO_Pin, uint32_t timer);

/* Defines `-------------------------------------------------------------------*/
#define CMD_HIDDEN(CMD)	(strcmp(CMD->name,"EepromWritePage")==0 || \
		strcmp(CMD->name,"EepromReadPage")==0 || strcmp(CMD->name,"EepromStatus")==0 || \
		strcmp(CMD->name,"interactive")==0 || \
		strcmp(CMD->name,"dfu")==0 || \
		strcmp(CMD->name,"RegulateVRf")==0 || \
		strcmp(CMD->name,"GetVRf")==0)

/* Global variables-----------------------------------------------------------*/
static volatile uint8_t interactive = 1;
volatile uint8_t raw_input=0;

/* Extern variables-----------------------------------------------------------*/
extern uint8_t gpio_timer_mask;
extern IrqQueue s_xIrqQueue;
extern volatile FlagStatus xIRQRaised;

CommandEntry CommandTable[] = {
  { "help", helpAction, "", "List commands"},
  { "reboot", rebootAction, "", "Reboot device"},
  { "dfu", dfuAction, "", "put the device in dfu mode (cli disabled)"},
  { "interactive", interactiveAction, "u", "Set interactive mode"},
  { "fwVersion", fwVersionAction, "", "Get fw version"},
  S2LP_COMMAND_TABLE,
  SDK_EVAL_COMMAND_TABLE,
  { NULL, NULL, NULL, NULL } // NULL action makes this a terminator
};

/**
* @brief  The LED blinks indefinitely. The applicaiton stucks.
* @param  None
* @retval None
*/
static void Fatal_Error(void)
{
  BSP_LED_Init(LED2);

  while(1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED2);
  }
}

uint8_t processCmdInput (uint8_t interactive)
{
  static uint8_t buff[COMMAND_BUFFER_LENGTH];
  static uint16_t len;
  static uint16_t currIndex = 0;
  if (interactive) {
    // continue calling SerialReadPartialLine() until it returns success, indicating a full line has been read
    if (!serialReadPartialLine((char *)buff, COMMAND_BUFFER_LENGTH, &currIndex)) {
      return 0;
    }

    len=0;
    //search foward looking for first CR, LF, or NULL to determine length of the string
    while((buff[len]!='\n') && (buff[len] !='\r') && (buff[len]!='\0')) {
      len++;
    }
    buff[len ++] = '\r'; //set the final char to be CR
    buff[len ++] = '\n'; //set the final char to be NL

    currIndex = 0;
    return processCommandString(buff, len);
  } else {
    return processCommandInput();
  }
}

void interactiveAction(void)
{
  interactive = unsignedCommandArgument(0);
  responsePrintf("{&N utility call... &t2x}\r\n", "interactive", "mode", interactive);
}

void helpAction(void)
{
  CommandEntry *cmd;

  for (cmd = CommandTable; cmd->name != NULL; cmd++) {
    if(!CMD_HIDDEN(cmd))
    {
      printf ("%s %s %s\r\n\r\n", cmd->name, cmd->argumentTypes, cmd->description);
    }
  }
}

void fwVersionAction(void)
{
  responsePrintf("{&N API call...&ts}\r\n",
     "fwVersionAction",
     "value", FW_VERSION);
}

void rebootAction(void)
{
  responsePrintf("{&N API call...&ts}\r\n", "reboot");

#if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
  HAL_NVIC_SystemReset();
#else
  NVIC_SystemReset();
#endif
}

void dfuAction(void)
{
  responsePrintf("{&N API call...}\r\n", "dfuAction");
}

void MX_X_CUBE_SUBG2_Init(void)
{
   BSP_COM_Init(COM1);
   SdkEvalComInit();

  /* Init MCU + S2LP */
  if(ST_CLI_Init())
    Fatal_Error();

  /* Wait for SPI CS set to high*/
  HAL_Delay(10);

  /* Reset S2LP */
  S2915A1_RADIO_EnterShutdown();
  HAL_Delay(10);
  S2915A1_RADIO_ExitShutdown();

}

/**
* @brief  Former CLI main function
* @param  None
* @retval 0 when done
*/
void MX_X_CUBE_SUBG2_Process(void)
{

  printf("S2-LP CLI test application\r\n");
  responsePrintf("{&N API call... &t2x}\r\n", "halGetResetInfo", "resetInfo", 0);

  while(1)
  {
    /* CLI parser loop */
    if(processCmdInput(interactive))
    {
	if (interactive) {
	  printf(">");
	}
    }
  }
}

#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval : None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number */
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
* @brief  Init function.
* @param  None
* @retval nRet when done
*/
int ST_CLI_Init(void)
{
  int nRet = 0;

  S2LPInterfaceInit();

  return nRet;
}

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE*****/

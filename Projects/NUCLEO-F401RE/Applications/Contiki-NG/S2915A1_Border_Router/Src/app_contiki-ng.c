/**
  ******************************************************************************
  * @file    app_contiki-ng.c
  * @author  SRA Application Team
  * @brief   Implementation of applicative interface with ST32CubeMX
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "contiki-platform.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

void MX_X_CUBE_SUBG2_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN S2LP_Library_Init_PreTreatment */

  /* USER CODE END S2LP_Library_Init_PreTreatment */

  /* Initialize the peripherals and the SUBG2 components */
  MX_SUBG2_Contiki_NG_Init();

  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN S2LP_Library_Init_PostTreatment */

  /* USER CODE END S2LP_Library_Init_PostTreatment */
}
/*
 * LM background task
 */
void MX_X_CUBE_SUBG2_Process(void)
{
  /* USER CODE BEGIN S2LP_Library_Process */
  MX_SUBG2_Contiki_NG_Process();
  /* USER CODE END S2LP_Library_Process */
}

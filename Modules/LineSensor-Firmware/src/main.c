/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define whoamidata 0x68

char transferdata[3];

void setPinMux(int a, int b, int c, int d) {
  if (a == 1) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  }

  if (b == 1) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
  }

  if (c == 1) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
  }

  if (d == 1) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
  }
}
int val = 0;
char transmitdata[2];

char cutoffValue[2] = {0, 0};
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_ADC_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  int sensorRead[13];

  while (val == 0) {
    if (HAL_GetTick() >= 5000) {
      break;
      val = 1150;
    }

    if (HAL_I2C_Slave_Receive(&hi2c1, (uint8_t *)cutoffValue, 2, 1000) ==
        HAL_OK) {
      val = cutoffValue[0] * 256 + cutoffValue[1];
    }
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    HAL_SYSTICK_Config(SystemCoreClock / (10000U / uwTickFreq));
    int number = 0;
    setPinMux(0, 0, 0, 0);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 1;
    setPinMux(0, 0, 0, 1);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 2;
    setPinMux(0, 0, 1, 0);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 3;
    setPinMux(0, 0, 1, 1);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 4;
    setPinMux(0, 1, 0, 0);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 5;
    setPinMux(0, 1, 0, 1);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 6;
    setPinMux(0, 1, 1, 0);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 7;
    setPinMux(0, 1, 1, 1);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 10);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 8;
    setPinMux(1, 0, 0, 0);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 9;
    setPinMux(1, 0, 0, 1);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 10;
    setPinMux(1, 0, 1, 0);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 11;
    setPinMux(1, 0, 1, 1);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    number = 12;
    setPinMux(1, 1, 0, 0);
    HAL_Delay(1);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);  // ADC変換終了を待機
    HAL_ADC_Stop(&hadc);
    sensorRead[number] = HAL_ADC_GetValue(&hadc);
    if (sensorRead[number] >
        val) {  //あげると感度がよくなります（誤検知します）
      sensorRead[number] = 1;
    } else {
      sensorRead[number] = 0;
    }

    transmitdata[0] = sensorRead[0] * 128 + sensorRead[1] * 64 +
                      sensorRead[2] * 32 + sensorRead[3] * 16 +
                      sensorRead[4] * 8 + sensorRead[5] * 4 +
                      sensorRead[6] * 2 + sensorRead[7];

    transmitdata[1] = sensorRead[8] * 128 + sensorRead[9] * 64 +
                      sensorRead[10] * 32 + sensorRead[11] * 16 +
                      sensorRead[12] * 8;
    int checkDegit = 0;

    for (int i = 0; i < 13; i++) {
      checkDegit += sensorRead[i];
    }

    checkDegit %= 8;
    transmitdata[1] += checkDegit;

    HAL_SYSTICK_Config(SystemCoreClock / (10000U / uwTickFreq));

    HAL_I2C_Slave_Transmit(&hi2c1, (uint8_t *)transmitdata, 2, 1);
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    Error_Handler();
  }
}

/**
 * @brief ADC Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC_Init(void) {
  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data
   * Alignment and number of conversion)
   */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = ENABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK) {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted.
   */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {
  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000708;
  hi2c1.Init.OwnAddress1 = 0x08 << 1;  // left:0x10(7bit) right:0x40 rear:0x20 front:0x08
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
    Error_Handler();
  }
  /** Configure Analogue filter
   */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
    Error_Handler();
  }
  /** Configure Digital filter
   */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {
  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 16;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 16;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
                    GPIO_PIN_RESET);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

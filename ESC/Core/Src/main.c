 #include "main.h"
 ADC_HandleTypeDef hadc;
 TIM_HandleTypeDef htim2;
 UART_HandleTypeDef huart2;

 void SystemClock_Config(void);
 static void MX_GPIO_Init(void);
 static void MX_USART2_UART_Init(void);
 static void MX_ADC_Init(void);
 static void MX_TIM2_Init(void);
 void sekuta(int sek, int power) {
   if (sek == 0) {  // u-v
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, power);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1);

     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);    //! U
     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);   //! V
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);  //! W
   } else if (sek == 1) {                                   // w-v
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, power);

     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);  //! U
     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);   //! V
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);    //! W
   } else if (sek == 2) {                                   // w-u
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, power);

     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);     //! U
     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);  //! V
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);     //! W
   } else if (sek == 3) {                                    // v-u
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, power);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1);

     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);    //! U
     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);   //! V
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);  //! W
   } else if (sek == 4) {                                   // v-w
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, power);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1);

     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);  //! U
     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);   //! V
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);    //! W
   } else if (sek == 5) {                                   // u-w
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, power);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1);

     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);     //! U
     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);  //! V
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);     //! W
   }
 }

 void HAL_DelayUS(uint32_t Delay) {
   long neko = 0;
   while (neko <= 1000) {
     neko++;
   }
 }

 int main(void) {
   HAL_Init();

   /* USER CODE BEGIN Init */

   /* USER CODE END Init */

   /* Configure the system clock */
   SystemClock_Config();

   /* USER CODE BEGIN SysInit */

   /* USER CODE END SysInit */

   /* Initialize all configured peripherals */
   MX_GPIO_Init();
   MX_USART2_UART_Init();
   MX_ADC_Init();
   MX_TIM2_Init();
   /* USER CODE BEGIN 2 */
   HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
   HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
   HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
   HAL_TIM_Base_Start_IT(&htim2);
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);   //! U
   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);  //! V
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);   //! W

   int neko = 0;
   int val;  // AD変換後の数値を入れる
   char sval[100];
   while (1) {
     // neko++;
     // neko %= 360;

     // double power = 0.05;  // 0.6ぐらいまでが望ましい

     // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pwm[neko] * power);
     // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4,
     //                       pwm[(neko + 120) % 360] * power);
     // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,
     //                       pwm[(neko + 240) % 360] * power);

     // HAL_Delay(1);

     // HAL_UART_Transmit(&huart2, "UART Timeout.\r\n", 15, 10);
     HAL_ADC_Start(&hadc);
     HAL_ADC_PollForConversion(&hadc, 100);  // ADC変換終了を待機
     HAL_ADC_Stop(&hadc);
     val = HAL_ADC_GetValue(&hadc);
     // UART送信
     sprintf(sval, "%d\r\n", val);
     HAL_UART_Transmit(&huart2, sval, strlen(sval) + 1, 0xFFFF);
     //待機(100[ms])
     HAL_Delay(100);
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
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
   RCC_OscInitStruct.HSIState = RCC_HSI_ON;
   RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
   RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
   RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
     Error_Handler();
   }
   /** Initializes the CPU, AHB and APB buses clocks
    */
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
     Error_Handler();
   }
   PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
   PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
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
   hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
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
   hadc.Init.LowPowerFrequencyMode = DISABLE;
   hadc.Init.LowPowerAutoPowerOff = DISABLE;
   if (HAL_ADC_Init(&hadc) != HAL_OK) {
     Error_Handler();
   }
   /** Configure for the selected ADC regular channel to be converted.
    */
   sConfig.Channel = ADC_CHANNEL_0;
   sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
   if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
     Error_Handler();
   }
   /** Configure for the selected ADC regular channel to be converted.
    */
   sConfig.Channel = ADC_CHANNEL_1;
   if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
     Error_Handler();
   }
   /** Configure for the selected ADC regular channel to be converted.
    */
   sConfig.Channel = ADC_CHANNEL_4;
   if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
     Error_Handler();
   }
   /** Configure for the selected ADC regular channel to be converted.
    */
   sConfig.Channel = ADC_CHANNEL_7;
   if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
     Error_Handler();
   }
   /* USER CODE BEGIN ADC_Init 2 */

   /* USER CODE END ADC_Init 2 */
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
   TIM_OC_InitTypeDef sConfigOC = {0};

   /* USER CODE BEGIN TIM2_Init 1 */

   /* USER CODE END TIM2_Init 1 */
   htim2.Instance = TIM2;
   htim2.Init.Prescaler = 0;
   htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
   htim2.Init.Period = 255;
   htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
   htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
   if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
     Error_Handler();
   }
   sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
   if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
     Error_Handler();
   }
   if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
     Error_Handler();
   }
   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
   if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) !=
   HAL_OK) {
     Error_Handler();
   }
   sConfigOC.OCMode = TIM_OCMODE_PWM1;
   sConfigOC.Pulse = 0;
   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
   if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
   {
     Error_Handler();
   }
   if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
   {
     Error_Handler();
   }
   if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
   {
     Error_Handler();
   }
   /* USER CODE BEGIN TIM2_Init 2 */

   /* USER CODE END TIM2_Init 2 */
   HAL_TIM_MspPostInit(&htim2);
 }

 /**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
 static void MX_USART2_UART_Init(void) {
   /* USER CODE BEGIN USART2_Init 0 */

   /* USER CODE END USART2_Init 0 */

   /* USER CODE BEGIN USART2_Init 1 */

   /* USER CODE END USART2_Init 1 */
   huart2.Instance = USART2;
   huart2.Init.BaudRate = 115200;
   huart2.Init.WordLength = UART_WORDLENGTH_8B;
   huart2.Init.StopBits = UART_STOPBITS_1;
   huart2.Init.Parity = UART_PARITY_NONE;
   huart2.Init.Mode = UART_MODE_TX_RX;
   huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   huart2.Init.OverSampling = UART_OVERSAMPLING_16;
   huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
   huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
   if (HAL_UART_Init(&huart2) != HAL_OK) {
     Error_Handler();
   }
   /* USER CODE BEGIN USART2_Init 2 */

   /* USER CODE END USART2_Init 2 */
 }

 /**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
 static void MX_GPIO_Init(void) {
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();

   /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);

   /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6 | GPIO_PIN_9, GPIO_PIN_RESET);

   /*Configure GPIO pin : PC14 */
   GPIO_InitStruct.Pin = GPIO_PIN_14;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

   /*Configure GPIO pin : PC15 */
   GPIO_InitStruct.Pin = GPIO_PIN_15;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

   /*Configure GPIO pins : PA6 PA9 */
   GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_9;
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
   /* User can add his own implementation to report the HAL error return state
   */
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
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
      */
   /* USER CODE END 6 */
 }
 #endif /* USE_FULL_ASSERT */

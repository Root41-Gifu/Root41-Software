void buzzer001(void) {
  HAL_SYSTICK_Config(SystemCoreClock / (10000U / uwTickFreq));
  for (int j = 0; j < 3; j++) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);    //! U
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);   //! V
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);  //! W
    for (int i = 0; i < 300; i++) {
      HAL_Delay(2);

      if (i % 2) {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 80);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1);
      } else {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 80);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1);
      }
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);   //! U
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);  //! V
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);   //! W

    HAL_Delay(600);
  }
  HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq));
}
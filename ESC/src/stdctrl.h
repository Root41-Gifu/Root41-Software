#include "main.h"

#define lowByte(w) ((uint8_t)((w)&0xff))
#define highByte(w) ((uint8_t)((w) >> 8))
ADC_HandleTypeDef hadc;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2;

int encoderRead(void) {
  HAL_ADC_Start(&hadc);
  HAL_ADC_PollForConversion(&hadc, 10);  // ADC変換終了を待機
  HAL_ADC_Stop(&hadc);
  return HAL_ADC_GetValue(&hadc);
}

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
#include "main.h"

#define lowByte(w) ((uint8_t)((w)&0xff))
#define highByte(w) ((uint8_t)((w) >> 8))
#define constrain(amt, low, high) \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

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

char pwmDrive[] = {
    76,  79,  82,  86,  89,  92,  95,  98,  101, 104, 107, 110, 113, 116, 119,
    121, 124, 126, 128, 131, 133, 135, 137, 139, 140, 142, 143, 145, 146, 147,
    148, 149, 150, 150, 151, 151, 151, 151, 151, 151, 150, 150, 149, 148, 147,
    146, 145, 143, 142, 140, 139, 137, 135, 133, 131, 128, 126, 124, 121, 119,
    116, 113, 110, 107, 104, 101, 98,  95,  92,  89,  86,  82,  79,  76,  73,
    70,  66,  63,  60,  57,  54,  51,  48,  45,  42,  39,  36,  33,  31,  28,
    26,  24,  21,  19,  17,  15,  13,  12,  10,  9,   7,   6,   5,   4,   3,
    2,   2,   1,   1,   1,   1,   1,   1,   2,   2,   3,   4,   5,   6,   7,
    9,   10,  12,  13,  15,  17,  19,  21,  24,  26,  28,  31,  33,  36,  39,
    42,  45,  48,  51,  54,  57,  60,  63,  66,  70,  73};

void driveDirect(int a, float b) {
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,
                        (int)((float)pwmDrive[(a + 0) % 146] * b));
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4,
                        (int)((float)pwmDrive[(a + 49) % 146] * b));
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,
                        (int)((float)pwmDrive[(a + 96) % 146] * b));

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);   //! U
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);  //! V
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);   //! W
}

void release(void) {
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);   //! U
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);  //! V
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);   //! W
}

int offsetSet(int data) {
  int temp = encoderRead();
  temp = temp >> 2;
  temp %= 146;
  temp = 145 - temp;
  temp = data - temp;
  return temp;
}

int calibration(void) {
  int offsetRaw[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  sekuta(1, 30);
  HAL_Delay(100);
  sekuta(0, 30);
  HAL_Delay(300);
  offsetRaw[0] = encoderRead();
  sekuta(5, 30);
  HAL_Delay(100);
  sekuta(0, 30);
  HAL_Delay(300);
  offsetRaw[1] = encoderRead();
  sekuta(1, 30);
  HAL_Delay(100);
  sekuta(0, 30);
  HAL_Delay(300);
  offsetRaw[2] = encoderRead();
  sekuta(5, 30);
  HAL_Delay(100);
  sekuta(0, 30);
  HAL_Delay(300);
  offsetRaw[3] = encoderRead();

  return (offsetRaw[0] + offsetRaw[1] + offsetRaw[2] + offsetRaw[3]) / 4;
}
//周波数変更
void changeFreq(unsigned long freq) {
	LL_TIM_InitTypeDef TIM_InitStruct = { 0 };
	LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = { 0 };

	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	TIM_InitStruct.Prescaler = (125000 / freq) - 1;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 255;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	LL_TIM_Init(TIM2, &TIM_InitStruct);
	LL_TIM_DisableARRPreload(TIM2);
	LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH1);
	TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
	TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.CompareValue = 0;
	TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
	LL_TIM_OC_DisableFast(TIM2, LL_TIM_CHANNEL_CH1);
	LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH3);
	LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH3, &TIM_OC_InitStruct);
	LL_TIM_OC_DisableFast(TIM2, LL_TIM_CHANNEL_CH3);
	LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH4);
	LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH4, &TIM_OC_InitStruct);
	LL_TIM_OC_DisableFast(TIM2, LL_TIM_CHANNEL_CH4);
	LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
	LL_TIM_DisableMasterSlaveMode(TIM2);
	LL_TIM_OC_DisablePreload(TIM2, LL_TIM_CHANNEL_CH1);
	LL_TIM_OC_DisablePreload(TIM2, LL_TIM_CHANNEL_CH3);
	LL_TIM_OC_DisablePreload(TIM2, LL_TIM_CHANNEL_CH4);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	LL_TIM_EnableCounter(TIM2);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH4);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
}

void sekuta(int sek, int power) {
//	if (sek == 0) {  // u-v
//		LL_TIM_OC_SetCompareCH3(TIM2, power);
//		LL_TIM_OC_SetCompareCH4(TIM2, 1);
//		LL_TIM_OC_SetCompareCH1(TIM2, 1);
//
//		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_9);    //! U
//		LL_GPIO_SetOutputPin(GPIOC, GPIO_PIN_14);   //! V
//		LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_6);  //! W
//	} else if (sek == 1) {                                   // w-v
//		LL_TIM_OC_SetCompareCH3(TIM2, 1);
//		LL_TIM_OC_SetCompareCH4(TIM2, 1);
//		LL_TIM_OC_SetCompareCH1(TIM2, power);
//
//		LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_9);    //! U
//		LL_GPIO_SetOutputPin(GPIOC, GPIO_PIN_14);   //! V
//		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_6);  //! W
//	} else if (sek == 2) {                                   // w-u
//		LL_TIM_OC_SetCompareCH3(TIM2, 1);
//		LL_TIM_OC_SetCompareCH4(TIM2, 1);
//		LL_TIM_OC_SetCompareCH1(TIM2, power);
//
//		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_9);    //! U
//		LL_GPIO_ResetOutputPin(GPIOC, GPIO_PIN_14);   //! V
//		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_6);  //! W
//	} else if (sek == 3) {                                    // v-u
//		LL_TIM_OC_SetCompareCH3(TIM2, 1);
//		LL_TIM_OC_SetCompareCH4(TIM2, power);
//		LL_TIM_OC_SetCompareCH1(TIM2, 1);
//
//		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_9);    //! U
//		LL_GPIO_SetOutputPin(GPIOC, GPIO_PIN_14);   //! V
//		LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_6);  //! W
//	} else if (sek == 4) {                                   // v-w
//		LL_TIM_OC_SetCompareCH3(TIM2, 1);
//		LL_TIM_OC_SetCompareCH4(TIM2, power);
//		LL_TIM_OC_SetCompareCH1(TIM2, 1);
//
//		LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_9);    //! U
//		LL_GPIO_SetOutputPin(GPIOC, GPIO_PIN_14);   //! V
//		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_6);  //! W
//	} else if (sek == 5) {                                   // u-w
//		LL_TIM_OC_SetCompareCH3(TIM2, power);
//		LL_TIM_OC_SetCompareCH4(TIM2, 1);
//		LL_TIM_OC_SetCompareCH1(TIM2, 1);
//
//		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_9);    //! U
//		LL_GPIO_ResetOutputPin(GPIOC, GPIO_PIN_14);   //! V
//		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_6);  //! W
//	}
}

char waveTable[] = { 127, 133, 139, 146, 152, 158, 164, 170, 176, 181, 187, 192,
		198, 203, 208, 212, 217, 221, 225, 229, 233, 236, 239, 242, 244, 247,
		249, 250, 252, 253, 253, 254, 254, 254, 253, 253, 252, 250, 249, 247,
		244, 242, 239, 236, 233, 229, 225, 221, 217, 212, 208, 203, 198, 192,
		187, 181, 176, 170, 164, 158, 152, 146, 139, 133, 127, 121, 115, 108,
		102, 96, 90, 84, 78, 73, 67, 62, 56, 51, 46, 42, 37, 33, 29, 25, 21, 18,
		15, 12, 10, 7, 5, 4, 2, 1, 1, 0, 0, 0, 1, 1, 2, 4, 5, 7, 10, 12, 15, 18,
		21, 25, 29, 33, 37, 42, 46, 51, 56, 62, 67, 73, 78, 84, 90, 96, 102,
		108, 115, 121 };

int counter = 0;
int neko = 0;
int nekoCounter = 0;
const int speed = 115;

void ESC_Drive() {
	HAL_SYSTICK_Config(SystemCoreClock / (50000U / uwTickFreq));

	LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_9);    //! U
	LL_GPIO_SetOutputPin(GPIOC, GPIO_PIN_14);   //! V
	LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_6);  //! W
	for (int i = 0; i < 128; i++) {
		LL_TIM_OC_SetCompareCH3(TIM2, waveTable[i] * 0.3);
		LL_TIM_OC_SetCompareCH4(TIM2, waveTable[(i + 43) % 128] * 0.3);
		LL_TIM_OC_SetCompareCH1(TIM2, waveTable[(i + 85) % 128] * 0.3);
//		sekuta(i, 20);
		HAL_Delay(1);
	}
}


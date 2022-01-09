void changeFreq(unsigned long freq) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	LL_TIM_InitTypeDef TIM_InitStruct = { 0 };
	LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = { 0 };

	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
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
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	/**TIM2 GPIO Configuration
	 PA5   ------> TIM2_CH1
	 PB1   ------> TIM2_CH4
	 PA10   ------> TIM2_CH3
	 */
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
	if (sek == 0) {  // u-v
		LL_TIM_OC_SetCompareCH3(TIM2, power);
		LL_TIM_OC_SetCompareCH4(TIM2, 1);
		LL_TIM_OC_SetCompareCH1(TIM2, 1);

		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_9);    //! U
		LL_GPIO_SetOutputPin(GPIOC, GPIO_PIN_14);   //! V
		LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_6);  //! W
	} else if (sek == 1) {                                   // w-v
		LL_TIM_OC_SetCompareCH3(TIM2, 1);
		LL_TIM_OC_SetCompareCH4(TIM2, 1);
		LL_TIM_OC_SetCompareCH1(TIM2, power);

		LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_9);    //! U
		LL_GPIO_SetOutputPin(GPIOC, GPIO_PIN_14);   //! V
		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_6);  //! W
	} else if (sek == 2) {                                   // w-u
		LL_TIM_OC_SetCompareCH3(TIM2, 1);
		LL_TIM_OC_SetCompareCH4(TIM2, 1);
		LL_TIM_OC_SetCompareCH1(TIM2, power);

		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_9);    //! U
		LL_GPIO_ResetOutputPin(GPIOC, GPIO_PIN_14);   //! V
		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_6);  //! W
	} else if (sek == 3) {                                    // v-u
		LL_TIM_OC_SetCompareCH3(TIM2, 1);
		LL_TIM_OC_SetCompareCH4(TIM2, power);
		LL_TIM_OC_SetCompareCH1(TIM2, 1);

		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_9);    //! U
		LL_GPIO_SetOutputPin(GPIOC, GPIO_PIN_14);   //! V
		LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_6);  //! W
	} else if (sek == 4) {                                   // v-w
		LL_TIM_OC_SetCompareCH3(TIM2, 1);
		LL_TIM_OC_SetCompareCH4(TIM2, power);
		LL_TIM_OC_SetCompareCH1(TIM2, 1);

		LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_9);    //! U
		LL_GPIO_SetOutputPin(GPIOC, GPIO_PIN_14);   //! V
		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_6);  //! W
	} else if (sek == 5) {                                   // u-w
		LL_TIM_OC_SetCompareCH3(TIM2, power);
		LL_TIM_OC_SetCompareCH4(TIM2, 1);
		LL_TIM_OC_SetCompareCH1(TIM2, 1);

		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_9);    //! U
		LL_GPIO_ResetOutputPin(GPIOC, GPIO_PIN_14);   //! V
		LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_6);  //! W
	}
}

const int gakfuLength = 128;
const int gakfu[] = { 1568, 1568, 1760, 1760, 1245, 1319, 1319, 1175, 1245,
		1175, 1047, 1047, 1047, 1047, 1175, 1175, 1245, 1245, 1245, 1175, 1047,
		1175, 1319, 1568, 1760, 1319, 1568, 1175, 1319, 1047, 1175, 1047, 1319,
		1319, 1568, 1568, 1760, 1319, 1568, 1047, 1175, 1047, 1245, 1319, 1245,
		1175, 1047, 1175, 1245, 1245, 1245, 1175, 1047, 1175, 1319, 1568, 1175,
		1047, 1175, 1175, 1047, 1047, 1047, 1047, 1047, 1047, 784, 880, 1047,
		1047, 784, 880, 1047, 1319, 1397, 1319, 1397, 1319, 1397, 1568, 1047,
		1047, 1047, 1047, 784, 880, 1047, 1319, 1397, 1319, 1397, 1568, 1047,
		1047, 988, 988, 1047, 1047, 784, 880, 1047, 1047, 784, 880, 1047, 1319,
		1397, 1319, 1397, 1319, 1397, 1568, 1047, 1047, 1047, 1047, 784, 880,
		1047, 1319, 1397, 1319, 1175, 1175, 1047, 1047, 1047, 1047 };

const int gakfuIntroLength = 32;
const int gakfuIntro[] = { 1319, 1397, 1568, 1568, 2093, 2093, 1319, 1397, 1568,
		2093, 2349, 2637, 2349, 1976, 2093, 2093, 1568, 1568, 1319, 1397, 1568,
		1568, 2093, 2093, 2349, 1976, 2093, 2349, 2794, 2637, 2794, 2349 };

int counter = 0;
int neko = 0;
int nekoCounter = 0;
const int speed = 115;

void ESC_Drive() {
	for (int i = 0; i < 6; i++) {
		sekuta(i, 20);
		HAL_Delay(3);
	}
	if (neko == 0) {
		if (nekoCounter * speed + 1000 < HAL_GetTick()) {
			changeFreq(gakfuIntro[nekoCounter]);
			nekoCounter++;
			if (nekoCounter == gakfuIntroLength) {
				neko = 1;
			}
		}
	} else {
		if (counter * speed + speed * gakfuIntroLength
				+ gakfuLength * speed * (neko - 1) + 1000 < HAL_GetTick()) {
			changeFreq(gakfu[counter]);
			counter++;
			if (counter == gakfuLength) {
				neko++;
				counter = 0;
			}
		}
	}
}


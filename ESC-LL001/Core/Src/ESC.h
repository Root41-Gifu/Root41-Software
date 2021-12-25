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

void ESC_Drive() {
	for(int i = 0; i < 6; i++){
		sekuta(2,20);
//		HAL_Delay(100);
	}
}


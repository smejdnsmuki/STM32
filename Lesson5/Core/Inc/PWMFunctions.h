#ifndef INC_PWMFUNCTIONS_H_
#define INC_PWMFUNCTIONS_H_


void PWMChannel4SetParameters(int prescaler, int period, int duty_cycle)
{
	TIM1->PSC = prescaler;
	TIM1->ARR = period;
	TIM1->CCR4 = duty_cycle;
}

void PWMSetUpChannel4(int prescaler, int period, int duty_cycle)
{
	RCC ->APB2ENR |= RCC_APB2ENR_TIM1EN;

	RCC ->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA ->MODER &= ~(GPIO_MODER_MODER11_0);
	GPIOA ->MODER |= GPIO_MODER_MODER11_1;	// Alternate function - for PWM

	GPIOA->AFR[1] = 0b1000000000000000;

	PWMChannel4SetParameters(prescaler, period, duty_cycle);
	TIM1->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE;	//4M: Output compare 4 mode & PRELOAD ENABLE
	TIM1->CCER |= TIM_CCER_CC4E;											//Send signal from Channel to pin
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM1->CR1 |= TIM_CR1_CEN;												// CNT starts counting
	TIM1->EGR |= TIM_EGR_UG;												// Use seetings NOW (Preload set to active register, PSC and AAR used immididately,...)

}

void PWMChannel4SetParameters(int prescaler, int period, int duty_cycle)
{
	TIM1->PSC = prescaler;
	TIM1->ARR = period;
	TIM1->CCR4 = duty_cycle;
}

void notExactTimeDelay(int timeDelay)
{
	volatile int i;
	for (int i; i < timeDelay; i++)
	{
	}
}

#endif

#ifndef ADCFunctionsheader
#define ADCFunctionsheader

void ADCInitialize()
{
	// ADC Calibration
	ADC1 ->CR &= ~ADC_CR_ADEN;	// Control Register - disabled
	ADC1 ->CR |= ADC_CR_ADCAL;	// Set bit for Calibration
	while ((ADC1 ->CR & ADC_CR_ADCAL) != 0)	// When Calibration is done, the bit goes to 0
	{
	}

	// Select a clock source for ADC
	RCC ->APB2ENR |= RCC_APB2ENR_ADC1EN;	// Clock source for ADC
	RCC ->CR2 |= RCC_CR2_HSI14ON;			// ADC uses HSI Clock - this commands turns it on
	while ((RCC ->CR2 & RCC_CR2_HSI14RDY) == 0)
	{
	}
	ADC1 ->CFGR2 |= ADC_CFGR2_CKMODE;		// Determination which clock will aDC use ()

	// ADC Enable
	ADC1 ->CR |= ADC_CR_ADEN;
	while ((ADC1 ->ISR & ADC_ISR_ADRDY) == 0)	// Waiting to be ready
	{
	}

	ADC1 ->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;	// Slowest sampling time
}

void ADCChannelSelect(uint32_t Channel)
{
	ADC1 ->CHSELR |= 1 << Channel;	 										// Internal Temp. Sensor
}

#endif



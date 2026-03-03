
#include "main.h"


void SystemClock_Config(void);

int main(void)
{
  HAL_Init();

  SystemClock_Config();

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
  ADC1 -> CHSELR |= ADC_CHSELR_CHSEL16;								// Internal TEmp. Sensor
  ADC1 ->CR |= ADC_CCR_TSEN;
  ADC1 ->CR |= ADC_CCR_VREFEN;

  while (1)
  {
	  // Start Conversion
	  ADC1 ->CR |= ADC_CR_ADSTART;
	  while((ADC1 ->ISR & ADC_ISR_EOC) == 0 )		// End of conversion
	  {

	  }

  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

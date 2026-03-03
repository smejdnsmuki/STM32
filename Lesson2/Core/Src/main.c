
#include "main.h"


void SystemClock_Config(void);

int main(void)
{


  HAL_Init();
  SystemClock_Config();

  // Reset and clock control - AHB enabling GPIO port C and port B
  RCC ->AHBENR |= RCC_AHBENR_GPIOCEN;
  RCC ->AHBENR |= RCC_AHBENR_GPIOBEN;

  GPIOC ->MODER   |=  GPIO_MODER_MODER6_0;
  GPIOC ->MODER   &= ~GPIO_MODER_MODER6_1;
  GPIOC ->OTYPER  &= ~GPIO_OTYPER_OT_6;
  GPIOC ->OSPEEDR |=  GPIO_OSPEEDR_OSPEEDR6_0 | GPIO_OSPEEDR_OSPEEDR6_1;
  GPIOC ->PUPDR   &= ~GPIO_PUPDR_PUPDR6;

  GPIOB ->MODER &= ~GPIO_MODER_MODER1;
  GPIOB ->PUPDR &= ~GPIO_PUPDR_PUPDR1;

  // Initialize Button Pressed Variable
  volatile char buttonPressed = 0;
  // Initialize Button Pressed Confidence Level
  volatile int buttonPressedConfidenceLevel = 0;
  // Initialize Button Released Confidence Level
  volatile int buttonReleasedConfidenceLevel = 0;
  // Initialize Confidence threshold Level
  volatile int confidenceThreshold = 200;
  volatile char LEDState = 0;

  while (1)
  {
	  if (GPIOB ->IDR & GPIO_IDR_1)
	  {
		//GPIOC ->BSRR |= GPIO_BSRR_BS_6;
		  if (buttonPressed == 0) {
			if (buttonPressedConfidenceLevel > confidenceThreshold)
			{
				if (LEDState == 0)
				{
					LEDState = 1;
					GPIOC ->BSRR |= GPIO_BSRR_BS_6;
				}
				else
				{
					LEDState = 0;
					GPIOC ->BSRR |= GPIO_BSRR_BR_6;
				}
				buttonPressed = 1;
			}
			else
			{
				buttonPressedConfidenceLevel++;
				buttonReleasedConfidenceLevel = 0;
			}
		  }
	  }
	  else
	  {
		//GPIOC ->BSRR |= GPIO_BSRR_BR_6;
		  if (buttonPressed == 1)
		  {
			  buttonReleasedConfidenceLevel++;
			  buttonReleasedConfidenceLevel = 0;
			  if (buttonReleasedConfidenceLevel > confidenceThreshold)
			  {
				  buttonPressed = 0;
			  }
		  }
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

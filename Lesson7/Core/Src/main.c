
#include "main.h"

void SystemClock_Config(void);

volatile int portAPin15Counter;
volatile int portAPin11Counter;

void EXTI4_15_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR15)
    // EXTI->PR = Pending Register → shows which EXTI line triggered interrupt
    // EXTI_PR_PR15 = bit corresponding to EXTI line 15
    // If set → interrupt came from line 15 (PA15)
    {
        portAPin15Counter++;	// Increment event counter

        EXTI->PR |= EXTI_PR_PR15;
        // Clear pending interrupt flag (write 1 to clear)
        // Required to prevent repeated interrupt triggering
        //small change
    }
    if (EXTI->PR & EXTI_PR_PR11)
    {
        portAPin11Counter++;
        EXTI->PR |= EXTI_PR_PR11;
        EXTI->PR |= EXTI_PR_PR11;
    }
}

int main(void)
{

  HAL_Init();
  SystemClock_Config();

  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;	// Enable clock for GPIO Port A → required before using GPIOA

  GPIOA->MODER &= ~GPIO_MODER_MODER15;	// Clear mode bits for pin 15 → sets PA15 as INPUT
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR15;	// Clear bits → no pull-up, no pull-down (floating input)
  GPIOA->MODER &= ~GPIO_MODER_MODER15;
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR15;

  SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI15_PA;
  // EXTICR[3] configures EXTI lines 12–15
  // Connect EXTI line 15 to Port A → PA15 generates EXTI15 interrupt

  EXTI->IMR |= EXTI_IMR_MR15;			// IMR = Interrupt Mask Register
  EXTI->RTSR |= EXTI_RTSR_TR15;			// RTSR = Rising Trigger Selection Register    // Interrupt occurs on rising edge signal transition

  EXTI->IMR |= EXTI_IMR_MR11;
  EXTI->RTSR |= EXTI_RTSR_TR11;

  NVIC_EnableIRQ(EXTI4_15_IRQn);
  NVIC_SetPriority(EXTI4_15_IRQn, 0);

  while (1)
  {

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

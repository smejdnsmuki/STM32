
#include "main.h"
#include "stm32f0xx.h"
#include "LCDFunctions.h"
#include "stdio.h"

void SystemClock_Config(void);
int main(void)
{
  HAL_Init();

  SystemClock_Config();

  /*
  RCC ->AHBENR |= RCC_AHBENR_GPIOBEN;
  RCC ->AHBENR |= RCC_AHBENR_GPIOCEN;
  */
  //GPIOB ->MODER |= GPIO_MODER_MODER12_0;
  //GPIOB ->MODER &= ~GPIO_MODER_MODER12_1;
  /*
  GPIOB ->MODER = 0b01010101000000000000000000000000;	//0x55000000
  GPIOB ->OTYPER = 0;
  GPIOB ->OSPEEDR = 0b11111111000000000000000000000000; //0xFF000000;
  GPIOB ->PUPDR = 0;

  GPIOC ->MODER = 0b000000000000010101010000000000000;	//0x55000000
  GPIOC ->OTYPER = 0;
  GPIOC ->OSPEEDR = 0b000000000000011111110000000000000;//0xFF000000;
  GPIOC->PUPDR = 0;
  */

  InitializePortsForLCD();
  //GPIOB ->BSRR |= 00000000000000001111000000000000;		//TEST
  //GPIOC ->BSRR |= 00000000000000000000001111000000;		//TEST

  // Character 'A' = 01000001 (0100 = port b, 0001 = port c)

  // 15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00
  //  0000  0000  0000  0000  0000  0000  0000  0000

  char character = 'A';

  LCDSendAnInstruction(0b00111000);	//Set to 8-bit mode and 2 line display
  LCDSendAnInstruction(0b00001110); //Turn on display and cursor
  LCDSendAnInstruction(0b00000110); // Set the mode to increment address by one
  LCDSendAnInstruction(0b00000001); // Clears the display
  LCDSendCharacter('A');

  LCDSendAstring('NewbieHack');
  LCDSendAnInteger(IntegerToBeDisplayed, LengthOfDigits);

  /*
  // Test bit 0 on byte and send it to the port B pin 12
  if (character & 0b00000001) {
	GPIOB ->BSRR |= GPIO_BSRR_BS_12;
  }
  else
  {
	GPIOB ->BSRR |= GPIO_BSRR_BR_12;
  };

  // Test bit 1 on byte and send it to the port B pin 13
  if (character & 0b00000010) {
	GPIOB ->BSRR |= GPIO_BSRR_BS_13;
  }
  else
  {
	GPIOB ->BSRR |= GPIO_BSRR_BR_13;
  };

  // Test bit 2 on byte and send it to the port B pin 14
  if (character & 0b00000100) {
	GPIOB ->BSRR |= GPIO_BSRR_BS_14;
  }
  else
  {
	GPIOB ->BSRR |= GPIO_BSRR_BR_14;
  };

  // Test bit 3 on byte and send it to the port B pin 15
  if (character & 0b00001000) {
	GPIOB ->BSRR |= GPIO_BSRR_BS_15;
  }
  else
  {
	GPIOB ->BSRR |= GPIO_BSRR_BR_15;
  };

  if (character & 0b00010000) {
	GPIOC ->BSRR |= GPIO_BSRR_BS_6;
  }
  else
  {
	GPIOC ->BSRR |= GPIO_BSRR_BR_6;
  };

  if (character & 0b00100000) {
	GPIOC ->BSRR |= GPIO_BSRR_BS_7;
  }
  else
  {
	GPIOC ->BSRR |= GPIO_BSRR_BR_7;
  };

  if (character & 0b01000000) {
	GPIOC ->BSRR |= GPIO_BSRR_BS_8;
  }
  else
  {
	GPIOC ->BSRR |= GPIO_BSRR_BR_8;
  };

  if (character & 0b10000000) {
	GPIOC ->BSRR |= GPIO_BSRR_BS_9;
  }
  else
  {
	GPIOB ->BSRR |= GPIO_BSRR_BR_9;
  };
  */

  while (1)
  {
	  for (int i = 0; i < 128; i++) {
		  LCDSendAnInstruction(0b10000000);
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

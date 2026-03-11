#include "stm32f0xx.h"
#include "main.h"


void SystemClock_Config(void);

void I2C_StartWrite(int Number_of_Bytes_to_write)
{
	 I2C1->CR2 &= ~I2C_CR2_RD_WRN;
	 I2C1->CR2 |= (Number_of_Bytes_to_write << 16);
	 I2C1 ->CR2 |= I2C_CR2_START;
	 while(I2C1 ->CR2 & I2C_CR2_START);
}

void I2C_StartRead(int Number_of_Bytes_to_read)
{
	 I2C1->CR2 &= ~I2C_CR2_RD_WRN;
	 I2C1->CR2 |= (Number_of_Bytes_to_read << 16);
	 I2C1 ->CR2 |= I2C_CR2_START;
	 while(I2C1 ->CR2 & I2C_CR2_START);
}

int main(void)
{

  HAL_Init();
  SystemClock_Config();

  RCC ->AHBENR |= RCC_AHBENR_GPIOBEN;

  // Enable clock for GPIO port B
     RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

     // ------------------------------------------------
     // Configure PB6 and PB7 as Alternate Function mode
     // (needed for I2C communication)
     // MODER bits: 10 = Alternate Function
     // ------------------------------------------------

     GPIOB->MODER &= ~GPIO_MODER_MODER6_0; // Clear bit 0 of MODER6
     GPIOB->MODER |=  GPIO_MODER_MODER6_1; // Set bit 1 -> PB6 in Alternate Function mode

     GPIOB->MODER &= ~GPIO_MODER_MODER7_0; // Clear bit 0 of MODER7
     GPIOB->MODER |=  GPIO_MODER_MODER7_1; // Set bit 1 -> PB7 in Alternate Function mode


     // ------------------------------------------------
     // Configure output type
     // I2C requires open-drain outputs -> There is no need for Push-Pull because the HIGH staet is provided by outer voltage
     // ------------------------------------------------

     GPIOB->OTYPER |= GPIO_OTYPER_OT_6; // Set PB6 as open-drain
     GPIOB->OTYPER |= GPIO_OTYPER_OT_7; // Set PB7 as open-drain


     // ------------------------------------------------
     // Configure output speed
     // Lower speed is usually sufficient for I2C
     // ------------------------------------------------

     GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6_0; // Low speed for PB6
     GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR7_0; // Low speed for PB7


     // ------------------------------------------------
     // Disable internal pull-up and pull-down resistors
     // (I2C typically uses external pull-up resistors)
     // ------------------------------------------------

     GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR6; // No pull-up/pull-down on PB6
     GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR7; // No pull-up/pull-down on PB7


     // ------------------------------------------------
     // Configure Alternate Function for PB6 and PB7
     // These pins are set to AF1 for I2C1 on many STM32 devices
     // AFR[0] controls pins 0–7
     // ------------------------------------------------

     GPIOB->AFR[0] = 0b00010001000000000000000000000000;


     // ------------------------------------------------
     // Enable clock for I2C1 peripheral
     // ------------------------------------------------

     RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;


     // ------------------------------------------------
     // Configure I2C timing register
     // Value depends on system clock and desired I2C speed -> The number is provided by Excel official document where you specify parameters
     // ------------------------------------------------

     I2C1->TIMINGR = (uint32_t)0x0010020A;


     // ------------------------------------------------
     // Enable the I2C peripheral
     // ------------------------------------------------

     I2C1->CR1 |= I2C_CR1_PE;


     // ------------------------------------------------
     // Set the slave device address
     // Address is shifted left because the lowest bit is R/W
     // ------------------------------------------------

     I2C1->CR2 |= (0x1D << 1);


     // ------------------------------------------------
     // Select write operation
     // RD_WRN = 0 means write
     // ------------------------------------------------

     I2C1->CR2 &= ~I2C_CR2_RD_WRN;


     // ------------------------------------------------
     // Set number of bytes to transfer (1 byte)
     // Bits 16–23 of CR2 represent NBYTES
     // ------------------------------------------------

     I2C1->CR2 |= (1 << 16);


     // ------------------------------------------------
	 // Generate START condition
	 // This begins the I2C transfer on the bus
	 // ------------------------------------------------

	 I2C1 ->CR2 |= I2C_CR2_START;


	 // Wait until START condition is cleared by hardware
	 // This means the start condition has been generated
	 while(I2C1 ->CR2 & I2C_CR2_START);


	 // ------------------------------------------------
	 // Write data byte into TXDR register
	 // Hardware will automatically transmit it
	 // ------------------------------------------------

	 I2C1 ->TXDR = 0x2C;


	 // ------------------------------------------------
	 // Wait until transmit register becomes empty
	 // TXE = transmit data register empty
	 // Means byte has been moved to shift register
	 // ------------------------------------------------

	 while(!(I2C1 ->ISR & I2C_ISR_TXE));

	 I2C1->CR2 |= I2C_CR2_RD_WRN;

	 // ------------------------------------------------
	 // Generate STOP condition
	 // Ends the I2C communication
	 // ------------------------------------------------
	 I2C1 ->CR2 |= I2C_CR2_STOP;

	 // Wait until STOP condition is cleared
	 while(I2C1 ->CR2 & I2C_CR2_STOP);


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

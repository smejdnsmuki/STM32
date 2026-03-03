#ifndef LCDFunctionsHeader
#define LCDFunctionsHeader

#include "stdio.h"

#define LCDD0pin 15
#define LCDD0Port GPIOB
#define LCDD1pin 6
#define LCDD1Port GPIOC
#define LCDD2pin 7
#define LCDD2Port GPIOC
#define LCDD3pin 8
#define LCDD3Port GPIOC
#define LCDD4pin 9
#define LCDD4Port GPIOC
#define LCDD5pin 8
#define LCDD5Port GPIOA
#define LCDD6pin 9
#define LCDD6Port GPIOA
#define LCDD7pin 10
#define LCDD7Port GPIOA

#define LCDEnablePin 14
#define LCDEnablePort GPIOB
#define LCDReadWritePin 13
#define LCDReadWritePort GPIOB
#define LCDRegisterSelectPin 12
#define LCDRegisterSelectPort GPIOB

#define TimeDelayBeforeEnable 40000
#define TimeDelayBeforeDisable 80000

void notExactTimeDelay(int timeDelay)
{	
	volatile int i;
	for (int i; i < timeDelay; i++) 
	{
	
	}
}

void SetPortAndPinForOutput(GPIO_TypeDef *port, int pinNumber)
{
	  if (port == GPIOA) {
		  RCC ->AHBENR |= RCC_AHBENR_GPIOAEN;
	  }
	  if (port == GPIOB) {
		  RCC ->AHBENR |= RCC_AHBENR_GPIOBEN;
	  }
	  if (port == GPIOC) {
		  RCC ->AHBENR |= RCC_AHBENR_GPIOCEN;
	  }
	  if (port == GPIOD) {
		  RCC ->AHBENR |= RCC_AHBENR_GPIODEN;
	  }
	  if (port == GPIOF) {
		  RCC ->AHBENR |= RCC_AHBENR_GPIOFEN;
	  }

	  port ->MODER &= ~(1 << ((pinNumber*2) + 1));
	  port ->MODER |= (1 << pinNumber*2);

	  port ->OTYPER &= ~(1 << pinNumber);

	  port ->OSPEEDR |= (1 << ((pinNumber*2) + 1)) | (1 << (pinNumber*2));

	  port ->PUPDR &= ~(1 << pinNumber);
}

void InitializePortsForLCD()
{
	  SetPortAndPinForOutput(LCDD0Port, LCDD0pin);
	  SetPortAndPinForOutput(LCDD1Port, LCDD1pin);
	  SetPortAndPinForOutput(LCDD2Port, LCDD2pin);
	  SetPortAndPinForOutput(LCDD3Port, LCDD3pin);
	  SetPortAndPinForOutput(LCDD4Port, LCDD4pin);
	  SetPortAndPinForOutput(LCDD5Port, LCDD5pin);
	  SetPortAndPinForOutput(LCDD6Port, LCDD6pin);
	  SetPortAndPinForOutput(LCDD7Port, LCDD7pin);
	  
	  SetPortAndPinForOutput(LCDEnablePort, LCDEnablePin);
	  SetPortAndPinForOutput(LCDReadWritePort, LCDReadWritePin);
	  SetPortAndPinForOutput(LCDRegisterSelectPort, LCDRegisterSelectPin);
	  
}

void SendBitToPortAndPin(GPIO_TypeDef *port, int pinNumber, uint8_t bitState)
{
	  if (bitState) {
		port ->BSRR |= (1 << pinNumber);	//GPIO_BSRR_Bs_12	// BSRR uses all 32 bits
	  }
	  else
	  {
		port ->BRR |= (1 << pinNumber);	// BSS uses only first 16 bits -> now we have both 0x00001000
	  };
}


void LCDEnable()
{
	notExactTimeDelay(TimeDelayBeforeEnable);
	SendBitToPortAndPin(LCDEnablePort, LCDEnablePin, 0);
}

void LCDSetToWrite()
{
	SendBitToPortAndPin(LCDReadWritePort, LCDReadWritePin, 0);
	LCDEnable();
}

void LCDSetToRead()
{
	SendBitToPortAndPin(LCDReadWritePort, LCDReadWritePin, 1);
	LCDEnable();
}

void LCDInstructionMode()
{
	SendBitToPortAndPin(LCDRegisterSelectPort, LCDRegisterSelectPin, 0);
	LCDEnable();
}

void LCDCharacterMode()
{
	SendBitToPortAndPin(LCDRegisterSelectPort, LCDRegisterSelectPin, 1);
	LCDEnable();
}

void LCDSendByteToTheLCDDataPins(char character)
{
	  SendBitToPortAndPin(LCDD0Port, LCDD0pin, character & 0b00000001);
	  SendBitToPortAndPin(LCDD1Port, LCDD1pin, character & 0b00000010);
	  SendBitToPortAndPin(LCDD2Port, LCDD2pin, character & 0b00000100);
	  SendBitToPortAndPin(LCDD3Port, LCDD3pin, character & 0b00001000);
	  SendBitToPortAndPin(LCDD4Port, LCDD4pin, character & 0b00010000);
	  SendBitToPortAndPin(LCDD5Port, LCDD5pin, character & 0b00100000);
	  SendBitToPortAndPin(LCDD6Port, LCDD6pin, character & 0b01000000);
	  SendBitToPortAndPin(LCDD7Port, LCDD7pin, character & 0b10000000);
	  notExactTimeDelay(TimeDelayBeforeDisable);
	  SendBitToPortAndPin(LCDEnablePort, LCDEnablePin, 0);
}


void LCDSendCharacter(char character)
{
	LCDSetToWrite();
	LCDCharacterMode();
	LCDSendByteToTheLCDDataPins(character);
	LCDEnable();
}

void LCDSendAnInstruction(char character)
{
	LCDSetToWrite();
	LCDInstructionMode();
	LCDSendByteToTheLCDDataPins(character);
	LCDEnable();
}

void LCDSendAstring(char *StringOfCharacters)
{
	// The condition is not true only when 0 or null
	while(*StringOfCharacters)
	{
		LCDSendCharacter(*StringOfCharacters++);  
	}
}

void LCDSendAnInteger(int IntegerToBeDisplayed, uint8_t LengthOfDigits)
{
	char StringNumber[LengthOfDigits];
	sprintf(StringNumber, "%d", IntegerToBeDisplayed);
	
	LCDSendAStirng(StirngNumber);
}

void LCDSetCursorLocation(uint8_t X, uint8_t Y)
{
	LCDSendAnInstruction(0x80 + X + (Y*40)); // 40 is for particular LCD - has to be changed 
}


#endif
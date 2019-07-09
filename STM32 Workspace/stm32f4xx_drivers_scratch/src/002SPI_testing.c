/*
 * 002SPI_testing.c
 *
 *  Created on: Jun 2, 2019
 *      Author: dhilan22
 */

#include <string.h>
#include "stm32f407xx.h"

/* For this source file, the exercise will be to send "Hello World" data over SPI2 communication:
 * Initialize the appropriate GPIO pins to work for SPI communication (refer to Alt. Function Mapping and
 * select the set of SPI pins for the correct application)
 * 	PB14 --> SPI2_MISO
 * 	PB15 --> SPI2_MOSI
 * 	PB13 --> SPI2_SCLK
 * 	PB12 --> SPI2_NSS
 * 	Alternate Function Mode --> 5
 */
void SPI_GPIOInit(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFUN;
	SPIPins.GPIO_PinConfig.GPIO_AltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;	// Required since you are driving a slave, and want to avoid possibility of junk data being sent to it
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// MISO
	//SPIPins.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_14;
	//GPIO_Init(&SPIPins);

	// MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_15;
	GPIO_Init(&SPIPins);

	// SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_13;
	GPIO_Init(&SPIPins);

	// NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_12;
	GPIO_Init(&SPIPins);
}

void SPI2_Init(void)
{
	SPI_Handle_t SPI2Handle;
	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPI_PinConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD; // Full-duplex
	SPI2Handle.SPI_PinConfig.SPI_DeviceMode = SPI_MODE_MASTER;	// Master Mode
	SPI2Handle.SPI_PinConfig.SPI_SclkSpeed = SPI_SPEED_DIV8; 	// Generates SCLK of 2 MHz
	SPI2Handle.SPI_PinConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPI_PinConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPI_PinConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPI_PinConfig.SPI_SSM = SPI_SSM_DI;				// SSM disabled for NSS pin

	SPI_Init(&SPI2Handle);
}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GpioBlueBtn;
	memset(&GpioBlueBtn,0,sizeof(GpioBlueBtn));						// Clear default garbage bits
	GpioBlueBtn.pGPIOx = GPIOA;										// Create structure pointer and set to port A (user button found there)
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_0;			// Set pin number to 0 (corresponds to blue user button)
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;			// Pin mode is input
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;		// Set speed to fast
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;	// Push-pull
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&GpioBlueBtn);

	GPIO_Handle_t GpioLED1;										// Create a handle for the GPIO LED pin
	memset(&GpioLED1,0,sizeof(GpioLED1));						// Clear default garbage bits
	GpioLED1.pGPIOx = GPIOD;									// Create structure pointer and set to port D (since LED pins are found there)
	GpioLED1.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_12;			// Set pin number to 12 (corresponds to green LED)
	GpioLED1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;		// Set pin mode to output (default is push-pull)
	GpioLED1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;	// Set speed to fast
	GpioLED1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;	// Push-pull
	GpioLED1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&GpioLED1);
}

void delay(void)
{
	for (uint32_t i=0; i<500000/2; i++); // ~200 ms if system clock is at 16 MHz
}

int main(void)
{
	char user_data[] = "Hello World";

	SPI_GPIOInit();		// This function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_Init();		// This function is used to initialize the SPI2 peripheral parameters
	// SPI_SSIConfig(SPI2, ENABLE);	// If SSM is enabled, must set SSI=1 to avoid MODF error which occurs when SSI=0
	SPI_SSOEConfig(SPI2, ENABLE);	// Set SSOE bit to 1 to enable NSS output
	GPIO_ButtonInit();

	while(1)
	{
		while( ! GPIO_ReadPin(GPIOA,GPIO_PIN_0) );	// When the user button is pressed, the state will be high (1), and so the whole expression evaluates to false (0), leaving the loop
		delay();									// To avoid button de-bouncing

		GPIO_TogglePin(GPIOD, GPIO_PIN_12);		// testing/debugging to ensure button is working

		SPI_PeriControl(SPI2, ENABLE);			// Enable the SPI2 peripheral by pulling NSS to low
		uint8_t dataLen = strlen(user_data); 	// First send length information since Arduino sketch expects 1 byte of length info followed by data
		SPI_SendData(SPI2, &dataLen, 1);		// 1 byte of data is sent representing how much data will be sent thereafter
		SPI_SendData(SPI2,(uint8_t*)user_data,strlen(user_data));	// Send "Hello World" string using SPI2 communication

		// while ( ( SPI2->SR & (1 << SPI_SR_BSY) ) ); // Confirm that SPI is not busy (BSY=0)
		while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) );	// Confirm that SPI is not busy (BSY=0)
		SPI_PeriControl(SPI2, DISABLE);	// Disable the SPI2 peripheral after data transfer is complete by pulling NSS to high
	}
	return 0;
}

/*
 * 001LED_toggle.c
 *
 *  Created on: May 28, 2019
 *      Author: dhilan22
 */

#include <string.h>
#include "stm32f407xx.h"

void delay(void)
{
	for (uint32_t i=0; i<500000/2; i++); // ~200 ms if system clock is at 16 MHz
}

int main(void)
{
	GPIO_Handle_t GpioLED1;										// Create a handle for the GPIO LED pin
	memset(&GpioLED1,0,sizeof(GpioLED1));						// Clear default garbage bits
	GpioLED1.pGPIOx = GPIOD;									// Create structure pointer and set to port D (since LED pins are found there)
	GpioLED1.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_12;			// Set pin number to 12 (corresponds to green LED)
	GpioLED1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;		// Set pin mode to output (default is push-pull)
	GpioLED1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;	// Set speed to fast
	GpioLED1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;	// Push-pull
	GpioLED1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;	// No pull-up or pull-down resistor control required

	GPIO_Handle_t GpioLED2;
	memset(&GpioLED2,0,sizeof(GpioLED2));						// Clear default garbage bits
	GpioLED2.pGPIOx = GPIOD;									// Create structure pointer and set to port D (since LED pins are found there)
	GpioLED2.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_13;			// Set pin number to 13 (corresponds to orange LED)
	GpioLED2.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;		// Set pin mode to output (default is push-pull)
	GpioLED2.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;	// Set speed to fast
	GpioLED2.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;	// Open-Drain
	GpioLED2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;	// Activate pull-up resistor (this will toggle the red LED but at a low intensity since the internal resistor value is high ~40kOhm - for full intensity in OD configuration, use external resistor)

	GPIO_Handle_t GpioLED3;										// Create a handle for the GPIO LED pin
	memset(&GpioLED3,0,sizeof(GpioLED3));						// Clear default garbage bits
	GpioLED3.pGPIOx = GPIOD;									// Create structure pointer and set to port D (since LED pins are found there)
	GpioLED3.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_15;			// Set pin number to 15 (corresponds to blue LED)
	GpioLED3.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;		// Set pin mode to output (default is push-pull)
	GpioLED3.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;	// Set speed to fast
	GpioLED3.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;	// Push-pull
	GpioLED3.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;	// No pull-up or pull-down resistor control required

	GPIO_Handle_t GpioBlueBtn;
	memset(&GpioBlueBtn,0,sizeof(GpioBlueBtn));						// Clear default garbage bits
	GpioBlueBtn.pGPIOx = GPIOA;										// Create structure pointer and set to port A (user button found there)
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_0;			// Set pin number to 0 (corresponds to blue user button)
	// GpioBlueBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;		// Pin mode can simply be input, but FT mode is used for demonstration
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;		// The user button will be used to issue an interrupt during the falling edge, and consequently toggle the blue LED
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;		// Set speed to fast
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;	// No pull-up or pull-down required because the schematic for the button already shows built-in resistors

	// GPIO_PeriCLKControl(GPIOA, ENABLE);	// Peripheral clocks are now enabled during GPIO_Init, no need to here
	// GPIO_PeriCLKControl(GPIOD, ENABLE);
	GPIO_Init(&GpioLED1);					// Initialize pin 12 with all the above settings by passing the handle containing all the information
	GPIO_Init(&GpioLED2);					// Initialize pin 13
	GPIO_Init(&GpioLED3);					// Initialize pin 14
	GPIO_Init(&GpioBlueBtn);				// Initialize user button for issuing ISR

	// IRQ configurations
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0,NVIC_IRQ_PRI15);	// Set interrupt priority on EXTI0 line to highest (15) priority
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0,ENABLE);			// Enable interrupt over EXTI0 line

	while(1)
	{	// Toggle green and orange LEDs sequentially
		GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		delay(); delay();
		GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	}

	return 0;
}

/*
 * Strictly speaking, ISRs are not actually implemented in the driver code, since they are application
 * specific and should be implemented in user application i.e. main.c / this file. The ISR implemented in the
 * application layer should then call the driver-supported GPIO handling in order to process the interrupt.
 * The startup code will give the weak implementation of ISRs. Whenever the real ISR implementation is needed,
 * you need to override that function in the application, as done below, using EXTI0 as an example since the
 * ISR will be issued over that line due to the user button being GPIO_PIN_0.
 */
void EXTI0_IRQHandler(void)
{
	// Handle the interrupt
	delay();							// Small delay is necessary to avoid button de-bouncing problem
	GPIO_IRQHandling(GPIO_PIN_0);		// Issue the interrupt once the user button is pressed
	GPIO_TogglePin(GPIOD,GPIO_PIN_15);	// Toggle the blue LED after the ISR
}

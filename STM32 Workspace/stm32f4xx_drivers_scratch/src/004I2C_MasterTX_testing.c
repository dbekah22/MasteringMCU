/*
 * 004I2C_MasterTX_testing.c
 *
 *  Created on: Jun 25, 2019
 *      Author: dhilan22
 */

#include <stdio.h>
#include <string.h>
#include "stm32f407xx.h"

#define MY_ADDR		0x61
#define SLAVE_ADDR	0x68

// Data to be sent for TX mode
uint8_t my_data[] = "We are testing I2C Master TX\n";

I2C_Handle_t I2C1Handle;

/* For this source file, the exercise will be to test Master TX (transmission) of a string through I2C:
 * Initialize the appropriate GPIO pins to work for I2C communication (refer to Alt. Function Mapping and
 * select the set of I2C pins for the correct application)
 * 	PB6 --> I2C1_SCL
 * 	PB7 --> I2C1_SDA
 * 	Alternate Function Mode --> 4
 */
void I2C_GPIOInit(void)
{
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFUN;
	I2CPins.GPIO_PinConfig.GPIO_AltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;	// Required since you are driving a slave, and want to avoid possibility of junk data being sent to it
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCL
	I2CPins.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_6;
	GPIO_Init(&I2CPins);

	// SDA
	I2CPins.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_7;
	GPIO_Init(&I2CPins);

}

void I2C1_Init(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_PinConfig.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_PinConfig.I2C_DeviceAddress = MY_ADDR;	// Random address (as long as it is unreserved) since it does not matter in this case because we are programming the master and not the slave
	I2C1Handle.I2C_PinConfig.I2C_FMDutyCycle = I2C_FM_2;
	I2C1Handle.I2C_PinConfig.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);
}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GpioBlueBtn;
	memset(&GpioBlueBtn,0,sizeof(GpioBlueBtn));						// Clear default garbage bits
	GpioBlueBtn.pGPIOx = GPIOA;										// Create structure pointer and set to port A (user button found there)
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_0;			// Set pin number to 0 (corresponds to blue user button)
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;			// Pin mode is input
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;		// Set speed to fast
	GpioBlueBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&GpioBlueBtn);
}

void delay(void)
{
	for (uint32_t i=0; i<500000/2; i++); // ~200 ms if system clock is at 16 MHz
}

int main(void)
{
	// Initialize all required pins and ports
	GPIO_ButtonInit();
	I2C_GPIOInit();
	I2C1_Init();

	// Set ACK bit to 1 after PE=1 (hardware clears ACK bit to 0 when PE=0)
	I2C_PeriControl(I2C1,ENABLE);

	while(1)
	{
		// TX Testing - Master sending some data (my_data) to slave to be read
		while( ! GPIO_ReadPin(GPIOA,GPIO_PIN_0) );	// Wait for User Button Press
		delay();

		// Send some data to the slave
		I2C_MasterSendData(&I2C1Handle, my_data, strlen((char*)my_data), SLAVE_ADDR);
	}
}


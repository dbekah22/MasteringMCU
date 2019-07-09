/*
 * stm32f407xx_gpio.h
 *
 *  Created on: May 21, 2019
 *      Author: dhilan22
 *
 *      GPIO Driver Header File
 */

#ifndef INC_STM32F407XX_GPIO_H_
#define INC_STM32F407XX_GPIO_H_

#include "stm32f407xx.h"

typedef struct
{
	uint8_t GPIO_PinNum;			// Possible values from @GPIO_PIN_NUMBERS
	uint8_t GPIO_PinMode;			// Possible values from @GPIO_PIN_MODES
	uint8_t GPIO_PinSpeed;			// Possible values from @GPIO_PIN_SPEEDS
	uint8_t GPIO_PinPuPdControl;	// Possible values from @GPIO_PIN_PUPD_CONFIGS
	uint8_t GPIO_PinOPType;			// Possible values from @GPIO_PIN_OUT_TYPES
	uint8_t GPIO_AltFunMode;

}GPIO_PinConfig_t;

// This is the handle structure for a GPIO pin
typedef struct
{
	GPIO_RegDef_t *pGPIOx;				// Pointer to hold base address of GPIOx peripheral
	GPIO_PinConfig_t GPIO_PinConfig;

}GPIO_Handle_t;


// @GPIO_PIN_NUMBERS - GPIO Possible Pin Numbers
#define GPIO_PIN_0			0
#define GPIO_PIN_1			1
#define GPIO_PIN_2			2
#define GPIO_PIN_3			3
#define GPIO_PIN_4			4
#define GPIO_PIN_5			5
#define GPIO_PIN_6			6
#define GPIO_PIN_7			7
#define GPIO_PIN_8			8
#define GPIO_PIN_9			9
#define GPIO_PIN_10			10
#define GPIO_PIN_11			11
#define GPIO_PIN_12			12
#define GPIO_PIN_13			13
#define GPIO_PIN_14			14
#define GPIO_PIN_15			15

// @GPIO_PIN_MODES - GPIO Possible Pin Modes
#define GPIO_MODE_IN		0
#define GPIO_MODE_OUT		1
#define GPIO_MODE_ALTFUN	2
#define GPIO_MODE_ANALOG	3
#define GPIO_MODE_IT_FT		4	// Interrupt mode
#define GPIO_MODE_IT_RT		5	// Interrupt mode
#define GPIO_MODE_IT_RFT	6	// Interrupt mode

// @GPIO_PIN_OUT_TYPES - GPIO pin Possible Output Types
#define GPIO_OP_TYPE_PP		0
#define GPIO_OP_TYPE_OD		1

// @GPIO_PIN_SPEEDS - GPIO pin Possible Output Speeds
#define GPIO_SPEED_LOW		0
#define GPIO_SPEED_MEDIUM	1
#define GPIO_SPEED_FAST		2
#define GPIO_SPEED_HIGH		3

// @GPIO_PIN_PUPD_CONFIGS - GPIO pin Pull-up and Pull-down Configurations
#define GPIO_NO_PUPD		0
#define GPIO_PIN_PU			1
#define GPIO_PIN_PD			2


/*******************************************/
/***** 		Driver-supported APIs 	   *****/
/*******************************************/

// Peripheral Clock Setup
void GPIO_PeriCLKControl(GPIO_RegDef_t *pGPIOx, uint8_t ENorDI);

// Initialize and De-initialize
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

//Data Read & Write
uint8_t GPIO_ReadPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNum);
uint16_t GPIO_ReadPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WritePin(GPIO_RegDef_t *pGPIOx, uint8_t PinNum, uint8_t Value);
void GPIO_WritePort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_TogglePin(GPIO_RegDef_t *pGPIOx, uint8_t PinNum);

// IRQ Configuration and ISR Handling
void GPIO_IRQInterruptConfig(uint8_t IRQNum, uint8_t ENorDI);
void GPIO_IRQPriorityConfig(uint8_t IRQNum, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNum);


#endif /* INC_STM32F407XX_GPIO_H_ */

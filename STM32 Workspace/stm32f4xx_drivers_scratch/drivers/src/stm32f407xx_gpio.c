/*
 * stm32f407xx_gpio.c
 *
 *  Created on: May 21, 2019
 *      Author: dhilan22
 *
 *      GPIO Driver Source File
 */

#include "stm32f407xx_gpio.h"

/************************************************************************
 * @fn			- GPIO_PeriCLKControl
 *
 * @brief		- This function enables or disables peripheral clocks for GPIO ports
 *
 * @pGPIOx		- Base address pointer to GPIO peripheral
 * @ENorDI		- Enable or Disable macros
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void GPIO_PeriCLKControl(GPIO_RegDef_t *pGPIOx, uint8_t ENorDI)
{
	if (ENorDI == ENABLE)
	{
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_EN();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_EN();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_EN();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_EN();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_EN();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_EN();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_EN();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLK_EN();
		} else if (pGPIOx == GPIOJ) {
			GPIOJ_PCLK_EN();
		} else if (pGPIOx == GPIOK) {
			GPIOK_PCLK_EN();
		}
	}
	else
	{
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_DI();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_DI();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_DI();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_DI();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_DI();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_DI();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_DI();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_DI();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLK_DI();
		} else if (pGPIOx == GPIOJ) {
			GPIOJ_PCLK_DI();
		} else if (pGPIOx == GPIOK) {
			GPIOK_PCLK_DI();
		}
	}
}


/************************************************************************
 * @fn			- GPIO_Init
 *
 * @brief		- This function initializes the appropriate GPIO ports / pins
 *
 * @pGPIOHandle	- Pointer to handle access to different GPIO registers for configuration
 *
 * @return		- none
 *
 * @Note		- It is important to use bitwise OR to set the physical registers in
 * 				  order to prevent affecting all the bits in the register
 *
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;

	// Enable the GPIO peripheral clock in the driver instead of the user application to avoid the user forgetting
	GPIO_PeriCLKControl(pGPIOHandle->pGPIOx, ENABLE);

	// 1. Configure Mode
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		// Non-interrupt Modes
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNum) );
		// The following will clear (reset) the bit field by negating binary 11 (represented by 0x3 in hexadecimal) in the two-bit field
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);
		// The following then sets the register's bit field to the value stored in temp
		pGPIOHandle->pGPIOx->MODER |= temp;

	} else {
		// Interrupt Modes
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			// 1. Configure FTSR (falling trigger selection register)
			EXTI->FTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum );
			// Clear corresponding RTSR bit
			EXTI->RTSR &= ~( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum );

		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT) {
			// 1. Configure RTSR (rising trigger selection register)
			EXTI->RTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum );
			// Clear corresponding FTSR bit
			EXTI->FTSR &= ~( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum );

		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT) {
			// 1. Configure RFTSR (i.e. set both)
			EXTI->RTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum );
			// Clear corresponding FTSR bit
			EXTI->FTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum );

		}

		// 2. Configure GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNum / 4;	// Get EXTICR number in array
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNum % 4;	// Get bit position for EXTIx
		uint8_t portCode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);	// Get corresponding port code
		SYSCFG_PCLK_EN();												// Enable peripheral clock
		SYSCFG->EXTICR[temp1] = portCode << (temp2 *4);					// Set the appropriate bit field of the EXTIx register

		// 3. Enable EXTI interrupt delivery using IMR
		EXTI->IMR |= 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum;
	}
	temp = 0;

	// 2. Configure Speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNum) );
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	temp = 0;

	// 3. Configure Pull-up / Pull-down
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNum) );
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);
	pGPIOHandle->pGPIOx->PUPDR |= temp;
	temp = 0;

	// 4. Configure OPType
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum );
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	temp = 0;

	// 5. Configure Alt. Functionality
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ALTFUN)
	{
		// Configure alt.function registers
		uint8_t temp1, temp2;
		temp1 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNum) / 8;
		temp2 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNum) % 8;
		// Reset 4-bit field by negating binary 1111, represented as 0xF in hexadecimal
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2) );
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_AltFunMode << (4 * temp2) );
	}
}


/************************************************************************
 * @fn			- GPIO_DeInit
 *
 * @brief		- This function de-initializes the appropriate GPIO ports / pins
 *
 * @pGPIOHandle	-
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if (pGPIOx == GPIOA) {
		GPIOA_REG_RESET();
	} else if (pGPIOx == GPIOB) {
		GPIOB_REG_RESET();
	} else if (pGPIOx == GPIOC) {
		GPIOC_REG_RESET();
	} else if (pGPIOx == GPIOD) {
		GPIOD_REG_RESET();
	} else if (pGPIOx == GPIOE) {
		GPIOE_REG_RESET();
	} else if (pGPIOx == GPIOF) {
		GPIOF_REG_RESET();
	} else if (pGPIOx == GPIOG) {
		GPIOG_REG_RESET();
	} else if (pGPIOx == GPIOH) {
		GPIOH_REG_RESET();
	} else if (pGPIOx == GPIOI) {
		GPIOI_REG_RESET();
	} else if (pGPIOx == GPIOJ) {
		GPIOJ_REG_RESET();
	} else if (pGPIOx == GPIOK) {
		GPIOK_REG_RESET();
	}
}


/************************************************************************
 * @fn			- GPIO_ReadPin
 *
 * @brief		- This function reads data from the input pin
 *
 * @pGPIOx		- Base address pointer to GPIO peripheral
 * @PinNum		- Pin Number
 *
 * @return		- 0 or 1
 *
 * @Note		- To read the value at the nth pin in the input data register (IDR), right shift the
 * 				  bit position by n to bring it to the 0th position, and then mask all other bits and
 * 				  return just the value at the 0th position (i.e. only 0th position will have a 1)
 *
 */
uint8_t GPIO_ReadPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNum)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNum) & 0x00000001);
	return value;
}


/************************************************************************
 * @fn			- GPIO_ReadPort
 *
 * @brief		- This function reads data from the input port
 *
 * @pGPIOx		- Base address pointer to GPIO peripheral
 *
 * @return		- Value
 *
 * @Note		- none
 *
 */
uint16_t GPIO_ReadPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t)(pGPIOx->IDR);
	return value;
}


/************************************************************************
 * @fn			- GPIO_WritePin
 *
 * @brief		- This function writes data to the ODR for the appropriate output pin
 *
 * @pGPIOx		- Base address pointer to GPIO peripheral
 * @PinNum		- Pin Number
 * @Value		- Value to be written to pin (0 or 1)
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void GPIO_WritePin(GPIO_RegDef_t *pGPIOx, uint8_t PinNum, uint8_t Value)
{
	if (Value == GPIO_PIN_SET) { pGPIOx->ODR |= (1 << PinNum); } // Write 1
	else { pGPIOx->ODR &= ~(1 << PinNum); }						 // Write 0
}


/************************************************************************
 * @fn			- GPIO_WritePort
 *
 * @brief		- This function writes data to the ODR for the appropriate port
 *
 * @pGPIOx		- Base address pointer to GPIO peripheral
 * @Value		- Value to be written to port
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void GPIO_WritePort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}


/************************************************************************
 * @fn			- GPIO_TogglePin
 *
 * @brief		- This function toggles the required bit field for the corresponding pin
 *
 * @pGPIOx		- Base address pointer to GPIO peripheral
 * @PinNum		- Pin Number
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void GPIO_TogglePin(GPIO_RegDef_t *pGPIOx, uint8_t PinNum)
{
	pGPIOx->ODR ^= (1 << PinNum); // XOR to toggle the bit state
}


/************************************************************************
 * @fn			- GPIO_IRQInterruptConfig
 *
 * @brief		- This function configures the IRQ interrupt by either setting or clearing (ENorDI) it
 *
 * @IRQNum		- IRQ Number
 * @ENorDI		- Enable or Disable
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNum, uint8_t ENorDI)
{
	if (ENorDI==ENABLE)	// If true, set the interrupt with ISE register
	{
		if (IRQNum <= 31)
		{
			// Program ISER0 register
			*NVIC_ISER0 |= (1 << IRQNum);	// Set the appropriate bit field of the register corresponding to the IRQ number to 1

		} else if (IRQNum > 31 && IRQNum < 64)
		{
			// Program ISER1 register
			*NVIC_ISER1 |= (1 << (IRQNum % 32));

		} else if (IRQNum >= 64 && IRQNum < 95)
		{
			// Program ISER2 register (this is sufficient because the number of IRQ numbers implemented in this MCU is <90)
			*NVIC_ISER2 |= (1 << (IRQNum % 64));
		}
	} else				// // Else, clear the interrupt with ICE register
	{
		if (IRQNum <= 31)
		{
			// Program ICER0 register
			*NVIC_ICER0 |= (1 << IRQNum);

		} else if (IRQNum > 31 && IRQNum < 64)
		{
			// Program ICER1 register
			*NVIC_ICER1 |= (1 << (IRQNum % 32));

		} else if (IRQNum >= 64 && IRQNum < 95)
		{
			// Program ICER2 register
			*NVIC_ICER2 |= (1 << (IRQNum % 64));
		}
	}
}


/************************************************************************
 * @fn			- GPIO_IRQPriorityConfig
 *
 * @brief		- This function configures the IRQ priority
 *
 * @IRQNum		- IRQ Number
 * @IRQPriority	- IRQ Priority
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNum, uint32_t IRQPriority)
{
	// 1. Find out the IPR Register
	uint8_t IPRx = IRQNum / 4;
	uint8_t IPRx_section = IRQNum % 4;

	uint8_t shift_amount = (8*IPRx_section) + (8-NO_PR_BITS_IMP); // In this MCU device, lower 4 bits of each section are not implemented so you must shift the bits by 4

	*(NVIC_PR_BASE + IPRx) |= ( IRQPriority << shift_amount ); // LS jumps to appropriate register address, RS sets the correct section to the IRQ Priority

}


/************************************************************************
 * @fn			- GPIO_IRQHandling
 *
 * @brief		- This function
 *
 * @PinNum		- Pin Number
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void GPIO_IRQHandling(uint8_t PinNum)
{
	// Clear the EXTI PR corresponding to the Pin Number
	if (EXTI->PR & (1 << PinNum))
	{
		// Clear pending register bit
		EXTI->PR |= (1 << PinNum);
	}
}

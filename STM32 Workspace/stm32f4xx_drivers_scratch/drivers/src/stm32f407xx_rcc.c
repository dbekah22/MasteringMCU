/*
 * stm32f407xx_rcc.c
 *
 *  Created on: Jul 4, 2019
 *      Author: dhilan22
 */

#include "stm32f407xx_rcc.h"

uint16_t AHB_PreScalar[8] = {2,4,8,16,32,64,128,256};
uint16_t APB1_PreScalar[4] = {2,4,8,16};

/************************************************************************
 * @fn			- RCC_GetPCLK1Value
 *
 * @brief		- This function calculates and returns the PCLK1 value
 *
 * @none		- none
 *
 * @return		- pclk1
 *
 * @Note		- none
 *
 */
uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1, SystemClk;

	uint8_t clksrc, temp, ahbp, apb1p;
	clksrc = ((RCC->CFGR >> 2) & 0x3); 	// Bring those 2 clock-switch bits (SWS) to the lsb (least significant bit) position and mask
										// 0x3 => 11 in binary
	if (clksrc == 0)
	{
		SystemClk = 16000000;
	} else if (clksrc == 1) {
		SystemClk = 8000000;
	} else if (clksrc == 2)
	{
		SystemClk = RCC_GetPLLOutputClk();	// Call function to return system clock when PLL is used
	}

	// For AHB
	temp = ((RCC->CFGR >> 4) & 0xF); 	// Bring the 4 AHB prescalar bits (HPRE) to lsb position and mask
										// 0xF => 1111 in binary
	if (temp < 8)
	{
		ahbp = 1;
	} else
	{
		ahbp = AHB_PreScalar[temp-8];
	}

	// For APB1
	temp = ((RCC->CFGR >> 10) & 0x7); 	// Bring the 3 APB1 prescalar bits (PPRE1) to lsb position and mask
										// 0x7 => 111 in binary
	if (temp < 8)
	{
		apb1p = 1;
	} else
	{
		apb1p = APB1_PreScalar[temp-4];
	}

	pclk1 = (SystemClk/ahbp) / apb1p;

	return pclk1;
}


/************************************************************************
 * @fn			- RCC_GetPCLK2Value
 *
 * @brief		- This function calculates and returns the PCLK2 value
 *
 * @none		- none
 *
 * @return		- pclk2
 *
 * @Note		- none
 *
 */
uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t pclk2, temp, SystemClk=0;

	uint8_t clksrc, ahbp, apb2p;
	clksrc = ((RCC->CFGR >> 2) & 0x3); 	// Bring those 2 clock-switch bits (SWS) to the lsb (least significant bit) position and mask
										// 0x3 => 11 in binary
	if (clksrc == 0)
	{
		SystemClk = 16000000;
	} else
	{
		SystemClk = 8000000;
	}

	// For AHB
	temp = ((RCC->CFGR >> 4) & 0xF); 	// Bring the 4 AHB prescalar bits (HPRE) to lsb position and mask
										// 0xF => 1111 in binary
	if (temp < 0x08)
	{
		ahbp = 1;
	} else
	{
		ahbp = AHB_PreScalar[temp-8];
	}

	// For APB2
	temp = ((RCC->CFGR >> 13) & 0x7); 	// Bring the 3 APB2 prescalar bits (PPRE1) to lsb position and mask
										// 0x7 => 111 in binary
	if (temp < 0x04)
	{
		apb2p = 1;
	} else
	{
		apb2p = APB1_PreScalar[temp-4];
	}

	pclk2 = (SystemClk/ahbp) / apb2p;

	return pclk2;
}


/************************************************************************
 * @fn			- RCC_GetPLLOutputClk
 *
 * @brief		- This function returns the system clock frequency when PLL is used
 *
 * @none		- none
 *
 * @return		- PLL_Clk
 *
 * @Note		- This function is not yet implemented since the PLL oscillator will not be used in the
 * 				  scope of this MCU development course
 *
 */
uint32_t RCC_GetPLLOutputClk(void)
{
	return 0;
}

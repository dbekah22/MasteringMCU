/**
  ******************************************************************************
  * @file    main.c
  * @author  Dhilan Bekah
  * @version V1.0
  * @date    10-May-2019
  * @brief   Making HSE as system clock.
  ******************************************************************************
*/

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
			

int main(void)
{
	//1. Turn on HSE by changing the HSEON bit (bit 16) of the CR register to 1
	RCC_TypeDef *pRCC;
	pRCC = RCC;
	pRCC->CR |= ( 1 << 16 );

	//2. Wait until HSE becomes stable, using HSERDY bit (crystal osc. takes more time than RC osc.)
	while( ! ( pRCC->CR & ( 1 << 17 ) ) ); // if bit 17 is not ready (0), then the expression evaluates to 1 and we stay in the loop

	//3. Select HSE as system clock
	pRCC->CFGR &= ~( 0x3 << 0 ); // This will reset the first two bits corresponding to the system clock switch
	pRCC->CFGR |= ( 0x1 << 0 ); // This will set the first two bits to 01 (for HSE)

	return 0;
}

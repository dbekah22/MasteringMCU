/*
 * stm32f407xx_usart.c
 *
 *  Created on: Jul 4, 2019
 *      Author: dhilan22
 */

#include "stm32f407xx_usart.h"

/************************************************************************
 * @fn			- USART_PeriControl
 *
 * @brief		- This function enables or disables USART peripherals
 *
 * @pUSARTx		- Base address pointer to USART peripheral
 * @ENorDI		- Enable or Disable macros
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void USART_PeriControl(USART_RegDef_t *pUSARTx, uint8_t ENorDI)
{
	if (ENorDI == ENABLE)
	{
		pUSARTx->CR1 |= (1 << USART_CR1_UE);
	} else
	{
		pUSARTx->CR1 &= ~(1 << USART_CR1_UE);
	}
}


/************************************************************************
 * @fn			- USART_PeriCLKControl
 *
 * @brief		- This function enables or disables peripheral clocks for USART ports
 *
 * @pUSARTx		- Base address pointer to USART peripheral
 * @ENorDI		- Enable or Disable macros
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void USART_PeriCLKControl(USART_RegDef_t *pUSARTx, uint8_t ENorDI)
{
	if (ENorDI == ENABLE)
	{
		if (pUSARTx == USART1) {
			USART1_PCLK_EN();
		} else if (pUSARTx == USART2) {
			USART2_PCLK_EN();
		} else if (pUSARTx == USART3) {
			USART3_PCLK_EN();
		} else if (pUSARTx == UART4) {
			UART4_PCLK_EN();
		} else if (pUSARTx == UART5) {
			UART5_PCLK_EN();
		} else if (pUSARTx == USART6) {
			USART6_PCLK_EN();
		}
	}
	else
	{
		if (pUSARTx == USART1) {
			USART1_PCLK_DI();
		} else if (pUSARTx == USART2) {
			USART2_PCLK_DI();
		} else if (pUSARTx == USART3) {
			USART3_PCLK_DI();
		} else if (pUSARTx == UART4) {
			UART4_PCLK_DI();
		} else if (pUSARTx == UART5) {
			UART5_PCLK_DI();
		} else if (pUSARTx == USART6) {
			USART6_PCLK_DI();
		}
	}
}


/************************************************************************
 * @fn				- USART_Init
 *
 * @brief			- This function initializes the appropriate USART ports / pins
 *
 * @pUSARTHandle	- Pointer to handle access to different USART registers for configuration
 *
 * @return			- none
 *
 * @Note			- none
 *
 */
void USART_Init(USART_Handle_t *pUSARTHandle)
{
	//Temporary variable
	uint32_t tempreg=0;

/******************************** Configuration of CR1******************************************/

	// 1. Enable the Clock for given USART peripheral
	USART_PeriCLKControl(pUSARTHandle->pUSARTx, ENABLE);

	// 2. Enable USART Tx and Rx engines according to the USART_Mode configuration item
	if ( pUSARTHandle->USART_PinConfig.USART_Mode == USART_MODE_RX_ONLY)
	{
		// Enable the Receiver bit field
		tempreg |= (1 << USART_CR1_RE);
	} else if (pUSARTHandle->USART_PinConfig.USART_Mode == USART_MODE_TX_ONLY)
	{
		// Enable the Transmitter bit field
		tempreg |= (1 << USART_CR1_TE);

	} else if (pUSARTHandle->USART_PinConfig.USART_Mode == USART_MODE_TXRX)
	{
		// Enable both Transmitter and Receiver bit fields
		tempreg |= ( ( 1 << USART_CR1_RE) | ( 1 << USART_CR1_TE) );
	}

    // 3. Configure the Word length configuration item
	tempreg |= pUSARTHandle->USART_PinConfig.USART_WordLength << USART_CR1_M;

    // 4. Configure parity control bit fields
	if ( pUSARTHandle->USART_PinConfig.USART_ParityControl == USART_PARITY_EN_EVEN)
	{
		// Enable the parity control
		tempreg |= ( 1 << USART_CR1_PCE);

		// Enabling EVEN parity not required because by default EVEN parity will be selected once you enable the parity control

	} else if (pUSARTHandle->USART_PinConfig.USART_ParityControl == USART_PARITY_EN_ODD )
	{
		// Enable the parity control
	    tempreg |= ( 1 << USART_CR1_PCE);

	    // Enable ODD parity
	    tempreg |= ( 1 << USART_CR1_PS);

	}

   // 5. Program the CR1 register
	pUSARTHandle->pUSARTx->CR1 = tempreg;

/******************************** Configuration of CR2******************************************/

	tempreg=0;	// Reset temporary variable after CR1 has been configured

	// 6. Configure the number of stop bits inserted during USART frame transmission
	tempreg |= pUSARTHandle->USART_PinConfig.USART_NoStopBits << USART_CR2_STOP;

	// 7. Program the CR2 register
	pUSARTHandle->pUSARTx->CR2 = tempreg;

/******************************** Configuration of CR3******************************************/

	tempreg=0;	// Reset temporary variable after CR2 has been configured

	// 8. Configure USART hardware flow control
	if ( pUSARTHandle->USART_PinConfig.USART_HWFlowControl == USART_HWFC_CTS)
	{
		// Enable CTS flow control
		tempreg |= ( 1 << USART_CR3_CTSE);

	} else if (pUSARTHandle->USART_PinConfig.USART_HWFlowControl == USART_HWFC_RTS)
	{
		// Enable RTS flow control
		tempreg |= ( 1 << USART_CR3_RTSE);

	} else if (pUSARTHandle->USART_PinConfig.USART_HWFlowControl == USART_HWFC_CTS_RTS)
	{
		// Enable both CTS and RTS Flow control
		tempreg |= ( 1 << USART_CR3_CTSE);
		tempreg |= ( 1 << USART_CR3_RTSE);
	}

	// 9. Program the CR3 register
	pUSARTHandle->pUSARTx->CR3 = tempreg;

/******************************** Configuration of BRR (Baudrate register)******************************************/

	// Configure the baud rate
	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_PinConfig.USART_BaudRate);
}


/************************************************************************
 * @fn			- USART_DeInit
 *
 * @brief		- This function de-initializes the appropriate USART ports / pins
 *
 * @pUSARTx		- Base address pointer to USART peripheral
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void USART_DeInit(USART_RegDef_t *pUSARTx)
{
	if (pUSARTx == USART1) {
		USART1_REG_RESET();
	} else if (pUSARTx == USART2) {
		USART2_REG_RESET();
	} else if (pUSARTx == USART3) {
		USART3_REG_RESET();
	} else if (pUSARTx == UART4) {
		UART4_REG_RESET();
	} else if (pUSARTx == UART5) {
		UART5_REG_RESET();
	} else if (pUSARTx == USART6) {
		USART6_REG_RESET();
	}
}


/************************************************************************
 * @fn			- USART_SendData
 *
 * @brief		- This function sends data over USART communication
 *
 * @pUSARTx		- Base address pointer to USART peripheral
 * @pTxBuffer	- Indicator in status register to confirm whether data can be programmed yet or not
 * @Len			- Number of bytes to transmit (if 0, no data to send)
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint16_t *pdata;
   //Loop over until "Len" number of bytes are transferred
	for (uint32_t i = 0 ; i < Len; i++)
	{
		// Wait until TXE flag is set in the SR
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE));

         // Check the USART_WordLength item for 9BIT or 8BIT in a frame
		if(pUSARTHandle->USART_PinConfig.USART_WordLength == USART_WORDLEN_9BITS)
		{
			// if 9BIT, load the DR with 2bytes masking the bits other than first 9 bits
			pdata = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

			// check for USART_ParityControl
			if(pUSARTHandle->USART_PinConfig.USART_ParityControl == USART_PARITY_DI)
			{
				// No parity is used in this transfer. so, 9bits of user data will be sent
				// Increment pTxBuffer twice
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				// Parity bit is used in this transfer . so , 8bits of user data will be sent
				// The 9th bit will be replaced by parity bit by the hardware
				pTxBuffer++;
			}
		}
		else
		{
			//This is 8bit data transfer
			pUSARTHandle->pUSARTx->DR = (*pTxBuffer  & (uint8_t)0xFF);

			// Increment the buffer address
			pTxBuffer++;
		}
	}

	// Wait until TC flag is set in the SR
	while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TC));
}


/************************************************************************
 * @fn			- USART_ReceiveData
 *
 * @brief		- This function receives data over USART communication
 *
 * @pUSARTx		- Base address pointer to USART peripheral
 * @pRxBuffer	- Indicator in status register to confirm whether data can be programmed yet or not
 * @Len			- Number of bytes to receive (if 0, no data to receive)
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	// Loop over until "Len" number of bytes are transferred
	for (uint32_t i = 0 ; i < Len; i++)
	{
		// Wait until RXNE flag is set in the SR
		while (! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE));

		// Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
		if (pUSARTHandle->USART_PinConfig.USART_WordLength == USART_WORDLEN_9BITS)
		{
			// We are going to receive 9bit data in a frame
			// Check are we using USART_ParityControl control or not
			if (pUSARTHandle->USART_PinConfig.USART_ParityControl == USART_PARITY_DI)
			{
				// No parity is used. so, all 9bits will be of user data
				// Read only first 9 bits. so, mask the DR with 0x01FF
				*((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

				// Now increment the pRxBuffer two times
				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				// Parity is used, so, 8bits will be of user data and 1 bit is parity
				 *pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);

				 // Increment the pRxBuffer
				 pRxBuffer++;
			}
		}
		else
		{
			// We are going to receive 8bit data in a frame
			// Check whether we are using USART_ParityControl control or not
			if(pUSARTHandle->USART_PinConfig.USART_ParityControl == USART_PARITY_DI)
			{
				// No parity is used , so all 8bits will be of user data
				// Read 8 bits from DR
				 *pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
			}
			else
			{
				// Parity is used, so 7 bits will be of user data and 1 bit is parity
				// Read only 7 bits , hence mask the DR with 0X7F
				 *pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0x7F);
			}
			// Increment the pRxBuffer
			pRxBuffer++;
		}
	}
}


/************************************************************************
 * @fn				- USART_SendDataIT
 *
 * @brief			- This function saves the pointers and length information and sets the TXEIE and TCIE bits
 * 				      so that the data can be handled and sent by the ISR code
 *
 * @pUSARTHandle	- Pointer to handle access to different USART registers for configuration
 * @pTxBuffer		- Indicator in status register to confirm whether data can be programmed yet or not
 * @Len				- Number of bytes to send (if 0, no data to receive)
 *
 * @return			- state of Tx
 *
 * @Note			- none
 *
 */
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t txstate = pUSARTHandle->TxBusyState;

	if (txstate != USART_BUSY_IN_TX)
	{
		pUSARTHandle->TxLen = Len;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

		// Enable interrupt for TXE
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

		// Enable interrupt for TC
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
	}
	return txstate;
}


/************************************************************************
 * @fn				- USART_ReceiveDataIT
 *
 * @brief			- This function saves the pointers and length information and sets the RXNEIE bit
 * 				      so that the data can be handled and sent by the ISR code
 *
 * @pUSARTHandle	- Pointer to handle access to different USART registers for configuration
 * @pRxBuffer		- Indicator in status register to confirm whether data can be programmed yet or not
 * @Len				- Number of bytes to receive (if 0, no data to receive)
 *
 * @return			- state of Rx
 *
 * @Note			- none
 *
 */
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t rxstate = pUSARTHandle->RxBusyState;

	if(rxstate != USART_BUSY_IN_RX)
	{
		pUSARTHandle->RxLen = Len;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

		// Enable interrupt for RXNE
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
	}
	return rxstate;
}


/*
 * IRQ configuration
 */
void USART_IRQInterruptConfig(uint8_t IRQNum, uint8_t ENorDI)
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

void USART_IRQPriorityConfig(uint8_t IRQNum, uint32_t IRQPriority)
{
	// 1. Find out the IPR Register
	uint8_t IPRx = IRQNum / 4;
	uint8_t IPRx_section = IRQNum % 4;

	uint8_t shift_amount = (8*IPRx_section) + (8-NO_PR_BITS_IMP); // In this MCU device, lower 4 bits of each section are not implemented so you must shift the bits by 4

	*(NVIC_PR_BASE + IPRx) |= ( IRQPriority << shift_amount ); // LS jumps to appropriate register address, RS sets the correct section to the IRQ Priority
}


/*
 * ISR Handling
 */
void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{
	uint32_t temp1 , temp2, temp3;

	uint16_t *pdata;

/*************************Check for TC flag ********************************************/

    // Check the state of TC bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_TC);

	 // Check the state of TCEIE bit
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_TCIE);

	if (temp1 && temp2 )
	{
		// This interrupt is because of TC

		// Close transmission and call application callback if TxLen is zero
		if ( pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			// Check the TxLen. If it is zero then close the data transmission
			if(! pUSARTHandle->TxLen )
			{
				// Clear the TC flag
				pUSARTHandle->pUSARTx->SR &= ~( 1 << USART_SR_TC);

				// Clear the TCIE control bit

				// Reset the application state
				pUSARTHandle->TxBusyState = USART_READY;

				// Reset Buffer address to NULL
				pUSARTHandle->pTxBuffer = NULL;

				// Reset the length to zero
				pUSARTHandle->TxLen = 0;

				// Call the application call back with event USART_EVENT_TX_CMPLT
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_TX_CMPLT);
			}
		}
	}

/*************************Check for TXE flag ********************************************/

	// Check the state of TXE bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_TXE);

	// Check the state of TXEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_TXEIE);


	if (temp1 && temp2 )
	{
		// This interrupt is because of TXE

		if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			// Keep sending data until Txlen reaches to zero
			if(pUSARTHandle->TxLen > 0)
			{
				// Check the USART_WordLength item for 9BIT or 8BIT in a frame
				if(pUSARTHandle->USART_PinConfig.USART_WordLength == USART_WORDLEN_9BITS)
				{
					// If 9BIT load the DR with 2bytes masking  the bits other than first 9 bits
					pdata = (uint16_t*) pUSARTHandle->pTxBuffer;
					pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

					// Check for USART_ParityControl
					if(pUSARTHandle->USART_PinConfig.USART_ParityControl == USART_PARITY_DI)
					{
						// No parity is used in this transfer, so 9bits of user data will be sent
						// Increment pTxBuffer twice
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->TxLen-=2;
					}
					else
					{
						// Parity bit is used in this transfer, so 8bits of user data will be sent
						// The 9th bit will be replaced by parity bit by the hardware
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->TxLen-=1;
					}
				}
				else
				{
					// This is 8bit data transfer
					pUSARTHandle->pUSARTx->DR = (*pUSARTHandle->pTxBuffer  & (uint8_t)0xFF);

					// Increment the buffer address
					pUSARTHandle->pTxBuffer++;
					pUSARTHandle->TxLen-=1;
				}

			}
			if (pUSARTHandle->TxLen == 0 )
			{
				// TxLen is zero
				// Clear the TXEIE bit (disable interrupt for TXE flag)
				pUSARTHandle->pUSARTx->CR1 &= ~( 1 << USART_CR1_TXEIE);
			}
		}
	}

/*************************Check for RXNE flag ********************************************/

	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_RXNE);
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_RXNEIE);


	if (temp1 && temp2 )
	{
		// This interrupt is because of RXNE
		if(pUSARTHandle->RxBusyState == USART_BUSY_IN_RX)
		{
			// TXE is set so send data
			if(pUSARTHandle->RxLen > 0)
			{
				// Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
				if(pUSARTHandle->USART_PinConfig.USART_WordLength == USART_WORDLEN_9BITS)
				{
					// We are going to receive 9bit data in a frame

					// Now, check whether we are using USART_ParityControl or not
					if(pUSARTHandle->USART_PinConfig.USART_ParityControl == USART_PARITY_DI)
					{
						// No parity is used, so all 9bits will be of user data

						// Read only first 9 bits so mask the DR with 0x01FF
						*((uint16_t*) pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

						// Now increment the pRxBuffer two times
						pUSARTHandle->pRxBuffer++;
						pUSARTHandle->pRxBuffer++;
						pUSARTHandle->RxLen-=2;
					}
					else
					{
						// Parity is used, so 8bits will be of user data and 1 bit is parity
						 *pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
						 pUSARTHandle->pRxBuffer++;
						 pUSARTHandle->RxLen-=1;
					}
				}
				else
				{
					// We are going to receive 8bit data in a frame

					// Now, check whether we are using USART_ParityControl or not
					if(pUSARTHandle->USART_PinConfig.USART_ParityControl == USART_PARITY_DI)
					{
						// No parity is used , so all 8bits will be of user data
						// Read 8 bits from DR
						 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);

					}

					else
					{
						// Parity is used, so, 7 bits will be of user data and 1 bit is parity
						// Read only 7 bits , hence mask the DR with 0X7F
						 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0x7F);

					}

					// Now, increment the pRxBuffer
					pUSARTHandle->pRxBuffer++;
					pUSARTHandle->RxLen-=1;
				}


			} // if of >0

			if(! pUSARTHandle->RxLen)
			{
				// Disable the rxne
				pUSARTHandle->pUSARTx->CR1 &= ~( 1 << USART_CR1_RXNEIE );
				pUSARTHandle->RxBusyState = USART_READY;
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_RX_CMPLT);
			}
		}
	}


/*************************Check for CTS flag ********************************************/
//Note : CTS feature is not applicable for UART4 and UART5

	// Check the status of CTS bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_CTS);

	// Check the state of CTSE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR3 & ( 1 << USART_CR3_CTSE);

	// Check the state of CTSIE bit in CR3 (This bit is not available for UART4 & UART5.)
	temp3 = pUSARTHandle->pUSARTx->CR3 & ( 1 << USART_CR3_CTSIE);


	if(temp1  && temp2 )
	{
		// Clear the CTS flag in SR
		pUSARTHandle->pUSARTx->SR &=  ~( 1 << USART_SR_CTS);

		// This interrupt is because of CTS
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_CTS);
	}

/*************************Check for IDLE detection flag ********************************************/

	// Check the status of IDLE flag bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_IDLE);

	// Check the state of IDLEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_IDLEIE);


	if(temp1 && temp2)
	{
		// Clear the IDLE flag. Refer to the RM to understand the clear sequence
		temp1 = pUSARTHandle->pUSARTx->SR &= ~( 1 << USART_SR_IDLE);

		// This interrupt is because of IDLE
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_IDLE);
	}

/*************************Check for Overrun detection flag ********************************************/

	// Check the status of ORE flag  in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & USART_SR_ORE;

	// Check the status of RXNEIE  bit in the CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & USART_CR1_RXNEIE;


	if(temp1  && temp2 )
	{
		// Need not to clear the ORE flag here, instead give an api for the application to clear the ORE flag.

		// This interrupt is because of Overrun error
		USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_ORE);
	}



/*************************Check for Error Flag ********************************************/

//Noise Flag, Overrun error and Framing Error in multibuffer communication
//We dont discuss multibuffer communication in this course. please refer to the RM
//The blow code will get executed in only if multibuffer mode is used.

	temp2 =  pUSARTHandle->pUSARTx->CR3 & ( 1 << USART_CR3_EIE) ;

	if(temp2 )
	{
		temp1 = pUSARTHandle->pUSARTx->SR;
		if(temp1 & ( 1 << USART_SR_FE))
		{
			/*
				This bit is set by hardware when a de-synchronization, excessive noise or a break character
				is detected. It is cleared by a software sequence (an read to the USART_SR register
				followed by a read to the USART_DR register).
			*/
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_FE);
		}

		if(temp1 & ( 1 << USART_SR_NF) )
		{
			/*
				This bit is set by hardware when noise is detected on a received frame. It is cleared by a
				software sequence (an read to the USART_SR register followed by a read to the
				USART_DR register).
			*/
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_NE);
		}

		if(temp1 & ( 1 << USART_SR_ORE) )
		{
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_ORE);
		}
	}
}


/*
 * Other Peripheral Control APIs
 */

 /************************************************************************
  * @fn				- USART_GetFlagStatus
  *
  * @brief			- This function return the status of the appropriate flag
  *
  * @pUSARTx		- Base address pointer to USART peripheral
  * @FlagName		- Name of flag
  *
  * @return			- SET or RESET
  *
  * @Note			- none
  *
  */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName)
{
	if (pUSARTx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}


/************************************************************************
 * @fn			- USART_ClearFlag
 *
 * @brief		- This function clears the flag status for the given flag
 *
 * @pUSARTx		- Base address pointer to USART peripheral
 * @FlagName	- Name of flag
 *
 * @return		- none
 *
 * @Note		- Applicable to only USART_FLAG_CTS, USART_FLAG_LBD, and USART_FLAG_TC
 *
 */
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint32_t FlagName)
{
	pUSARTx->SR &= ~(FlagName);
}


/************************************************************************
 * @fn			- USART_SetBaudRate
 *
 * @brief		- This function sets the baud rate for a given USART peripheral
 *
 * @pUSARTx		- Base address pointer to USART peripheral
 * @BaudRate	- Desired baud rate
 *
 * @return		- none
 *
 * @Note		- none
 *
 */
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{
	// Variable to hold the APB clock
	uint32_t PCLKx;

	uint32_t usartdiv;

	// Variables to hold Mantissa and Fraction values
	uint32_t M_part, F_part;

	uint32_t tempreg=0;

	// Get the value of APB bus clock in to the variable PCLKx
	if (pUSARTx == USART1 || pUSARTx == USART6)
	{
	   // USART1 and USART6 are hanging on APB2 bus
	   PCLKx = RCC_GetPCLK2Value();
	} else
	{
	   PCLKx = RCC_GetPCLK1Value();
	}

	// Check for OVER8 configuration bit
	if (pUSARTx->CR1 & (1 << USART_CR1_OVER8))
	{
	   // OVER8 = 1 , over sampling by 8
	   usartdiv = ( (25 * PCLKx) / (2 *BaudRate) );
	} else
	{
	   // Oversampling by 16
		usartdiv = ( (25 * PCLKx) / (4 *BaudRate) );
	}

	// Calculate the Mantissa part
	M_part = usartdiv/100;

	// Place the Mantissa part in appropriate bit position. Refer USART_BRR
	tempreg |= (M_part << 4);

	// Extract the fraction part
	F_part = ( usartdiv - (M_part * 100) );

	// Calculate the final fractional
	if (pUSARTx->CR1 & ( 1 << USART_CR1_OVER8))
	{
	  // OVER8 = 1 , over sampling by 8
	  F_part = ( ( (F_part * 8) + 50 ) / 100 ) & ((uint8_t)0x07);
	} else
	{
	   // Oversampling by 16
	   F_part = ( ( (F_part * 16) + 50 ) / 100 ) & ((uint8_t)0x0F);
	}

	// Place the fractional part in appropriate bit position . refer USART_BRR
	tempreg |= F_part;

	// Copy the value of tempreg in to BRR register
	pUSARTx->BRR = tempreg;
}


__weak void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t event)
{

}

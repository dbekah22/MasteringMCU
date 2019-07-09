/*
 * stm32f407xx_usart.h
 *
 *  Created on: Jul 4, 2019
 *      Author: dhilan22
 */

#ifndef INC_STM32F407XX_USART_H_
#define INC_STM32F407XX_USART_H_

#include "stm32f407xx.h"

// Configuration structure for USARTx Peripheral
typedef struct
{
	uint8_t USART_Mode;				// Possible values from @USART_MODES
	uint8_t USART_BaudRate;			// Possible values from @USART_BAUD_RATES
	uint8_t USART_NoStopBits;		// Possible values from @USART_STOP_BITS
	uint8_t USART_WordLength;		// Possible values from @USART_WORD_LENGTHS
	uint8_t USART_ParityControl;	// Possible values from @USART_PARITY_CONTROL
	uint8_t USART_HWFlowControl;	// Possible values from @USART_HW_FLOW_CONTROL
}USART_Config_t;

// Handle structure for USARTx Peripheral
typedef struct
{
	USART_RegDef_t	*pUSARTx;			// Pointer to hold base address of USARTx peripheral
	USART_Config_t	USART_PinConfig;	// For configuring USARTx peripheral parameters
	uint8_t 		*pTxBuffer;			// To store app. Tx buffer address
	uint8_t 		*pRxBuffer;			// To store app. Rx buffer address
	uint32_t		TxLen;				// Data transmission length
	uint32_t		RxLen;				// Data reception length
	uint8_t 		TxBusyState;		// TX Communication state
	uint8_t 		RxBusyState;		// RX Communication state
}USART_Handle_t;


// @USART_MODES
#define USART_MODE_TX_ONLY		0
#define USART_MODE_RX_ONLY		1
#define USART_MODE_TXRX			2

// @USART_BAUD_RATES - [bps]
#define USART_BAUD_1200			1200
#define USART_BAUD_2400			2400
#define USART_BAUD_9600			9600
#define USART_BAUD_19200		19200
#define USART_BAUD_38400		38400
#define USART_BAUD_57600		57600
#define USART_BAUD_115200		115200
#define USART_BAUD_230400		230400
#define USART_BAUD_460800		460800
#define USART_BAUD_921600		921600
#define USART_BAUD_2M			2000000
#define USART_BAUD_3M			3000000

// @USART_STOP_BITS
#define USART_STOPBITS_1		0
#define USART_STOPBITS_0_5		1
#define USART_STOPBITS_2		2
#define USART_STOPBITS_1_5		3


// @USART_WORD_LENGTHS
#define USART_WORDLEN_8BITS		0
#define USART_WORDLEN_9BITS		1

// @USART_PARITY_CONTROL
#define USART_PARITY_DI			0
#define USART_PARITY_EN_EVEN	1
#define USART_PARITY_EN_ODD		2

// @USART_HW_FLOW_CONTROL
#define USART_HWFC_NONE			0
#define USART_HWFC_CTS			1
#define USART_HWFC_RTS			2
#define USART_HWFC_CTS_RTS		3

// USART Flags
#define USART_FLAG_TXE		(1 << USART_SR_TXE)
#define USART_FLAG_RXNE		(1 << USART_SR_RXNE)
#define USART_FLAG_TC		(1 << USART_SR_TC)

// USART Application States
#define USART_READY			0
#define USART_BUSY_IN_RX	1
#define USART_BUSY_IN_TX	2

// USART Application Events
#define USART_EVENT_TX_CMPLT	0
#define	USART_EVENT_RX_CMPLT	1
#define	USART_EVENT_IDLE		2
#define	USART_EVENT_CTS			3
#define	USART_EVENT_PE			4
#define	USART_ERR_FE			5
#define	USART_ERR_NE			6
#define	USART_ERR_ORE			7


/*******************************************/
/***** 		Driver-supported APIs 	   *****/
/*******************************************/

// Peripheral Setup
void USART_PeriControl(USART_RegDef_t *pUSARTx, uint8_t ENorDI);
void USART_PeriCLKControl(USART_RegDef_t *pUSARTx, uint8_t ENorDI);

// Initialize and De-initialize
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);

// Data Send & Receive
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

// Data Send & Receive w/ Interrupts
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

// IRQ Configuration and ISR Handling
void USART_IRQInterruptConfig(uint8_t IRQNum, uint8_t ENorDI);
void USART_IRQPriorityConfig(uint8_t IRQNum, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pUSARTHandle);

// Other Peripheral Control APIs
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint32_t FlagName);
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);

// Application callback
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv);

#endif /* INC_STM32F407XX_USART_H_ */
/*
 * stm32f407xx_i2c.h
 *
 *  Created on: Jun 21, 2019
 *      Author: dhilan22
 */

#ifndef INC_STM32F407XX_I2C_H_
#define INC_STM32F407XX_I2C_H_

#include "stm32f407xx.h"

// Configuration structure for I2Cx Peripheral
typedef struct
{
	uint8_t I2C_SCLSpeed;			// Possible values from @I2C_SCL_SPEEDS
	uint8_t I2C_DeviceAddress;		// Possible values from @I2C_DEVICE_ADDRESSES
	uint8_t I2C_ACKControl;			// Possible values from @I2C_ACK_CONTROL
	uint8_t I2C_FMDutyCycle;		// Possible values from @I2C_FM_CYCLES
}I2C_Config_t;

// Handle structure for I2Cx Peripheral
typedef struct
{
	I2C_RegDef_t	*pI2Cx;			// Pointer to hold base address of I2Cx peripheral
	I2C_Config_t	I2C_PinConfig;	// For configuring I2Cx peripheral parameters
	uint8_t 		*pTxBuffer;		// To store app. Tx buffer address
	uint8_t 		*pRxBuffer;		// To store app. Rx buffer address
	uint32_t		TxLen;			// Data transmission length
	uint32_t		RxLen;			// Data reception length
	uint8_t 		TxRxState;		// Communication state
	uint8_t 		DevAddr;		// Slave device address
	uint32_t		RxSize;			// Rx size
	uint8_t 		Sr;				// Repeated start condition value
}I2C_Handle_t;


// @I2C_SCL_SPEEDS
#define I2C_SCL_SPEED_SM	100000
#define I2C_SCL_SPEED_FM2K	200000
#define I2C_SCL_SPEED_FM4K	400000

// @I2C_DEVICE_ADDRESSES - no initialization options for this since it is dependent on the device itself and will be given by user

// @I2C_ACK_CONTROL
#define I2C_ACK_ENABLE		1
#define I2C_ACK_DISABLE		0

// @I2C_FM_CYCLES
#define I2C_FM_2			0
#define I2C_FM_16_9			1

// I2C Related Flag status definitions
#define I2C_FLAG_TXE		(1 << I2C_SR1_TXE)
#define I2C_FLAG_RXNE		(1 << I2C_SR1_RXNE)
#define I2C_FLAG_SB			(1 << I2C_SR1_SB)
#define I2C_FLAG_ADDR		(1 << I2C_SR1_ADDR)
#define I2C_FLAG_BTF		(1 << I2C_SR1_BTF)
#define I2C_FLAG_ADD10		(1 << I2C_SR1_ADD10)
#define I2C_FLAG_STOPF		(1 << I2C_SR1_STOPF)
#define I2C_FLAG_BERR		(1 << I2C_SR1_BERR)
#define I2C_FLAG_ADDR		(1 << I2C_SR1_ADDR)
#define I2C_FLAG_ARLO		(1 << I2C_SR1_ARLO)
#define I2C_FLAG_AF			(1 << I2C_SR1_AF)
#define I2C_FLAG_OVR		(1 << I2C_SR1_OVR)
#define I2C_FLAG_TIMEOUT	(1 << I2C_SR1_TIMEOUT)

// I2C Repeated Start Conditions
#define I2C_DI_SR			RESET
#define I2C_EN_SR			SET

// I2C Application States
#define I2C_READY			0
#define I2C_BUSY_IN_RX		1
#define I2C_BUSY_IN_TX		2

// I2C Application Events
#define I2C_EVENT_TX_CMPLT		0
#define I2C_EVENT_RX_CMPLT		1
#define I2C_EVENT_STOPF			2
#define I2C_EVENT_DATA_REQ		3
#define I2C_EVENT_DATA_RCV		4
#define I2C_ERROR_BERR			5
#define I2C_ERROR_ARLO			6
#define I2C_ERROR_AF			7
#define I2C_ERROR_OVR			8
#define I2C_ERROR_TIMEOUT		9


/*******************************************/
/***** 		Driver-supported APIs 	   *****/
/*******************************************/

// Peripheral Setup
void I2C_PeriControl(I2C_RegDef_t *pI2Cx, uint8_t ENorDI);
void I2C_PeriCLKControl(I2C_RegDef_t *pI2Cx, uint8_t ENorDI);

// Initialize and De-initialize
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

// Master - Data Send & Receive
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTXBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRXBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

// Master - Data Send & Receive w/ Interrupts
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTXBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRXBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);

// Slave - Data Send & Receive
void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx);

// IRQ Configuration and ISR Handling
void I2C_IRQInterruptConfig(uint8_t IRQNum, uint8_t ENorDI);
void I2C_IRQPriorityConfig(uint8_t IRQNum, uint32_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);

// Other Peripheral Control APIs
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t ENorDI);
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);
void I2C_SlaveENorDICallbackEvents(I2C_RegDef_t *pI2Cx, uint8_t ENorDI);

// Application callback
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);


#endif /* INC_STM32F407XX_I2C_H_ */
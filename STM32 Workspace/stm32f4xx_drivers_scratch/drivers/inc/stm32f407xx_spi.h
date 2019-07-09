/*
 * stm32f407xx_spi.h
 *
 *  Created on: May 31, 2019
 *      Author: dhilan22
 */

#ifndef INC_STM32F407XX_SPI_H_
#define INC_STM32F407XX_SPI_H_

#include "stm32f407xx.h"

// Configuration structure for SPIx Peripheral
typedef struct
{
	uint8_t SPI_DeviceMode;			// Possible values from @SPI_DEVICE_MODES
	uint8_t SPI_BusConfig;			// Possible values from @SPI_BUS_CONFIGURATIONS
	uint8_t SPI_SclkSpeed;			// Possible values from @SPI_SCLK_SPEEDS
	uint8_t SPI_DFF;				// Possible values from @SPI_DFF
	uint8_t SPI_CPOL;				// Possible values from @SPI_CPOL
	uint8_t SPI_CPHA;				// Possible values from @SPI_CPHA
	uint8_t SPI_SSM;				// Possible values from @SPI_SSM

}SPI_Config_t;

// This is the handle structure for a SPI pin
typedef struct
{
	SPI_RegDef_t *pSPIx;			// Pointer to hold base address of SPIx peripheral
	SPI_Config_t SPI_PinConfig;
	uint8_t *pTXBuffer;
	uint8_t *pRXBuffer;
	uint32_t TXLen;
	uint32_t RXLen;
	uint8_t TXState;
	uint8_t RXState;

}SPI_Handle_t;

// Possible SPI Application States
#define SPI_READY				0
#define SPI_BUSY_IN_RX			1
#define SPI_BUSY_IN_TX			2

// Possible SPI Application Events
#define SPI_EVENT_TX_CMPLT		1
#define SPI_EVENT_RX_CMPLT		2
#define SPI_EVENT_OVR_ERR		3
#define SPI_EVENT_CRR_ERR		4

// @SPI_DEVICE_MODES - SPI Possible Device Modes
#define SPI_MODE_MASTER			1
#define SPI_MODE_SLAVE			0

// @SPI_BUS_CONFIGURATIONS - SPI Possible BUS Configurations
#define SPI_BUS_CONFIG_FD		1		// Full-duplex configuration
#define SPI_BUS_CONFIG_HD		2		// Half-duplex configuration
#define SPI_BUS_CONFIG_S_RX		3		// Simplex configuration with RX only

// @SPI_SCLK_SPEEDS - SPI Possible System Clock Speeds
#define SPI_SPEED_DIV2			0		// DIV corresponds to prescalar applied to the f_pclk (peripheral clock speed)
#define SPI_SPEED_DIV4			1
#define SPI_SPEED_DIV8			2
#define SPI_SPEED_DIV16			3
#define SPI_SPEED_DIV32			4
#define SPI_SPEED_DIV64			5
#define SPI_SPEED_DIV128		6
#define SPI_SPEED_DIV256		7

// @SPI_DFF - SPI Possible Data Frame Formats
#define SPI_DFF_8BITS			0
#define SPI_DFF_16BITS			1

// @SPI_CPOL - SPI Possible SCLK Polarities
#define SPI_CPOL_HIGH			1
#define SPI_CPOL_LOW			0

// @SPI_CPHA - SPI Possible SCLK Phases
#define SPI_CPHA_HIGH			1
#define SPI_CPHA_LOW			0

// @SPI_SSM - SPI Possible NSS Management
#define SPI_SSM_EN			1		// Software NSS Management
#define SPI_SSM_DI			0		// Hardware NSS Management

// SPI Related Flag status definitions
#define SPI_TXE_FLAG		(1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG		(1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG		(1 << SPI_SR_BSY)


/*******************************************/
/***** 		Driver-supported APIs 	   *****/
/*******************************************/

// Peripheral Clock Setup
void SPI_PeriCLKControl(SPI_RegDef_t *pSPIx, uint8_t ENorDI);

// Initialize and De-initialize
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

//Data Read & Write
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTXBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRXBuffer, uint32_t Len);

// Data R&W With interrupt
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTXBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRXBuffer, uint32_t Len);

// IRQ Configuration and ISR Handling
void SPI_IRQInterruptConfig(uint8_t IRQNum, uint8_t ENorDI);
void SPI_IRQPriorityConfig(uint8_t IRQNum, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

// Other Peripheral Control APIs
void SPI_PeriControl(SPI_RegDef_t *pSPIx, uint8_t ENorDI);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t ENorDI);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t ENorDI);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

// Application callback
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv);

#endif /* INC_STM32F407XX_SPI_H_ */

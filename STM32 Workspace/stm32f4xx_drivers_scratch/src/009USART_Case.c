/*
 * 009USART_Case.c
 *
 *  Created on: Jul 9, 2019
 *      Author: dhilan22
 */

#include<stdio.h>
#include<string.h>
#include "stm32f407xx.h"


// Here are 3 different messages as examples that we transmit to Arduino

char *msg[3] = {"hihihihihihi123", "Hello How are you ?" , "Today is Monday !"};

// Reply from Arduino will be stored here
char rx_buf[1024];

USART_Handle_t usart2_handle;


// This flag indicates reception completion
uint8_t rxCmplt = RESET;

uint8_t g_data = 0;

extern void initialise_monitor_handles();

void USART2_Init(void)
{
	usart2_handle.pUSARTx = USART2;
	usart2_handle.USART_PinConfig.USART_BaudRate = USART_BAUD_115200;
	usart2_handle.USART_PinConfig.USART_HWFlowControl = USART_HWFC_NONE;
	usart2_handle.USART_PinConfig.USART_Mode = USART_MODE_TXRX;
	usart2_handle.USART_PinConfig.USART_NoStopBits = USART_STOPBITS_1;
	usart2_handle.USART_PinConfig.USART_WordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_PinConfig.USART_ParityControl = USART_PARITY_DI;
	USART_Init(&usart2_handle);
}

void USART_GPIOInit(void)
{
	GPIO_Handle_t usart_gpios;

	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFUN;
	usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	usart_gpios.GPIO_PinConfig.GPIO_AltFunMode = 7;

	usart_gpios.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_2;
	GPIO_Init(&usart_gpios);

	usart_gpios.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_3;
	GPIO_Init(&usart_gpios);

}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn,GpioLed;

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

	//this is led gpio configuration
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNum = GPIO_PIN_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriCLKControl(GPIOD,ENABLE);

	GPIO_Init(&GpioLed);

}

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

int main(void)
{
	uint32_t cnt = 0;

	initialise_monitor_handles();

	USART_GPIOInit();
    USART2_Init();

    USART_IRQInterruptConfig(IRQ_NO_USART2,ENABLE);

    USART_PeriControl(USART2,ENABLE);

    printf("Application is running\n");

    while(1)
    {
		// Wait till button is pressed
		while( ! GPIO_ReadPin(GPIOA,GPIO_PIN_0) );
		delay();

		// Next message index ; make sure that cnt value doesn't cross 2
		cnt = cnt % 3;

		// First lets enable the reception in interrupt mode
		// This code enables the receive interrupt
		while ( USART_ReceiveDataIT(&usart2_handle,rx_buf,strlen(msg[cnt])) != USART_READY );

		//Send the msg indexed by cnt in blocking mode
    	USART_SendData(&usart2_handle,(uint8_t*)msg[cnt],strlen(msg[cnt]));

    	printf("Transmitted : %s\n",msg[cnt]);

    	// Now lets wait until all the bytes are received from the Arduino
    	// When all the bytes are received rxCmplt will be SET in application callback
    	while(rxCmplt != SET);

    	// Make sure that the last byte should be null otherwise %s fails while printing
    	rx_buf[strlen(msg[cnt])+ 1] = '\0';

    	//Print what we received from the arduino
    	printf("Received    : %s\n",rx_buf);

    	//invalidate the flag
    	rxCmplt = RESET;

    	//move on to next message indexed in msg[]
    	cnt ++;
    }

	return 0;
}


void USART2_IRQHandler(void)
{
	USART_IRQHandling(&usart2_handle);
}


void USART_ApplicationEventCallback( USART_Handle_t *pUSARTHandle,uint8_t ApEv)
{
   if(ApEv == USART_EVENT_RX_CMPLT)
   {
			rxCmplt = SET;

   }else if (ApEv == USART_EVENT_TX_CMPLT)
   {
	   ;
   }
}


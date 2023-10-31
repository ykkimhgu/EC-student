#ifndef __EC_USART_SIMPLE_H
#define __EC_USART_SIMPLE_H

#include "stm32f411xe.h"
#include <stdio.h>
#include "ecGPIO.h"
#include "ecRCC.h"

#define POL 0
#define INT 1

// You can modify this
#define BAUD_9600		0
#define BAUD_19200	1
#define BAUD_57600	2
#define BAUD_115200 3
#define BAUD_921600 4


// ********************** USART 2 (USB) ***************************
	// PA_2 = USART2_TX
	// PA_3 = USART2_RX
	// Alternate function(AF7), High Speed, Push pull, Pull up
	// APB1
// **********************************************************



/* UART2 Initialization to use printf() */
void UART2_init();
void USART_write(USART_TypeDef* USARTx, uint8_t* buffer, uint32_t nBytes);
void USART_delay(uint32_t us);  


										

#endif
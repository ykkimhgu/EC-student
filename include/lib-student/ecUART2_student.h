/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 10-08-2025
Modified         : 10-08-2026 
Language/ver     : C in VS Code

Description      : USART2  library  [your description here] 
/----------------------------------------------------------------*/

#ifndef __EC_USART2_STUDENT_H
#define __EC_USART2_STUDENT_H

#include <stdio.h>
#include <math.h>
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"


#define POL 0
#define INT 1

// You can modify this
#define BAUD_9600	9600
#define BAUD_19200	19200
#define BAUD_38400  38400
#define BAUD_57600	57600
#define BAUD_115200 115200
#define BAUD_921600 921600


// ********************** USART 2 (USB) ***************************
	// PA_2 = USART2_TX
	// PA_3 = USART2_RX
	// Alternate function(AF7), High Speed, Push pull, Pull up
	// APB1
// **********************************************************

// ********************** USART 1 ***************************
	// PA_9 = USART1_TX (default)	// PB_6  (option)
	// PA_10 = USART1_RX (default)	// PB_3 (option)
	// APB2
// **********************************************************

// ********************** USART 6 ***************************
	// PA_11 = USART6_TX (default)	// PC_6  (option)
	// PA_12 = USART6_RX (default)	// PC_7 (option)
	// APB2
// **********************************************************


/*================ EXERCISE ================*/
/*================ General Configuration ================*/
// General configuration
void USART_init(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud);	// copy of USART_setting
void USART_baud(USART_TypeDef* USARTx, uint32_t baud);
void USART_pins(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX);

void USART1_baud(uint32_t baud);
void USART2_baud(uint32_t baud);


/*================ PROVIDED  ================*/
/*================ Configuration for each USART ================*/
// Configuration UART 1, 2 using default settings
void USART1_init(void);
void USART2_init(void);

// USART write & read
void USART1_write(uint8_t* buffer, uint32_t nBytes);
void USART2_write(uint8_t* buffer, uint32_t nBytes);
uint8_t USART1_read(void);										
uint8_t USART2_read(void);	

// RX Inturrupt Flag USART1,2
uint32_t is_USART1_RXNE(void);
uint32_t is_USART2_RXNE(void);


/*================ General Configuration ================*/
void USART_clock_enable(USART_TypeDef* USARTx);
void USART_write(USART_TypeDef* USARTx, uint8_t* buffer, uint32_t nBytes);
uint8_t USART_read(USART_TypeDef * USARTx);										
uint32_t is_USART_RXNE(USART_TypeDef * USARTx);
void USART_delay(uint32_t us);  

								

#endif // __EC_USART2_STUDENT_H




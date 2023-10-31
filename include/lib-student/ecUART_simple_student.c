#include "ecUART_simple.h"
#include <math.h>

// ********************** DO NOT MODIFY HERE ***************************
// 
// Implement a dummy __FILE struct, which is called with the FILE structure.
//#ifndef __stdio_h
struct __FILE {
    //int dummy;
		int handle;

};

FILE __stdout;
FILE __stdin;
//#endif

// Retarget printf() to USART2
int fputc(int ch, FILE *f) { 
  uint8_t c;
  c = ch & 0x00FF;
  USART_write(USART2, (uint8_t *)&c, 1);
  return(ch);
}



void UART2_init(){
	// Enable the clock of USART2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  // Enable USART 2 clock (APB1 clock: AHB clock / 2 = 42MHz)
	
	// Enable the peripheral clock of GPIO Port
	RCC->AHB1ENR |=   RCC_AHB1ENR_GPIOAEN;	
	
	// ********************** USART 2 ***************************
	// PA2 = USART2_TX
	// PA3 = USART2_RX
	// Alternate function(AF7), High Speed, Push pull, Pull up
	// **********************************************************
	int TX_pin = 2;
	
	GPIOA->MODER   &= ~(0xF << (2*TX_pin));	// Clear bits
	GPIOA->MODER   |=   0xA << (2*TX_pin);  // Alternate Function(10)    		
	GPIOA->AFR[0]  |=   0x77<< (4*TX_pin);  // AF7 - USART2     	
	GPIOA->OSPEEDR |=   0xF<<(2*TX_pin); 		// High speed (11)			 		
	GPIOA->PUPDR   &= ~(0xF<<(2*TX_pin));
	GPIOA->PUPDR   |=   0x5<<(2*TX_pin);    // Pull-up (01)			
	GPIOA->OTYPER  &=  ~(0x3<<TX_pin) ; 		// push-pull (0, reset)
	
	
	USART_TypeDef *USARTx = USART2;
	// No hardware flow control, 8 data bits, no parity, 1 start bit and 1 stop bit      
	USARTx->CR1 &= ~USART_CR1_UE;  			// Disable USART

	// Configure word length to 8 bit
	USARTx->CR1 &= ~USART_CR1_M;       	// M: 0 = 8 data bits, 1 start bit   
	USARTx->CR1 &= ~USART_CR1_PCE;   		// No parrity bit 
	USARTx->CR2 &= ~USART_CR2_STOP;  		// 1 stop bit    

	// Configure oversampling mode (to reduce RF noise)
	USARTx->CR1 &= ~USART_CR1_OVER8;  	// 0 = oversampling by 16   
															 
	// CSet Baudrate to 9600 using APB frequency (42MHz)
	// If oversampling by 16, Tx/Rx baud = f_CK / (16*USARTDIV),  
	// If oversampling by 8,  Tx/Rx baud = f_CK / (8*USARTDIV)
	// USARTDIV = 42MHz/(16*9600) = 237.4375

	//USARTx->BRR  = 42000000/ baud_rate;
	float Hz = 42000000;

	float USARTDIV = (float)Hz/16/9600;
	uint32_t MNT = (uint32_t)USARTDIV;
	uint32_t FRC = round((USARTDIV - MNT) * 16);
	if (FRC > 15) {
		MNT += 1;
		FRC = 0;
	}
	USARTx->BRR |= (MNT << 4) | FRC;

	USARTx->CR1 |= (USART_CR1_RE | USART_CR1_TE);   	// Transmitter and Receiver enable
	USARTx->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
	USARTx->CR1 |= USART_CR1_UE; 										// USART enable

	USARTx->CR1 |= USART_CR1_RXNEIE;                 	// Enable Read Interrupt
	NVIC_SetPriority(USART2_IRQn, 1);		// Set Priority to 1
	NVIC_EnableIRQ(USART2_IRQn);			 	// Enable interrupt of USART2 peripheral

}

void USART_write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	int i;
	for (i = 0; i < nBytes; i++) {
		// wait until TXE (TX empty) bit is set
		while (!(USARTx->SR & USART_SR_TXE));  
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->DR = buffer[i] & 0xFF;
		USART_delay(300);
	}
	// wait until TC bit is set
	while (!(USARTx->SR & USART_SR_TC));		
	// TC bit clear
	USARTx->SR &= ~USART_SR_TC;	
	
}  

void USART_delay(uint32_t us) {
   uint32_t time = 100*us/7;    
   while(--time);   
}


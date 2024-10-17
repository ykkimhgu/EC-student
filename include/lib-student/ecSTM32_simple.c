/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : YKKim
Created          : 05-03-2021
Modified         : 09-04-2024
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for Simple Tutorials 
/----------------------------------------------------------------*/



#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "ecSTM32_simple.h"


void GPIO_init(PinName_t pinName, uint32_t mode) {
	GPIO_TypeDef* port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);

	// mode  : Input(0), Output(1), AlterFunc(2), Analog(3)   
	if (port == GPIOA)
		RCC_GPIOA_enable();
	if (port == GPIOB)
		RCC_GPIOB_enable();
	if (port == GPIOC)
		RCC_GPIOC_enable();
	
	GPIO_mode(pinName, mode);
	
}


// GPIO Mode          : Input(00), Output(01), AlterFunc(10), Analog(11, reset)
void GPIO_mode(PinName_t pinName, uint32_t mode) {
	GPIO_TypeDef* port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);

	port->MODER &= ~(3UL << (2 * pin));
	port->MODER |= mode << (2 * pin);
}

// GPIO Read
int GPIO_read(PinName_t pinName) {
	GPIO_TypeDef* port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);

	return ((port->IDR) & (1UL << pin)) >> pin; // 0 or 1 
}

// GPIO Write
void GPIO_write(PinName_t pinName, int Output) {
	GPIO_TypeDef* port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);

	port->ODR &= ~(1 << pin);
	port->ODR |= Output << pin;
}


// RCC HSI
void RCC_HSI_init() {
	// Enable High Speed Internal Clock (HSI = 16 MHz)
  	RCC->CR |= 0x00000001U;

	// wait until HSI is ready
	while ((RCC->CR & 0x00000002U) == 0) { ; }

	// Select HSI as system clock source 
	RCC->CFGR &= (uint32_t)(~RCC_CFGR_SW); 								// not essential
	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI; 							//00: HSI16 oscillator used as system clock

	// Wait till HSI is used as system clock source
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != 0);
		
}


void RCC_GPIOA_enable()
{
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}

void RCC_GPIOB_enable()
{
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
}

void RCC_GPIOC_enable()
{
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
}

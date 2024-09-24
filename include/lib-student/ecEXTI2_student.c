#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecEXTI2.h"


void EXTI_init(PinName_t pinName, int trig_type,int priority){

	GPIO_Typedef *port;
	unsigned int pin;
	ecPinmap(pinName,&port,&pin);
	// SYSCFG peripheral clock enable	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;		
	
	// Connect External Line to the GPIO
	int EXTICR_port;
	if			(Port == GPIOA) EXTICR_port = 0;
	else if	(Port == GPIOB) EXTICR_port = 1;
	else if	(Port == GPIOC) EXTICR_port = 2;
	else if	(Port == GPIOD) EXTICR_port = 3;
	else 										EXTICR_port = 4;
	
	SYSCFG->EXTICR[_______] &= ________________;			// clear 4 bits
	SYSCFG->EXTICR[_______] |= ________________;			// set 4 bits
	
	// Configure Trigger edge
	if (trig_type == FALL) EXTI->FTSR |= _______;   // Falling trigger enable 
	else if	(trig_type == RISE) EXTI->RTSR |= _______;   // Rising trigger enable 
	else if	(trig_type == BOTH) {			// Both falling/rising trigger enable
		EXTI->RTSR |= _______; 
		EXTI->FTSR |= _______;
	} 
	
	// Configure Interrupt Mask (Interrupt enabled)
	EXTI->IMR  |= _______;     // not masked
	
	
	// NVIC(IRQ) Setting
	int EXTI_IRQn = 0;
	
	if (Pin < 5) 	EXTI_IRQn = _______;
	else if	(Pin < 10) 	EXTI_IRQn = _______;
	else 			EXTI_IRQn = _______;
								
	NVIC_SetPriority(EXTI_IRQn, _______);	// EXTI priority
	NVIC_EnableIRQ(EXTI_IRQn); 	// EXTI IRQ enable
}


void EXTI_enable(PinName_t pinName) {
	//Todo Port, Pin Configuration 
	EXTI->IMR __= _______;     // not masked (i.e., Interrupt enabled)
}
void EXTI_disable(PinName_t pinName) {
	//Todo Port, Pin Configuration 
	EXTI->IMR __= _______;     // masked (i.e., Interrupt disabled)
}

uint32_t is_pending_EXTI(PinName_t pinName) {
	//Todo Port, Pin Configuration 
	uint32_t EXTI_PRx = _______;     	// check  EXTI pending 	
	return ((EXTI->PR __ _______) == _______);
}


void clear_pending_EXTI(PinName_t pinName) {
	//Todo Port, Pin Configuration 
	EXTI->PR __= _______;     // clear EXTI pending 
}

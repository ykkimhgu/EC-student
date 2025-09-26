/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2025-9-25 by YKKIM
* @brief   Embedded Controller:  Tutorial ___
*					 - _________________________________
*
******************************************************************************
*/



//#include "ecSTM32F4v2.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN   PB_12
#define BUTTON_PIN PA_4

void LED_toggle(PinName_t pinName);

// Initialiization 
void setup(void)
{
	RCC_PLL_init();                         // System Clock = 84MHz
	// Initialize GPIOB_12 for Output
	GPIO_init(LED_PIN, OUTPUT);    // LED for EVAL board	
	// Initialize GPIOA_4 for Input Button
	GPIO_init(BUTTON_PIN, INPUT);  // OUTPUT for EVAL borad
	EXTI_init_tutorial(PA_4);
		
}

// MAIN  ----------------------------------------
int main(void) {
	
	setup();

	while (1);
}


// EXTI Initialiization ------------------------------------------------------	
void EXTI_init_tutorial(PinName_t pinName){
	GPIO_Typedef *port;
	unsigned int pin;
	ecPinmap(pinName,&port,&pin);
	

	// SYSCFG peripheral clock enable
	RCC->APB2ENR |= __________________

	// Connect External Line to the GPIO
	// Button: PA_4 -> EXTICR2(EXTI4)
	SYSCFG->EXTICR[____] &= ~SYSCFG_EXTICR2_EXTI4;
	SYSCFG->EXTICR[____] |= ______________________;

	// Falling trigger enable (Button: pull-up)
	EXTI->FTSR |= 1UL << __________;

	// Unmask (Enable) EXT interrupt
	EXTI->IMR |= 1UL << ___________;

	// Interrupt IRQn, Priority
	NVIC_SetPriority(EXTI4_IRQn, 0);  		// Set EXTI priority as 0	
	NVIC_EnableIRQ(EXTI4_IRQn); 			// Enable EXTI 
	
}

void EXTI4_IRQHandler(void) {
	if ((EXTI->PR & EXTI_PR_PR4) == _________) {
		LED_toggle(LED_PIN);
		EXTI->PR |= EXTI_PR_PR4; // cleared by writing '1'
	}
}


void LED_toggle(PinName_t pinName){
	GPIO_Typedef *port;
	unsigned int pin;
	ecPinmap(pinName,&port,&pin);
	// YOUR CODE GOES HERE
}

/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM
* @brief   Embedded Controller:  Tutorial ___
*					 - _________________________________
*
******************************************************************************
*/



//#include "ecSTM32F4v2.h"

#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN   PA_5
#define BUTTON_PIN PC_13

void setup(void);

int main(void) {

	// System CLOCK, GPIO Initialiization ----------------------------------------
	setup();



	while (1);
}

void EXTI_init_tutorial(PinName_t pinName){

	// EXTI Initialiization ------------------------------------------------------	

	// SYSCFG peripheral clock enable
	RCC->APB2ENR |= __________________

	// Connect External Line to the GPIO
	// Button: PC_13 -> EXTICR3(EXTI13)
	SYSCFG->EXTICR[____] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[____] |= ______________________;

	// Falling trigger enable (Button: pull-up)
	EXTI->FTSR |= 1UL << __________;

	// Unmask (Enable) EXT interrupt
	EXTI->IMR |= 1UL << ___________;

	// Interrupt IRQn, Priority
	NVIC_SetPriority(EXTI15_10_IRQn, 0);  		// Set EXTI priority as 0	
	NVIC_EnableIRQ(EXTI15_10_IRQn); 			// Enable EXTI 
	
}

void EXTI15_10_IRQHandler(void) {
	if ((EXTI->PR & EXTI_PR_PR13) == _________) {
		LED_toggle();
		EXTI->PR |= EXTI_PR_PR13; // cleared by writing '1'
	}
}


// Initialiization 
void setup(void)
{
	RCC_PLL_init();                         // System Clock = 84MHz
	// Initialize GPIOA_5 for Output
	GPIO_init(LED_PIN, OUTPUT);    // calls RCC_GPIOA_enable()	
	// Initialize GPIOC_13 for Input Button
	GPIO_init(BUTTON_PIN, INPUT);  // calls RCC_GPIOC_enable()
	EXTI_init_tutorial(PC_13);
		
}


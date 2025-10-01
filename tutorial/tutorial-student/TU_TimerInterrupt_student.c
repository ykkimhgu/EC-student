/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2024-8-23 by YKKIM  	
* @brief   Embedded Controller:  Tutorial _____
*					 - ____________________
* 
******************************************************************************
*/
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"


PinName_t LED_pin = PA_5;
uint32_t count = 0;

void TIM2_init_tutorial();

void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){}
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                       // System Clock = 84MHz
	GPIO_init(LED_pin, OUTPUT);    // calls RCC_GPIOA_enable()	
	TIM2_init_tutorial();
}

// YOUR CODE GOES HERE
void TIM2_init_tutorial(){
	TIM_TypeDef* timerx;
	timerx = TIM2;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	timerx->PSC =______ 			// Timer counter clock: 1MHz(1us)
	timerx->ARR =______ 			// Set auto reload register to maximum (count up to 65535)
	timerx->DIER |=______           // Enable Interrupt
	timerx->CR1 |=______            // Enable counter
	
	NVIC_SetPriority(___ );       	// TIM2_IRQHandler Set priority as 2
	NVIC_EnableIRQ(___);			// TIM2_IRQHandler Enable
}

// YOUR CODE GOES HERE
void TIM2_IRQHandler(void){
	if((TIM2->SR & TIM_SR_UIF) ==__________      ){ // update interrupt flag
		//Create the code to toggle LED by 1000ms

		TIM2->SR &=____________________             // clear by writing 0
	}
}

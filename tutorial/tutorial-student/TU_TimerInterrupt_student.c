/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2022-8-12 by YKKIM  	
* @brief   Embedded Controller:  Tutorial _____
*					 - ____________________
* 
******************************************************************************
*/
#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"


#define LED_PIN 5
uint32_t count = 0;


void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	TIM_TypeDef* timerx;
	timerx = TIM2;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	timerx->PSC = 				// Timer counter clock: 1MHz(1us)
	timerx->ARR = 				// Set auto reload register to maximum (count up to 65535)
	timerx->DIER |=                     	// Enable Interrupt
	timerx->CR1 |=                      	// Enable counter
	
	NVIC_SetPriority(, );               	// TIM2_IRQHandler Set priority as 2
	NVIC_EnableIRQ();			// TIM2_IRQHandler Enable
	
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){}
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                       // System Clock = 84MHz
	GPIO_init(GPIOA, LED_PIN, OUTPUT);    // calls RCC_GPIOA_enable()	
}

void TIM2_IRQHandler(void){
	if((TIM2->SR & TIM_SR_UIF) ==      ){ // update interrupt flag
		//Create the code to toggle LED by 1000ms

		TIM2->SR &=                     // clear by writing 0
	}
}

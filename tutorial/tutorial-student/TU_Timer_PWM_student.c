/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2024-8-23 by Jin Kwak  	
* @brief   Embedded Controller:  Tutorial ___
*					 - _________________________________
* 
******************************************************************************
*/
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"
#include "ecSysTick2.h"

#define LED_PIN    PA_5

void setup(void);
void PWM_init_tutorial();


int main(void) { 
	// Initialiization --------------------------------------------------------	
	setup();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
			//Create the code to change the brightness of LED as 10kHZ (use "delay(1000)")
		}
	}



// Initialiization 
void setup(void)
{	
	RCC_PLL_init();       // System Clock = 84MHz
	SysTick_init();       // for delay_ms()
	// YOUR CODE GOES HERE
	GPIO_init(LED_PIN, ______);     // GPIOA 5 ALTERNATE function
	// YOUR CODE GOES HERE
	GPIO_ospeed(LED_PIN, ______);   // GPIOA 5 HIGH SPEED
	PWM_init_tutorial();
}

// YOUR CODE GOES HERE
void PWM_init_tutorial(){	
	// TEMP: TIMER Register Initialiization --------------------------------------------------------		
	TIM_TypeDef *TIMx;
	TIMx = TIM2;
	
	// GPIO: ALTERNATIVE function setting
	GPIOA->AFR[0]	 =           				// AF1 at PA5 = TIM2_CH1 (p.150)
	
	// TIMER: PWM setting
	RCC->APB1ENR |=               				// Enable TIMER clock
	
	TIMx->CR1 &= 				              	// Direction Up-count
	
	TIMx->PSC = 						         // Set Timer CLK = 100kHz : (PSC + 1) = 84MHz/100kHz --> PSC = ?
	
	TIMx->ARR = 								 // Auto-reload: Upcounting (0...ARR). 
												// Set Counter CLK = 1kHz : (ARR + 1) = 100kHz/1kHz --> ARR = ?
	
	TIMx->CCMR1 &= ~TIM_CCMR1_OC1M;  			// Clear ouput compare mode bits for channel 1
	TIMx->CCMR1 |=                   			// OC1M = 110 for PWM Mode 1 output on ch1
	TIMx->CCMR1	|= TIM_CCMR1_OC1PE;    			// Output 1 preload enable (make CCR1 value changable)
	
	TIMx->CCR1 =       							// Output Compare Register for channel 1 	
	
	TIMx->CCER &= ~TIM_CCER_CC1P;    			// select output polarity: active high	
	TIMx->CCER |= 												// Enable output for ch1
	
	TIMx->CR1  |= TIM_CR1_CEN;      			// Enable counter
}

/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  Tutorial ___
*					 - _________________________________
* 
******************************************************************************
*/


#include "stm32f411xe.h"
#include "math.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecUART_simple_student.h"
#include "ecSysTick.h"

uint32_t ovf_cnt = 0;
uint32_t ccr1 = 0;
uint32_t ccr2 = 0;
float    period = 0;

void setup(void);


int main(void){
	
	setup();
	while(1){
		printf("period = %f[msec]\r\n", period);		// print out the period on TeraTerm
		delay_ms(100);
	}
}


void setup(void) {	
	RCC_PLL_init();
	UART2_init();
	SysTick_init();

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;			//TIM2 Clock enabled

	// GPIO configuration ---------------------------------------------------------------------
	GPIO_init(GPIOA, 0, AF);					// PA_0: Alternate Function Mode

	// TIM Input Caputre Pin configuration/ Configure GPIO AFR by Pin num.	-------------------------------------------------
	GPIOA->AFR[] |=                              // AF for PA_0 (TIM2)  see PWM_init()


	// Timer2 with Counter clock of 100kHz  
	// TIM_init(TIM2);
	TIM2->PSC = 						  		// Timer counter clock: 1MHz(1us)  for PLL
	TIM2->ARR = 								// Set auto reload register to maximum (ARR=0XFFFF)

	// Timer Capture configuration -----------------------------------------------------------------------
	TIM2->CCMR1 |=  					        // Capture/Compare Selection: CC1 is mapped on TI1 
	TIM2->CCMR1 &=                              // Clear IC1F
	TIM2->CCMR1 |=                              // Set filter N=4
	TIM2->CCER &=               				// Clear CCER
	TIM2->CCER &= 						        // Capture rising edge
	TIM2->CCER |= 								// Capture enabled

	// Timer Capture Interrupt configuration -----------------------------------------------------------------------
	TIM2->DIER |= 								// CC-Interrupt enabled
	TIM2->DIER |= 								// Update Event interrupt enable	

	TIM2->CR1 |= 								// Counter enable

	NVIC_SetPriority(, 2);						// Set the priority of TIM2 interrupt request
	NVIC_EnableIRQ();							// TIM2 interrupt request enable
}





void TIM2_IRQHandler(void){
	if(TIM2->SR & TIM_SR_UIF){                  // If Update-event interrupt Occurs
		// Handle overflow
		// [YOUR_CODE Goes Here]
		
		TIM2->SR &=________   					// clear update-event interrupt flag
	}
	if((TIM2->SR & TIM_SR_CC1IF)){				// if CC interrupt occurs	
		// Calculate the period of 1Hz pulse
		ccr2 = TIM2->CCR1;						// capture counter value
		period = (____________) / 1000; 		// calculate the period with ovf_cnt, ccr1, and ccr2
		
		ccr1 = ccr2;
		ovf_cnt = 0;
		
		TIM2->SR &= 							// clear capture/compare interrupt flag ( it is also cleared by reading TIM2_CCR1)
	}
}

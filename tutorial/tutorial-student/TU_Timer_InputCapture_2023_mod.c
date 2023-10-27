/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2023-10-27 by YKKIM  	
* @brief   Embedded Controller:  Tutorial ___
*					 - Input Capture
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
	// Configuration Clock PLL
	RCC_PLL_init();
	
	// UART2 Configuration to use printf()
	UART2_init();
	
	// SysTick Configuration to use delay_ms()
	SysTick_init();

	// Input Capture Configuration PA_0(TIM2, 1)
	ICAP_init(PA_0);
	
	// Priority Configuration
	NVIC_SetPriority(TIM2_IRQn, 2);						// Set the priority of TIM2 interrupt request
	NVIC_EnableIRQ(TIM2_IRQn);							// TIM2 interrupt request enable
}




// Timer2 IRQ Handler (timer & Input Capture)
void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                  // If Update-event interrupt Occurs
		// Handle overflow
		ovf_cnt++;
		
		clear_UIF(TIM2);   					// clear update-event interrupt flag
	}
	if(is_CCIF(TIM2, IC_1)){				// if CC interrupt occurs	
		// Calculate the period of 1Hz pulse
		ccr2 = ICAP_capture(TIM2, IC_1);						// capture counter value
		period = ((ccr2 - ccr1) + ovf_cnt * (TIM2->ARR + 1)) / 1000; 		// calculate the period with ovf_cnt, ccr1, and ccr2
		
		ccr1 = ccr2;
		ovf_cnt = 0;
		
		clear_CCIF(TIM2, IC_1);	// clear capture/compare interrupt flag ( it is also cleared by reading TIM2_CCR1)
	}
}

/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  LAB - _____
*					 - _________________________________
* 
******************************************************************************
*/

#include "stm32f411xe.h"
#include "math.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecPWM.h"
#include "ecUART.h"
#include "ecSysTIck.h"

uint32_t ovf_cnt = 0;
float distance = 0;
float timeInterval = 0;
float time1 = 0;
float time2 = 0;

void setup(void);

int main(void){
	
	setup();
	
	while(1){
		distance = (float) timeInterval * 340.0 / 2.0 / 10.0; 	// [mm] -> [cm]
		printf("%f [cm]\r\n", distance);
		delay_ms(500);
	}
}

void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                     	// Update interrupt
		____________													// overflow count
		clear_UIF(TIM2);  							    	// clear update interrupt flag
	}
	if(is_CCIF(TIM2, 3)){ 									// TIM2_Ch3 (IC3) Capture Flag. Rising Edge Detect
		time1 = ___________;									// Capture TimeStart
		clear_CCIF(TIM2, 3);                 	// clear capture/compare interrupt flag 
	}								                      
	else if(____________)){ 								// TIM2_Ch3 (IC4) Capture Flag. Falling Edge Detect
		time2 = __________;										// Capture TimeEnd
		timeInterval = ___________; 					// Total time of echo pulse (10us * counter pulses -> [msec] unit)
		ovf_cnt = 0;                        	// overflow reset
		clear_CCIF(TIM2, 4);								  // clear capture/compare interrupt flag 
	}
}

void setup(){

	RCC_PLL_init(); 
	SysTick_init();
	UART2_init();
  
// PWM configuration ---------------------------------------------------------------------	
	PWM_t trig;												// PWM1 for trig
	________________;			 						// PA_6: Ultrasonic trig pulse
	PWM_period_us(&trig, 50000);    	// PWM of 50ms period. Use period_us()
	PWM_pulsewidth_us(&trig, 10);   	// PWM pulse width of 10us
	
// Input Capture configuration -----------------------------------------------------------------------	
	IC_t echo;												// Input Capture for echo
	_________________________;    		// PB10 as input caputre
 	ICAP_counter_us(&echo, 10);   		// ICAP counter step time as 10us
	ICAP_setup(&echo, 3, IC_RISE);   	// TIM2_CH3 as IC3 , rising edge detect
	_____________________________;   	// TIM2_CH3 as IC4 , falling edge detect
}

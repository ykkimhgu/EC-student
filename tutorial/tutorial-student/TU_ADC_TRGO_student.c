

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
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecSysTick.h"
#include "ecUART.h"

float result_v = 0;
void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	
// GPIO configuration ---------------------------------------------------------------------	
// 1. Initialize GPIO port and pin as ANALOG, no pull up / pull down
	GPIO_init(GPIOA, 1, ANALOG);				// ANALOG = 3
	GPIO_pupd(GPIOA, 1, EC_NONE);			// EC_NONE = 0
	
	
	/* ADC_initiation --------------------------------------------------------------------*/
  // 1) Enable the clock of ADC
  // 2) ...
	// 3) ...
  
	
	//Single conversion mode
	ADC1->CR2 &= ~ADC_CR2_CONT;   // Discontinuous conversion mode
	ADC1->CR2 |= ADC_CR2_EOCS;  	// Enable EOCS
	
// HW Trigger configuration -------------------------------------------------------------
// 1. TIMx Trigger Output Config
	// Enable TIMx Clock
	TIM_TypeDef *TIMx = TIM3;			// Default TRGO : TIM3
	int msec = 1;									// Default msec : 1 msec
	TIM_init(TIMx, msec);					// TIM3 init (use user defined HAL)
	TIMx->CR1 &= ~1; 							// counter disable
	
	// Set PSC, ARR
  TIM_period_ms(TIMx, msec);
	
  // Master Mode Selection MMS[2:0]: Trigger output (TRGO)
  TIMx->CR2 &= 								// reset MMS
  TIMx->CR2 |=    						//100: Compare - OC1REF signal is used as trigger output (TRGO)
   
	// Output Compare Mode
  TIMx->CCMR1 &=       				// OC1M : output compare 1 Mode 
  TIMx->CCMR1 |=          		// OC1M = 110 for compare 1 Mode ch1 
	
  // OC1 signal 
  TIMx->CCER |=            		// CC1E Capture enabled
	TIMx->CCR1  = 							// CCR set
   
  // Enable TIMx 
  TIMx->CR1 |= TIM_CR1_CEN; 						//counter enable
	
// 2. HW Trigger
	// Select Trigger Source
	ADC1->CR2 &=  							// reset EXTSEL
	ADC1->CR2 |=  							// TIMx TRGO event (ADC : TIM2, TIM3 TRGO) default : TIM3
	
	//Select Trigger Polarity 
	ADC1->CR2 &= 								// reset EXTEN, default
	ADC1->CR2 |= 								// trigger detection rising edge
	
	// Enable ADC 
	ADC1->CR2 |= ADC_CR2_ADON;
	
  // ADC Start (SW, once)	  
  ADC1->CR2 |= ADC_CR2_SWSTART;

	/* Inifinite Loop --------------------------------------------------------------------*/
	while(1);
}

// Initialiization 
void setup(void){	
	RCC_PLL_init();                 // System Clock = 84MHz
	UART2_init();
}

void ADC_IRQHandler(void){		
	if((ADC1->SR & ADC_SR_OVR) == ADC_SR_OVR){
		ADC1->SR &= ~ADC_SR_OVR;
	}
	
	if(is_ADC_EOC()){       			
	  result_v = ADC1->DR;
		printf("voltage = %.3f\r\n",result_v*3.3/4095);
 }
}
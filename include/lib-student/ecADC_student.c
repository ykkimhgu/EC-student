#include "stm32f411xe.h"
#include "ecSysTick.h"
#include "ecADC.h"
#include "ecGPIO.h"
#include "ecTIM.h"
#include <stdint.h>

/* -------------------------------------------------------------------------------------*/
//									ADC Configuration																										//
/* -------------------------------------------------------------------------------------*/

void ADC_init(PinName_t pinName){  // trigmode 0 : SW, 1 : TRGO

// 0. Match Port and Pin for ADC channel	
	GPIO_TypeDef *port;
	unsigned int pin;	
	ecPinmap(pinName, &port, &pin);
	int chN;
	ADC_pinmap(pinName, &chN);			// ADC Channel <->Port/Pin mapping

// GPIO configuration ---------------------------------------------------------------------	
// 1. Initialize GPIO port and pin as ANALOG, no pull up / pull down
	GPIO_init(port, pin, ANALOG);  				// ANALOG = 3
	GPIO_pupd(port, pin, EC_NONE);  			// EC_NONE = 0

// ADC configuration	---------------------------------------------------------------------			
// 1. Total time of conversion setting
	// Enable ADC pheripheral clock
	RCC->APB2ENR  |= RCC_APB2ENR_ADC1EN; 		// Enable the clock of RCC_APB2ENR_ADC1EN
	
	// Configure ADC clock pre-scaler
	ADC->CCR &= ~ADC_CCR_ADCPRE;				// 0000: PCLK2 divided by 2	(42MHz)
	
	// Configure ADC resolution 
	ADC1->CR1 &= ~ADC_CR1_RES;     				// 00: 12-bit resolution (15cycle+)
	
	// Configure channel sampling time of conversion.	
	// Software is allowed to write these bits only when ADSTART=0 and JADSTART=0	!!
	// ADC clock cycles @42MHz = 2us
	if(chN < 10) {
		ADC1->SMPR2 &= ~(7UL << (3* chN));			// clear bits
		ADC1->SMPR2 |= 4U << (3* chN);				// sampling time conversion : 84  			
	}
	else{
		ADC1->SMPR1 &= ~(7UL << (3* (chN - 10)));
		ADC1->SMPR1 |= 4U << (3* (chN - 10));
	}
	
// 2. Regular / Injection Group 
	//Regular: SQRx, Injection: JSQx

// 3. Repetition: Single scan or Continuous scan conversion
	ADC1->CR2 |= ___________;      			// default : Continuous conversion mode	
	
// 4. Single(one) Channel or Scan(multi-channel) mode  
	// Configure the sequence length			// default: one-channel length
	ADC1->SQR1 &= ___________; 					// 0000: one channel length in the regular channel conversion sequence
	
	// Configure the multiple channel sampling sequence 
	ADC1->SQR3 &= ~ADC_SQR3_SQ1;				// SQ1 clear 
	ADC1->SQR3 |= ___________; 		// Choose the first channelID to sample
	
	// Default:  Single(one-channel) Channel mode 
	ADC1->CR1 &= ___________;					// 0: One-channel mode 		
	
// 5. Interrupt Enable
	// Enable EOC(conversion) interrupt. 
	ADC1->CR1 &= ~ADC_CR1_EOCIE;          // Interrupt reset
	ADC1->CR1 |= ___________;           // Interrupt enable
	
	// Enable ADC_IRQn 
	NVIC_SetPriority(ADC_IRQn, 2); 				// Set Priority to 2
	NVIC_EnableIRQ(ADC_IRQn);      				// Enable interrupt form ACD1 peripheral	


	// Hardware Trigger Configuration : TIM3, 1msec, RISE edge
	ADC_trigger(TIM3, 1, RISE);

	// Start ADC 
	ADC_start();
}


void ADC_trigger(TIM_TypeDef* TIMx, int msec, int edge){
	// set timer
	int timer = 0;
	if(TIMx == TIM2) timer = 2;
	else if(TIMx == TIM3) timer = 3;	
	
	// Single conversion mode (disable continuous conversion)
	ADC1->CR2 &= ~ADC_CR2_CONT;     // Discontinuous conversion mode
	ADC1->CR2 |= ADC_CR2_EOCS;  		// Enable EOCS
	
	// Enable TIMx Clock as TRGO mode	
// 1. TIMx Trigger Output Config
	// Enable TIMx Clock
	TIM_init(TIMx, msec);
	TIMx->CR1 &= ~TIM_CR1_CEN; 								//counter disable
	
	// Set PSC, ARR
  TIM_period_ms(TIMx, msec);
	
  // Master Mode Selection MMS[2:0]: Trigger output (TRGO)
  TIMx->CR2 &= ~TIM_CR2_MMS; 				// reset MMS
  TIMx->CR2 |= TIM_CR2_MMS_2 | TIM_CR2_MMS_1;   		//100: Compare - OC1REF signal is used as trigger output (TRGO)
   
	// Output Compare Mode
  TIMx->CCMR2 &= ~TIM_CCMR2_OC3M;       							// OC1M : output compare 1 Mode 
  TIMx->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2; // OC1M = 110 for compare 1 Mode ch1 

  // OC1 signal 
  TIMx->CCER |= TIM_CCER_CC3E;  // CC1E Capture enabled
	TIMx->CCR3  = (TIMx->ARR)/2;  // duty ratio 50%
   
  // Enable TIMx 
  TIMx->CR1 |= TIM_CR1_CEN; 							//counter enable
	
// 2. ADC HW Trigger Config.
	// Select Trigger Source  			
	ADC1->CR2 &= ~ADC_CR2_EXTSEL; 			// reset EXTSEL
	ADC1->CR2 |= (timer*2 + 2) << 24; 	// TIMx TRGO event (ADC : TIM2, TIM3 TRGO)
	
	//Select Trigger Polarity
	ADC1->CR2 &= ~ADC_CR2_EXTEN;		// reset EXTEN, default
	if(edge == RISE) ADC1->CR2 |= ADC_CR2_EXTEN_0;				// trigger detection rising edge
	else if(edge == FALL) ADC1->CR2 |= ADC_CR2_EXTEN_1;		// trigger detection falling edge
	else if(edge == BOTH) ADC1->CR2 |= ADC_CR2_EXTEN_Msk;	// trigger detection both edge
}

void ADC_conversion(int convMode){
	if(convMode == CONT){
		// Repetition: Continuous conversion
		ADC1->CR2 |= ADC_CR2_CONT;      		// Enable Continuous conversion mode	
	}
	else if(convMode == SINGLE){
		// Repetition: Single conversion
		ADC1->CR2 &= ~ADC_CR2_CONT;      		// Disable Continuous conversion mode	
	}
} 

void ADC_sequence(PinName_t *seqCHn, int seqCHnums){

	// Disable ADC 
	ADC1->CR2 &= ~ADC_CR2_ADON;
	
	// Initialize ADC channels
	int chN[seqCHnums];

	// Change to Multi-Channel mode(scan mode) 
	if (seqCHnums > 1)
		ADC1->CR1 |= ADC_CR1_SCAN;					// 1: (multi-channel)Scan mode  
	
	// ADC channels mapping
	for(int k = 0; k < seqCHnums; k++)
		ADC_pinmap(seqCHn[k], &(chN[k]));

	ADC1->SQR1 &= ~ ADC_SQR1_L; 					// reset length of conversions in the regular channel 	
	ADC1->SQR1 |= (seqCHnums - 1) << ADC_SQR1_L_Pos; // conversions in the regular channel conversion sequence
	
	for(int i = 0; i < seqCHnums; i++){
		if (i < 6){
			ADC1->SQR3 &= ~(0x1F << i*5);				// SQn clear bits
			ADC1->SQR3 |= chN[i] << i*5;				// Choose the channel to convert sequence
		}
		else if (i <12){
			ADC1->SQR2 &= ~(0x1F << (i-6)*5);		// SQn clear bits
			ADC1->SQR2 |= chN[i] << (i-6)*5;		// Choose the channel to convert sequence
		}
		else{
			ADC1->SQR1 &= ~(0x1F << (i-12)*5);	// SQn clear bits
			ADC1->SQR1 |= chN[i] << (i-12)*5;		// Choose the channel to convert sequence
		}
	}

	// Start ADC 
	ADC_start();	
}


void ADC_start(void){
	// Enable ADON, SW Trigger-------------------------------------------------------------------------------
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

// ADC value read
uint32_t ADC_read(void){
	return ADC1->DR;
}

// ADC interrupt flag
uint32_t is_ADC_EOC(void){
	return (ADC1->SR & ADC_SR_EOC) == ADC_SR_EOC;
}

// ADC overflow flag
uint32_t is_ADC_OVR(void){
	return (ADC1->SR & ADC_SR_OVR) == ADC_SR_OVR;
}

// ADC clear flag
void clear_ADC_OVR(void){
	ADC1->SR &= ~ADC_SR_OVR;
}


///////////////////////////////////////////////////
void ADC_pinmap(PinName_t pinName, uint32_t *chN){
	GPIO_TypeDef *port;
	unsigned int pin;		
	ecPinmap(pinName, &port, &pin);

	if(port == GPIOA){
		switch(pin){
			case 0 : *chN = 0; break;
        	case 1 : *chN = 1; break;
        	case 4 : *chN = 4; break;
        	case 5 : *chN = 5; break;
        	case 6 : *chN = 6; break;
        	case 7 : *chN = 7; break;
        	default: break;
		}
	}
	else if(port == GPIOB){
		switch(pin){
			case 0 : *chN = 8; break;
        	case 1 : *chN = 9; break;
        	default: break;
		}
	}
	else if(port == GPIOC){
		switch(pin){
			case 0 : *chN = 10; break;
        	case 1 : *chN = 11; break;
			case 2 : *chN = 12; break;
        	case 3 : *chN = 13; break;
			case 4 : *chN = 14; break;
        	case 5 : *chN = 15; break;
        	default: break;
		}
	}
}


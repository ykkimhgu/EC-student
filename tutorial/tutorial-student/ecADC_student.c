#include "stm32f411xe.h"
#include "ecSysTick.h"
#include "ecADC_student.h"
#include "ecGPIO.h"
#include "ecTIM.h"
#include <stdint.h>
uint32_t result;

//******************************************************************************************
// ZHU: STM32L4x6xx Errata sheet
// When the delay between two consecutive ADC conversions is higher than 1 ms the result of 
// the second conversion might be incorrect. The same issue occurs when the delay between the 
// calibration and the first conversion is higher than 1 ms.
// Workaround
// When the delay between two ADC conversions is higher than the above limit, perform two ADC 
// consecutive conversions in single, scan or continuous mode: the first is a dummy conversion 
// of any ADC channel. This conversion should not be taken into account by the application.


void ADC_init(GPIO_TypeDef *port, int pin, int mode){  //mode 0 : SW, 1 : TRGO
// 0. Match Port and Pin for ADC channel	
	int CHn = ADC_pinmap(port, pin);			// ADC Channel <->Port/Pin mapping

// GPIO configuration ---------------------------------------------------------------------	
// 1. Initialize GPIO port and pin as ANALOG, no pull up / pull down
	GPIO_init(port, pin, ANALOG);  				// ANALOG = 3
	GPIO_pupdr(port, pin, EC_NONE);  			// EC_NONE = 0

// ADC configuration	---------------------------------------------------------------------			
// 1. Total time of conversion setting
	// Enable ADC pheripheral clock
	RCC->APB2ENR  |= ___________; 		// Enable the clock of RCC_APB2ENR_ADC1EN
	
	// Configure ADC clock pre-scaler
	ADC->CCR &= ___________;					// 0000: PCLK2 divided by 2	(42MHz)
	
	// Configure ADC resolution 
	ADC1->CR1 &= ___________;     		// 00: 12-bit resolution (15cycle+)
	
	// Configure channel sampling time of conversion.	
	// Software is allowed to write these bits only when ADSTART=0 and JADSTART=0	!!
	// ADC clock cycles @42MHz = 2us
	if(CHn < 10) ADC1->SMPR2  |= 4U << ___________;					// sampling time conversion : 84  			
	else				 ADC1->SMPR1  |= 4U << ___________;
	
// 2. Regular / Injection Group 
	//Regular: SQRx, Injection: JSQx

// 3. Repetition: Single or Continuous conversion
	ADC1->CR2 |= ___________;      			// Enable Continuous conversion mode	
	
// 4. Single Channel or Scan mode
	//  - Single Channel: scan mode, right alignment
	ADC1->CR1 |= ___________;						// 1: Scan mode enable 
	ADC1->CR2 &= ___________;   				// 0: Right alignment	
	
	// Configure the sequence length
	ADC1->SQR1 &= ___________; 					// 0000: 1 conversion in the regular channel conversion sequence
	
	// Configure the channel sequence 
	ADC1->SQR3 &= ___________;				 	// SQ1 clear bits
	ADC1->SQR3 |= (CHn & ___________); 	// Choose the channel to convert firstly
		
	
// 5. Interrupt Enable
	// Enable EOC(conversion) interrupt. 
	ADC1->CR1 &= ___________;          	// Interrupt reset
	ADC1->CR1 |= ___________;           // Interrupt enable
	
	// Enable ADC_IRQn 
	NVIC_SetPriority(___________); 			// Set Priority to 2
	NVIC_EnableIRQ(___________);      	// Enable interrupt form ACD1 peripheral	

}



void ADC_start(ADC_TypeDef *ADCx){
	// Enable ADON, SW Trigger-------------------------------------------------------------------------------
	ADCx->CR2 |= ___________;
	ADCx->CR2 |= ___________;
}


uint32_t ADC_read(){
	return ADC1->___________;
}

uint32_t ADC_pinmap(GPIO_TypeDef *Port, int Pin){
	if(Port == GPIOA){
		if(Pin == 0) 			return 0;
		else if(Pin == 1) return 1;
		else if(Pin == 4) return 4;
		else if(Pin == 5) return 5;
		else if(Pin == 6) return 6;
		else if(Pin == 7) return 7;
		else 							while(1);
	}
	else if(Port == GPIOB){
		if(Pin == 0) 			return 8;
		else if(Pin == 1)	return 9;
		else 							while(1);
	}
	else if(Port == GPIOC){
		if(Pin == 0)			return 10;
		else if(Pin == 1)	return 11;
		else if(Pin == 2)	return 12;
		else if(Pin == 3)	return 13;
		else if(Pin == 4)	return 14;
		else if(Pin == 5)	return 15;
		else							while(1);
	}
}


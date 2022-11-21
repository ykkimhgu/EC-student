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

float result_v = 0.0;

void setup(void);
	
int main(void) { 
	setup();
	
// GPIO configuration ---------------------------------------------------------------------	
// 1. Initialize GPIO port and pin as ANALOG, no pull up / pull down
	GPIO_init(GPIOA, 1, ANALOG);				// ANALOG = 3
	GPIO_pupdr(GPIOA, 1, EC_NONE);			// EC_NONE = 0
	
// ADC configuration	---------------------------------------------------------------------			
// 1. Total time of conversion setting
	// Enable ADC pheripheral clock	
	RCC->APB2ENR  |=  					// Enable the clock of RCC_APB2ENR_ADC1EN
	
	// Configure ADC clock pre-scaler
	ADC->CCR &= 								// 0000: PCLK2 divided by 2	(42MHz)
	
	// Configure ADC resolution 
	ADC1->CR1 &=     						// 00: 12-bit resolution (15cycle+)
	
	// Configure channel sampling time of conversion.	
	// Software is allowed to write these bits only when ADSTART=0 and JADSTART=0	!!
	// ADC clock cycles @42MHz = 2us
	ADC1->SMPR2 &= 						// Sampling Time:  clear bits
	ADC1->SMPR2 |=         		// Sampling Time:  84cycles/42Mhz=2us
	   
// 2. Regular / Injection Group 
	//Regular: SQRx, Injection: JSQx
	
// 3. Repetition: Single or Continuous conversion
	ADC1->CR2 |=       					// Enable Continuous conversion mode	
 
// 4. Single Channel or Scan mode
	// Configure the sequence length
	ADC1->SQR1 &=  							// 0000: 1 conversion in the regular channel conversion sequence
	
	// Configure the channel sequence 
	ADC1->SQR3 &= 							// Choose CH of 1st Conversion: clear bits
	ADC1->SQR3 |=    						// Choose CH of 1st Conversion: Select CH (0~15)	
	
	//  - Single Channel: scan mode, right alignment	
	ADC1->CR1 |= 								// 1: Scan mode enable 
	ADC1->CR2 &= ~ADC_CR2_ALIGN // 0: Right alignment	
	
// 5. Interrupt Enable
	// Enable EOC(conversion) interrupt. 
	ADC1->CR1 &=          			// Interrupt reset
	ADC1->CR1 |=          			// Interrupt enable
	
	// Enable ADC_IRQn 
	NVIC_SetPriority(    ,  );	// Set Priority to 2
	NVIC_EnableIRQ(    );      	// Enable interrupt form ACD1 peripheral	

// 6. Enable ADC
	ADC1->CR2 |= ADC_CR2_ADON;

// ADC SW configuration	---------------------------------------------------------------------	    
  ADC1->CR2 |= ADC_CR2_SWSTART;
	
  /* Infinite Loop    -----------------------------------------------------------------------*/
	while(1){
		printf("voltage = %.3f\r\n",result_v*3.3/4095);
		delay_ms(100);
	}
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                 // System Clock = 84MHz
	UART2_init();
	SysTick_init();
}

void ADC_IRQHandler(void){
	if(ADC1->SR & __________){
		result_v = ADC1->_____;
 }
}


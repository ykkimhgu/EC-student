/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2024-8-23 by YKKIM  	
* @brief   Embedded Controller:  Tutorial ___
*					 - _________________________________
* 
******************************************************************************
*/
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"
#include "ecTIM2.h"
#include "ecSysTick2.h"
#include "ecUART2.h"
#include "ecADC2.h"

//IR parameter//
float result_v = 0.0;
PinName_t IR_pin = PA_1;

void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();

	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		printf("voltage = %.3f\r\n",result_v*3.3/4095);
		delay_ms(100);
	}
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	UART2_init();
	SysTick_init();
	
	ADC_init(IR_pin ,SW);
	ADC_continue(CONT);
	ADC_start();
}


void ADC_IRQHandler(void){
	if(is_ADC_EOC()){       //after finishing sequence
	  result_v = ADC_read();
 }
}

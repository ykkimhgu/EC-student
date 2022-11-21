

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
#include "ecADC.h"

float result_v =0;
void setup(void);
	
int main(void) { 
	setup();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1);
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                 // System Clock = 84MHz
	UART2_init();

	ADC_init(GPIOA, 1, TRGO);
	ADC_start();
}


void ADC_IRQHandler(void){
	if(is_ADC_OVR()){
		clear_ADC_OVR();
	}
	
	if(is_ADC_EOC()){       //after finishing sequence
	  result_v = ADC_read();
		printf("voltage = %.3f\r\n",result_v*3.3/4095);
	
 }
}
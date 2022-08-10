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
#include "ecUART_student.h"
#include "ecADC_student.h"

//IR parameter//
float result_v =0;

void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();

	
	// Inifinite Loop ----------------------------------------------------------
	while(1);
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	UART2_init();
	ADC_init(GPIOA,1,SW);
	ADC_start(ADC1);
}


void ADC_IRQHandler(void){
	if(is_ADC_EOC(ADC1)){       //after finishing sequence
	  result_v =ADC_read();
		printf("voltage = %.3f\r\n",result_v*3.3/4095);
	
 }
}

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
#include "ecSysTick.h"
#include "ecUART.h"
#include "ecADC.h"

float result_v = 0.0;
void setup(void);
	
int main(void) { 

	setup();
	
  /* Infinite Loop    -------------------------------------------------------------------------------*/
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
	
	ADC_init(GPIOA, 1, SW);
	ADC_continue(CONT);
	ADC_start();
}

void ADC_IRQHandler(void){
	if(is_ADC_EOC()){
		result_v = ADC_read();
 }
}
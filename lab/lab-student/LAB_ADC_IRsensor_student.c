/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2024-8-23 by YKKIM  	
* @brief   Embedded Controller:  LAB ___
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
uint32_t IR1, IR2;
int flag = 0;
int seqCHn[16] = {8,9,};

void setup(void);
	
int main(void) { 

	// Initialiization --------------------------------------------------------
	setup();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		printf("IR1 = %d \r\n",IR1);
		printf("IR2 = %d \r\n",IR2);
		printf("\r\n");
		
		if (IR1 > 1000) printf("GO RIGHT\r\n");
		else if (IR2 > 1000) printf("GO LEFT\r\n");
		printf("\r\n");
		delay_ms(1000);
	}
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	UART2_init();
	SysTick_init();
	
	// ADC setting
  	ADC_init(PB_0, TRGO);
	ADC_init(PB_1, TRGO);

	// ADC channel sequence setting
	ADC_sequence(2, seqCHn);
	
	// ADON, SW Trigger enable
	ADC_start();
}


void ADC_IRQHandler(void){
	if((is_ADC_OVR())){
		clear_ADC_OVR();
	}
	
	if(is_ADC_EOC()){       //after finishing sequence
			if (flag==0){
				IR1 = ADC_read();
			}  
			else if (flag==1){
				IR2 = ADC_read();
			}
		flag =! flag;
	}
}

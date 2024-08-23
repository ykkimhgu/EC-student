/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2024-08-23 by YKKIM  	
* @brief   Embedded Controller:  Tutorial Digital In/Out 7-segment Display
* 
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"

void setup(void);
	
int main(void) {	
	// Initialiization --------------------------------------------------------
	setup();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		GPIO_write(PA_5, LOW);
		GPIO_write(PA_6, LOW);
		GPIO_write(PA_7, HIGH);
		GPIO_write(PB_6, HIGH);
		GPIO_write(PC_7, HIGH);
		GPIO_write(PA_9, LOW);
		GPIO_write(PA_8, LOW);
		GPIO_write(PB_10, LOW);
	}
}

void setup(void){
	RCC_HSI_init();
	GPIO_init(PA_5, OUTPUT);
	GPIO_init(PA_6, OUTPUT);
	GPIO_init(PA_7, OUTPUT);
	GPIO_init(PB_6, OUTPUT);
	GPIO_init(PC_7, OUTPUT);
	GPIO_init(PA_9, OUTPUT);
	GPIO_init(PA_8, OUTPUT);
	GPIO_init(PB_10, OUTPUT);
}

/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2022-08-12 by YKKIM  	
* @brief   Embedded Controller:  Tutorial Digital In/Out 7-segment Display
* 
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecRCC.h"
#include "ecGPIO.h"

void setup(void);
	
int main(void) {	
	// Initialiization --------------------------------------------------------
	setup();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		GPIO_write(GPIOA, 5, LOW);
		GPIO_write(GPIOA, 6, LOW);
		GPIO_write(GPIOA, 7, HIGH);
		GPIO_write(GPIOB, 6, HIGH);
		GPIO_write(GPIOC, 7, HIGH);
		GPIO_write(GPIOA, 9, LOW);
		GPIO_write(GPIOA, 8, LOW);
		GPIO_write(GPIOB, 10, LOW);
	}
}

void setup(void){
	RCC_HSI_init();
	GPIO_init(GPIOA, 5, OUTPUT);
	GPIO_init(GPIOA, 6, OUTPUT);
	GPIO_init(GPIOA, 7, OUTPUT);
	GPIO_init(GPIOB, 6, OUTPUT);
	GPIO_init(GPIOC, 7, OUTPUT);
	GPIO_init(GPIOA, 9, OUTPUT);
	GPIO_init(GPIOA, 8, OUTPUT);
	GPIO_init(GPIOB, 10, OUTPUT);
}

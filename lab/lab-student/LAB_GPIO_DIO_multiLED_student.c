/**
******************************************************************************
* @author	your name
* @Mod		date
* @brief	Embedded Controller:  LAB Digital In/Out
*					 - Toggle multiple LEDs by Button B1 pressing
* 
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecRCC.h"
#include "ecGPIO.h"

#define LED_PIN 	5
#define BUTTON_PIN 13

void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		
	}
}


// Initialiization 
void setup(void)
{
	RCC_HSI_init();
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);  // calls RCC_GPIOC_enable()
	GPIO_init(GPIOA, LED_PIN, OUTPUT);    // calls RCC_GPIOA_enable()	
	GPIO_init(GPIOA, 6, OUTPUT);    // calls RCC_GPIOA_enable()	
	GPIO_init(GPIOA, 7, OUTPUT);    // calls RCC_GPIOA_enable()	
	GPIO_init(GPIOB, 6, OUTPUT);    // calls RCC_GPIOB_enable()
}
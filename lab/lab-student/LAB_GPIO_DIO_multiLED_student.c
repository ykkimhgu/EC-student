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
#include "ecRCC2.h"
#include "ecGPIO2.h"

PinName_t LED_pin = PA_5;
PinName_t button_pin = PC_13;

void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		// TODO
		// TODO
	}
}


// Initialiization 
void setup(void)
{
	RCC_HSI_init();

	// Configuration button pin
	GPIO_init(button_pin, TODO);  // calls RCC_GPIOC_enable()

	// Configuration LED pins
	GPIO_init(LED_pin, TODO);    // calls RCC_GPIOA_enable()	
	// TODO
	// TODO
	// TODO
}

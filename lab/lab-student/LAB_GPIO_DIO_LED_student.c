/**
******************************************************************************
* @author	your name
* @Mod		date
* @brief	Embedded Controller:  LAB Digital In/Out
*					 - Toggle LED LD2 by Button B1 pressing
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
		
		if(GPIO_read(button_pin) == 0)	GPIO_write(LED_pin, HIGH);
		else 				GPIO_write(LED_pin, LOW);
	}
}


// Initialiization 
void setup(void)
{
	RCC_HSI_init();	
	GPIO_init(button_pin, INPUT);  // calls RCC_GPIOC_enable()
	GPIO_init(LED_pin, OUTPUT);    // calls RCC_GPIOA_enable()
	GPIO_pupd(LED_pin, EC_PU);
	GPIO_otype(LED_pin, 0);
}

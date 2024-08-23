/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2024-8-23 by YKKIM  	
* @brief   Embedded Controller:  LAB Digital In/Out with API
*					 - Toggle LED LD2 by Button B1  pressing
* 
******************************************************************************
*/

#include "ecGPIO_API2.h"

#define LED_PIN 	5
#define BUTTON_PIN 13

EC_DigitalIn button(GPIOC, BUTTON_PIN);
EC_DigitalOut led(GPIOA, LED_PIN);

	
int main(void) { 
	// Initialiization --------------------------------------------------------

	// Inifinite Loop ----------------------------------------------------------
	while(1){
		//if(button.read() == 0)	led.write(HIGH);
		//else 						led.write(LOW);
		if(!button)	led = 1;
		else 				led = 0;

	}
}


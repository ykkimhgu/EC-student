/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 05-03-2021
Modified         : 00-00-2026 [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : [WRITE BRIEF DESCRIPTION] !!!!!!
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN  		PA_5		//LD2
#define BUTTON_PIN  	PC_13		// B1 Button


void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();

	// Inifinite Loop ----------------------------------------------------------
	while(1){		
		if(GPIO_read(BUTTON_PIN) == 0)	GPIO_write(LED_PIN, HIGH);
		else 							GPIO_write(LED_PIN, LOW);
		delay_ms_HSI(100);             					// delay 100 ms
	}
}


// Initialiization 
void setup(void)
{
	RCC_HSI_init();	
	GPIO_init(BUTTON_PIN, INPUT);  // calls RCC_GPIOC_enable()
	GPIO_init(LED_PIN, OUTPUT);    // calls RCC_GPIOA_enable()
	GPIO_otype(LED_PIN, 0);
}
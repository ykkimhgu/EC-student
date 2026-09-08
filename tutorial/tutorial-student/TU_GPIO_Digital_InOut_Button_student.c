/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 05-03-2021
Modified         : 00-00-2026 [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : Tutorial Digital InOut
/----------------------------------------------------------------*/



// GPIO Mode			: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
// GPIO Speed			: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
// GPIO Output Type		: Output push-pull (0, reset), Output open drain (1)
// GPIO Push-Pull	 	: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)



#include "stm32f411xe.h"
#include "ecRCC2.h"

#define LED_PIN    5		//LD2
#define BUTTON_PIN 13		// B1 Button

int main(void) {	
	/* Part 1. RCC GPIOA Register Setting */
		RCC_HSI_init();
		RCC_GPIOA_enable();
		RCC_GPIOC_enable();

	/*---------------------------------------------------------------- 
                    [EXERCISE]
	---------------------------------------------------------------- */

	/*
	/// Part 2. GPIO Register Setting for OUTPUT ///			
		// GPIO Mode Register MODE=OUTPUT
		GPIOA->MODER &=  										// Clear '00' for Pin 5
		GPIOA->MODER |=  										// Set '01' for Pin 5
		
		
	
	/// Part 3. GPIO Register Setting for INPUT ///			
		// GPIO Mode Register  MODE=INPUT
		GPIOC->MODER &= 										// 00: Input	 		
   
		// GPIO Pull-Up/Pull-Down Register 
		GPIOC->PUPDR &= 	
		GPIOC->PUPDR  |=										// 10: Pull-down		    

	*/



	/* Button Value Initialization */	
		unsigned int btVal=0;
	
	/* Part 4. loop  */	
		while(1){
			//Read bit value of Button
			btVal= (GPIOC->IDR) & (1UL << BUTTON_PIN);	
			if(btVal == 0)
				GPIOA->ODR |= (1UL << LED_PIN);	 		
			else
				GPIOA->ODR &= ~(1UL << LED_PIN); 
		}
}

/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 05-03-2021
Modified         : 00-00-2026 [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : Tutorial Digital Out
/----------------------------------------------------------------*/



// GPIO Mode			 : Input(00), Output(01), AlterFunc(10), Analog(11, reset)
// GPIO Speed			 : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
// GPIO Push-Pull	 : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)


#include "stm32f4xx.h"
#include "ecRCC2.h"

#define LED_PIN    PA_5		//LD2


int main(void) {	
		/* Part 1. RCC GPIOA Register Setting */
		RCC_HSI_init();
		RCC_GPIOA_enable();
		
		/* Part 2. GPIO Register Setting */			
		// GPIO Mode Register
		GPIOA->MODER &= 											// Clear '00' for Pin 5
		GPIOA->MODER |=  											// Set '01' for Pin 5
		
		// GPIO Output Type Register  
		GPIOA->OTYPER &= 											// Clear '00'   
		GPIOA->OTYPER |=											// 0:Push-Pull
			
		// GPIO Pull-Up/Pull-Down Register 
		GPIOA->PUPDR  &= 											// 00: none
		
		// GPIO Output Speed Register 
		GPIOA->OSPEEDR &= 
		GPIOA->OSPEEDR |= 										//10:Fast Speed
	
		// Dead loop & program hangs here
		while(1){
			//	 GPIOA->ODR = 1UL << LED_PIN; 	// Set LED_PIN = H, others=L
			GPIOA->ODR |= (1UL << LED_PIN);	 		// Change only LED_PIN = H  
		}
}

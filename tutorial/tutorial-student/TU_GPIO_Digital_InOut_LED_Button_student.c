/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2024-09-06 by YKKIM  	
* @brief   Embedded Controller:  Tutorial Digital In
*					 - Turn on LED LD2 while Button B1 is pressing
* 
******************************************************************************
*/


// GPIO Mode			 : Input(00), Output(01), AlterFunc(10), Analog(11, reset)
// GPIO Speed			 : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
// GPIO Push-Pull	 : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)


#include "stm32f4xx.h"
#include "ecRCC2.h"

#define LED_PIN    PA_5		//LD2
#define BUTTON_PIN PC_13

int main(void) {	
	/* Part 1. RCC GPIOA Register Setting */
		RCC_HSI_init();
		RCC_GPIOA_enable();
		RCC_GPIOC_enable();
		
	/* Part 2. GPIO Register Setting for OUTPUT*/			
		// GPIO Mode Register
		GPIOA->MODER &=  										// Clear '00' for Pin 5
		GPIOA->MODER |=  										// Set '01' for Pin 5
		
		// GPIO Output Type Register  
		GPIOA->OTYPER &= 									 	// 0:Push-Pull   
		
		// GPIO Pull-Up/Pull-Down Register 
		GPIOA->PUPDR  &= 										// 00: none
		
		// GPIO Output Speed Register 
		GPIOA->OSPEEDR &= 	
		GPIOA->OSPEEDR |=  									 //10:Fast Speed
	
	
	/* Part 3. GPIO Register Setting for INPUT*/			
		// GPIO Mode Register
		GPIOC->MODER &= 										// 00: Input	 		
   
		// GPIO Pull-Up/Pull-Down Register 
		GPIOC->PUPDR &= 	
		GPIOC->PUPDR  |=										// 10: Pull-down		    

	/* Button Value Initialization */	
		unsigned int btVal=0;
	 
	/* Part 4. Deal loop  */	
		while(1){
			//Read bit value of Button
			btVal= (GPIOC->IDR) & (1UL << BUTTON_PIN);	
			if(btVal == 0)
				GPIOA->ODR |= (1UL << LED_PIN);	 		
			else
				GPIOA->ODR &= ~(1UL << LED_PIN); 
		}
}

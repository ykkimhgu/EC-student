/**
  ******************************************************************************
  * @author  SSSLAB
	* @Mod		 2021-8-12 by YKKIM  	
  * @brief   Embedded Controller:  Tutorial Digital Out
  *					 - Turn on LED LD2
  * 
  ******************************************************************************
*/


// GPIO Mode			 : Input(00), Output(01), AlterFunc(10), Analog(11, reset)
// GPIO Speed			 : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
// GPIO Push-Pull	 : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)


#include "stm32f4xx.h"
#define LED_PIN    5		//LD2

void RCC_HSI_init(void);   							//defined in ecRcc.h
void RCC_GPIOA_enable(void);

int main(void) {	
		/* Part 1. RCC GPIOA Register Setting */
		RCC_GPIOA_enable();
		
		/* Part 2. GPIO Register Setting */			
		// GPIO Mode Register
		GPIOA->MODER &= 											// Clear '00' for Pin 5
		GPIOA->MODER |=  											// Set '01' for Pin 5
		
		// GPIO Output Type Register  
		GPIOA->OTYPER &= 											// 0:Push-Pull   
		
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


void RCC_GPIOA_enable()
{
		// HSI is used as system clock         
		RCC_HSI_init();	
	
		// RCC Peripheral Clock for GPIO_A Enable 
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}


void RCC_HSI_init() {
	// Enable High Speed Internal Clock (HSI = 16 MHz)
  RCC->CR |= ((uint32_t)RCC_CR_HSION);

  // wait until HSI is ready
  while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
	
  // Select HSI as system clock source 
  RCC->CFGR &= (uint32_t)(~RCC_CFGR_SW); 									
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI; 				
			
	// Wait till HSI is used as system clock source
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != 0 ); 
}


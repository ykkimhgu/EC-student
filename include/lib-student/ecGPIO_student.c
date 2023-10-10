/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : SSS LAB
Created          : 05-03-2021
Modified         : 10-10-2023
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/



#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "ecGPIO_student.h"

void GPIO_init(GPIO_TypeDef *Port, int pin, int mode){     
	// mode  : Input(0), Output(1), AlterFunc(2), Analog(3)   
	if (Port == GPIOA)
		RCC_GPIOA_enable();
	if (Port == GPIOC)
		RCC_GPIOC_enable();
	
	//[TO-DO] YOUR CODE GOES HERE
	// Make it for GPIOB, GPIOD..GPIOH

	// You can also make a more general function of
	// void RCC_GPIO_enable(GPIO_TypeDef *Port); 

	GPIO_mode(Port, pin, mode);
	
}


// GPIO Mode          : Input(00), Output(01), AlterFunc(10), Analog(11)
void GPIO_mode(GPIO_TypeDef *Port, int pin, int mode){
   Port->MODER &= ~(3UL<<(2*pin));     
   Port->MODER |= mode<<(2*pin);    
}


// GPIO Speed          : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
void GPIO_ospeed(GPIO_TypeDef *Port, int pin, int speed){
	//[TO-DO] YOUR CODE GOES HERE
	//[TO-DO] YOUR CODE GOES HERE
}

// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
void GPIO_otype(GPIO_TypeDef *Port, int pin, int type){
   	//[TO-DO] YOUR CODE GOES HERE
	//[TO-DO] YOUR CODE GOES HERE
}

// GPIO Push-Pull    : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
void GPIO_pupd(GPIO_TypeDef *Port, int pin, int pupd){
   	//[TO-DO] YOUR CODE GOES HERE
	//[TO-DO] YOUR CODE GOES HERE
}

int GPIO_read(GPIO_TypeDef *Port, int pin){
   	//[TO-DO] YOUR CODE GOES HERE
	//[TO-DO] YOUR CODE GOES HERE
	return 0;    	//[TO-DO] YOUR CODE GOES HERE	
}


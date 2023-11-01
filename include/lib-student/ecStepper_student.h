#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecSysTick.h"
			
#ifndef __EC_STEPPER_H
#define __EC_STEPPER_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

//State mode
#define HALF 0
#define FULL 1	 
	 
/* Stepper Motor */
//stepper motor function

typedef struct{
	GPIO_TypeDef *port1;
	int pin1;
	GPIO_TypeDef *port2;
	int pin2;
	GPIO_TypeDef *port3;
	int pin3;
	GPIO_TypeDef *port4;
	int pin4;
	uint32_t _step_num;
} Stepper_t;

	 
void Stepper_init(GPIO_TypeDef* port1, int pin1, GPIO_TypeDef* port2, int pin2, GPIO_TypeDef* port3, int pin3, GPIO_TypeDef* port4, int pin4);
void Stepper_setSpeed(long whatSpeed);
void Stepper_step(uint32_t steps, uint32_t direction, uint32_t mode); 
void Stepper_stop(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

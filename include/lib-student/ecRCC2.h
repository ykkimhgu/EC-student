/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 05-03-2021
Modified         : 09-15-2026 [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : System Clock Library. Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/


#ifndef __EC_RCC2_H
#define __EC_RCC2_H


#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "ecPinNames.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


extern volatile int EC_SYSCLK;		// current SYSCLK [Hz], set by RCC_HSI_init()/RCC_PLL_init()
void delay_ms_HSI(uint32_t ms); 
void RCC_HSE_init(void); 
void RCC_PLL_init(void);
void RCC_GPIOA_enable(void);
void RCC_GPIOB_enable(void);
void RCC_GPIOC_enable(void);
void RCC_GPIOD_enable(void);


/*---------------------------------------------------------------- 
                    [EXERCISE]
---------------------------------------------------------------- */

// void RCC_GPIOE_enable(void);

/*---------------------------------------------------------------- 
                    [Advanced EXERCISE]
---------------------------------------------------------------- */
// void RCC_GPIO_enable(GPIO_TypeDef * GPIOx);
// void RCC_HSI_init(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_RCC2_H

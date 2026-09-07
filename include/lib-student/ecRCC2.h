/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 05-03-2021
Modified         : [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : System Clock Library. Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/


#ifndef __EC_RCC2_H
#define __EC_RCC2_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "stm32f4xx.h"
#include "stm32f411xe.h"

extern int EC_SYSCL;
void RCC_HSI_init(void);
void RCC_PLL_init(void);
void RCC_GPIOA_enable(void);
void RCC_GPIOB_enable(void);
void RCC_GPIOC_enable(void);

/*---------------------------------------------------------------- 
                    [EXERCISE]
---------------------------------------------------------------- */
// void RCC_GPIOD_enable(void);
// void RCC_GPIOE_enable(void);
// void RCC_GPIO_enable(GPIO_TypeDef * GPIOx);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_RCC2_H

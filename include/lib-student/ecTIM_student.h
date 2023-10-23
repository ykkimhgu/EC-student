/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  EC_HAL_for_student_exercise 
* 
******************************************************************************
*/

#ifndef __EC_TIM_H 
#define __EC_TIM_H
#include "stm32f411xe.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


/* Timer Configuration */
///////  Step1:  Initialize  TIMERx
void TIM_init(TIM_TypeDef* TIMx); 
// Default Setting:  TIM_period_ms(TIMx, 1 msec) with Counter_Clk 100kHz / PSC=840-1, ARR=100-1
// Previous version:  void TIM_init(TIM_TypeDef* TIMx, uint32_t msec);

///////  Step2:   Choose Timer Update Period  (a) msec or  (b) usec
void TIM_period(TIM_TypeDef* TIMx, uint32_t msec);    // msec of TimerUEV with Counter_Clk 100kHz / PSC=840, ARR=100*msec
void TIM_period_ms(TIM_TypeDef* TIMx, uint32_t msec);
void TIM_period_us(TIM_TypeDef* TIMx, uint32_t usec);  // usec of TimerUEV with Counter_Clk 1MHz / PSC=84, ARR=100*msec


/* Timer UI Interrupt Configuration */
///////  Step1:   Initialize TIM_UI with TIMERx
void TIM_UI_init(TIM_TypeDef* TIMx, uint32_t msec); 

///////  Step2:   Start by Enabling TIM_UI 
void TIM_UI_enable(TIM_TypeDef* TIMx);
void TIM_UI_disable(TIM_TypeDef* TIMx);

uint32_t is_UIF(TIM_TypeDef *TIMx);
void clear_UIF(TIM_TypeDef *TIMx);


///////  Example Code 1:  Configure TIM2 with 100usec 
// void setup(){
//   TIM_init(TIM2);	
//   TIM_period_us(TIM2, 100); 
//   
// }

///////  Example Code 2:  Configure TIM2 Update Interrupt with 10 msec 
// void setup(){
//   TIM_UI_init(TIM2,10);	
//   TIM_UI_enable(TIM2);    
// }



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

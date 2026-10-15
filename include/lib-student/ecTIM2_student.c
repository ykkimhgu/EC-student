/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 05-03-2021
Modified         : 09-22-2026 [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : [Write description here!!] 

				Search for [YOUR CODE GOES HERE !!!!]  and fill in the code.
/----------------------------------------------------------------*/

#include "ecTIM2_student.h"


//////////////////////////////////////////////
/* 			Timer Configuration 			*/
//////////////////////////////////////////////

// Timer Counter Initialization
// Default : f_cnt =  10kHz, msec= 1 to 6,553 msec 
void TIM_init(TIM_TypeDef* TIMx, uint32_t msec){ 
// 1. Enable Timer CLOCK
	TIM_clock_enable(TIMx);
// 2. Set CNT period
	TIM_period_ms(TIMx, msec); 			
// 3. COUNTER Direction
	// Up-Counter	
	TIMx->CR1 _________________;	// [YOUR CODE GOES HERE !!!!]
	// Down-Counter
	//TIMx->CR1 |= TIM_CR1_DIR;
// 4. Enable Timer Counter
	TIMx->CR1 |= TIM_CR1_CEN;		
}


// Timer Counter Initialization
// Default : f_cnt =  1 MHz, usec= 1 to 65,530 usec
void TIM_init_us(TIM_TypeDef* TIMx, uint32_t usec){ 
// 1. Enable Timer CLOCK
	// [YOUR CODE GOES HERE !!!!]
// 2. Set CNT period
	TIM_period_us(TIMx, usec); 
// 3. CNT Direction
	// [YOUR CODE GOES HERE !!!!]	
// 4. Enable Timer Counter
	// [YOUR CODE GOES HERE !!!!]
}


// Limits    : 16-bit ARR -> f_cnt =  10kHz, msec= 1 to 6,553 msec (ARR < 0xFFFF)
void TIM_period_ms(TIM_TypeDef* TIMx, uint32_t msec){

	uint32_t Sys_CLK = EC_SYSCLK;					// 84MHz (PLL) or 16MHz (HSI)

	// Counter Frequency (f_cnt) 
	uint32_t f_cnt   =  10000UL;					// 16-bit CNT: f_cnt=10kHz (default)
	
	// Prescaler Value
	uint32_t PSCval = Sys_CLK / f_cnt;				// 84MHz -> 10 kHz (PSC=8400)
	TIMx->PSC = PSCval - 1;

	// Reload Value (ARR) for msec unit
	uint32_t ARRval = (f_cnt / 1000UL) * msec;		// Reload values needed for msec 	
	TIMx->ARR = ARRval - 1;
}


// Limits    : 16-bit ARR -> f_cnt =  1 MHz, usec= 1 to 65,530 usec (ARR < 0xFFFF)
void TIM_period_us(TIM_TypeDef* TIMx, uint32_t usec){

	uint32_t Sys_CLK = EC_SYSCLK;					// 84MHz (PLL) or 16MHz (HSI)

	// Counter Frequency (f_cnt) 
	uint32_t f_cnt   =  1000000UL;					// 16-bit CNT: f_cnt=1MHz (default)
	
	// Prescaler Value
	uint32_t PSCval = Sys_CLK / f_cnt;				// 84MHz -> 1 MHz (PSC=84)
	TIMx->PSC = _______________;					// [YOUR CODE GOES HERE !!!!]

	// Reload Value (ARR) for msec unit
	uint32_t ARRval = ___________________;			// [YOUR CODE GOES HERE !!!!]
	TIMx->ARR = ARRval - 1;
}



// Same as TIM_period_ms;
void TIM_period(TIM_TypeDef* TIMx, uint32_t msec){
	TIM_period_ms(TIMx, msec);
}


void TIM_UI_init(TIM_TypeDef* TIMx, uint32_t msec){
// 1. Initialize Timer	
	TIM_init(TIMx,msec);
	
// 2. Enable Update Interrupt
	TIM_UI_enable(TIMx);
	
// 3. NVIC Setting
	uint32_t IRQn_reg = get_TIM_IRQn(TIMx);

	NVIC_EnableIRQ(IRQn_reg);				
	NVIC_SetPriority(IRQn_reg,2);
}

void TIM_UI_init_us(TIM_TypeDef* TIMx, uint32_t usec){
// 1. Initialize Timer	
	TIM_init_us(TIMx,usec);
	
// 2. Enable Update Interrupt
	TIM_UI_enable(TIMx);
	
// 3. NVIC Setting
	uint32_t IRQn_reg = get_TIM_IRQn(TIMx);

	NVIC_EnableIRQ(IRQn_reg);				
	NVIC_SetPriority(IRQn_reg,2);
}


// Enable Timer Update Interrupt	
void TIM_UI_enable(TIM_TypeDef* TIMx){
	TIMx->DIER _____________________;		// [YOUR CODE GOES HERE !!!!]
}

// Disable Timer Update Interrupt
void TIM_UI_disable(TIM_TypeDef* TIMx){
	TIMx->DIER &= ~(TIM_DIER_UIE);					
}

uint32_t is_UIF(TIM_TypeDef *TIMx){
	return TIMx->SR & TIM_SR_UIF;
}
void clear_UIF(TIM_TypeDef *TIMx){
	TIMx->SR &= ~TIM_SR_UIF;
}


uint32_t get_TIM_IRQn(TIM_TypeDef* TIMx){
	uint32_t IRQn_reg = 0x00000000;
	if(TIMx == TIM1)       IRQn_reg = TIM1_UP_TIM10_IRQn;
	else if(TIMx == TIM2)  IRQn_reg = TIM2_IRQn;
	else if(TIMx == TIM3)  IRQn_reg = TIM3_IRQn;
	else if(TIMx == TIM4)  IRQn_reg = TIM4_IRQn;
	else if(TIMx == TIM5)  IRQn_reg = TIM5_IRQn;
	else if(TIMx == TIM9)  IRQn_reg = TIM1_BRK_TIM9_IRQn;
	else if(TIMx == TIM10) IRQn_reg = TIM1_UP_TIM10_IRQn;
	else if(TIMx == TIM11) IRQn_reg = TIM1_TRG_COM_TIM11_IRQn;
	return IRQn_reg;	
}

void TIM_clock_enable(TIM_TypeDef* TIMx){ 
	if(TIMx ==TIM1) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	else if(TIMx ==TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	else if(TIMx ==TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	else if(TIMx ==TIM4) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	else if(TIMx ==TIM5) RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	else if(TIMx ==TIM9) RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	else if(TIMx ==TIM10) RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	else if(TIMx ==TIM11) RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;	
}
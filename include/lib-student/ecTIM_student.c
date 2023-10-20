/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  EC_HAL_for_student_exercise 
* 
******************************************************************************
*/


#include "ecTIM.h"
#include "ecGPIO.h"

/* Timer Configuration */

void TIM_init(TIM_TypeDef* TIMx, uint32_t msec){ 
	
// 1. Enable Timer CLOCK
	if(TIMx ==TIM1) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	else if(TIMx ==TIM2) RCC->APB1ENR |= __________________;
	else if(TIMx ==TIM3) __________________________________;
	// repeat for TIM4, TIM5, TIM9, TIM11
  // YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	
	
// 2. Set CNT period
	TIM_period_ms(TIMx, msec); 
	
	
// 3. CNT Direction
	TIMx->CR1 _________________;					// Upcounter	
	
// 4. Enable Timer Counter
	TIMx->CR1 |= TIM_CR1_CEN;		
}


//	Q. Which combination of PSC and ARR for msec unit?
// 	Q. What are the possible range (in sec ?)
void TIM_period_us(TIM_TypeDef *TIMx, uint32_t usec){   
	// Period usec = 1 to 1000

	// 1us(1MHz, ARR=1) to 65msec (ARR=0xFFFF)
	uint16_t PSCval;
	uint32_t Sys_CLK;
	
	if((RCC->CFGR & RCC_CFGR_SW_PLL) == RCC_CFGR_SW_PLL)
		Sys_CLK = 84000000;
	
	else if((RCC->CFGR & RCC_CFGR_SW_HSI) == RCC_CFGR_SW_HSI) 
		Sys_CLK = 16000000;
	
	
	if (TIMx == TIM2 || TIMx == TIM5){
		uint32_t ARRval;
		
		PSCval = _____;									// 84 or 16	--> f_cnt = 1MHz
		ARRval = Sys_CLK/PSCval/1000000 * usec;		// 1MHz*usec
		TIMx->PSC = ______________;
		TIMx->ARR = ARRval - 1;
	}
	else{
		uint16_t ARRval;

		PSCval = _____;										// 84 or 16	--> f_cnt = 1MHz
		ARRval = Sys_CLK/PSCval/1000000 * usec;		// 1MHz*usec
		TIMx->PSC = ______________;
		TIMx->ARR = ARRval - 1;
	}			
}


void TIM_period_ms(TIM_TypeDef* TIMx, uint32_t msec){ 
	// Period msec = 1 to 6000
	//f_psc=f_sysClk/(PSCval)

	// Let's make f_psc=100kHz
	//f_psc=84000000[Hz] / (PSCval)= 100000[Hz]

	//PSCval =(PSC+1)=f_sysClk/f_psc 
       	//	= 84000000[Hz] / 100000[Hz] 

	//(ARRval)= f_psc[Hz] * time[sec]
	// Let Period is msec
	
	//(ARRval)= 100000[Hz] *  msec/1000 [sec]
	//        = (f_sysClk/PSCval)* msec/1000 
	//        = (f_sysClk/PSCval)/1000* msec 
	//	= (ARR+1)

	// 0.1ms(10kHz, ARR = 1) to 6.5sec (ARR = 0xFFFF)
	// uint16_t PSCval = 8400;
	// uint16_t ARRval = ______________;  			// 84MHz/1000ms

	// TIMx->PSC = PSCval - 1;
	// TIMx->ARR = ___________;
	uint16_t PSCval;
	uint32_t Sys_CLK;
	
	if((RCC->CFGR & RCC_CFGR_SW_PLL) == RCC_CFGR_SW_PLL )
		 Sys_CLK = 84000000;
	
	else if((RCC->CFGR & RCC_CFGR_SW_HSI) == RCC_CFGR_SW_HSI) 
		Sys_CLK = 16000000;
	
	
	if (TIMx == TIM2 || TIMx == TIM5){
		uint32_t ARRval;
		
		PSCval = Sys_CLK/100000;									// 840 or 160	--> f_cnt = 100kHz
		ARRval = ______________;		// 100kHz*msec
		TIMx->PSC = PSCval - 1;
		TIMx->ARR = ___________;
	}
	else{
		uint16_t ARRval;

		PSCval = Sys_CLK/100000;									
		ARRval = ______________;		
		TIMx->PSC = PSCval - 1;
		TIMx->ARR = ___________;
	}
}

// msec = 1 to 6000
void TIM_period(TIM_TypeDef* TIMx, uint32_t msec){
	TIM_period_ms(TIMx, msec);
}

// Update Event Interrupt
void TIM_UI_init(TIM_TypeDef* TIMx, uint32_t msec){
// 1. Initialize Timer	
	TIM_init(TIMx,msec);
	
// 2. Enable Update Interrupt
	TIM_UI_enable(TIMx);
	
// 3. NVIC Setting
	uint32_t IRQn_reg =0;
	if(TIMx == TIM1)       IRQn_reg = TIM1_UP_TIM10_IRQn;
	else if(TIMx == TIM2)  IRQn_reg = ____________;
	// repeat for TIM3, TIM4, TIM5, TIM9, TIM10, TIM11
  // YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	
	NVIC_EnableIRQ(IRQn_reg);				
	NVIC_SetPriority(IRQn_reg,2);
}



void TIM_UI_enable(TIM_TypeDef* TIMx){
	TIMx->DIER _____________________;			// Enable Timer Update Interrupt		
}

void TIM_UI_disable(TIM_TypeDef* TIMx){
	TIMx->DIER &= ________________;				// Disable Timer Update Interrupt		
}

uint32_t is_UIF(TIM_TypeDef *TIMx){
	return TIMx->SR & _____________;
}

void clear_UIF(TIM_TypeDef *TIMx){
	TIMx->SR &= ~_________________;
}

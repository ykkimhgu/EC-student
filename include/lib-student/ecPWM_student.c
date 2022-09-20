/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  EC_HAL_for_student_exercise 
* 
******************************************************************************
*/


#include "ecPWM_student.h"

/* PWM Configuration */

void PWM_init(PWM_t *pwm, GPIO_TypeDef *port, int pin){
// 0. Match Output Port and Pin for TIMx 	
		pwm->port = port;
		pwm->pin  = pin;
		PWM_pinmap(pwm);
		TIM_TypeDef *TIMx = pwm->timer;
		int CHn = pwm->ch;	

// 1. Initialize GPIO port and pin as AF
		// YOUR CODE GOES HERE
		GPIO_init(port, pin, __);  // AF=2
		GPIO_ospeed(port, pin,__);  // speed VHIGH=3

	
// 2. Configure GPIO AFR by Pin num.				
	//  AFR[0] for pin: 0~7,     AFR[1] for pin 8~15
	//  AFR=1 for TIM1,TIM2	AFR=2 for TIM3 etc
	
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE

			
// 3. Initialize Timer 
		TIM_init(TIMx, 1);	// with default msec=1 value.		

// 3-2. Direction of Counter
		//YOUR CODE GOES HERE
		TIMx->CR1  ________________;    // Counting direction: 0 = up-counting, 1 = down-counting
	
			
// 4. Configure Timer Output mode as PWM
	uint32_t ccVal=TIMx->ARR/2;  // default value  CC=ARR/2
	if(CHn == 1){
		TIMx->CCMR1 &= ~TIM_CCMR1_OC1M;                     // Clear ouput compare mode bits for channel 1
		TIMx->CCMR1 |= ___________; 												// OC1M = 110 for PWM Mode 1 output on ch1. #define TIM_CCMR1_OC1M_1          (0x2UL << TIM_CCMR1_OC1M_Pos)
		TIMx->CCMR1	|= TIM_CCMR1_OC1PE;                     // Output 1 preload enable (make CCR1 value changable)
		TIMx->CCR1  = ccVal; 																// Output Compare Register for channel 1 (default duty ratio = 50%)		
		TIMx->CCER &= ~TIM_CCER_CC1P;                       // select output polarity: active high	
		TIMx->CCER  |= _____________;												// Enable output for ch1
	}
	else if(CHn == 2){
		TIMx->CCMR1 &= ~TIM_CCMR1_OC2M;                     // Clear ouput compare mode bits for channel 2
		// YOUR CODE GOES HERE 															// OC1M = 110 for PWM Mode 1 output on ch2
		// YOUR CODE GOES HERE                     					// Output 1 preload enable (make CCR2 value changable)	
		// YOUR CODE GOES HERE 															// Output Compare Register for channel 2 (default duty ratio = 50%)		
		// YOUR CODE GOES HERE                       				// select output polarity: active high	
		// YOUR CODE GOES HERE															// Enable output for ch2
	}
	else if(CHn == 3){
		TIMx->CCMR2 &= ~TIM_CCMR2_OC3M;                     // Clear ouput compare mode bits for channel 3
		// YOUR CODE GOES HERE 															// OC1M = 110 for PWM Mode 1 output on ch3
		// YOUR CODE GOES HERE                     					// Output 1 preload enable (make CCR3 value changable)	
		// YOUR CODE GOES HERE 															// Output Compare Register for channel 3 (default duty ratio = 50%)		
		// YOUR CODE GOES HERE                       				// select output polarity: active high	
		// YOUR CODE GOES HERE															// Enable output for ch3
	}
	else if(CHn == 4){
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE		
	}	
	
	
	
// 5. Enable Timer Counter
	if(TIMx == TIM1) TIMx->BDTR |= TIM_BDTR_MOE;					// Main output enable (MOE): 0 = Disable, 1 = Enable	
	TIMx->CR1  |= TIM_CR1_CEN;  													// Enable counter
}


void PWM_period_ms(PWM_t *PWM_pin, uint32_t msec){
	TIM_TypeDef *TIMx = PWM_pin->timer;
	TIM_period_ms(___, _____);  //YOUR CODE GOES HERE
}

void PWM_period_us(PWM_t *PWM_pin, uint32_t usec){
	TIM_TypeDef *TIMx = PWM_pin->timer;
	TIM_period_us(___, _____); 	//YOUR CODE GOES HERE
}


void PWM_pulsewidth_ms(PWM_t *pwm, float pulse_width_ms){ 
	TIM_TypeDef *TIMx = pwm->timer;
	int CHn = pwm->ch;
	uint32_t fsys = 0;
	uint32_t psc=pwm->timer->PSC;
	
	// Check System CLK: PLL or HSI
	if((RCC->CFGR & (3<<0)) == 2)      { fsys = 84000; }  // for msec 84MHz/1000
	else if((RCC->CFGR & (3<<0)) == 0) { fsys = 16000; }
	
	//YOUR CODE GOES HERE
	float fclk = _______________					// fclk=fsys/(psc+1);
	uint32_t ccval = ____________					// width_ms *fclk - 1;
	
	//YOUR CODE GOES HERE
	switch(CHn){
		case 1: TIMx->CCR1 = ccval; break;
		// REPEAT for CHn=2,  3, 4
		// REPEAT for CHn=2,  3, 4
		// REPEAT for CHn=2,  3, 4
		default: break;
	}
}


void   PWM_duty(PWM_t *pwm, float duty) {                 //  duty=0 to 1	
		float ccval = ___________________;    								// (ARR+1)*dutyRatio - 1          
		int CHn = pwm->ch;
  
		//YOUR CODE GOES HERE	
		switch(CHn){
			case 1: TIMx->CCR1 = ccval; break;
			// REPEAT for CHn=2,  3, 4
			// REPEAT for CHn=2,  3, 4
			// REPEAT for CHn=2,  3, 4
			default: break;
		}
}


// DO NOT MODIFY HERE
void PWM_pinmap(PWM_t *PWM_pin){
   GPIO_TypeDef *port = PWM_pin->port;
   int pin = PWM_pin->pin;
   
   if(port == GPIOA) {
      switch(pin){
         case 0 : PWM_pin->timer = TIM2; PWM_pin->ch = 1; break;
         case 1 : PWM_pin->timer = TIM2; PWM_pin->ch = 2; break;
         case 5 : PWM_pin->timer = TIM2; PWM_pin->ch = 1; break;
         case 6 : PWM_pin->timer = TIM3; PWM_pin->ch = 1; break;
         //case 7: PWM_pin->timer = TIM1; PWM_pin->ch = 1N; break;
         case 8 : PWM_pin->timer = TIM1; PWM_pin->ch = 1; break;
         case 9 : PWM_pin->timer = TIM1; PWM_pin->ch = 2; break;
         case 10: PWM_pin->timer = TIM1; PWM_pin->ch = 3; break;
         case 15: PWM_pin->timer = TIM2; PWM_pin->ch = 1; break;
         default: break;
      }         
   }
   else if(port == GPIOB) {
      switch(pin){
         //case 0: PWM_pin->timer = TIM1; PWM_pin->ch = 2N; break;
         //case 1: PWM_pin->timer = TIM1; PWM_pin->ch = 3N; break;
         case 3 : PWM_pin->timer = TIM2; PWM_pin->ch = 2; break;
         case 4 : PWM_pin->timer = TIM3; PWM_pin->ch = 1; break;
         case 5 : PWM_pin->timer = TIM3; PWM_pin->ch = 2; break;
         case 6 : PWM_pin->timer = TIM4; PWM_pin->ch = 1; break;
         case 7 : PWM_pin->timer = TIM4; PWM_pin->ch = 2; break;
         case 8 : PWM_pin->timer = TIM4; PWM_pin->ch = 3; break;
         case 9 : PWM_pin->timer = TIM4; PWM_pin->ch = 3; break;
         case 10: PWM_pin->timer = TIM2; PWM_pin->ch = 3; break;
         
         default: break;
      }
   }
   else if(port == GPIOC) {
      switch(pin){
         case 6 : PWM_pin->timer = TIM3; PWM_pin->ch = 1; break;
         case 7 : PWM_pin->timer = TIM3; PWM_pin->ch = 2; break;
         case 8 : PWM_pin->timer = TIM3; PWM_pin->ch = 3; break;
         case 9 : PWM_pin->timer = TIM3; PWM_pin->ch = 4; break;
         
         default: break;
      }
   }
	 // TIM5 needs to be added, if used.
}

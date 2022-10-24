#include "ecTIM.h"
#include "ecGPIO.h"
#include "math.h"

/* Input Capture  */

void ICAP_init(IC_t *ICx, GPIO_TypeDef *port, int pin){
// 0. Match Input Capture Port and Pin for TIMx
	ICx->port = port;
	ICx->pin  = pin;
	ICAP_pinmap(ICx);	  										// Port, Pin --(mapping)--> TIMx, Channel
	
	TIM_TypeDef *TIMx = ICx->timer;
	int TIn = ICx->ch; 		
	int ICn=TIn;
	ICx->ICnum=ICn;													// (default) TIx=ICx

// GPIO configuration ---------------------------------------------------------------------	
// 1. Initialize GPIO port and pin as AF
	________________________;  							// GPIO init as AF=2
	GPIO_ospeed(port, pin, ___);  						// speed VHIGH=3	

// 2. Configure GPIO AFR by Pin num.
	if(TIMx == TIM1 || TIMx == TIM2)											 port->AFR[pin >> 3] |= 0x01 << (4*(pin % 8)); // TIM1~2
	else if  // for TIM3~
	________________________;  

	
// TIMER configuration ---------------------------------------------------------------------			
// 1. Initialize Timer 
	TIM_init(TIMx, 1);
// 2. Initialize Timer Interrpt 
	TIM_INT_init(TIMx, 1);        					// TIMx Interrupt initialize 
// 3. Modify ARR Maxium for 1MHz
	TIMx->PSC = 84-1;						  					// Timer counter clock: 1MHz(1us)  for PLL
	TIMx->ARR = 0xFFFF;											// Set auto reload register to maximum (count up to 65535)
// 4. Disable Counter during configuration
	TIMx->CR1 &= ~TIM_CR1_CEN;  						// Disable Counter during configuration


	
// Input Capture configuration ---------------------------------------------------------------------			
// 1. Select Timer channel(TIx) for Input Capture channel(ICx)
	// Default Setting
	TIMx->CCMR1 &= 	~TIM_CCMR1_CC1S;
	TIMx->CCMR1 &=
	TIMx->CCMR2 &=
	TIMx->CCMR2 &=
	TIMx->CCMR1 |= 	TIM_CCMR1_CC1S_0;      	//01<<0   CC1S    TI1=IC1
	________________________;  				     	//01<<8   CC2s    TI2=IC2
	________________________;        				//01<<0   CC3s    TI3=IC3
	________________________;  							//01<<8   CC4s    TI4=IC4


// 2. Filter Duration (use default)

// 3. IC Prescaler (use default)


// 4. Activation Edge: CCyNP/CCyP	
	TIMx->CCER ___________________;					// CCy(Rising) for ICn


// 5.	Enable CCy Capture, Capture/Compare interrupt
	TIMx->CCER ___________________;					// CCn(ICn) Capture Enable	

// 6.	Enable Interrupt of CC(CCyIE), Update (UIE)
	TIMx->DIER ___________________;					// Capture/Compare Interrupt Enable	for ICn
	TIMx->DIER |= TIM_DIER_UIE;							// Update Interrupt enable	

// 7.	Enable Counter 
	TIMx->CR1	 |= TIM_CR1_CEN;							// Counter enable	
}




// Configure Selecting TIx-ICy and Edge Type
void ICAP_setup(IC_t *ICx, int ICn, int edge_type){
	TIM_TypeDef *TIMx = ICx->timer;	// TIMx
	int 				CHn 	= ICx->ch;		// Timer Channel CHn
	ICx->ICnum = ICn;

// Disable  CC. Disable CCInterrupt for ICn. 
	TIMx->CCER ___________________;															// Capture Disable
	TIMx->DIER ___________________;															// CCn Interrupt Disable	
	
	
// Configure  IC number(user selected) with given IC pin(TIMx_CHn)
	switch(ICn){
			case 1:
					TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;											//reset   CC1S
					if (ICn==CHn) TIMx->CCMR1 |= 	TIM_CCMR1_CC1S_0;     //01<<0   CC1S    Tx_Ch1=IC1
					else ___________________;      											//10<<0   CC1S    Tx_Ch2=IC1
					break;
			case 2:
					TIMx->CCMR1 ___________________;										//reset   CC2S
					if (ICn==CHn) TIMx->CCMR1 ___________________;     	//01<<0   CC2S    Tx_Ch2=IC2
					else ___________________;     											//10<<0   CC2S    Tx_Ch1=IC2
					break;
			case 3:
					TIMx->CCMR2 &= ~TIM_CCMR2_CC3S;											//reset   CC3S
					if (ICn==CHn) TIMx->CCMR2 ___________________;	    //01<<0   CC3S    Tx_Ch3=IC3
					else TIMx->CCMR2 ___________________;		     				//10<<0   CC3S    Tx_Ch4=IC3
					break;
			case 4:
					TIMx->CCMR2 ___________________;										//reset   CC4S
					if (ICn==CHn) ___________________;	   						  //01<<0   CC4S    Tx_Ch4=IC4
					else TIMx->CCMR2 ___________________;	     					//10<<0   CC4S    Tx_Ch3=IC4
					break;
			default: break;
		}


// Configure Activation Edge direction
	TIMx->CCER  ___________________;	  									// Clear CCnNP/CCnP bits for ICn
	switch(edge_type){
		case RISE: TIMx->CCER ___________________;	 break; //rising:  00
		case FALL: TIMx->CCER ___________________;	 break; //falling: 01
		case BOTH: TIMx->CCER ___________________;	 break; //both:    11
	}
	
// Enable CC. Enable CC Interrupt. 
	TIMx->CCER |= 1 << (4*(ICn - 1)); 										// Capture Enable
	TIMx->DIER |= 1 << ICn; 															// CCn Interrupt enabled	
}



// Time span for one counter step
void ICAP_counter_us(IC_t *ICx, int usec){	
	TIM_TypeDef *TIMx = ICx->timer;	
	TIMx->PSC = 84*usec-1;						  // Timer counter clock: 1us * usec
	TIMx->ARR = 0xFFFF;									// Set auto reload register to maximum (count up to 65535)
}


uint32_t is_CCIF(TIM_TypeDef *TIMx, uint32_t ccNum){
	return (TIMx->SR ___________________;	
}

void clear_CCIF(TIM_TypeDef *TIMx, uint32_t ccNum){
	TIMx->SR ___________________;	
}



//DO NOT MODIFY THIS
void ICAP_pinmap(IC_t *timer_pin){
   GPIO_TypeDef *port = timer_pin->port;
   int pin = timer_pin->pin;
   
   if(port == GPIOA) {
      switch(pin){
         case 0 : timer_pin->timer = TIM2; timer_pin->ch = 1; break;
         case 1 : timer_pin->timer = TIM2; timer_pin->ch = 2; break;
         case 5 : timer_pin->timer = TIM2; timer_pin->ch = 1; break;
         case 6 : timer_pin->timer = TIM3; timer_pin->ch = 1; break;
         //case 7: timer_pin->timer = TIM1; timer_pin->ch = 1N; break;
         case 8 : timer_pin->timer = TIM1; timer_pin->ch = 1; break;
         case 9 : timer_pin->timer = TIM1; timer_pin->ch = 2; break;
         case 10: timer_pin->timer = TIM1; timer_pin->ch = 3; break;
         case 15: timer_pin->timer = TIM2; timer_pin->ch = 1; break;
         default: break;
      }         
   }
   else if(port == GPIOB) {
      switch(pin){
         //case 0: timer_pin->timer = TIM1; timer_pin->ch = 2N; break;
         //case 1: timer_pin->timer = TIM1; timer_pin->ch = 3N; break;
         case 3 : timer_pin->timer = TIM2; timer_pin->ch = 2; break;
         case 4 : timer_pin->timer = TIM3; timer_pin->ch = 1; break;
         case 5 : timer_pin->timer = TIM3; timer_pin->ch = 2; break;
         case 6 : timer_pin->timer = TIM4; timer_pin->ch = 1; break;
         case 7 : timer_pin->timer = TIM4; timer_pin->ch = 2; break;
         case 8 : timer_pin->timer = TIM4; timer_pin->ch = 3; break;
         case 9 : timer_pin->timer = TIM4; timer_pin->ch = 3; break;
         case 10: timer_pin->timer = TIM2; timer_pin->ch = 3; break;
         
         default: break;
      }
   }
   else if(port == GPIOC) {
      switch(pin){
         case 6 : timer_pin->timer = TIM3; timer_pin->ch = 1; break;
         case 7 : timer_pin->timer = TIM3; timer_pin->ch = 2; break;
         case 8 : timer_pin->timer = TIM3; timer_pin->ch = 3; break;
         case 9 : timer_pin->timer = TIM3; timer_pin->ch = 4; break;
         
         default: break;
      }
   }
}

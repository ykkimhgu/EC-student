#include "ecICAP.h"
#include "ecGPIO.h"
#include "math.h"

/* -------- Timer Input Capture -------- */

void ICAP_init(PinName_t pinName){
// 0. Match Input Capture Port and Pin for TIMx
	GPIO_TypeDef *port;
	unsigned int pin;	
	ecPinmap(pinName, &port, &pin);	
	TIM_TypeDef *TIMx;
	int TIn;
	
	ICAP_pinmap(pinName, &TIMx, &TIn);
	int ICn = TIn;													// (default) TIx=ICx

// GPIO configuration ---------------------------------------------------------------------	
// 1. Initialize GPIO port and pin as AF
	GPIO_init(port, pin, AF);  							// AF=2
	GPIO_ospeed(port, pin, EC_HIGH);  						// speed VHIGH=3		

// 2. Configure GPIO AFR by Pin num.
	if(TIMx == TIM1 || TIMx == TIM2)											 port->AFR[pin >> 3] |= 0x01 << (4*(pin % 8)); // TIM1~2
	else if(TIMx == TIM3 || TIMx == TIM4 || TIMx == TIM5)  port->AFR[pin >> 3] |= 0x02 << (4*(pin % 8)); // TIM3~5 
	else if(TIMx == TIM9 || TIMx == TIM10|| TIMx == TIM11) port->AFR[pin >> 3] |= 0x03 << (4*(pin % 8)); // TIM9~11 


	
// TIMER configuration ---------------------------------------------------------------------			
// 1. Initialize Timer Interrpt 
	TIM_UI_init(TIMx, 1);        					// TIMx Interrupt initialize 

// 2. Modify ARR Maxium for 1MHz
	TIMx->PSC = 84-1;						  					// Timer counter clock: 1MHz(1us)  for PLL
	TIMx->ARR = 0xFFFF;											// Set auto reload register to maximum (count up to 65535)

// 3. Disable Counter during configuration
	TIMx->CR1 &= ~TIM_CR1_CEN;  						// Disable Counter during configuration


	
// Input Capture configuration ---------------------------------------------------------------------			
// 1. Select Timer channel(TIx) for Input Capture channel(ICx)
	// Default Setting
	TIMx->CCMR1 &= 	~TIM_CCMR1_CC1S;
	TIMx->CCMR1 &= 	~TIM_CCMR1_CC2S;
	TIMx->CCMR2 &= 	~TIM_CCMR2_CC3S;
	TIMx->CCMR2 &= 	~TIM_CCMR2_CC4S;
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
void ICAP_setup(PinName_t pinName, int ICn, int edge_type){
// 0. Match Input Capture Port and Pin for TIMx
	GPIO_TypeDef *port;
	unsigned int pin;	
	ecPinmap(pinName, &port, &pin);	
	TIM_TypeDef *TIMx;
	int CHn;		
	ICAP_pinmap(pinName, &TIMx, &CHn);

// 1. Disable  CC. Disable CCInterrupt for ICn. 
	TIMx->CCER ___________________;															// Capture Disable
	TIMx->DIER ___________________;															// CCn Interrupt Disable	
	
	
// 2. Configure  IC number(user selected) with given IC pin(TIMx_CHn)
	switch(ICn){
			case 1:
					TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;											//reset   CC1S
					if (ICn==CHn) TIMx->CCMR1 |= 	TIM_CCMR1_CC1S_0;     //01<<0   CC1S    Tx_Ch1=IC1
					else TIMx->CCMR1 |= 	TIM_CCMR1_CC1S_1;      											//10<<0   CC1S    Tx_Ch2=IC1
					break;
			case 2:
					TIMx->CCMR1 ___________________;										//reset   CC2S
					if (ICn==CHn) TIMx->CCMR1 ___________________;     	//01<<0   CC2S    Tx_Ch2=IC2
					else ___________________;     											//10<<0   CC2S    Tx_Ch1=IC2
					break;
			case 3:
					TIMx->CCMR2 &= ~TIM_CCMR2_CC3S;											//reset   CC3S
					if (ICn==CHn) TIMx->CCMR2 ___________________;	    //01<<8   CC3S    Tx_Ch3=IC3
					else TIMx->CCMR2 ___________________;		     				//10<<8   CC3S    Tx_Ch4=IC3
					break;
			case 4:
					TIMx->CCMR2 ___________________;										//reset   CC4S
					if (ICn==CHn) ___________________;	   						  //01<<8   CC4S    Tx_Ch4=IC4
					else TIMx->CCMR2 ___________________;	     					//10<<8   CC4S    Tx_Ch3=IC4
					break;
			default: break;
		}


// 3. Configure Activation Edge direction
	TIMx->CCER  &= ~(0b1010 << 4*(ICn - 1));	  									// Clear CCnNP/CCnP bits for ICn
	switch(edge_type){
		case IC_RISE: TIMx->CCER ___________________;	 break; //rising:  00
		case IC_FALL: TIMx->CCER ___________________;	 break; //falling: 01
		case IC_BOTH: TIMx->CCER ___________________;	 break; //both:    11
	}
	
// 4. Enable CC. Enable CC Interrupt. 
	TIMx->CCER |= 1 << (4*(ICn - 1)); 										// Capture Enable
	TIMx->DIER |= 1 << ICn; 															// CCn Interrupt enabled	
}

// Time span for one counter step
void ICAP_counter_us(PinName_t pinName, int usec){
// 0. Match Input Capture Port and Pin for TIMx	
	GPIO_TypeDef *port;
	unsigned int pin;	
	ecPinmap(pinName, &port, &pin);	
	TIM_TypeDef *TIMx;
	int CHn;		
	ICAP_pinmap(pinName, &TIMx, &CHn);

// 1. Configuration Timer Prescaler and ARR
	TIMx->PSC = 84*usec-1;						  // Timer counter clock: 1us * usec
	TIMx->ARR = 0xFFFF;									// Set auto reload register to maximum (count up to 65535)
}

uint32_t is_CCIF(TIM_TypeDef *TIMx, uint32_t ccNum){
	return (TIMx->SR & (0x1UL << ccNum)) != 0;	
}

void clear_CCIF(TIM_TypeDef *TIMx, uint32_t ccNum){
	TIMx->SR &= ~(1 << ccNum);	
}

uint32_t ICAP_capture(TIM_TypeDef* TIMx, uint32_t ICn){
	uint32_t capture_Value;
	
	if (ICn == 1)
		capture_Value = TIMx->CCR1;
	else if (ICn == 2)
		capture_Value = TIMx->CCR2;
	else if (ICn == 2)
		capture_Value = TIMx->CCR3;
	else
		capture_Value = TIMx->CCR4;

	return capture_Value;
}



//DO NOT MODIFY THIS
void ICAP_pinmap(PinName_t pinName, TIM_TypeDef **TIMx, int *chN){
	 GPIO_TypeDef *port;
	 unsigned int pin;		
	 ecPinmap(pinName, &port, &pin);	
   
   if(port == GPIOA) {
      switch(pin){
         case 0 : *TIMx = TIM2; *chN = 1; break;
         case 1 : *TIMx = TIM2; *chN = 2; break;
         case 5 : *TIMx = TIM2; *chN = 1; break;
         case 6 : *TIMx = TIM3; *chN = 1; break;
         //case 7: *TIMx = TIM1; *chN = 1N; break;
         case 8 : *TIMx = TIM1; *chN = 1; break;
         case 9 : *TIMx = TIM1; *chN = 2; break;
         case 10: *TIMx = TIM1; *chN = 3; break;
         case 15: *TIMx = TIM2; *chN = 1; break;
         default: break;
      }         
   }
   else if(port == GPIOB) {
      switch(pin){
         //case 0: *TIMx = TIM1; *chN = 2N; break;
         //case 1: *TIMx = TIM1; *chN = 3N; break;
         case 3 : *TIMx = TIM2; *chN = 2; break;
         case 4 : *TIMx = TIM3; *chN = 1; break;
         case 5 : *TIMx = TIM3; *chN = 2; break;
         case 6 : *TIMx = TIM4; *chN = 1; break;
         case 7 : *TIMx = TIM4; *chN = 2; break;
         case 8 : *TIMx = TIM4; *chN = 3; break;
         case 9 : *TIMx = TIM4; *chN = 3; break;
         case 10: *TIMx = TIM2; *chN = 3; break;
         
         default: break;
      }
   }
   else if(port == GPIOC) {
      switch(pin){
         case 6 : *TIMx = TIM3; *chN = 1; break;
         case 7 : *TIMx = TIM3; *chN = 2; break;
         case 8 : *TIMx = TIM3; *chN = 3; break;
         case 9 : *TIMx = TIM3; *chN = 4; break;
         
         default: break;
      }
   }
}

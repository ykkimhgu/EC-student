/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 05-03-2021
Modified         : 10-21-2026 [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : [Write description here!!]
/----------------------------------------------------------------*/



#include "ecICAP2.h"


////////////////////////////////////////////////////////////////
/* Timer Input Capture  Initialization                        */
////////////////////////////////////////////////////////////////

// Default: 1 usec Tick Timer,  Assume TIn=ICn
void ICAP_init(PinName_t pinName, uint32_t edge_type) {
    TIM_TypeDef *TIMx;
    int TIn;
    ICAP_pinmap(pinName, &TIMx, &TIn);

    ICAP_ICn_init(pinName, edge_type, TIn);		// ICn = TIn
}


// User Deinfed ICn with 1 usec Tick Timer
//     Initialize in this order
//
//     ICAP_init(ECHO, IC_RISE);				// IC1
//     ICAP_ICn_init(ECHO, IC_FALL, IC_2);		// IC2
//     ICAP_tick(ECHO, 10);					    // one tick = 10 usec  <- last
//
void ICAP_ICn_init(PinName_t pinName, uint32_t edge_type, uint32_t ICn) {
// 0. Match Input Capture Port and Pin for TIMx
    GPIO_TypeDef *port;
    unsigned int pin;
    ecPinmap(pinName, &port, &pin);
    TIM_TypeDef *TIMx;
    int TIn;
    ICAP_pinmap(pinName, &TIMx, &TIn);

// GPIO configuration
// ---------------------------------------------------------------------
// 1. Initialize GPIO port and pin as AF
    GPIO_init(pinName, AF);         // AF=2
    GPIO_pupd(pinName,EC_NONE);
    GPIO_ospeed(pinName, EC_HIGH);  // speed VHIGH=3

// 2. Configure GPIO AFR by Pin num.
    // AFR[0] holds pin 0~7, AFR[1] holds pin 8~15 
	uint32_t afrIdx   = pin >> 3;	// same as (pin/8)
	uint32_t afrShift = 4 * (pin % 8);
	uint32_t afNum    = 0;
	
	//  AF1 for TIM1,TIM2     AF2 for TIM3,TIM4
	if((TIMx == TIM1)||(TIMx == TIM2))		afNum = 1;	// AF1
	else if(TIMx== TIM3 || TIMx == TIM4 || TIMx == TIM5)	afNum = 2;	// AF2
    else if (TIMx == TIM9 || TIMx == TIM10 || TIMx == TIM11) afNum = 3;// AF3
	
	// Set AFR register for the pin
	port->AFR[afrIdx] &= ~(0xFUL << afrShift);	// 4 bits per pin
	port->AFR[afrIdx] |=  (afNum << afrShift);


// TIMER configuration (shared by all four channels)
// ---------------------------------------------------------------------
// 1. Timer clock, Update Interrupt and NVIC
    TIM_UI_init_us(TIMx, 1);        // TIMx Interrupt initialize at 1us=1MHz

// 2. Default tick = 1 usec, ARR at maximum
    ICAP_tick(pinName, 1);

// 3. Disable Counter during configuration
    TIMx->CR1 &= ~TIM_CR1_CEN;   


// Input Capture configuration (this channel only)
// ---------------------------------------------------------------------
// 1. Select Timer channel(TIx) for Input Capture channel(ICx)            
    ICAP_ICn_select(TIMx, TIn, ICn);

// 2. Filter Duration for THIS channel. Writing all four channels would
//    corrupt any channel of this timer that is used as an output: in CCMRx
//    the ICxF bits are the OCxM bits of an output channel.
    ICAP_filter(TIMx, ICn, IC_FILTER_3);    // N = 8 samples at fCK_INT
    
// 3. IC Prescaler (use default)

// 4. Activation Edge: CCyNP/CCyP        
    TIMx->CCER  &= ~(0b1010 << 4*(ICn - 1));  			// Clear CCnNP/CCnP bits
	switch(edge_type){
		case IC_RISE: TIMx->CCER &= ~(0b1010 << 4*(ICn - 1)); break; //rising:  00
		case IC_FALL: TIMx->CCER |= 0b0010 << 4*(ICn - 1); 	 break; //falling: 01
		case IC_BOTH: TIMx->CCER |= 0b1010 << 4*(ICn - 1); 	 break; //both:    11
	}

// 5.	Enable CCy Capture, Capture/Compare interrupt
    TIMx->CCER |= 1 << (4 * (ICn - 1));  // CCn(ICn) Capture Enable

// 6.	Enable Interrupt of CC(CCyIE), Update (UIE) per ICn
    TIMx->DIER |= 1 << ICn;      // Capture/Compare Interrupt Enable
    TIMx->DIER |= TIM_DIER_UIE;  // Update Interrupt enable

// 7.	Enable Counter
    TIMx->CR1 |= TIM_CR1_CEN;  // Counter enable
}


//  Timer Counter Tick in usec.  
//  This modifies PSC. Then, ARR is kept at maximum
//  CK_CNT = EC_SYSCLK / (PSC+1)   ->   PSC = usec * EC_SYSCLK/1e6 - 1
//  NOTE:  TIM_init_us() / TIM_period_us() cannot be used here: they FIX the PSC
void ICAP_tick(PinName_t pinName, uint32_t usec) {
    // 0. Match Input Capture Port and Pin for TIMx
    TIM_TypeDef *TIMx;
    int CHn;
    ICAP_pinmap(pinName, &TIMx, &CHn);

    if(usec == 0)	usec = 1;			// usec is unsigned : 0 would give PSC = -1
    
    // 1. Prescaler for the requested tick. PSC is 16-bit, < 65536
    uint32_t presc = (uint32_t)(((uint64_t)usec * (uint32_t)EC_SYSCLK) / 1000000UL);
    if(presc < 1)		presc = 1;
    if(presc > 65536)	presc = 65536;			// clamp, PSC = presc - 1
    TIMx->PSC = presc - 1;

    // 2. Configure ARR to be Maximum 
    TIMx->ARR = 0xFFFF;  // Set auto reload register to maximum (count up to 65535)
    if((TIMx == TIM2)||(TIMx == TIM5))		TIMx->ARR = 0xFFFFFFFF;

    // 3. Clear the UIF 
    TIMx->EGR |= TIM_EGR_UG;
    TIMx->SR   = ~TIM_SR_UIF;
}


uint32_t ICAP_read(PinName_t pinName, uint32_t ICn) {
    TIM_TypeDef *TIMx;
	int CHn;		
	ICAP_pinmap(pinName, &TIMx, &CHn);
    
    uint32_t capture_Value;

    if (ICn == 1)
        capture_Value = TIMx->CCR1;
    else if (ICn == 2)
        capture_Value = TIMx->CCR2;
    else if (ICn == 3)
        capture_Value = TIMx->CCR3;
    else
        capture_Value = TIMx->CCR4;

    return capture_Value;
}


//////////////////////////////////////////////
/* Timer Input CC Interrupt Configuration         */
//////////////////////////////////////////////


uint32_t is_CCIF(TIM_TypeDef *TIMx, uint32_t ICn) {
    return (TIMx->SR & (0x1UL <<  ICn)) != 0;
}

void clear_CCIF(TIM_TypeDef *TIMx, uint32_t  ICn) {
    // Clear the UIF  by writing
    TIMx->SR = ~(0x1UL << ICn);
}




//////////////////////////////////////////////
/* Internal Functions                       */
//////////////////////////////////////////////
void ICAP_filter(TIM_TypeDef *TIMx, uint32_t ICn, uint16_t filterOpt)
{
	// 4 bits per channel: ICnF at bit 4 (ch 1, 3) and bit 12 (ch 2, 4)
	uint32_t shift = (ICn % 2) ? 4 : 12;			// odd channel -> 4, even -> 12

	if(ICn == 1 || ICn == 2){
		TIMx->CCMR1 &= ~(0xFUL  << shift);
		TIMx->CCMR1 |=  (filterOpt << shift);
	}
	else{
		TIMx->CCMR2 &= ~(0xFUL  << shift);
		TIMx->CCMR2 |=  (filterOpt << shift);
	}
}

// Selecting TIx-ICy
void ICAP_ICn_select(TIM_TypeDef *TIMx, uint32_t TIn, uint32_t ICn){
// 2. Configure  IC number(user selected) with given IC pin(TIMx_CHn)
	switch(ICn){
			case 1:
					TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;						//reset   CC1S
					if (ICn==TIn) TIMx->CCMR1 |= 	TIM_CCMR1_CC1S_0;   //01<<0   CC1S    Tx_Ch1=IC1
					else TIMx->CCMR1 |= 	TIM_CCMR1_CC1S_1;      		//10<<0   CC1S    Tx_Ch2=IC1
					break;
			case 2:
					TIMx->CCMR1 &= ~TIM_CCMR1_CC2S;						//reset   CC2S
					if (ICn==TIn) TIMx->CCMR1 |= 	TIM_CCMR1_CC2S_0;   //01<<0   CC2S    Tx_Ch2=IC2
					else TIMx->CCMR1 |= 	TIM_CCMR1_CC2S_1;      		//10<<0   CC2S    Tx_Ch1=IC2
					break;
			case 3:
					TIMx->CCMR2 &= ~TIM_CCMR2_CC3S;						//reset   CC3S
					if (ICn==TIn) TIMx->CCMR2 |= 	TIM_CCMR2_CC3S_0;   //01<<0   CC3S    Tx_Ch3=IC3
					else TIMx->CCMR2 |= 	TIM_CCMR2_CC3S_1;      		//10<<0   CC3S    Tx_Ch4=IC3
					break;
			case 4:
					TIMx->CCMR2 &= ~TIM_CCMR2_CC4S;						//reset   CC4S
					if (ICn==TIn) TIMx->CCMR2 |= 	TIM_CCMR2_CC4S_0;   //01<<0   CC4S    Tx_Ch4=IC4
					else TIMx->CCMR2 |= 	TIM_CCMR2_CC4S_1;      		//10<<0   CC4S    Tx_Ch3=IC4
					break;
			default: break;
		}

}


// Matching ICn Pin Number
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
         case 9 : *TIMx = TIM4; *chN = 4; break;
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




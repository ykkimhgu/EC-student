/*----------------------------------------------------------------
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-22-2026
Modified         : 10-22-2026
Language/ver     : C in VS Code

Description      : ADC library (Nucleo-F411RE), regular and injected groups.
                   See ecADC2.h for the usage patterns.
----------------------------------------------------------------*/

#include "ecADC2.h"


////////////////////////////////////////////////////////////////
/* Basic Syntax                                                */
////////////////////////////////////////////////////////////////

// Analog pin, ADC clock, 12-bit, sampling time, one-channel sequence.
// software trigger, one conversion,
void ADC_init(PinName_t pinName){
	uint32_t chN;
	ADC_pinmap(pinName, &chN);

// 1. GPIO as ANALOG, no pull-up / pull-down
	GPIO_init(pinName, ANALOG);				// ANALOG = 3
	GPIO_pupd(pinName, EC_NONE);

// 2. ADC clock, prescaler, resolution
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC->CCR     &= ~ADC_CCR_ADCPRE;		// 00 : PCLK2 / 2 = 42 MHz
	ADC1->CR1    &= ~ADC_CR1_RES;			// 00 : 12-bit

// 3. Channel sampling time : 84 ADC cycles = 2 usec at 42 MHz
	if(chN < 10){
		ADC1->SMPR2 &= ~(7UL << (3 * chN));
		ADC1->SMPR2 |=  (4UL << (3 * chN));
	}
	else{
		ADC1->SMPR1 &= ~(7UL << (3 * (chN - 10)));
		ADC1->SMPR1 |=  (4UL << (3 * (chN - 10)));
	}

// 4. Default: Regular Group, One-channel sequence
	ADC1->CR1  &= ~ADC_CR1_SCAN;			// 0 : one-channel mode
	ADC1->SQR1 &= ~ADC_SQR1_L;				// 0000 : sequence length 1
	ADC1->SQR3 &= ~ADC_SQR3_SQ1;
	ADC1->SQR3 |= (chN & ADC_SQR3_SQ1);		// SQ1 = this channel

// 5. EOC after EVERY conversion, so one sample can be polled or read per EOC.
//    EOCS = 1 also switches overrun detection on - see clear_ADC_OVR().
	ADC1->CR2 |= ADC_CR2_EOCS;

// 6. Default : software trigger, ONE conversion, NO interrupt
	ADC1->CR2 &= ~ADC_CR2_EXTEN;			// no hardware trigger
	ADC1->CR2 &= ~ADC_CR2_CONT;				// single conversion
	ADC1->CR1 &= ~ADC_CR1_EOCIE;
	ADC1->CR1 &= ~ADC_CR1_OVRIE;

// 7. Enable ADC
	ADC1->SR   = 0;							// flags are cleared by writing 0
	ADC1->CR2 |= ADC_CR2_ADON;
}


// single conversion : start, wait for EOC, return the result.
// For the software-trigger mode, i.e. without ADC_trigger_init().
uint32_t ADC_read_poll(void){
	ADC1->CR2 &= ~ADC_CR2_CONT;				// one conversion only
	ADC_start();							// clears a latched OVR, then SWSTART

	while(!is_ADC_EOC()){;}					// 12-bit + 84 cycles = about 2.5 usec

	return ADC_read();						// reading DR clears EOC
}


// Use inside ADC_IRQHandler() after is_ADC_EOC().
uint32_t ADC_read(void){
	return ADC1->DR;
}


// Hardware trigger : the TIMx UPDATE event (TRGO) every msec.
// Also enables the EOC interrupt
// Only TIM2 and TIM3 reach the ADC regular group on the F411
// Returns 0 on success, -1 if TIMx has no TRGO entry for the regular group.
int ADC_trigger_init(TIM_TypeDef *TIMx, uint32_t msec, uint32_t edge){
	uint32_t extsel;

	// Allowed : TIM2, TIM3 ONLY
	if     (TIMx == TIM2)	extsel = 0x6;	// 0110 : TIM2 TRGO
	else if(TIMx == TIM3)	extsel = 0x8;	// 1000 : TIM3 TRGO
	else					return -1;

// 1. TIMx Update Event : one TRGO pulse per update event, every msec.
	TIM_init(TIMx, msec);					// clock on, 10 kHz tick, ARR, CEN
	TIMx->CR2 &= ~TIM_CR2_MMS;
	TIMx->CR2 |= TIM_CR2_MMS_1;				// 010 : Update event as TRGO

// 2. ADC HW Trigger Select : trigger source and edge
	ADC1->CR2 &= ~ADC_CR2_EXTSEL;
	ADC1->CR2 |= (extsel << ADC_CR2_EXTSEL_Pos);

	ADC1->CR2 &= ~ADC_CR2_EXTEN;
	if     (edge == ADC_RISE)	ADC1->CR2 |= ADC_CR2_EXTEN_0;	// 01 rising
	else if(edge == ADC_FALL)	ADC1->CR2 |= ADC_CR2_EXTEN_1;	// 10 falling
	else if(edge == ADC_BOTH)	ADC1->CR2 |= ADC_CR2_EXTEN;		// 11 both

// 3. Arm the EOC interrupt - without it, a triggered conversion that nothing
//    reads in time just gets overwritten by the next one (see ecADC2.h).
	ADC_EOC_enable();

	return 0;
}


////////////////////////////////////////////////////////////////
/* JADC : the INJECTED group,                                  */
////////////////////////////////////////////////////////////////

// Analog pin + one-channel injected sequence.
// software trigger, no interrupt,
void JADC_init(PinName_t pinName){
	uint32_t chN;
	ADC_pinmap(pinName, &chN);

// 1. GPIO as ANALOG, no pull-up / pull-down
	GPIO_init(pinName, ANALOG);
	GPIO_pupd(pinName, EC_NONE);

// 2. ADC clock, prescaler, resolution, alignment
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC->CCR     &= ~ADC_CCR_ADCPRE;		// 00 : PCLK2 / 2 = 42 MHz
	ADC1->CR1    &= ~ADC_CR1_RES;			// 00 : 12-bit
	ADC1->CR2    &= ~ADC_CR2_ALIGN;			// 0  : right alignment

// 3. Channel sampling time : 84 ADC cycles = 2 usec at 42 MHz
	if(chN < 10){
		ADC1->SMPR2 &= ~(7UL << (3 * chN));
		ADC1->SMPR2 |=  (4UL << (3 * chN));
	}
	else{
		ADC1->SMPR1 &= ~(7UL << (3 * (chN - 10)));
		ADC1->SMPR1 |=  (4UL << (3 * (chN - 10)));
	}

// 4. One-channel injected sequence.
//    The injected sequence is RIGHT-aligned : with JL = 0 the single channel
//    belongs in JSQ4, not JSQ1, and its result lands in JDR1.
	ADC1->CR1  |= ADC_CR1_SCAN;				// JSQR drives the sequence
	ADC1->JSQR &= ~ADC_JSQR_JL;				// 00 : one injected conversion
	ADC1->JSQR &= ~ADC_JSQR_JSQ4;
	ADC1->JSQR |= (chN << ADC_JSQR_JSQ4_Pos);

// 5. Idle defaults : software trigger, no interrupt.
//    RM0383 11.3.5 : "Injected channels cannot be converted continuously",
//    the only exception being JAUTO, so CONT stays 0 here.
	ADC1->CR2 &= ~ADC_CR2_JEXTEN;
	ADC1->CR2 &= ~ADC_CR2_CONT;
	ADC1->CR1 &= ~ADC_CR1_JEOCIE;

// 6. Power on. Nothing converts until JADC_read_poll() or a trigger arrives.
	ADC1->SR   = 0;
	ADC1->CR2 |= ADC_CR2_ADON;
}


// SW triggered single injected conversion
uint32_t JADC_read_poll(void){
	ADC1->CR2 |= ADC_CR2_JSWSTART;

	while(!is_ADC_JEOC()){;}

	uint32_t jvalue = JADC_read(1);		// conversion 1 of the sequence
	clear_ADC_JEOC();			// JEOC is cleared by SOFTWARE only - unlike EOC,
	return jvalue;				// reading JDRx does not clear it (RM0383)
}


// Result of the n-th conversion of the injected sequence, JDRn = 1 ~ 4.
// Reading JDRx does NOT clear JEOC - call clear_ADC_JEOC() yourself.
uint32_t JADC_read(uint32_t JDRn){
	switch(JDRn){
		case 1:	return ADC1->JDR1;
		case 2:	return ADC1->JDR2;
		case 3:	return ADC1->JDR3;
		case 4:	return ADC1->JDR4;
		default: return 0;
	}
}


// Hardware trigger : the TIMx UPDATE event (TRGO) every msec.
// Also enables the JEOC interrupt
// ADC_CR2 JEXTSEL : 0001 = TIM1 TRGO, 0011 = TIM2 TRGO,
//                   1001 = TIM4 TRGO, 1011 = TIM5 TRGO.
// Returns 0 on success, -1 if TIMx has no TRGO entry for the injected group.
int JADC_trigger_init(TIM_TypeDef *TIMx, uint32_t msec, uint32_t edge){
	uint32_t jextsel;

	if     (TIMx == TIM1)	jextsel = 0x1;
	else if(TIMx == TIM2)	jextsel = 0x3;
	else if(TIMx == TIM4)	jextsel = 0x9;
	else if(TIMx == TIM5)	jextsel = 0xB;
	else					return -1;

// 1. TIMx as the master : one TRGO pulse per update event, every msec.
//    No compare channel of TIMx is consumed.
	TIM_init(TIMx, msec);
	TIMx->CR2 &= ~TIM_CR2_MMS;
	TIMx->CR2 |= TIM_CR2_MMS_1;				// 010 : Update event as TRGO

// 2. ADC side : trigger source and edge
	ADC1->CR2 &= ~ADC_CR2_JEXTSEL;
	ADC1->CR2 |= (jextsel << ADC_CR2_JEXTSEL_Pos);

	ADC1->CR2 &= ~ADC_CR2_JEXTEN;
	if     (edge == ADC_RISE)	ADC1->CR2 |= ADC_CR2_JEXTEN_0;	// 01 rising
	else if(edge == ADC_FALL)	ADC1->CR2 |= ADC_CR2_JEXTEN_1;	// 10 falling
	else if(edge == ADC_BOTH)	ADC1->CR2 |= ADC_CR2_JEXTEN;	// 11 both

// 3. Arm the JEOC interrupt - same reason as ADC_trigger_init().
	JADC_JEOC_enable();

	return 0;
}


////////////////////////////////////////////////////////////////
/* Interrupt,  SW Start, multi-channel configuration                    */
////////////////////////////////////////////////////////////////

void ADC_EOC_enable(void){
	ADC1->SR = 0;							// do not enter the ISR on a stale flag
	ADC1->CR1 |= ADC_CR1_EOCIE;
	NVIC_SetPriority(ADC_IRQn, 2);
	NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_EOC_disable(void){
	ADC1->CR1 &= ~ADC_CR1_EOCIE;
	NVIC_DisableIRQ(ADC_IRQn);
}


void ADC_start(void){
	clear_ADC_OVR();						// a latched OVR blocks every start
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

void ADC_stop(void){						// clears ADON : the trigger stops converting
	ADC1->CR2 &= ~ADC_CR2_ADON;
}


void ADC_sequence(PinName_t *seqCHn, uint32_t seqCHnums){
	if(seqCHnums < 1)	return;

	ADC1->CR2 &= ~ADC_CR2_ADON;				// SQRx must not change while running

	if(seqCHnums > 1)	ADC1->CR1 |= ADC_CR1_SCAN;		// multi-channel scan
	else				ADC1->CR1 &= ~ADC_CR1_SCAN;

	ADC1->SQR1 &= ~ADC_SQR1_L;
	ADC1->SQR1 |= ((seqCHnums - 1) << ADC_SQR1_L_Pos);

	for(uint32_t i = 0; i < seqCHnums; i++){
		uint32_t chN;
		ADC_pinmap(seqCHn[i], &chN);

		if(i < 6){
			ADC1->SQR3 &= ~(0x1FUL << (i * 5));
			ADC1->SQR3 |=  (chN    << (i * 5));
		}
		else if(i < 12){
			ADC1->SQR2 &= ~(0x1FUL << ((i - 6) * 5));
			ADC1->SQR2 |=  (chN    << ((i - 6) * 5));
		}
		else{
			ADC1->SQR1 &= ~(0x1FUL << ((i - 12) * 5));
			ADC1->SQR1 |=  (chN    << ((i - 12) * 5));
		}
	}

	ADC1->CR2 |= ADC_CR2_ADON;
}


void ADC_conversion(uint32_t convMode){		// ADC_SINGLE / ADC_CONT
	if(convMode == ADC_CONT)	ADC1->CR2 |=  ADC_CR2_CONT;
	else						ADC1->CR2 &= ~ADC_CR2_CONT;
}


uint32_t is_ADC_EOC(void){
	return (ADC1->SR & ADC_SR_EOC) == ADC_SR_EOC;
}

void clear_ADC_EOC(void){
	ADC1->SR &= ~ADC_SR_EOC;
}


// OVR is set when a conversion completes before the previous result was read.
// it must be cleared or the ADC stays silent for good.
uint32_t is_ADC_OVR(void){
	return (ADC1->SR & ADC_SR_OVR) == ADC_SR_OVR;
}

void clear_ADC_OVR(void){
	ADC1->SR &= ~ADC_SR_OVR;
}


// JEOC interrupt + NVIC.
// ADC_IRQn is shared with the regular group.
void JADC_JEOC_enable(void){
	ADC1->SR = 0;
	ADC1->CR1 |= ADC_CR1_JEOCIE;
	NVIC_SetPriority(ADC_IRQn, 2);
	NVIC_EnableIRQ(ADC_IRQn);
}

void JADC_JEOC_disable(void){
	ADC1->CR1 &= ~ADC_CR1_JEOCIE;
	// ADC_IRQn is SHARED with the regular group, so the NVIC line is only
	// released once neither group wants it.
	if((ADC1->CR1 & ADC_CR1_EOCIE) == 0)	NVIC_DisableIRQ(ADC_IRQn);
}


// Software start of the injected group. Same rule as ADC_start() : not needed
// on the JADC_trigger_init() path (measured 499 conversions in 500 ms without
// it), only to kick a software-triggered conversion or to resume after ADON
// was cleared. JADC_read_poll() does its own start.
void JADC_start(void){
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_JSWSTART;
}


// Multi-channel injected sequence, 1 ~ 4 channels. Conversion i lands in JDR(i+1).
void JADC_sequence(PinName_t *seqCHn, uint32_t seqCHnums){
	if(seqCHnums < 1 || seqCHnums > 4)	return;

	ADC1->CR2 &= ~ADC_CR2_ADON;				// JSQR must not change while running
	ADC1->CR1 |= ADC_CR1_SCAN;

	ADC1->JSQR &= ~ADC_JSQR_JL;
	ADC1->JSQR |= ((seqCHnums - 1) << ADC_JSQR_JL_Pos);

	// RIGHT-aligned sequence : the i-th conversion of n sits at
	// JSQ(4 - (n-1-i)), i.e. bit 15 - 5*(n-1-i). Its result lands in JDR(i+1).
	for(uint32_t i = 0; i < seqCHnums; i++){
		uint32_t chN;
		ADC_pinmap(seqCHn[i], &chN);
		uint32_t shift = 15 - 5 * (seqCHnums - 1 - i);
		ADC1->JSQR &= ~(0x1FUL << shift);
		ADC1->JSQR |=  (chN    << shift);
	}

	ADC1->CR2 |= ADC_CR2_ADON;
}


uint32_t is_ADC_JEOC(void){
	return (ADC1->SR & ADC_SR_JEOC) == ADC_SR_JEOC;
}

void clear_ADC_JEOC(void){
	ADC1->SR &= ~ADC_SR_JEOC;
}


////////////////////////////////////////////////////////////////
/* Internal                                                    */
////////////////////////////////////////////////////////////////

// Nucleo-F411RE : ADC1_INx <-> port/pin
void ADC_pinmap(PinName_t pinName, uint32_t *chN){
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);

	*chN = 0;

	if(port == GPIOA){
		switch(pin){
			case 0 : *chN =  0; break;
			case 1 : *chN =  1; break;
			case 4 : *chN =  4; break;
			case 5 : *chN =  5; break;
			case 6 : *chN =  6; break;
			case 7 : *chN =  7; break;
			default: break;
		}
	}
	else if(port == GPIOB){
		switch(pin){
			case 0 : *chN =  8; break;
			case 1 : *chN =  9; break;
			default: break;
		}
	}
	else if(port == GPIOC){
		switch(pin){
			case 0 : *chN = 10; break;
			case 1 : *chN = 11; break;
			case 2 : *chN = 12; break;
			case 3 : *chN = 13; break;
			case 4 : *chN = 14; break;
			case 5 : *chN = 15; break;
			default: break;
		}
	}
}

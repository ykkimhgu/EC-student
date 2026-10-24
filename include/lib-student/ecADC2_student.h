/*----------------------------------------------------------------
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-22-2026
Modified         : 10-22-2026
Language/ver     : C in VS Code

Description      : ADC library (Nucleo-F411RE), regular and injected groups.

                   Polling (Arduino style):
                       ADC_init(PB_0);
                       value = ADC_read_poll();

                   Timer-triggered + interrupt:
                       ADC_init(PB_0);
                       ADC_trigger_init(TIM3, 1, ADC_RISE);   // also enables the EOC interrupt
                                                                // - needs ADC_IRQHandler()

                   The JADC_* half is the same calls for the injected
                   group, where JDR1~JDR4 hold a whole sequence.
----------------------------------------------------------------*/

#ifndef __EC_ADC2_H
#define __EC_ADC2_H

#include <stdint.h>
#include "stm32f411xe.h"
#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecTIM2.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


// Trigger edge for ADC_trigger_init()
#define ADC_RISE		1
#define ADC_FALL		2
#define ADC_BOTH		3

// Repetition for ADC_conversion()
#define ADC_SINGLE		0		// one conversion per trigger
#define ADC_CONT		1		// keep converting after the first trigger


////////////////////////////////////////////////////////////////
/* Basic Syntax                                                */
////////////////////////////////////////////////////////////////

// Analog pin, ADC clock, 12-bit, sampling time, one-channel sequence.
// software trigger, one conversion,
void ADC_init(PinName_t pinName);

// single conversion : start, wait for EOC, return the result.
// For the software-trigger mode, i.e. without ADC_trigger_init().
uint32_t ADC_read_poll(void);

// Use inside ADC_IRQHandler() after is_ADC_EOC().
uint32_t ADC_read(void);

// Hardware trigger : the TIMx UPDATE event (TRGO) every msec. 
// Also enables the EOC interrupt 
// Only TIM2 and TIM3 reach the ADC regular group on the F411
// Returns 0 on success, -1 if TIMx has no TRGO entry for the regular group.
int ADC_trigger_init(TIM_TypeDef *TIMx, uint32_t msec, uint32_t edge);


////////////////////////////////////////////////////////////////
/* JADC : the INJECTED group,                                  */
////////////////////////////////////////////////////////////////

// Analog pin + one-channel injected sequence. 
// software trigger, no interrupt, 
void JADC_init(PinName_t pinName);

// SW triggered single injected conversion
uint32_t JADC_read_poll(void);

// Result of the n-th conversion of the injected sequence, JDRn = 1 ~ 4.
// Reading JDRx does NOT clear JEOC - call clear_ADC_JEOC() yourself.
uint32_t JADC_read(uint32_t JDRn);

// Hardware trigger : the TIMx UPDATE event (TRGO) every msec. 
// Also enables the JEOC interrupt
// ADC_CR2 JEXTSEL : 0001 = TIM1 TRGO, 0011 = TIM2 TRGO,
//                   1001 = TIM4 TRGO, 1011 = TIM5 TRGO.
// Returns 0 on success, -1 if TIMx has no TRGO entry for the injected group.
int JADC_trigger_init(TIM_TypeDef *TIMx, uint32_t msec, uint32_t edge);



////////////////////////////////////////////////////////////////
/* Interrupt,  SW Start, multi-channel configuration                    */
////////////////////////////////////////////////////////////////

void ADC_EOC_enable(void);
void ADC_EOC_disable(void);

void ADC_start(void);
void ADC_stop(void);			// clears ADON : the trigger stops converting

void ADC_sequence(PinName_t *seqCHn, uint32_t seqCHnums);
void ADC_conversion(uint32_t convMode);		// ADC_SINGLE / ADC_CONT

uint32_t is_ADC_EOC(void);
void clear_ADC_EOC(void);

// OVR is set when a conversion completes before the previous result was read.
// it must be cleared or the ADC stays silent for good.
uint32_t is_ADC_OVR(void);
void clear_ADC_OVR(void);

// JEOC interrupt + NVIC.
// ADC_IRQn is shared with the regular group.
void JADC_JEOC_enable(void);
void JADC_JEOC_disable(void);

// Software start of the injected group. Same rule as ADC_start() : not needed
// on the JADC_trigger_init() path (measured 499 conversions in 500 ms without
// it), only to kick a software-triggered conversion or to resume after ADON
// was cleared. JADC_read_poll() does its own start.
void JADC_start(void);

// Multi-channel injected sequence, 1 ~ 4 channels. Conversion i lands in JDR(i+1).
void JADC_sequence(PinName_t *seqCHn, uint32_t seqCHnums);

uint32_t is_ADC_JEOC(void);
void clear_ADC_JEOC(void);



////////////////////////////////////////////////////////////////
/* Internal                                                    */
////////////////////////////////////////////////////////////////

void ADC_pinmap(PinName_t pinName, uint32_t *chN);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  // __EC_ADC2_H

/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 05-03-2021
Modified         : 10-21-2026 [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : [Write description here!!]
/----------------------------------------------------------------*/


#ifndef __EC_ICAP2_H
#define __EC_ICAP2_H

#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecTIM2.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Input Capture*/


// Edge Type
#define IC_RISE 0
#define IC_FALL 1
#define IC_BOTH 2

// Input Capture Number
#define IC_1 1
#define IC_2 2
#define IC_3 3
#define IC_4 4

// Input Capture Filter 
#define IC_FILTER_0		0		// no filter, transition sampled at fDTS
#define IC_FILTER_1		1		// N = 2 samples at fCK_INT
#define IC_FILTER_2		2		// N = 4 samples at fCK_INT
#define IC_FILTER_3		3		// N = 8 samples at fCK_INT


////////////////////////////////////////////////////////////////
/* Timer Input Capture  Initialization                        */
////////////////////////////////////////////////////////////////

// Default: 1 usec Tick Timer,  Assume TIn=ICn
void ICAP_init(PinName_t pinName, uint32_t edge_type);

// User-Defined IC channel for the pin, 1 usec tick
void ICAP_ICn_init(PinName_t pinName, uint32_t edge_type, uint32_t ICn);

// User-Defined tick in usec.  CALL THIS LAST
// usec range: 1 ~ 780 usec at 84 MHz (PSC is 16-bit).
void ICAP_tick(PinName_t pinName, uint32_t usec);

uint32_t ICAP_read(PinName_t pinName, uint32_t ICn);


//////////////////////////////////////////////
/* Timer Input CC Interrupt Configuration         */
//////////////////////////////////////////////
uint32_t is_CCIF(TIM_TypeDef *TIMx, uint32_t  ICn);  // CCnum= 1~4
void clear_CCIF(TIM_TypeDef *TIMx, uint32_t  ICn);


//////////////////////////////////////////////
/* Internal Functions                       */
//////////////////////////////////////////////
void ICAP_pinmap(PinName_t pinName, TIM_TypeDef **TIMx, int *chN);
void ICAP_filter(TIM_TypeDef *TIMx, uint32_t ICn, uint16_t filterOpt);
void ICAP_ICn_select(TIM_TypeDef *TIMx, uint32_t TIn, uint32_t ICn);




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  // __EC_ICAP2_H
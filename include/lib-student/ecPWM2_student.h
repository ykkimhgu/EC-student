/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 05-03-2021
Modified         : 09-22-2026 [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : [Write description here!!]
*          PWM library - exercise version.
*          Fill in every  "YOUR CODE GOES HERE"  and every  "____"  blank
*          in ecPWM2_student.c .  This header needs no editing.

/----------------------------------------------------------------*/

#ifndef __EC_PWM2_STUDENT_H
#define __EC_PWM2_STUDENT_H

#include "math.h"
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecTIM2.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* PWM Output Mode : OCxM field in CCMRx (3 bits) */
#define EC_PWM_MODE1	0	// output active while CNT <  CCR  (default)
#define EC_PWM_MODE2	1	// output active while CNT >= CCR  (inverted)


///////////////////////////////////////////////////
/* 		        EXERCISE					     */
///////////////////////////////////////////////////

// Internal Helper function: for PWM Initialization
void PWM_init_config(PinName_t pinName, uint32_t usec);
// Duty ratio 0~1.0
void PWM_duty(PinName_t pinName, float duty);



///////////////////////////////////////////////////
/* 		GIVEN:  PWM Initialization                */
///////////////////////////////////////////////////

//  Low Frequency PWM initialization in msec. (below 1kHz)
// 	Default: 		50% duty ratio. PWM_mode1, Period in msec.
//  TIM2/TIM5 		(32-bit ARR) : msec 1 ~ 4,294,967
//  TIM1/TIM3/TIM4 	(16-bit ARR) : msec 1 ~ 6,553   
void PWM_init(PinName_t pinName, uint32_t msec);	// 1 to 6,553 msec (16-bit)

//  High-Frequency PWM initialization in usec (1kHz to 100 kHz)
// 	Default: 		50% duty ratio.  PWM_mode1, Period in usec.
//  TIM2/TIM5 		(32-bit ARR) : 	usec 1 ~ 4,294,967
//  TIM1/TIM3/TIM4 	(16-bit ARR) : 	usec 1 ~ 6,553   
void PWM_init_us(PinName_t pinName, uint32_t usec);	// 10 usec and up


///////////////////////////////////////////////////
/* 		GIVEN: DUTY RATIO SETUP                 */
///////////////////////////////////////////////////

// Set Pulse width in msec
void PWM_pulsewidth(PinName_t pinName, uint32_t pulse_width_ms);

// Set Pulse width in usec
void PWM_pulsewidth_us(PinName_t pinName, uint32_t pulse_width_us);


////////////////////////////////////////////////////////////////
/* Internal Helper Functions - GIVEN. DO NOT MODIFY           */
////////////////////////////////////////////////////////////////

// Internal function for PWM output mode : EC_PWM_MODE1 or EC_PWM_MODE2
void PWM_mode_config(TIM_TypeDef *TIMx, int chN, uint32_t mode);
// Internal Helper function for PWM Pin Matching
void PWM_pinmap(PinName_t pinName, TIM_TypeDef **TIMx, int *chN);





#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_PWM2_STUDENT_H

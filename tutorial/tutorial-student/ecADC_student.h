#ifndef __MY_ADC_H
#define __MY_ADC_H
#include "stm32f411xe.h"

// ADC trigmode
#define SW 0
#define TRGO 1

// ADC contmode
#define CONT 0
#define SINGLE 1

// Edge Type
#define RISE 1
#define FALL 2
#define BOTH 3

#define _DEFAULT 0

// ADC setting
// Exercise- with Tutorial
void ADC_init(GPIO_TypeDef *port, int pin, int trigmode);			// trigmode : SW , TRGO

// Exercise- with LAB
void ADC_continue(int contmode); 													// contmode : CONT, SINGLE / Operate both ADC,JADC
void ADC_TRGO(TIM_TypeDef* TIMx, int msec, int edge);
void ADC_sequence(int length, int *seq); 

// Exercise- with Tutorial
void ADC_start(void);
uint32_t is_ADC_EOC(void);
uint32_t ADC_read(void);
uint32_t ADC_pinmap(GPIO_TypeDef *port, int pin);

// Exercise- with LAB
uint32_t is_ADC_OVR(void);
void clear_ADC_OVR(void);

#endif

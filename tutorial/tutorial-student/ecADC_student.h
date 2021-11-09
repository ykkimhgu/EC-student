#ifndef __MY_ADC_H
#define __MY_ADC_H
#include "stm32f411xe.h"

// ADC mode
#define DEFAULT 0
#define TRGO 1

// ADC setting
void ADC_init(GPIO_TypeDef *port, int pin, int mode);
void ADC_start(ADC_TypeDef *ADCx);


uint32_t is_ADC_EOC(ADC_TypeDef *ADCx);

uint32_t ADC_read();
uint32_t ADC_pinmap(GPIO_TypeDef *port, int pin);

#endif

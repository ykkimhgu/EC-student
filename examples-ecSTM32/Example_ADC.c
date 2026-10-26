/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-24-2026
Modified         : 10-24-2026
Language/ver     : C in VS Code

Description      : Example - ecADC2 API (docs/ecADC2.md Quick Start)
                   PB_0, hardware-triggered by TIM3 every 1 ms. ADC_trigger_init()
                   also arms the EOC interrupt, so ADC_IRQHandler() is required.
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecTIM2.h"
#include "ecADC2.h"

volatile uint32_t value = 0;

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    ADC_init(PB_0);
    ADC_trigger_init(TIM3, 1, ADC_RISE);     // TIM3 TRGO every 1 ms - also arms the EOC interrupt
}

int main(void){
    setup();
    while(1){}
}

void ADC_IRQHandler(void){
    if (is_ADC_EOC()) value = ADC_read();    // reading DR clears EOC
}

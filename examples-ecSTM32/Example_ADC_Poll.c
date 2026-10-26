/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-25-2026
Modified         : 10-25-2026
Language/ver     : C in VS Code

Description      : Example - ecADC2 API (docs/ecADC2.md Quick Start)
                   PB_0, single channel, software-triggered polling
                   (Arduino style) - no interrupt, no ADC_IRQHandler() needed.
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecADC2.h"

volatile uint32_t value = 0;

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    ADC_init(PB_0);
}

int main(void){
    setup();
    while(1){
        value = ADC_read_poll();   // blocking: start, wait for EOC, return result
        delay_ms(100);
    }
}

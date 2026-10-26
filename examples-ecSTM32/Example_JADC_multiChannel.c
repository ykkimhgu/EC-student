/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-25-2026
Modified         : 10-25-2026
Language/ver     : C in VS Code

Description      : Example - ecADC2 API (docs/ecADC2.md Quick Start)
                   PB_0/PB_1, injected-group 2-channel sequence, hardware
                   TIM2-triggered. JEOC fires once per whole sequence, not
                   once per channel.
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecTIM2.h"
#include "ecADC2.h"

PinName_t seqCHn[2] = {PB_0, PB_1};
volatile uint32_t value1 = 0, value2 = 0;

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    JADC_init(PB_0);
    JADC_init(PB_1);
    JADC_sequence(seqCHn, 2);
    JADC_trigger_init(TIM2, 1, ADC_RISE);   // TIM2 TRGO every 1 ms - also arms JEOC
}

int main(void){
    setup();
    while(1){}
}

void ADC_IRQHandler(void){        // JEOC shares ADC_IRQn with the regular group's EOC
    if (is_ADC_JEOC()){
        value1 = JADC_read(1);
        value2 = JADC_read(2);
        clear_ADC_JEOC();          // reading JDRx does NOT clear JEOC
    }
}

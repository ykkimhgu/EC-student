/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-24-2026
Modified         : 10-24-2026
Language/ver     : C in VS Code

Description      : Example - ecTIM2 API (docs/ecTIM2.md Quick Start)
                   LD2 toggles every 1 second via a 1 ms periodic TIM2
                   update-event interrupt.
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecTIM2.h"

#define LD2_PIN  PA_5
uint32_t _count = 0;

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    GPIO_init(LD2_PIN, OUTPUT);
    TIM_UI_init(TIM2, 1);      // Update-event interrupt every 1 ms
}

int main(void){
    setup();
    while(1){}
}

void TIM2_IRQHandler(void){
    if (is_UIF(TIM2)){
        _count++;
        if (_count > 1000){ led_toggle(LD2_PIN); _count = 0; }
        clear_UIF(TIM2);
    }
}

/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-24-2026
Modified         : 10-24-2026
Language/ver     : C in VS Code

Description      : Example - ecPWM2 API (docs/ecPWM2.md Quick Start)
                   100 Hz PWM on the Nucleo's on-board LED (PA_5, TIM2_CH1),
                   duty sweeping 0% -> 80% every second.
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecTIM2.h"
#include "ecPWM2.h"

#define PWM_LED  PA_5   // TIM2_CH1

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    PWM_init(PWM_LED, 10);        // 10 ms period -> 100 Hz
    PWM_duty(PWM_LED, 0.2f);      // 20% duty
}

int main(void){
    setup();
    while(1){
        for (int i = 0; i < 5; i++){
            PWM_duty(PWM_LED, 0.2f * i);
            delay_ms(1000);
        }
    }
}

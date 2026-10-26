/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-24-2026
Modified         : 10-24-2026
Language/ver     : C in VS Code

Description      : Example - ecSysTick2 API (docs/ecSysTick2.md Quick Start)
                   LED blinks at 1 Hz using delay_ms().
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"

#define LED_JK  PB_12

void setup(void){
    RCC_PLL_init();        // SYSCLK first - SysTick_init() reads EC_SYSCLK
    SysTick_init();
    GPIO_init(LED_JK, OUTPUT);
}

int main(void){
    setup();
    while(1){
        GPIO_write(LED_JK, HIGH);
        delay_ms(1000);
        GPIO_write(LED_JK, LOW);
        delay_ms(1000);
    }
}

/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-24-2026
Modified         : 10-24-2026
Language/ver     : C in VS Code

Description      : Example - ecRCC2 API (docs/ecRCC2.md Quick Start)
                   Blink an LED with the HSI clock. Swap RCC_HSI_init() for
                   RCC_PLL_init() to run at 84 MHz instead - nothing else
                   needs to change, since delay_ms_HSI() scales off EC_SYSCLK.
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN  PA_5

void setup(void){
    RCC_HSI_init();              // SYSCLK = 16 MHz, sets EC_SYSCLK
    // RCC_PLL_init();           // or: SYSCLK = 84 MHz, sets EC_SYSCLK
    GPIO_init(LED_PIN, OUTPUT);
}

int main(void){
    setup();
    while(1){
        GPIO_write(LED_PIN, HIGH);
        delay_ms_HSI(500);
        GPIO_write(LED_PIN, LOW);
        delay_ms_HSI(500);
    }
}

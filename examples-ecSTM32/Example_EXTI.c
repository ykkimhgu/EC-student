/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-24-2026
Modified         : 10-24-2026
Language/ver     : C in VS Code

Description      : Example - ecEXTI2 API (docs/ecEXTI2.md Quick Start)
                   SW2_PIN press toggles LED0_PIN via interrupt, while the
                   main loop blinks LED7_PIN on its own.
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecEXTI2.h"

#define LED0_PIN  PB_12
#define LED7_PIN  PC_3
#define SW2_PIN   PA_4

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    GPIO_init(LED0_PIN, OUTPUT);
    GPIO_init(LED7_PIN, OUTPUT);
    GPIO_init(SW2_PIN, INPUT);
    GPIO_pupd(SW2_PIN, EC_PU);
    EXTI_init(SW2_PIN, FALL, 10);     // priority 10
}

int main(void){
    setup();
    while (1){
        GPIO_write(LED7_PIN, HIGH);
        delay_ms(1000);
        GPIO_write(LED7_PIN, LOW);
        delay_ms(1000);
    }
}

void EXTI4_IRQHandler(void){          // SW2_PIN = PA_4 -> line 4
    if (is_pending_EXTI(SW2_PIN)){
        led_toggle(LED0_PIN);
        clear_pending_EXTI(SW2_PIN);
    }
}

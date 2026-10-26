/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-24-2026
Modified         : 10-24-2026
Language/ver     : C in VS Code

Description      : Example - ecStepper3 API (docs/ecStepper3.md Quick Start)
                   One revolution clockwise, then counter-clockwise,
                   stoppable by a button interrupt (Nucleo B1, PC_13 -
                   onboard pull-up, no GPIO_pupd() needed).
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"
#include "ecStepper3.h"

#define BUTTON_PIN  PC_13
PinName_t pinStepper[4] = {PB_10, PB_4, PB_5, PB_3};   // A, B, A', B'

void setup(void){
    RCC_PLL_init();
    SysTick_init();

    GPIO_init(BUTTON_PIN, INPUT);
    EXTI_init(BUTTON_PIN, FALL, 0);

    stepper_init(FULL, pinStepper);
    stepper_speed(2);        // 2 rpm
}

int main(void){
    setup();
    uint32_t steps = STEP_PER_REV * 1;   // 1 revolution
    while(1){
        stepper_step(steps, DIR_CW);
        delay_ms(3000);
        stepper_step(steps, DIR_CCW);
        delay_ms(3000);
    }
}

void EXTI15_10_IRQHandler(void){
    if (is_pending_EXTI(BUTTON_PIN)){
        stepper_stop();
        clear_pending_EXTI(BUTTON_PIN);
    }
}

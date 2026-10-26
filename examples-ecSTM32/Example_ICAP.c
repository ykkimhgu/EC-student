/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-24-2026
Modified         : 10-24-2026
Language/ver     : C in VS Code

Description      : Example - ecICAP2 API (docs/ecICAP2.md Quick Start)
                   HC-SR04 ultrasonic sensor. TRIG pulse via ecPWM2; the
                   echo pulse is captured on BOTH edges of the SAME pin
                   (IC1 = rising, IC2 = falling, same TI1 signal).
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecTIM2.h"
#include "ecPWM2.h"
#include "ecICAP2.h"

#define TRIG  PA_6   // TIM3_CH1
#define ECHO  PB_6   // TIM4_CH1

volatile uint32_t ovf_cnt = 0;
volatile float time1 = 0, time2 = 0;
volatile float echo_ticks = 0;

void setup(void){
    RCC_PLL_init();
    SysTick_init();

    // TRIG : 10 usec HIGH pulse every 50 msec
    PWM_init(TRIG, 50);
    PWM_pulsewidth_us(TRIG, 10);

    // ECHO : capture both edges of the same pin.
    // ICAP_tick() must be called LAST - it resets PSC/ARR for the whole timer.
    ICAP_init(ECHO, IC_RISE);               // TI1 -> IC1, rising edge
    ICAP_ICn_init(ECHO, IC_FALL, IC_2);     // TI1 -> IC2, falling edge, SAME pin
    ICAP_tick(ECHO, 10);                    // one tick = 10 usec
}

int main(void){
    setup();
    while(1){}
}

void TIM4_IRQHandler(void){
    if (is_UIF(TIM4)){ ovf_cnt++; clear_UIF(TIM4); }

    if (is_CCIF(TIM4, IC_1)){
        time1 = ICAP_read(ECHO, IC_1);           // rising edge timestamp
        clear_CCIF(TIM4, IC_1);
    }
    else if (is_CCIF(TIM4, IC_2)){
        time2 = ICAP_read(ECHO, IC_2);           // falling edge timestamp
        // overflow-corrected tick count between the two edges (10 usec/tick):
        echo_ticks = (time2 - time1) + ovf_cnt * ((float)(TIM4->ARR) + 1);
        ovf_cnt = 0;
        clear_CCIF(TIM4, IC_2);
    }
}

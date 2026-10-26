/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-25-2026
Modified         : 10-25-2026
Language/ver     : C in VS Code

Description      : Code template - one setup() block per peripheral, for
                   copy-paste reference (see docs/ecSTM32-template.md). This
                   is NOT a working sketch: several blocks below configure
                   the SAME timer for different purposes (e.g. TIM4 - the
                   Timer-Interrupt block and the Ultrasonic/ICAP block both
                   claim it; TIM2 - the free-running, periodic-interrupt, and
                   Injected-ADC-trigger blocks all claim it), so keep only
                   the blocks your own sketch needs.
                   It builds clean (valid syntax, every call is real) so
                   that copy-pasting a block never hands you a typo.
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecEXTI2.h"
#include "ecTIM2.h"
#include "ecPWM2.h"
#include "ecStepper3.h"
#include "ecICAP2.h"
#include "ecADC2.h"
#include "ecUART2.h"

#define LED_PIN      PA_5
#define BUTTON_PIN   PC_13   // EXTI15_10 (shared line 10-15)
#define BUTTON2_PIN  PA_0    // EXTI0 (own dedicated line)
#define TRIG         PA_6    // TIM3_CH1
#define ECHO         PB_6    // TIM4_CH1

PinName_t pinStepper[4] = {PB_10, PB_4, PB_5, PB_3};   // A, B, A', B'
PinName_t seqCH_ADC[2]  = {PB_0, PB_1};                // regular group sequence
PinName_t seqCH_JADC[2] = {PB_0, PB_1};                // injected group sequence

void setup(void){
    // RCC Configuration (PLL: 84 MHz)
    RCC_PLL_init();

    // SysTick Configuration (delay_ms) - SysTick_Handler() is already
    // provided by ecSysTick2.c; do NOT define your own, or the link fails
    // with "multiple definition of SysTick_Handler"
    SysTick_init();

    // GPIO Configuration
    GPIO_init(LED_PIN, OUTPUT);
    GPIO_otype(LED_PIN, EC_PUSH_PULL);
    GPIO_ospeed(LED_PIN, EC_HIGH);
    GPIO_write(LED_PIN, LOW);

    // External Interrupt Configuration (EXTI15_10 - shared with pins 10~15)
    GPIO_init(BUTTON_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, EC_PD);
    EXTI_init(BUTTON_PIN, FALL, 0);

    // External Interrupt Configuration (EXTI0 - pin 0 only, own line)
    GPIO_init(BUTTON2_PIN, INPUT);
    GPIO_pupd(BUTTON2_PIN, EC_PD);
    EXTI_init(BUTTON2_PIN, FALL, 1);

    // Timer Configuration (free-running, no interrupt)
    TIM_init(TIM2, 1);

    // Timer IRQ Interrupt Configuration (periodic update event)
    TIM_UI_init(TIM4, 1);

    // Timer IRQ Interrupt Configuration (second timer - TIM2)
    TIM_UI_init(TIM2, 1);

    // PWM Configuration
    PWM_init(PA_1, 1);
    PWM_duty(PA_1, 0.f);

    // Stepper Motor Configuration
    stepper_init(FULL, pinStepper);
    stepper_speed(5);

    // Ultrasonic Configuration
    // Trigger: PWM
    PWM_init(TRIG, 50);
    PWM_pulsewidth_us(TRIG, 10);
    // Echo: Input Capture, both edges of the same pin. ICAP_tick() must be
    // called LAST - it resets PSC/ARR for the whole timer.
    ICAP_init(ECHO, IC_RISE);
    ICAP_ICn_init(ECHO, IC_FALL, IC_2);
    ICAP_tick(ECHO, 10);

    // ADC Configuration (regular group, hardware-triggered)
    ADC_init(PB_0);
    ADC_init(PB_1);
    ADC_sequence(seqCH_ADC, 2);
    ADC_trigger_init(TIM3, 1, ADC_RISE);

    // Injected ADC Configuration
    JADC_init(PB_0);
    JADC_init(PB_1);
    JADC_sequence(seqCH_JADC, 2);
    JADC_trigger_init(TIM2, 1, ADC_RISE);

    // UART Configuration
    USART1_init();
    USART1_baud(BAUD_9600);
    USART2_init();
    USART2_baud(BAUD_38400);
}

// Main - Polling
int main(void){
    setup();
    while(1){
        // polling logic goes here
    }
}


// For interrupts:
// 1. Set the priority of each interrupt handler you actually use.
// 2. Keep every handler as short as possible.
// 3. For a periodic interrupt, check the handler's worst-case run time
//    against the interrupt period - it must never run longer than that.

void TIM4_IRQHandler(void){        // TIM1 ~ TIM8
    if (is_UIF(TIM4)){
        // periodic task: sensor read, controller output, etc.
        // keep it as short as possible

        clear_UIF(TIM4);
    }

    if (is_CCIF(TIM4, IC_1)){
        // capture value
        // uint32_t t = ICAP_read(ECHO, IC_1);

        clear_CCIF(TIM4, IC_1);
    }
}

void EXTI15_10_IRQHandler(void){   // shared by pins 10~15
    if (is_pending_EXTI(BUTTON_PIN)){
        // your handling code here

        clear_pending_EXTI(BUTTON_PIN);
    }
}

void EXTI0_IRQHandler(void){       // pin 0 only - its own dedicated line
    if (is_pending_EXTI(BUTTON2_PIN)){
        // your handling code here

        clear_pending_EXTI(BUTTON2_PIN);
    }
}

void TIM2_IRQHandler(void){
    if (is_UIF(TIM2)){
        // periodic task - keep it as short as possible

        clear_UIF(TIM2);
    }
}

void ADC_IRQHandler(void){
    // ADC overflow flag
    if (is_ADC_OVR()){
        clear_ADC_OVR();
    }

    // regular group: sequence finished
    if (is_ADC_EOC()){
        // periodic ADC acquisition - sampling rate set by the trigger timer
    }

    // injected group: sequence finished
    if (is_ADC_JEOC()){
        // periodic JADC acquisition - sampling rate set by the trigger timer

        clear_ADC_JEOC();
    }
}

void USART1_IRQHandler(void){      // USART1 ~ USART2
    if (is_USART1_RXNE()){
        // called whenever data is received
    }
}

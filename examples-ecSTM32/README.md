---
description: One runnable example per topic, in the same order as the lab course
---

# Example Code

Every example below is a real, build-verified file in this folder —
`pio run -e <env>` builds it standalone against the current `include/`
library. Topics are ordered the same way as the course site's
[Sample code](https://ykkim.gitbook.io/ec/ec-course/example-code) page, so the
two are easy to cross-reference. Unlike that page, this one shows only the
current library's code, not a multi-year comparison.

For the full function-by-function API, see the matching page in
[docs/](../docs/README.md) (linked under each heading below). For the same
topics written in plain Arduino/STM32duino style, for comparison, see
[examples-stm32duino/](../examples-stm32duino/README.md). For every
peripheral's setup() calls in one copy-paste reference file, see
[`Example_Template_main.c`](Example_Template_main.c) /
[docs/ecSTM32-template.md](../docs/ecSTM32-template.md).

Written by: Young-Keun Kim
Course: HGU Embedded Controller
MCU: STM32F411RE (Nucleo-64)

---

## Table of Contents

- [GPIO](#gpio)
  - [Blinking LED](#blinking-led)
  - [LED with Button](#led-with-button)
  - [Seven Segment](#seven-segment)
- [EXTI Interrupt](#exti-interrupt)
- [SysTick Interrupt](#systick-interrupt)
- [Timer Interrupt](#timer-interrupt)
- [PWM](#pwm)
- [Stepper Motor](#stepper-motor)
- [Timer Input Capture: Ultrasonic Distance Sensor](#timer-input-capture-ultrasonic-distance-sensor)
- [ADC](#adc)
  - [Polling](#polling)
  - [Hardware-Triggered + Interrupt](#hardware-triggered--interrupt)
- [JADC — Injected Group, Multi-Channel](#jadc--injected-group-multi-channel)
- [UART](#uart)
  - [USART2 Only](#usart2-only)
  - [USART1 + USART2 Simultaneously](#usart1--usart2-simultaneously)

---

## GPIO

### Blinking LED

[`ecRCC2`](../docs/ecRCC2.md) · [`Example_RCC.c`](Example_RCC.c)

```c
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
```

### LED with Button

[`ecGPIO2`](../docs/ecGPIO2.md) · [`Example_GPIO.c`](Example_GPIO.c)

```c
#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN     PB_12
#define BUTTON_PIN  PA_4

void setup(void){
    RCC_HSI_init();
    GPIO_init(BUTTON_PIN, INPUT);   // enables RCC for BUTTON_PIN's port
    GPIO_init(LED_PIN, OUTPUT);     // enables RCC for LED_PIN's port
}

int main(void){
    setup();
    while(1){
        if (GPIO_read(BUTTON_PIN) == 0)  GPIO_write(LED_PIN, HIGH);
        else                              GPIO_write(LED_PIN, LOW);
    }
}
```

### Seven Segment

[`ecFND`](../docs/ecFND.md) · [`Example_FND.c`](Example_FND.c)

```c
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"

PinName_t pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  // a..g, dp
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};                        // digit 0..3

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    FND_init(pinFND);
    FND_select_init(selectFND);
}

int main(void){
    setup();
    while (1){
        FND_select(0);  FND_display(1);   // digit 0 shows "1"
        delay_ms(1);
        FND_select(1);  FND_display(8);   // digit 1 shows "8"
        delay_ms(1);
    }
}
```

---

## EXTI Interrupt

[`ecEXTI2`](../docs/ecEXTI2.md) · [`Example_EXTI.c`](Example_EXTI.c)

```c
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
```

---

## SysTick Interrupt

[`ecSysTick2`](../docs/ecSysTick2.md) · [`Example_SysTick.c`](Example_SysTick.c)

```c
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
```

---

## Timer Interrupt

[`ecTIM2`](../docs/ecTIM2.md) · [`Example_TIM.c`](Example_TIM.c)

```c
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
```

---

## PWM

[`ecPWM2`](../docs/ecPWM2.md) · [`Example_PWM.c`](Example_PWM.c)

```c
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
```

---

## Stepper Motor

[`ecStepper3`](../docs/ecStepper3.md) · [`Example_Stepper.c`](Example_Stepper.c)

```c
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
```

> `ecStepper3`'s functions are lowercase (`stepper_init`, `stepper_step`, ...)
> — a deliberate naming change from the older, superseded `ecStepper2`, not
> an inconsistency.

---

## Timer Input Capture: Ultrasonic Distance Sensor

[`ecICAP2`](../docs/ecICAP2.md) · [`Example_ICAP.c`](Example_ICAP.c)

```c
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
```

---

## ADC

[`ecADC2`](../docs/ecADC2.md)

### Polling

[`Example_ADC_Poll.c`](Example_ADC_Poll.c)

```c
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
```

### Hardware-Triggered + Interrupt

[`Example_ADC.c`](Example_ADC.c)

```c
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecTIM2.h"
#include "ecADC2.h"

volatile uint32_t value = 0;

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    ADC_init(PB_0);
    ADC_trigger_init(TIM3, 1, ADC_RISE);     // TIM3 TRGO every 1 ms - also arms the EOC interrupt
}

int main(void){
    setup();
    while(1){}
}

void ADC_IRQHandler(void){
    if (is_ADC_EOC()) value = ADC_read();    // reading DR clears EOC
}
```

---

## JADC — Injected Group, Multi-Channel

[`ecADC2`](../docs/ecADC2.md) · [`Example_JADC_multiChannel.c`](Example_JADC_multiChannel.c)

```c
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
```

---

## UART

[`ecUART2`](../docs/ecUART2.md)

### USART2 Only

[`Example_UART.c`](Example_UART.c)

```c
#include <stdio.h>
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecUART2.h"

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    USART2_init();          // PA_2=TX, PA_3=RX, 9600 baud
}

int main(void){
    setup();
    printf("Hello Nucleo\r\n");          // retargeted to USART2
    uint8_t msg[] = "Hello Handong\r\n";
    while(1){
        USART2_write(msg, sizeof(msg));
        delay_ms(5000);
    }
}

void USART2_IRQHandler(void){
    if (is_USART2_RXNE()){
        uint8_t c = USART2_read();
        USART2_write(&c, 1);            // echo
    }
}
```

### USART1 + USART2 Simultaneously

[`Example_UART_Dual.c`](Example_UART_Dual.c)

```c
#include <stdio.h>
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecUART2.h"

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    USART2_init();          // PA_2=TX, PA_3=RX, 9600 baud - printf()/_write() target
    USART1_init();          // PA_9=TX, PA_10=RX, 9600 baud
}

int main(void){
    setup();
    printf("MCU Initialized\r\n");          // retargeted to USART2
    while(1){
        USART2_write((uint8_t*)"Loop:\r\n", 7);
        delay_ms(2000);
    }
}

void USART2_IRQHandler(void){
    if (is_USART2_RXNE()){
        uint8_t c = USART2_read();
        USART2_write(&c, 1);                // echo keyboard typing back to PC
    }
}

void USART1_IRQHandler(void){
    if (is_USART1_RXNE()){
        uint8_t c = USART1_read();
        printf("RX1: %c\r\n", c);           // relay USART1 traffic to the PC terminal
    }
}
```

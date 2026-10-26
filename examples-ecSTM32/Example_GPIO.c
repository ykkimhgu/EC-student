/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-24-2026
Modified         : 10-24-2026
Language/ver     : C in VS Code

Description      : Example - ecGPIO2 API (docs/ecGPIO2.md Quick Start)
                   Button on BUTTON_PIN drives LED_PIN, polled in the main loop.
/----------------------------------------------------------------*/

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

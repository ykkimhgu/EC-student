/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-24-2026
Modified         : 10-24-2026
Language/ver     : C in VS Code

Description      : Example - ecUART2 API (docs/ecUART2.md Quick Start)
                   printf()/_write() are retargeted to USART2; the main loop
                   sends a message every 5 sec, and USART2_IRQHandler()
                   echoes back anything received.
/----------------------------------------------------------------*/

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

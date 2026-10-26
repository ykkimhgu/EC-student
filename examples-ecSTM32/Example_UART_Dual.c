/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-25-2026
Modified         : 10-25-2026
Language/ver     : C in VS Code

Description      : Example - ecUART2 API (docs/ecUART2.md Quick Start)
                   USART1 (PA_9/PA_10, e.g. a Bluetooth module) and USART2
                   (PA_2/PA_3, ST-Link VCP) running at the same time: USART2
                   echoes keyboard input back to the PC, and USART1 relays
                   whatever it receives to the PC terminal via printf().
/----------------------------------------------------------------*/

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

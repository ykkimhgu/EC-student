/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-24-2026
Modified         : 10-24-2026
Language/ver     : C in VS Code

Description      : Example - ecFND API (docs/ecFND.md Quick Start)
                   4-digit 7-segment module, multiplexed by hand: digit 0
                   shows "1", digit 1 shows "8".
/----------------------------------------------------------------*/

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

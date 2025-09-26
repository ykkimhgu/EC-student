/**
******************************************************************************
* @author  IILAB
* @Mod	   2025-9-25 by YKKIM
* @brief   Embedded Controller:  Tutorial ____________________________________
*
******************************************************************************
*/


#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"

volatile uint32_t msTicks = 0;

void setup(void);

int main(void) {
	
// System CLOCK, GPIO Initialiization ----------------------------------------
	setup();

// While loop ------------------------------------------------------				

	while(1){
        GPIO_write(PB_12, HIGH);        
        delay_ms (1000);
        GPIO_write(PB_12, LOW);        
        delay_ms (1000);        
	}
}



void setup(void)
{
	RCC_PLL_init();              // System Clock = 84MHz	
	//GPIO_init(PA_5, OUTPUT);     // LED for Nucleo
   	GPIO_init(PB_12, OUTPUT);  // LED for Eval Board	JKIT
    SysTick_init(); 
}

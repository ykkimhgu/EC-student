/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2024-8-23 by YKKIM
* @brief   Embedded Controller:  Tutorial - DC motor (test)
*
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"
#include "ecUART2.h"
#include "ecTIM2.h"
#include "ecPWM2.h"

#define END_CHAR 13
#define A 0
#define B 1

uint8_t pcData = 0;
uint8_t mcu2Data = 0;

void setup(void);

PinName_t dcPwmPin[2] = {
	PC_9,	// TIM3 Ch3
	PC_8	// TIM3 Ch4
};

PWM_t dcPwm[2];

PinName_t dcDirPin[2] = {
	PB_8,
	PC_6
};

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello Nucleo\r\n");
	
	// Inifinite Loop ----------------------------------------------------------
	while (1){
		
		
	}
}

// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	
	PWM_init(dcPwmPin[A]);
	PWM_init(dcPwmPin[B]);
	
	PWM_period_us(dcPwmPin[A], 100);
	PWM_period_us(dcPwmPin[B], 100);
	
	PWM_duty(dcPwmPin[A], 0.5);
	PWM_duty(dcPwmPin[B], 0.5);
	
	for (int i = 0; i < 2; i++){
		GPIO_init(dcPwmPin[i], EC_DOUT);
		GPIO_pupd(dcPwmPin[i], EC_PD);
		GPIO_otype(dcPwmPin[i], EC_PUSH_PULL);
		GPIO_ospeed(dcPwmPin[i], EC_HIGH);
	}
	
	GPIO_write(dcPwmPin[A], LOW);
	GPIO_write(dcPwmPin[B], HIGH);
}


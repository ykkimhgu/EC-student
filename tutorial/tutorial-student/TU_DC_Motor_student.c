/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM
* @brief   Embedded Controller:  Tutorial - DC motor (test)
*
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecUART.h"
#include "ecTIM.h"
#include "ecPWM.h"

#define END_CHAR 13
#define A 0
#define B 1

uint8_t pcData = 0;
uint8_t mcu2Data = 0;

void setup(void);

_Pin dcPwmPin[2] = {
	{GPIOC, 9}, // TIM3 Ch3
	{GPIOC, 8}	// TIM3 Ch4
};

PWM_t dcPwm[2];

_Pin dcDirPin[2] = {
	{GPIOB, 8}, {GPIOC, 6}	
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
	
	PWM_init(&dcPwm[A], dcPwmPin[A].port, dcPwmPin[A].pin);
	PWM_init(&dcPwm[B], dcPwmPin[B].port, dcPwmPin[B].pin);
	
	PWM_period_us(&dcPwm[A], 100);
	PWM_period_us(&dcPwm[B], 100);
	
	PWM_duty(&dcPwm[A], 0.5);
	PWM_duty(&dcPwm[B], 0.5);
	
	for (int i = 0; i < 2; i++){
		GPIO_init(dcDirPin[i].port, dcDirPin[i].pin, EC_DOUT);
		GPIO_pupd(dcDirPin[i].port, dcDirPin[i].pin, EC_PD);
		GPIO_otype(dcDirPin[i].port, dcDirPin[i].pin, EC_PUSH_PULL);
		GPIO_ospeed(dcDirPin[i].port, dcDirPin[i].pin, EC_HIGH);
	}
	
	GPIO_write(dcDirPin[A].port, dcDirPin[A].pin, LOW);
	GPIO_write(dcDirPin[B].port, dcDirPin[B].pin, HIGH);
}


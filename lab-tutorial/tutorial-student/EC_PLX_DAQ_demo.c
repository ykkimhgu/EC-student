/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM
* @brief   Embedded Controller:  USART communication
*
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecUART.h"
#include "ecSysTick.h"
#include "String.h"


// USART2 : MCU to PC via usb 

uint8_t buf1[4];
uint8_t buf2[4];
uint8_t buf3[4];
uint8_t buf4[4];



uint32_t reflect=0, sound=0, lux=0, dist = 0;
int flag = 0;
int i =0;

void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello Nucleo\r\n");
	
	//USART1 excel_DAQ initialize 
	USART_write(USART2,(unsigned char*) "CLEARSHEET\r\n",12);	
	USART_write(USART2,(unsigned char*) "LABEL,Date,Time,Timer,Sound,Light,Dist\r\n",40);	
	
	// Inifinite Loop ----------------------------------------------------------
	while (1){
		
		lux = GPIO_read(GPIOA,0);
		sound++;
		dist++;
		
		sprintf(buf1, "%d", sound);
		sprintf(buf2, "%d", lux);
		sprintf(buf3, "%d", dist);
		
		//USART1 Trasnmit sensor value to server 

		USART_write(USART2,(unsigned char*) "DATA,DATE,TIME,TIMER,",21);	// transmit char to USART6
		USART_write(USART2,&buf1,4);
		USART_write(USART2,(unsigned char*) ",",1);	// transmit char to USART6
		USART_write(USART2,&buf2,4);
		USART_write(USART2,(unsigned char*) ",",1);	// transmit char to USART6
		USART_write(USART2,&buf3,4);
		USART_write(USART2,(unsigned char*) ",AUTOSCROLL_20\r\n",16);	// transmit char to USART6
		
		
		delay_ms(1000);
	}
}

// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	SysTick_init();
	
	// USART congfiguration
	USART_init(USART2, 9600);
	
	// GPIO configuration
	LED_init();
	GPIO_init(GPIOA,0,INPUT);
}


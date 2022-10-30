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
// USART1 : MCU to MCU2

#define END_CHAR 	13
#define MAX_BUF 	10

// char data
uint8_t mcu2Data = 0;
uint8_t pcData = 0;

uint8_t buffer[MAX_BUF]={0, };
int idx = 0;

int bReceive =0;
int ledOn = 0;


void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello Nucleo\r\n");
	
	// Inifinite Loop ----------------------------------------------------------
	while (1){
		
		if (bReceive == 1){
			printf("buffer: %s\r\n", buffer);
			if (buffer[0] == ___ ){
				if 			(buffer[1] == ___ ) ledOn = 0;		
				else if (buffer[1] == ___ ) ledOn = 1;
				else 										 printf("ERROR : Wrong command\r\n");
			}
			else
				printf("ERROR : Wrong command\r\n");
			
			bReceive = ___;
			memset(buffer, 0, sizeof(char) * MAX_BUF);
		}
				
		GPIO_write(GPIOA, 5, ledOn);
		delay_ms(500);
	}
}

// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	SysTick_init();
	
	// USART congfiguration
	USART_init(USART2, 38400);
	USART_begin(USART1, _____, ___, _____, ___, 9600); 	// PA9 - RXD , PA10 - TXD
	
	// GPIO configuration
	LED_init();
}

void USART1_IRQHandler(){		//USART1 INT 
	if(is_USART_RXNE(______)){
		mcu2Data = USART_getc(______);						
		if(mcu2Data == END_CHAR) {
			bReceive = ___;
			idx = 0;
		}
		else{
			if(idx > MAX_BUF){
				idx = 0;
				memset(buffer, 0, sizeof(char) * MAX_BUF);
				printf("ERROR : Too long string\r\n");
			}
			buffer[idx] = ______;
			idx++;
		}
	}
}

void USART2_IRQHandler(){		//USART2 INT 
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(______);
		USART_write(______, ______, ___);	// transmit char to USART1
		
		printf("%c", pcData); 						// echo to sender(pc)
		
		if(pcData == END_CHAR){
			printf("\r\n"); 							// to change line on PC display
		}
	}
}

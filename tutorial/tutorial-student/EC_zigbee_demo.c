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
// USART1 : MCU to MCU2 via zigbee

#define END_CHAR 13
#define MAX_BUF 10

// char data
uint8_t recvChar = 0;
uint8_t pcData = 0;

uint8_t buffer[MAX_BUF] = {0,};
int idx = 0;

int bReceive = 0;
int ledOn = 0;

void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello Nucleo\r\n");

	// Inifinite Loop ----------------------------------------------------------
	while (1){
		
		if(bReceive == 1 && buffer[0] == 'L'){
			printf("buffer : %s\r\n", buffer);
			
			if 			(buffer[1] == '0')	ledOn = 0;		
			else if (buffer[1] == '1') 	ledOn = 1;
			else 												printf("ERROR : Wrong command\r\n");
			
			bReceive = 0;
			memset(buffer, 0, sizeof(char) * MAX_BUF);
		}

		GPIO_write(GPIOA, 5, ledOn);
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
	USART_begin(USART6, GPIOA, 11, GPIOA, 12, 9600); 	// PA11: TXD, PA12: RXD
	
	// GPIO configuration
	LED_init();
}


void USART6_IRQHandler(){		//USART1 INT 
	if(is_USART_RXNE(USART6)){
		recvChar = USART_getc(USART6);
		printf("%c", recvChar); 						// echo to sender(pc)		
		
		if(recvChar == END_CHAR) {
			bReceive = 1;
			idx = 0;
		}
		else{
			if(idx > MAX_BUF){
				idx = 0;
				memset(buffer, 0, sizeof(char) * MAX_BUF);
				printf("ERROR : Too long string\r\n");
			}
			buffer[idx] = recvChar;
			idx++;
		}
	}
}

void USART2_IRQHandler(){		//USART2 INT 
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(USART2);
		USART_write(USART6, &pcData, 1);	// transmit char to USART1
		printf("%c",pcData); 							// echo to sender(pc)
		
		if(pcData == END_CHAR){
			printf("\r\n"); 							// to change line on PC display
		}
	}
}

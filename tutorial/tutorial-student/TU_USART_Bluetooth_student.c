/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM
* @brief   Embedded Controller:  Tutorial - USART communication (Bluetooth)
*
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecUART.h"

#define END_CHAR 13
#define MAX_BUF 100

uint8_t pcData = 0;
uint8_t btData = 0;
uint8_t buffer[MAX_BUF] = {0, };
int bReceive = 0;
int idx = 0;

void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello Nucleo\r\n");
	USART_write(USART1,(uint8_t*) "Hello bluetooth\r\n",17);
	// Inifinite Loop ----------------------------------------------------------
	while (1){
				
	}
			
}

// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	USART_init(USART2, 9600);
	USART_begin(USART1, GPIOA, 9, GPIOA, 10, 9600); 	// PA9: TXD , PA10: RXD
}

void USART1_IRQHandler(){         //USART1 INT 
	if(is_USART_RXNE(USART1)){
		btData = USART_getc(USART1);
		USART_write(USART1,(uint8_t*) "BT sent : ", 10);
		USART_write(USART1, &btData, 1);
		USART_write(USART1, "\r\n", 2);
		printf("NUCLEO got : %c (from BT)\r\n",btData);
	}
}


void USART2_IRQHandler(){         //USART2 INT 
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(USART2);
		USART_write(USART1, &pcData, 1);
		//printf("Nucleo got : %c \r\rn", pcData);
		
		printf("%c", pcData);
		
		if (pcData == END_CHAR)
			printf("\r\n");
		
	}
}
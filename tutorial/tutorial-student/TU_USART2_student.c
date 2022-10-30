/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM
* @brief   Embedded Controller:  Tutorial - USART communication
*
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecUART.h"

#define END_CHAR 13

uint8_t pcData = 0;
uint8_t mcu2Data = 0;

void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello Nucleo\r\n");
	// Inifinite Loop ----------------------------------------------------------
	while (1);
}

// Initialiization 
void setup(void)
{
	RCC_PLL_init();
	USART_init(USART2, 9600);
	USART_begin(USART1, GPIOA, 9, GPIOA, 10, 9600); 	// PA9 - RXD , PA10 - TXD
}

void USART1_IRQHandler(){         //USART1 INT 
	if(is_USART_RXNE(USART1)){
		mcu2Data = USART_getc(USART1);
		printf("received: %c\r\n", mcu2Data);
	}
}


void USART2_IRQHandler(){         //USART2 INT 
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(USART2);
		USART_write(USART1, &pcData, 1);
		printf("%c", pcData);
		
		if (pcData == END_CHAR)
			printf("\r\n");
	}
}
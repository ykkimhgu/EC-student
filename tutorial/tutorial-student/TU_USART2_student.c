/**
******************************************************************************
* @author  SSSLAB
* @Mod     2024-8-23 by Jin Kwak
* @brief   Embedded Controller:  Tutorial - USART communication
*
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"
#include "ecUART2.h"

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
	USART_setting(USART1, PA_9, PA_10, 9600); 	// PA9 - RXD , PA10 - TXD
}

void USART1_IRQHandler(){         //USART1 INT 
	if(is_USART_RXNE(USART1)){
		mcu2Data = fgetc(USART1);
		printf("received: %c\r\n", mcu2Data);
	}
}


void USART2_IRQHandler(){         //USART2 INT 
	if(is_USART_RXNE(USART2)){
		pcData = fgetc(USART2);
		USART_write(USART1, &pcData, 1);
		printf("%c", pcData);
		
		if (pcData == END_CHAR)
			printf("\r\n");
	}
}

/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM
* @brief   Embedded Controller:  USART Communication
*
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecUART.h"

#define END_CHAR 13

uint8_t pcData = 0;

void setup(void);

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("Hello Nucleo\r\n");
	// Inifinite Loop ----------------------------------------------------------
	while (1);
}

// Initialiization 
void setup(void){
	
	RCC_PLL_init();
	UART2_init();									// before complete library
	// USART_init(USART2, 9600);	// after complete library
}


void USART2_IRQHandler(){         //USART2 INT 
	if(is_USART_RXNE(USART2)){
		pcData = USART_getc(USART2);
		printf("%c", pcData);
		//USART_write(USART2, &pcData, 1);
		
		if (pcData == END_CHAR)
			printf("\r\n");
		}
}

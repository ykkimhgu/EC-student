#include "ecUART2.h"
#include <math.h>

// ********************** DO NOT MODIFY HERE ***************************
// 

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)

// Implement a dummy __FILE struct, which is called with the FILE structure.
struct __FILE
{
    int dummy;
    int handle;
};

FILE __stdout;
FILE __stdin;

// Retarget printf() to USART2
int fputc(int ch, FILE* f)
{
    uint8_t c = (uint8_t)ch & 0xFF;
    USART_write(USART2, &c, 1);

    return ch;
}

// Retarget getchar()/scanf() to USART2
int fgetc(FILE* f)
{
    return USART_read(USART2);
}

#elif defined(__GNUC__)

int _write(int file, char* data, int len)
{
    // replace it with your 'send to uart' function
    USART2_write((uint8_t*)data, len);

    return len;
}

int _read(int file, char* data, int len)
{
    return USART2_read();
}

#endif


/*================ private functions ================*/
void USART_write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	// TXE is set by hardware when the content of the TDR
	// register has been transferred into the shift register.
	int i;
	for (i = 0; i < nBytes; i++) {
		// wait until TXE (TX empty) bit is set
		while (!(USARTx->SR & USART_SR_TXE));
		// Writing USART_DR automatically clears the TXE flag
		USARTx->DR = buffer[i] & 0xFF;
		USART_delay(300);
	}
	// wait until TC bit is set
	while (!(USARTx->SR & USART_SR_TC));
	// TC bit clear
	USARTx->SR &= ~USART_SR_TC;
}  
 
uint32_t is_USART_RXNE(USART_TypeDef * USARTx){
	return (USARTx->SR & USART_SR_RXNE);
}


uint8_t USART_read(USART_TypeDef * USARTx){
	// Wait until RXNE (RX not empty) bit is set by HW -->Read to read
	while ((USARTx->SR & USART_SR_RXNE) != USART_SR_RXNE);
	// Reading USART_DR automatically clears the RXNE flag
	return ((uint8_t)(USARTx->DR & 0xFF)); 
}

void USART_setting(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud){
    //0. Port Pin Configuration
    GPIO_TypeDef *GPIO_TX;
    unsigned int pinTX;
    ecPinmap(pin_GPIO_TX, &GPIO_TX, &pinTX);

    GPIO_TypeDef *GPIO_RX;
    unsigned int pinRX;
    ecPinmap(pin_GPIO_RX, &GPIO_RX, &pinRX);
    
    //1. GPIO Pin for TX and RX
	// Enable GPIO peripheral clock
	// Alternative Function mode selection for Pin_y in GPIOx
	// AF, Push-Pull, No PUPD, High Speed
	GPIO_init(pin_GPIO_TX, AF);
	GPIO_otype(pin_GPIO_TX, 0);//PUSH-PULL
	GPIO_pupd(pin_GPIO_TX, 0);//NO PUPD
	GPIO_ospeed(pin_GPIO_TX, EC_HIGH);
	
	GPIO_init(pin_GPIO_RX, AF);
	GPIO_otype(pin_GPIO_RX, 0);
	GPIO_pupd(pin_GPIO_RX, 0);
	GPIO_ospeed(pin_GPIO_RX, EC_HIGH);
	
	// Set Alternative Function Register for USARTx.	
	// AF7 - USART1,2 
	// AF8 - USART6 	 
	if (USARTx == USART6){ 
		// USART_TX GPIO AFR
		if (pinTX < 8) GPIO_TX->AFR[0] |= 8 << (4*pinTX);
		else           GPIO_TX->AFR[1] |= 8 << (4*(pinTX-8));
		// USART_RX GPIO AFR
		if (pinRX < 8) GPIO_RX->AFR[0] |= 8 << (4*pinRX); 
		else 		   GPIO_RX->AFR[1] |= 8 << (4*(pinRX-8));
	}
	else{	//USART1,USART2
		// USART_TX GPIO AFR
		if (pinTX < 8) GPIO_TX->AFR[0] |= 7 << (4*pinTX);
		else 		   GPIO_TX->AFR[1] |= 7 << (4*(pinTX-8));
		// USART_RX GPIO AFR
		if( pinRX < 8) GPIO_RX->AFR[0] |= 7 << (4*pinRX);
		else 		   GPIO_RX->AFR[1] |= 7 << (4*(pinRX-8));
	}

	
//2. USARTx (x=2,1,6) configuration	
	// Enable USART peripheral clock 
	if (USARTx == USART1)
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 	// Enable USART 1 clock (APB2 clock: AHB clock = 84MHz)	
	else if(USARTx == USART2)
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  	// Enable USART 2 clock (APB1 clock: AHB clock/2 = 42MHz)
	else
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;  	// Enable USART 6 clock (APB2 clock: AHB clock = 84MHz)
	
	// Disable USARTx. 
	USARTx->CR1 &= ~USART_CR1_UE; 						// USART disable
	 
	// No Parity / 8-bit word length / Oversampling x16 
	USARTx->CR1&=~(1<<10);   		// No parrity bit
	USARTx->CR1&=~(1<<12);       	// M: 0 = 8 data bits, 1 start bit    
	USARTx->CR1 &= ~USART_CR1_OVER8;  	// 0 = oversampling by 16 (to reduce RF noise)	 
	// Configure Stop bit
	USARTx->CR2&=~(3<<12);  		// 1 stop bit																 

	// CSet Baudrate to 9600 using APB frequency (42MHz)
	// If oversampling by 16, Tx/Rx baud = f_CK / (16*USARTDIV),  
	// If oversampling by 8,  Tx/Rx baud = f_CK / (8*USARTDIV)
	// USARTDIV = 42MHz/(16*9600) = 237.4375

	UART_baud(USARTx, baud);

	// Enable TX, RX, and USARTx 
	USARTx->CR1 |= (USART_CR1_RE | USART_CR1_TE);   	// Transmitter and Receiver enable
	USARTx->CR1 |= USART_CR1_UE; 										// USART enable
	
	
// 3. Read USARTx Data (Interrupt)	
	// Set the priority and enable interrupt
	USARTx->CR1 |= USART_CR1_RXNEIE;       			// Received Data Ready to be Read Interrupt
	if (USARTx == USART1){
		NVIC_SetPriority(USART1_IRQn, 1);      		// Set Priority to 1
		NVIC_EnableIRQ(USART1_IRQn);             	// Enable interrupt of USART2 peripheral
	}
	else if (USARTx == USART2){
		NVIC_SetPriority(USART2_IRQn, 1);      		// Set Priority to 1
		NVIC_EnableIRQ(USART2_IRQn);             	// Enable interrupt of USART2 peripheral
	}
	else {																			// if(USARTx==USART6)
		NVIC_SetPriority(USART6_IRQn, 1);      		// Set Priority to 1
		NVIC_EnableIRQ(USART6_IRQn);            	// Enable interrupt of USART2 peripheral
	}
	USARTx->CR1 |= USART_CR1_UE; 							// USART enable
} 


void UART_baud(USART_TypeDef* USARTx, uint32_t baud){
	// Disable USARTx. 
	USARTx->CR1  &= ~USART_CR1_UE; 						// USART disable
	USARTx->BRR = 0;
	
// Configure Baud-rate 
	float fCK = 84000000;                                    // if(USARTx==USART1 || USARTx==USART6), APB2
	if(USARTx == USART2) fCK =fCK/2;      // APB1

// Method 1
	float USARTDIV = (float) fCK/16.0/baud;
	uint32_t mantissa = (uint32_t)USARTDIV;
	uint32_t fraction = round((USARTDIV-mantissa)*16);
	USARTx->BRR |= ((mantissa<<4)|fraction);
	
	// Enable TX, RX, and USARTx 
	USARTx->CR1 |= USART_CR1_UE;

}

void USART_delay(uint32_t us) {
   uint32_t time = 100*us/7;    
   while(--time);   
}
	

/*================ Use functions ================*/
void UART1_init(void){
	// ********************** USART 1 ***************************
	// PA_9 = USART1_TX (default)	// PB_6  (option)
	// PA_10 = USART1_RX (default)	// PB_3 (option)
	// APB2
	// **********************************************************
	USART_setting(USART1, PA_9, PA_10, 9600);
}
void UART2_init(void){
	// ********************** USART 2 ***************************
	// PA2 = USART2_TX
	// PA3 = USART2_RX
	// Alternate function(AF7), High Speed, Push pull, Pull up
	// **********************************************************
	USART_setting(USART2, PA_2, PA_3, 9600);
}

void UART1_baud(uint32_t baud){
	UART_baud(USART1, baud);
}
void UART2_baud(uint32_t baud){
	UART_baud(USART2, baud);
}

void USART1_write(uint8_t* buffer, uint32_t nBytes){
	USART_write(USART1, buffer, nBytes);
}

void USART2_write(uint8_t* buffer, uint32_t nBytes){
	USART_write(USART2, buffer, nBytes);
}
uint8_t USART1_read(void){
	return USART_read(USART1);
}	

uint8_t USART2_read(void){
	return USART_read(USART2);
}

uint32_t is_USART1_RXNE(void){
	return is_USART_RXNE(USART1);
}
uint32_t is_USART2_RXNE(void){
	return is_USART_RXNE(USART2);
}

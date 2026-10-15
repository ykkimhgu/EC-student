
/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 10-08-2026
Modified         : 10-08-2026
Language/ver     : C in VS Code

Description      : USART2  library
/----------------------------------------------------------------*/

#include "ecUART2_student.h"


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


/*----------------------------------------------------------------\
        EXERCISE: Implement the following functions
/----------------------------------------------------------------*/
/*================ General Configuration ================*/
void USART_init(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud){
//1. GPIO Pin for TX and RX
	USART_pins(USARTx, pin_GPIO_TX, pin_GPIO_RX);

//2. USARTx (x=2,1,6) configuration
	USART_clock_enable(USARTx);

	// Disable USARTx.
	USARTx->CR1 &= ~USART_CR1_UE; 	// USART disable

	// No Parity / 8-bit word length / Oversampling x16 / Stop bit =1
  // [YOUR CODE HERE] 
  // [YOUR CODE HERE] 
  // [YOUR CODE HERE] 

	// Set Baudrate using APB frequency (42MHz)
	// If oversampling by 16, Tx/Rx baud = f_CK / (16*USARTDIV),
	// If oversampling by 8,  Tx/Rx baud = f_CK / (8*USARTDIV)
	USART_baud(USARTx, baud);

	// Enable TX, RX, and USARTx
	USARTx->CR1 |= (USART_CR1_RE | USART_CR1_TE);   	// Transmitter and Receiver enable
	USARTx->CR1 |= USART_CR1_UE; 						          // USART enable

// 3. RX interrupt (data ready to be read)
  // Enable RXNE interrupt
	USARTx->CR1 |= USART_CR1_RXNEIE;
  // USART_IRQn
	uint32_t USART_IRQ = USART2_IRQn;
  // Set the priority and enable interrupt
  // [YOUR CODE HERE] 
  // [YOUR CODE HERE] 
 
	NVIC_SetPriority(USART_IRQ, 1);
	NVIC_EnableIRQ(USART_IRQ);

}


void USART_baud(USART_TypeDef* USARTx, uint32_t baud){
	// Disable USARTx.
	USARTx->CR1  &= ~USART_CR1_UE; 						// USART disable
	USARTx->BRR = 0;

	// Configure Baud-rate
	float fCK = EC_SYSCLK;                                   // if(USARTx==USART1 || USARTx==USART6), APB2
	                                                         // (84MHz after RCC_PLL_init(), 16MHz after RCC_HSI_init())
	if(USARTx == USART2) fCK =fCK/2;      // APB1
  float USARTDIV = (float) fCK/16.0/baud;

	// Method 1 : build the two BRR fields explicitly
	//   BRR[15:4] = DIV_Mantissa (integer part of USARTDIV)
	//   BRR[3:0]  = DIV_Fraction (counts 1/16 steps -> multiply the fraction by 16)
  //[YOUR CODE HERE]
  //[YOUR CODE HERE]
  //[YOUR CODE HERE]
  // USARTx->BRR = _______________ 

  
	// Enable TX, RX, and USARTx
	USARTx->CR1 |= USART_CR1_UE;
}


// GPIO pin setup for USARTx TX/RX (any of USART1, USART2, USART6)
void USART_pins(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX){
	GPIO_TypeDef *GPIO_TX;
	unsigned int pinTX;
	ecPinmap(pin_GPIO_TX, &GPIO_TX, &pinTX);

	GPIO_TypeDef *GPIO_RX;
	unsigned int pinRX;
	ecPinmap(pin_GPIO_RX, &GPIO_RX, &pinRX);

	// Alternate Function, Push-Pull, No Pull-up/down, High speed
	// [YOUR CODE HERE]
  // [YOUR CODE HERE]
  // [YOUR CODE HERE]
  // [YOUR CODE HERE]

	// Set Alternate Function Register for USARTx
	//   AF7 - USART1, USART2
	//   AF8 - USART6
	uint32_t AF_num = (USARTx == USART6) ? 8 : 7;
	if (pinTX < 8) { GPIO_TX->AFR[0] &= ~(0xFUL << (4*pinTX));     GPIO_TX->AFR[0] |= AF_num << (4*pinTX); }
	else           { GPIO_TX->AFR[1] &= ~(0xFUL << (4*(pinTX-8))); GPIO_TX->AFR[1] |= AF_num << (4*(pinTX-8)); }
	
  if (pinRX < 8) {
    // [YOUR CODE HERE] 
    // [YOUR CODE HERE] 
  }
  else{
    // [YOUR CODE HERE] 
    // [YOUR CODE HERE]   
  }

}


void USART1_baud(uint32_t baud){
	USART_baud(USART1, baud);
}
void USART2_baud(uint32_t baud){
	USART_baud(USART2, baud);
}


/*----------------------------------------------------------------\
        PROVIDED: DON'T MODIFY HERE
/----------------------------------------------------------------*/

/*================ Configuration for each USART ================*/
void USART1_init(void){
	// ********************** USART 1 ***************************
	// PA_9 = USART1_TX (default)	// PB_6  (option)
	// PA_10 = USART1_RX (default)	// PB_3 (option)
	// APB2
	// **********************************************************
	//USART_init(USART1, PA_9, PA_10, 9600);

  ////////////////////////////////////////////
  //  for USART1  ONLY
  ////////////////////////////////////////////
  USART_TypeDef *USARTx= USART1;

//1. GPIO Pin for TX and RX
// Default : PA_9 = USART1_TX, PA_10 = USART1_RX
	PinName_t pin_GPIO_TX  =PA_9;
	PinName_t pin_GPIO_RX  =PA_10;

	GPIO_TypeDef *GPIO_TX;
	unsigned int pinTX;
	ecPinmap(pin_GPIO_TX, &GPIO_TX, &pinTX);
	GPIO_TypeDef *GPIO_RX;
	unsigned int pinRX;
	ecPinmap(pin_GPIO_RX, &GPIO_RX, &pinRX);

	// Alternate Function, Push-Pull, No Pull-up/down, High speed
	GPIO_init(pin_GPIO_TX, AF);
	GPIO_otype(pin_GPIO_TX, EC_PUSH_PULL);
	GPIO_pupd(pin_GPIO_TX, EC_NONE);
	GPIO_ospeed(pin_GPIO_TX, EC_HIGH);

	GPIO_init(pin_GPIO_RX, AF);
	GPIO_otype(pin_GPIO_RX, EC_PUSH_PULL);
	GPIO_pupd(pin_GPIO_RX, EC_NONE);
	GPIO_ospeed(pin_GPIO_RX, EC_HIGH);

 	// Set Alternate Function Register for USARTx
	//   AF7 - USART1, USART2
	//   AF8 - USART6
	uint32_t AF_num = (USARTx == USART6) ? 8 : 7;
	if (pinTX < 8) { GPIO_TX->AFR[0] &= ~(0xFUL << (4*pinTX));     GPIO_TX->AFR[0] |= AF_num << (4*pinTX); }
	else           { GPIO_TX->AFR[1] &= ~(0xFUL << (4*(pinTX-8))); GPIO_TX->AFR[1] |= AF_num << (4*(pinTX-8)); }
	if (pinRX < 8) { GPIO_RX->AFR[0] &= ~(0xFUL << (4*pinRX));     GPIO_RX->AFR[0] |= AF_num << (4*pinRX); }
	else           { GPIO_RX->AFR[1] &= ~(0xFUL << (4*(pinRX-8))); GPIO_RX->AFR[1] |= AF_num << (4*(pinRX-8)); }


//2. USARTx (x=2,1,6) configuration
  // Enable USART peripheral clock
  	//      USART2   = APB1
  	//      USART1,6 = APB2
	USART_clock_enable(USARTx);

  // Disable USARTx.
	USARTx->CR1 &= ~USART_CR1_UE;			// disable while configuring

  // No Parity / 8-bit word length / Oversampling x16
	USARTx->CR1 &= ~USART_CR1_M;			// 8 data bits
	USARTx->CR1 &= ~USART_CR1_PCE;			// no parity
	USARTx->CR1 &= ~USART_CR1_OVER8;		// oversampling by 16
	USARTx->CR2 &= ~USART_CR2_STOP;			// 1 stop bit

  // BAUD RATE (Default: 9600)
  //    Baud rate : BRR[15:4]=mantissa, BRR[3:0]=fraction (1/16 steps)
  //    USARTDIV = f_APB1 / (16 x baud)
  //    USART_baud(USARTx, 9600);
  uint32_t baud=  9600;				// Default baud rate for USART2
  float fCK      = (float)EC_SYSCLK / 2;		// APB1 : 84MHz PLL -> 42MHz
  float USARTDIV = fCK / 16 / baud;
  uint32_t MNT   = (uint32_t)USARTDIV;
  uint32_t FRC   = round((USARTDIV - MNT) * 16);
	if (FRC > 15) { MNT += 1; FRC = 0; }		// carry into the mantissa
	USARTx->BRR = (MNT << 4) | FRC;

  // Enable TX, RX
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);	// transmitter + receiver
  // Enable USART
  	USARTx->CR1 |= USART_CR1_UE;

// 3. RX interrupt (data ready to be read)
  // Enable RXNE interrupt
	USARTx->CR1 |= USART_CR1_RXNEIE;
  // USART2 IRQ
	uint32_t USART_IRQ = USART2_IRQn;
	if (USARTx == USART1){
		USART_IRQ = USART1_IRQn;
	}
	NVIC_SetPriority(USART_IRQ, 1);
	NVIC_EnableIRQ(USART_IRQ);
}


void USART2_init(void){
// ********************** USART 2 ***************************
// PA2 = USART2_TX
// PA3 = USART2_RX
// Alternate function(AF7), High Speed, Push pull, Pull up
// **********************************************************
// USART_init(USART2, PA_2, PA_3, 9600);

////////////////////////////////////////////
//  for USART2  ONLY
////////////////////////////////////////////
  USART_TypeDef *USARTx= USART2;

//1. GPIO Pin for TX and RX
// Default : PA_2 = USART2_TX, PA_3 = USART2_RX
	PinName_t pin_GPIO_TX  =PA_2;
	PinName_t pin_GPIO_RX  =PA_3;

	GPIO_TypeDef *GPIO_TX;
	unsigned int pinTX;
	ecPinmap(pin_GPIO_TX, &GPIO_TX, &pinTX);
	GPIO_TypeDef *GPIO_RX;
	unsigned int pinRX;
	ecPinmap(pin_GPIO_RX, &GPIO_RX, &pinRX);

	// Alternate Function, Push-Pull, No Pull-up/down, High speed
	GPIO_init(pin_GPIO_TX, AF);
	GPIO_otype(pin_GPIO_TX, EC_PUSH_PULL);
	GPIO_pupd(pin_GPIO_TX, EC_NONE);
	GPIO_ospeed(pin_GPIO_TX, EC_HIGH);

	GPIO_init(pin_GPIO_RX, AF);
	GPIO_otype(pin_GPIO_RX, EC_PUSH_PULL);
	GPIO_pupd(pin_GPIO_RX, EC_NONE);
	GPIO_ospeed(pin_GPIO_RX, EC_HIGH);

 	// Set Alternate Function Register for USARTx
	//   AF7 - USART1, USART2
	//   AF8 - USART6
	uint32_t AF_num = (USARTx == USART6) ? 8 : 7;
	if (pinTX < 8) { GPIO_TX->AFR[0] &= ~(0xFUL << (4*pinTX));     GPIO_TX->AFR[0] |= AF_num << (4*pinTX); }
	else           { GPIO_TX->AFR[1] &= ~(0xFUL << (4*(pinTX-8))); GPIO_TX->AFR[1] |= AF_num << (4*(pinTX-8)); }
	if (pinRX < 8) { GPIO_RX->AFR[0] &= ~(0xFUL << (4*pinRX));     GPIO_RX->AFR[0] |= AF_num << (4*pinRX); }
	else           { GPIO_RX->AFR[1] &= ~(0xFUL << (4*(pinRX-8))); GPIO_RX->AFR[1] |= AF_num << (4*(pinRX-8)); }


//2. USARTx (x=2,1,6) configuration
  // Enable USART peripheral clock
  	//      USART2   = APB1
  	//      USART1,6 = APB2
	USART_clock_enable(USARTx);

  // Disable USARTx.
	USARTx->CR1 &= ~USART_CR1_UE;			// disable while configuring

  // No Parity / 8-bit word length / Oversampling x16
	USARTx->CR1 &= ~USART_CR1_M;			// 8 data bits
	USARTx->CR1 &= ~USART_CR1_PCE;			// no parity
	USARTx->CR1 &= ~USART_CR1_OVER8;		// oversampling by 16
	USARTx->CR2 &= ~USART_CR2_STOP;			// 1 stop bit

  // BAUD RATE (Default: 9600)
  //    Baud rate : BRR[15:4]=mantissa, BRR[3:0]=fraction (1/16 steps)
  //    USARTDIV = f_APB1 / (16 x baud)
  //    USART_baud(USARTx, 9600);
  uint32_t baud=  9600;				// Default baud rate for USART2
  float fCK      = (float)EC_SYSCLK / 2;		// APB1 : 84MHz PLL -> 42MHz
  float USARTDIV = fCK / 16 / baud;
  uint32_t MNT   = (uint32_t)USARTDIV;
  uint32_t FRC   = round((USARTDIV - MNT) * 16);
	if (FRC > 15) { MNT += 1; FRC = 0; }		// carry into the mantissa
	USARTx->BRR = (MNT << 4) | FRC;

  // Enable TX, RX
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);	// transmitter + receiver
  // Enable USART
  	USARTx->CR1 |= USART_CR1_UE;

// 3. RX interrupt (data ready to be read)
  // Enable RXNE interrupt
	USARTx->CR1 |= USART_CR1_RXNEIE;
  // USART2 IRQ
	uint32_t USART_IRQ = USART2_IRQn;
	if (USARTx == USART1){
		USART_IRQ = USART1_IRQn;
	}
	NVIC_SetPriority(USART_IRQ, 1);
	NVIC_EnableIRQ(USART_IRQ);
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


/*================ General Configuration ================*/
void USART_clock_enable(USART_TypeDef* USARTx){
	if (USARTx == USART1)
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 	// Enable USART 1 clock (APB2 clock: AHB clock = 84MHz)
	else if(USARTx == USART2)
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  	// Enable USART 2 clock (APB1 clock: AHB clock/2 = 42MHz)
	else
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;  	// Enable USART 6 clock (APB2 clock: AHB clock = 84MHz)
}

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

uint8_t USART_read(USART_TypeDef * USARTx){
	// Wait until RXNE (RX not empty) bit is set by HW -->Read to read
	while (is_USART_RXNE(USARTx) == 0);
	// Reading USART_DR automatically clears the RXNE flag
	return ((uint8_t)(USARTx->DR & 0xFF));
}

uint32_t is_USART_RXNE(USART_TypeDef * USARTx){
	return (USARTx->SR & USART_SR_RXNE) != 0;
}

void USART_delay(uint32_t us) {
   uint32_t time = 100*us/7;
   while(--time);
}


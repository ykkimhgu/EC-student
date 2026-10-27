/*----------------------------------------------------------------
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-01-2026
Modified         : 10-22-2026
Language/ver     : C in VS Code

Description      : I2C Library. i2c-LCD display control included
----------------------------------------------------------------*/

#include "ecI2C2.h"
#include "ecSysTick2.h"

#define I2C_SCL_HZ		100000UL		// standard mode
#define I2C_TIMEOUT		100000UL		// busy-wait budget per flag


// ========================================================
// Internal helpers
// ========================================================

// PCLK1 = HCLK / PPRE1. Both RCC_PLL_init() and RCC_HSI_init() leave the AHB
// prescaler at /1, so HCLK == EC_SYSCLK.
// RM0383 RCC_CFGR PPRE1 : 0xx = /1, 100 = /2, 101 = /4, 110 = /8, 111 = /16
static uint32_t I2C_pclk1(void){
	uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos;
	uint32_t div   = (ppre1 < 4) ? 1UL : (2UL << (ppre1 - 4UL));
	return (uint32_t)EC_SYSCLK / div;
}

// SCL and SDA are set up together, the way ecUART2.c handles TX/RX, so the AF
// number comes from the bus instead of the caller.
// AFR is 4 bits per pin : clear the nibble first, or a pin reused from another
// peripheral ends up with the two AF numbers OR-ed together.
static void I2C_pin_init(I2C_TypeDef *I2Cx, PinName_t pinSCL, PinName_t pinSDA){
	GPIO_TypeDef *port;
	unsigned int  pin;
	uint32_t      afrIdx, afrShift;

	// Every SCL pin of this package is AF4. SDA is AF4 as well, except the pins
	// that only reach a bus through AF9 : PB_3 (I2C2), PB_4 / PB_8 (I2C3), and
	// PB_9, which is AF4 for I2C1 but AF9 for I2C2.
	uint32_t afSDA = 4;
	if(pinSDA == PB_3 || pinSDA == PB_4 || pinSDA == PB_8)	afSDA = 9;
	else if(pinSDA == PB_9 && I2Cx == I2C2)					afSDA = 9;

	// ---- SCL
	ecPinmap(pinSCL, &port, &pin);
	GPIO_init(pinSCL, AF);
	GPIO_otype(pinSCL, EC_OPEN_DRAIN);
	GPIO_pupd(pinSCL, EC_NONE);
	GPIO_ospeed(pinSCL, EC_HIGH);

	afrIdx   = pin >> 3;						// AFR[0] holds pin 0~7, AFR[1] 8~15
	afrShift = 4 * (pin % 8);
	port->AFR[afrIdx] &= ~(0xFUL << afrShift);	// 4 bits per pin
	port->AFR[afrIdx] |=  (4UL   << afrShift);

	// ---- SDA
	ecPinmap(pinSDA, &port, &pin);
	GPIO_init(pinSDA, AF);
	GPIO_otype(pinSDA, EC_OPEN_DRAIN);
	GPIO_pupd(pinSDA, EC_NONE);
	GPIO_ospeed(pinSDA, EC_HIGH);

	afrIdx   = pin >> 3;
	afrShift = 4 * (pin % 8);
	port->AFR[afrIdx] &= ~(0xFUL  << afrShift);
	port->AFR[afrIdx] |=  (afSDA  << afrShift);
}

// The APB1 clock has to be on before any CR/CCR/TRISE write, so it is enabled
// here instead of by the caller. The GPIO port clocks need no such line :
// GPIO_init() already enables whichever port a pin belongs to.
static void I2C_timing_init(I2C_TypeDef *I2Cx){
	if     (I2Cx == I2C1)	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	else if(I2Cx == I2C2)	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	else if(I2Cx == I2C3)	RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;

	uint32_t pclk1 = I2C_pclk1();
	uint32_t freq  = pclk1 / 1000000UL;				// CR2 FREQ in MHz
	uint32_t ccr   = pclk1 / (2UL * I2C_SCL_HZ);	// standard mode : Thigh = Tlow
	if(ccr < 4)	ccr = 4;							// CCR minimum

	I2Cx->CR1   = 0;								// PE = 0 while configuring
	I2Cx->CR2   = freq;
	I2Cx->CCR   = ccr;
	I2Cx->TRISE = freq + 1;							// 1000 ns max rise time
	I2Cx->CR1  |= I2C_CR1_PE;
}

// AF (NACK), ARLO and BERR are terminal : give up at once instead of waiting
// out the whole budget, which would otherwise cost milliseconds per byte.
static int I2C_wait(I2C_TypeDef *I2Cx, uint32_t flag){
	uint32_t n = I2C_TIMEOUT;
	while(!(I2Cx->SR1 & flag)){
		if(I2Cx->SR1 & (I2C_SR1_AF | I2C_SR1_ARLO | I2C_SR1_BERR))	return -1;
		if(--n == 0)	return -1;
	}
	return 0;
}

static int I2C_write_byte(I2C_TypeDef *I2Cx, uint8_t devAddr, uint8_t data){
	int err;

	I2Cx->SR1 = 0;									// drop stale AF/ARLO/BERR

	I2Cx->CR1 |= I2C_CR1_START;
	err = I2C_wait(I2Cx, I2C_SR1_SB);

	if(!err){
		I2Cx->DR = (devAddr << 1);					// write : LSB = 0
		err = I2C_wait(I2Cx, I2C_SR1_ADDR);
	}
	if(!err){
		(void)I2Cx->SR2;							// SR1 then SR2 clears ADDR
		err = I2C_wait(I2Cx, I2C_SR1_TXE);
	}
	if(!err){
		I2Cx->DR = data;
		err = I2C_wait(I2Cx, I2C_SR1_BTF);
	}

	I2Cx->CR1 |= I2C_CR1_STOP;						// release the bus either way
	I2Cx->SR1 = 0;
	return err;
}

// Every failed read leaves the bus mid-transfer, so STOP and the ACK/POS bits
// are undone in one place.
static int I2C_abort(I2C_TypeDef *I2Cx, int err){
	I2Cx->CR1 |= I2C_CR1_STOP;
	I2Cx->CR1 &= ~(I2C_CR1_POS | I2C_CR1_ACK);
	I2Cx->SR1  = 0;
	return err;
}

// Master receiver, RM0383 18.3.3. The last byte must be NACK-ed, and the
// hardware holds two bytes (DR + shift register), so 1, 2 and 3+ bytes each
// need ACK dropped and STOP set at a different moment.
static int I2C_read_buf(I2C_TypeDef *I2Cx, uint8_t devAddr, uint8_t *buf, uint32_t len){
	int      err;
	uint32_t i = 0;

	if(len == 0)	return 0;

	I2Cx->SR1  = 0;									// drop stale AF/ARLO/BERR
	I2Cx->CR1 &= ~I2C_CR1_POS;

	I2Cx->CR1 |= I2C_CR1_START;
	err = I2C_wait(I2Cx, I2C_SR1_SB);
	if(err)	return I2C_abort(I2Cx, err);

	I2Cx->DR = (devAddr << 1) | 0x1;				// read : LSB = 1
	err = I2C_wait(I2Cx, I2C_SR1_ADDR);
	if(err)	return I2C_abort(I2Cx, err);

	if(len == 1){
		I2Cx->CR1 &= ~I2C_CR1_ACK;					// NACK the only byte
		(void)I2Cx->SR2;							// SR1 then SR2 clears ADDR
		I2Cx->CR1 |= I2C_CR1_STOP;					// STOP before the byte lands
		err = I2C_wait(I2Cx, I2C_SR1_RXNE);
		if(err)	return I2C_abort(I2Cx, err);
		buf[0] = (uint8_t)I2Cx->DR;
		return 0;
	}

	if(len == 2){
		I2Cx->CR1 |= I2C_CR1_POS | I2C_CR1_ACK;		// POS : ACK applies to byte 2
		(void)I2Cx->SR2;
		I2Cx->CR1 &= ~I2C_CR1_ACK;					// so byte 2 is NACK-ed
		err = I2C_wait(I2Cx, I2C_SR1_BTF);			// both bytes received
		if(err)	return I2C_abort(I2Cx, err);
		I2Cx->CR1 |= I2C_CR1_STOP;
		buf[0] = (uint8_t)I2Cx->DR;
		buf[1] = (uint8_t)I2Cx->DR;
		I2Cx->CR1 &= ~I2C_CR1_POS;
		return 0;
	}

	I2Cx->CR1 |= I2C_CR1_ACK;
	(void)I2Cx->SR2;
	while(len - i > 3){								// all but the last three
		err = I2C_wait(I2Cx, I2C_SR1_RXNE);
		if(err)	return I2C_abort(I2Cx, err);
		buf[i++] = (uint8_t)I2Cx->DR;
	}
	err = I2C_wait(I2Cx, I2C_SR1_BTF);				// N-2 in DR, N-1 shifted in
	if(err)	return I2C_abort(I2Cx, err);
	I2Cx->CR1 &= ~I2C_CR1_ACK;						// NACK byte N
	buf[i++] = (uint8_t)I2Cx->DR;					// N-2 : frees the register
	I2Cx->CR1 |= I2C_CR1_STOP;
	buf[i++] = (uint8_t)I2Cx->DR;					// N-1
	err = I2C_wait(I2Cx, I2C_SR1_RXNE);
	if(err)	return I2C_abort(I2Cx, err);
	buf[i]   = (uint8_t)I2Cx->DR;					// N
	return 0;
}


// ========================================================
// I2C1 (PB_8 = SCL AF4, PB_9 = SDA AF4)  = Nucleo D15 / D14
// ========================================================
void I2C1_init(void){
	I2C_pin_init(I2C1, PB_8, PB_9);
	I2C_timing_init(I2C1);
}

int I2C1_write(uint8_t devAddr, uint8_t data){
	return I2C_write_byte(I2C1, devAddr, data);
}

int I2C1_read(uint8_t devAddr, uint8_t *buf, uint32_t len){
	return I2C_read_buf(I2C1, devAddr, buf, len);
}


// ========================================================
// I2C2 (PB_10 = SCL AF4, PB_3 = SDA AF9)
// PB11 is absent on this package, so the PB10/PB11 pair cannot be used.
// ========================================================
void I2C2_init(void){
	I2C_pin_init(I2C2, PB_10, PB_3);
	I2C_timing_init(I2C2);
}

int I2C2_write(uint8_t devAddr, uint8_t data){
	return I2C_write_byte(I2C2, devAddr, data);
}

int I2C2_read(uint8_t devAddr, uint8_t *buf, uint32_t len){
	return I2C_read_buf(I2C2, devAddr, buf, len);
}


// ========================================================
// I2C3 (PA_8 = SCL AF4, PC_9 = SDA AF4)
// ========================================================
void I2C3_init(void){
	I2C_pin_init(I2C3, PA_8, PC_9);
	I2C_timing_init(I2C3);
}

int I2C3_write(uint8_t devAddr, uint8_t data){
	return I2C_write_byte(I2C3, devAddr, data);
}

int I2C3_read(uint8_t devAddr, uint8_t *buf, uint32_t len){
	return I2C_read_buf(I2C3, devAddr, buf, len);
}


// ========================================================
// LCD Function
// ========================================================
static void LCD_send(uint8_t data, uint8_t mode){
    uint8_t high = data & 0xF0;
    uint8_t low  = (data << 4) & 0xF0;

    I2C1_write(LCD_ADDR, high | mode | LCD_BACKLIGHT);
    I2C1_write(LCD_ADDR, high | mode | LCD_BACKLIGHT | LCD_ENABLE_BIT);
    I2C1_write(LCD_ADDR, high | mode | LCD_BACKLIGHT);

    I2C1_write(LCD_ADDR, low | mode | LCD_BACKLIGHT);
    I2C1_write(LCD_ADDR, low | mode | LCD_BACKLIGHT | LCD_ENABLE_BIT);
    I2C1_write(LCD_ADDR, low | mode | LCD_BACKLIGHT);
}

void LCD_command(uint8_t cmd){ LCD_send(cmd, LCD_CMD); }
void LCD_data(uint8_t data){ LCD_send(data, LCD_DATA); }

void LCD_init(void){
    delay_ms(50);
    LCD_command(0x33);  // Function set: Initialize LCD in 8-bit mode first (to reset LCD)
    LCD_command(0x32);  // Function set: Switch to 4-bit mode
    LCD_command(0x28);  // Function set: 4-bit, 2-line, 5x8 dots
    LCD_command(0x0C);  // Display ON, Cursor OFF, Blink OFF
    LCD_command(0x06);  // Entry mode set: Increment cursor, no display shift
    LCD_command(0x01);  // Clear display and return home
    delay_ms(2);
}

void LCD_print(char *str){
    while(*str){ LCD_data(*str++); }
}

void LCD_setCursor(uint8_t col, uint8_t row){
    uint8_t addr[] = {0x00, 0x40, 0x14, 0x54}; // 20x4
    LCD_command(0x80 | (addr[row] + col));
}

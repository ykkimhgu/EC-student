#include "ecI2C2.h"
#include "ecSysTick2.h"

// ========================================================
// I2C1 Initialize (PB6=SCL, PB7=SDA)
// ========================================================
void I2C1_init(void){
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; 
    RCC_GPIOB_enable();

    GPIO_init(PB_6, AF);
    GPIO_init(PB_7, AF);
    GPIO_otype(PB_6, 1);
    GPIO_otype(PB_7, 1);
    GPIO_pupd(PB_6, 0);
    GPIO_pupd(PB_7, 0);
    GPIO_ospeed(PB_6, EC_HIGH);
    GPIO_ospeed(PB_7, EC_HIGH);

    GPIOB->AFR[0] |= (4 << (4*6)); // AF4
    GPIOB->AFR[0] |= (4 << (4*7));

    I2C1->CR1 = 0;
    I2C1->CR2 = 42;
    I2C1->CCR = 210;
    I2C1->TRISE = 43;
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_write(uint8_t devAddr, uint8_t data){
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = (devAddr << 1);
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    volatile int tmp = I2C1->SR2;

    while(!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;

    while(!(I2C1->SR1 & I2C_SR1_BTF));
    I2C1->CR1 |= I2C_CR1_STOP;
}

// ========================================================
// I2C2 Initialize (PB10=SCL, PB11=SDA)
// ========================================================
void I2C2_init(void){
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    RCC_GPIOB_enable();

    GPIO_init(PB_10, AF);
    GPIO_init(PB_11, AF);
    GPIO_otype(PB_10, 1);
    GPIO_otype(PB_11, 1);
    GPIO_pupd(PB_10, 0);
    GPIO_pupd(PB_11, 0);
    GPIO_ospeed(PB_10, EC_HIGH);
    GPIO_ospeed(PB_11, EC_HIGH);

    GPIOB->AFR[1] |= (4 << (4*(10-8))); // PB10 AF4
    GPIOB->AFR[1] |= (4 << (4*(11-8))); // PB11 AF4

    I2C2->CR1 = 0;
    I2C2->CR2 = 42;
    I2C2->CCR = 210;
    I2C2->TRISE = 43;
    I2C2->CR1 |= I2C_CR1_PE;
}

void I2C2_write(uint8_t devAddr, uint8_t data){
    I2C2->CR1 |= I2C_CR1_START;
    while(!(I2C2->SR1 & I2C_SR1_SB));

    I2C2->DR = (devAddr << 1);
    while(!(I2C2->SR1 & I2C_SR1_ADDR));
    volatile int tmp = I2C2->SR2;

    while(!(I2C2->SR1 & I2C_SR1_TXE));
    I2C2->DR = data;

    while(!(I2C2->SR1 & I2C_SR1_BTF));
    I2C2->CR1 |= I2C_CR1_STOP;
}

// ========================================================
// I2C3 Initialize (PA8=SCL, PC9=SDA)
// ========================================================
void I2C3_init(void){
    RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
    RCC_GPIOA_enable();
    RCC_GPIOC_enable();

    GPIO_init(PA_8, AF);
    GPIO_init(PC_9, AF);
    GPIO_otype(PA_8, 1);
    GPIO_otype(PC_9, 1);
    GPIO_pupd(PA_8, 0);
    GPIO_pupd(PC_9, 0);
    GPIO_ospeed(PA_8, EC_HIGH);
    GPIO_ospeed(PC_9, EC_HIGH);

    GPIOA->AFR[1] |= (4 << (4*(8-8)));  // AF4 for PA8
    GPIOC->AFR[1] |= (4 << (4*(9-8)));  // AF4 for PC9

    I2C3->CR1 = 0;
    I2C3->CR2 = 42;
    I2C3->CCR = 210;
    I2C3->TRISE = 43;
    I2C3->CR1 |= I2C_CR1_PE;
}

void I2C3_write(uint8_t devAddr, uint8_t data){
    I2C3->CR1 |= I2C_CR1_START;
    while(!(I2C3->SR1 & I2C_SR1_SB));

    I2C3->DR = (devAddr << 1);
    while(!(I2C3->SR1 & I2C_SR1_ADDR));
    volatile int tmp = I2C3->SR2;

    while(!(I2C3->SR1 & I2C_SR1_TXE));
    I2C3->DR = data;

    while(!(I2C3->SR1 & I2C_SR1_BTF));
    I2C3->CR1 |= I2C_CR1_STOP;
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

#ifndef __EC_I2C2_H
#define __EC_I2C2_H

#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"

// Common LCD definitions
#define LCD_ADDR       0x27
#define LCD_BACKLIGHT  0x08
#define LCD_ENABLE_BIT 0x04
#define LCD_CMD        0x00
#define LCD_DATA       0x01

// ======== I2C1 / I2C2 / I2C3 Functions ========
void I2C1_init(void);
void I2C2_init(void);
void I2C3_init(void);

void I2C1_write(uint8_t devAddr, uint8_t data);
void I2C2_write(uint8_t devAddr, uint8_t data);
void I2C3_write(uint8_t devAddr, uint8_t data);

// ======== LCD Functions (Using I2C1 by default) ========
void LCD_init(void);
void LCD_command(uint8_t cmd);
void LCD_data(uint8_t data);
void LCD_print(char *str);
void LCD_setCursor(uint8_t col, uint8_t row);

#endif

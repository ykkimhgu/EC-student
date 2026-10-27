/*----------------------------------------------------------------
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Young-Keun Kim
Created          : 10-01-2026
Modified         : 10-22-2026
Language/ver     : C in VS Code

Description      : I2C Library. i2c-LCD display control included
----------------------------------------------------------------*/

#ifndef __EC_I2C2_H
#define __EC_I2C2_H

#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"

// Pin / AF per bus on the F411R(C-E)T package. Note PB11 does not exist here,
// so I2C2 cannot use the PB10/PB11 pair, and I2C2_SDA is AF9, not AF4.
//   I2C1 : SCL PB_8  (AF4)   SDA PB_9 (AF4)   = the board's SCL/SDA header
//   I2C2 : SCL PB_10 (AF4)   SDA PB_3 or PB_9 (AF9)
//   I2C3 : SCL PA_8  (AF4)   SDA PC_9 (AF4)
// PB_8/PB_9 are D15/D14 on the Nucleo, the pins a breakout plugs into. I2C1
// can also run on PB_6/PB_7 (AF4), but those are taken by the FND and UART1.
// SCL/SDA are open-drain with no internal pull-up : the bus needs external
// pull-ups, which a breakout module normally carries.

// LM75-family temperature sensor (LM75B / TMP75). TEMP is 16-bit two's
// complement in units of 1/256 degC, so (int16_t)raw / 256.0 is the reading
// whatever the part's resolution (9, 11 or 12 bit) is.
#define LM75_ADDR      0x4C
#define LM75_REG_TEMP  0x00
#define LM75_REG_CONF  0x01

// Common LCD definitions
#define LCD_ADDR       0x27
#define LCD_BACKLIGHT  0x08
#define LCD_ENABLE_BIT 0x04
#define LCD_CMD        0x00
#define LCD_DATA       0x01

// ======== I2C1 / I2C2 / I2C3 Functions ========
// Standard mode 100 kHz. CR2/CCR/TRISE are derived from EC_SYSCLK, so the
// bus rate follows RCC_PLL_init() (PCLK1 42MHz) or RCC_HSI_init() (16MHz).
void I2C1_init(void);
void I2C2_init(void);
void I2C3_init(void);

// Returns 0 on success, -1 on timeout (no device, stuck bus, no pull-ups).
int I2C1_write(uint8_t devAddr, uint8_t data);
int I2C2_write(uint8_t devAddr, uint8_t data);
int I2C3_write(uint8_t devAddr, uint8_t data);

// Reads len bytes from the device's current register pointer, like Arduino's
// Wire.requestFrom(). A register is read as I2Cx_write(addr, reg) first, then
// I2Cx_read(). Returns 0 on success, -1 on timeout or NACK.
int I2C1_read(uint8_t devAddr, uint8_t *buf, uint32_t len);
int I2C2_read(uint8_t devAddr, uint8_t *buf, uint32_t len);
int I2C3_read(uint8_t devAddr, uint8_t *buf, uint32_t len);

// ======== LCD Functions (Using I2C1 by default) ========
void LCD_init(void);
void LCD_command(uint8_t cmd);
void LCD_data(uint8_t data);
void LCD_print(char *str);
void LCD_setCursor(uint8_t col, uint8_t row);

#endif

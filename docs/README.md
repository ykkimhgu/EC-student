# EC-student

---

# Embedded Controller - STM32f411 HAL API

Documentation for HAL functions

Written by:   000

Course:  임베디드컨트롤러

Program: C/C++

IDE/Compiler: Keil uVision 5

OS: Win10

MCU: STM32F411RE (Nucleo-64)



---

[TOC]

  * [GPIO Digital In/Out](#gpio-digital-inout)
    + [Header File](#header-file)
    + [GPIO_init\(\)](#gpio_init)
    + [GPIO_mode\(\)](#gpio_mode)
    + [GPIO_write\(\)](#gpio_write)
    + [GPIO_read\(\)](#gpio_read)
    + [GPIO_ospeed\(\)](#gpio_ospeed)
    + [GPIO_otype\(\)](#gpio_otype)
    + [GPIO_pupd\(\)](#gpio_pupd)

---

## GPIO Digital InOut 

### Header File

 `#include "ecGPIO2.h"`


```c++
#include "stm32f411xe.h"
#include "ecRCC2.h"

#ifndef __ECGPIO2_H
#define __ECGPIO2_H

// MODER
#define INPUT  		0x00
#define OUTPUT 		0x01
#define AF     		0x02
#define ANALOG 		0x03

// IDR & ODR
#define HIGH 		1
#define LOW  		0

// OSPEED
#define LOW_SPEED		0x00
#define MID_SPEED		0x01
#define FAST_SPEED		0x02
#define HIGH_SPEED		0x03

// OTYPER
#define PUSH_PULL 		0	// Push-pull
#define OPEN_DRAIN 		1 	// Open-Drain

// PUDR
#define NO_PUPD			0x00 	// No pull-up, pull-down
#define PULL_UP			0x01 	// Pull-up
#define PULL_DOWN 		0x02 	// Pull-down	
#define RESERVED 		0x03 	// Reserved

// PIN
#define LED_PIN 		5
#define BUTTON_PIN 		13

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
void GPIO_init(PinName_t pinName, int mode);
void GPIO_mode(PinName_t pinName, int mode);
void GPIO_write(PinName_t pinName, int output);
int  GPIO_read(PinName_t pinName);
void GPIO_ospeed(PinName_t pinName, int speed);
void GPIO_otype(PinName_t pinName, int type);
void GPIO_pupd(PinName_t pinName, int pupd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
```




### GPIO_init\(\)

Initializes GPIO pins with default setting and Enables GPIO Clock. Mode: In/Out/AF/Analog

```c++
void GPIO_init(PinName_t pinName, int mode);
```

**Parameters**

* **pinName:**  Port Number and pin  GPIOA~GPIOH (e.g. PA_5)

* **mode**:   INPUT(0), OUTPUT(1),  AF(02), ANALOG (03)

  

**Example code**

```c++
GPIO_init(PA_5, OUTPUT);
GPIO_init(PC_13, INPUT); //GPIO_init(PC_13, 0);
```



### GPIO_mode\(\)

Configures  GPIO pin modes: In/Out/AF/Analog

```c++
void GPIO_mode(PinName_t pinName, uint32_t mode);
```

**Parameters**

* **pinName:**  Port Number and pin  GPIOA~GPIOH (e.g. PA_5)

* **mode**:   INPUT (0), OUTPUT (1),  AF(02), ANALOG (03)

  

**Example code**

```c++
GPIO_mode(PA_5, OUTPUT);
```



### GPIO_write\(\)

Write the data to GPIO pin: High, Low

```c++
void GPIO_write(PinName_t pinName, int output);
```

**Parameters**

* **pinName:**  Port Number and pin  GPIOA~GPIOH (e.g. PA_5)

* **output**:   LOW(0), HIGH(1)



**Example code**

```c++
GPIO_write(PA_5, 1);  // 1: High
```



### GPIO_read\(\)

Read the data from GPIO pin

```c++
int  GPIO_read(PinName_t pinName);
```

**Parameters**

* **pinName:**  Port Number and pin  GPIOA~GPIOH (e.g. PA_5)

**Example code**

```c++
GPIO_read(PC_13);
```



### GPIO_ospeed\(\)

Configures  output speed of GPIO pin : Low, Mid, Fast, High

```c++
void GPIO_ospeed(PinName_t pinName, int speed);
```

**Parameters**

* **pinName:**  Port Number and pin  GPIOA~GPIOH (e.g. PA_5)

* **speed**:   LOW_SPEED(0), MID_SPEED(1), FAST_SPEED(2) , HIGH_SPEED(3)



**Example code**

```c++
GPIO_ospeed(PA_5, 2);  // 2: FAST_SPEED
```



### GPIO_otype\(\)

Configures  output type of GPIO pin: Push-Pull / Open-Drain

```c++
void GPIO_otype(PinName_t pinName, int type);
```

**Parameters**

* **pinName:**  Port Number and pin  GPIOA~GPIOH (e.g. PA_5)

* **type**:   PUSH_PULL(0), OPEN_DRAIN(1)



**Example code**

```c++
GPIO_otype(PA_5, 0);  // 0: Push-Pull
```



### GPIO_pupd\(\)

Configures  Pull-up/Pull-down mode of GPIO pin: No Pull-up, Pull-down/ Pull-up/ Pull-down/ Reserved

```c++
void GPIO_pupd(PinName_t pinName, int pupd);
```

**Parameters**

* **pinName:**  Port Number and pin  GPIOA~GPIOH (e.g. PA_5)

* **pupd**:   NO_PUPD(0), PULL_UP(1), PULL_DOWN(2), RESERVED(3)



**Example code**

```c++
GPIO_pupd(PA_5, 0);  // 0: No Pull-up, Pull-down
```




------



## Class or Header name

### Function Name

```text

```

**Parameters**

* p1
* p2

**Example code**

```text

```

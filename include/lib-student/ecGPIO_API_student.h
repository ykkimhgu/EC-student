#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"


#ifndef __EC_GPIO_API_H
#define __EC_GPIO_API_H


#define EC_DOUT  	1
#define EC_DIN 		0

#define EC_PU 1
#define EC_PD 0
#define EC_NONE 0

#define EC_LOW 		0
#define EC_MEDIUM 1
#define EC_FAST 	2
#define EC_HIGH 	3

#define LED_PIN 	5
#define BUTTON_PIN 13


/* System CLOCK is HSI by default */

class EC_DigitalIn
{
public:
    EC_DigitalIn(GPIO_TypeDef *Port, int pin) 
    {
			uint8_t mode=EC_DIN; // mode=0
			GPIO_init(Port, pin, mode);
			Port_t=Port;
			pin_t=pin;
			mode_t=mode;	
    }

    ~EC_DigitalIn()
    {
			 delete[] Port_t;
    }

    int read()
    {
				val_t=GPIO_read(Port_t, pin_t);
				return val_t;
    }
		
		void pupd(int _pupd){
			GPIO_pupd(Port_t, pin_t, _pupd);
		}
    
    operator int()
    {
        return read();
    }

	private:
			GPIO_TypeDef *Port_t;
			int	pin_t;
			int mode_t;	
			int val_t;	
};



class EC_DigitalOut
{
public:
		EC_DigitalOut(GPIO_TypeDef *Port, int pin);
		// Exercise. Define the function in EC_GPIO.cpp

    ~EC_DigitalOut()
    {
			 delete[] Port_t;
    }

    void write(int _outVal);
		// Exercise. Define the function in EC_GPIO.cpp

  	void pupd(int _pupd);
		// Exercise. Define the function in EC_GPIO.cpp
		
		void otype(int _type);
		// Exercise. Define the function in EC_GPIO.cpp
		
		void ospeed(int _speed);
	// Exercise. Define the function in EC_GPIO.cpp	
	
		EC_DigitalOut &operator= (int value)
    {
				 write(value);
				 return *this;
    }
		int read()
    {
				return GPIO_read(Port_t, pin_t);
    }
		operator int()
		{
		// Underlying call is thread safe
			return read();
		}

	private:
			GPIO_TypeDef *Port_t;
			int	pin_t;
			int mode_t;	

};

#endif



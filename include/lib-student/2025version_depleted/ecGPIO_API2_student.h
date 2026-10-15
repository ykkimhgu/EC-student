#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"


#ifndef __EC_GPIO_API2_H
#define __EC_GPIO_API2_H


#define EC_DOUT  	1
#define EC_DIN 		0

#define EC_PU       1
#define EC_PD       0
#define EC_NONE     0

#define EC_LOW 		0
#define EC_MEDIUM   1
#define EC_FAST 	2
#define EC_HIGH 	3

#define LED_PIN 	5
#define BUTTON_PIN 13


/* System CLOCK is HSI by default */

class EC_DigitalIn
{
public:
    EC_DigitalIn(PinName_t pinName):
        pinName(pinName),
        mode_t(EC_DIN)
    {
			GPIO_init(pinName, mode_t);
    }

    ~EC_DigitalIn();

    int read()
    {
        return GPIO_read(pinName);
    }
		
		void pupd(int _pupd){
			GPIO_pupd(pinName, _pupd);
		}
    
    operator int()
    {
        return read();
    }

	private:
			PinName_t pinName;
			int mode_t;
};



class EC_DigitalOut
{
public:
	EC_DigitalOut(PinName_t pinName);
	// Exercise. Define the function in EC_GPIO2.cpp

    ~EC_DigitalOut();

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
    PinName_t pinName;
	int mode_t;

};

#endif // __EC_GPIO_API2_H



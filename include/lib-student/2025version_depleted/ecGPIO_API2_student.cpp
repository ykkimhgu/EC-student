#include "ecGPIO_API2.h"

/* System CLOCK is HSI by default */


/*
int EC_DigitalIn::read()
{
	val_t = GPIO_read(Port_t, pin_t);
	return val_t;
}

void EC_DigitalIn::pupd(int _pupd) {
	GPIO_pupd(Port_t, pin_t, _pupd);
}
*/

EC_DigitalOut::EC_DigitalOut(PinName_t pinName):
	pinName(pinName),
    mode_t(EC_DOUT)	// mode_t=1;
{		
			GPIO_init(pinName, mode_t);
}
	
		
void EC_DigitalOut::write(int _outVal)
{
    GPIO_write(pinName, _outVal);
}

void EC_DigitalOut::pupd(int _pupd){
	GPIO_pupd(pinName, _pupd);
}

void EC_DigitalOut::otype(int _type){
	GPIO_otype(pinName, _type);
}

void EC_DigitalOut::ospeed(int _speed){
	GPIO_ospeed(pinName, _speed);
}

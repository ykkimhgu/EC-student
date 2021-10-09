#include "EC_GPIO_API_student.h"

/* System CLOCK is HSI by default */

/*
int EC_DigitalIn::read()
{
	val_t = GPIO_read(Port_t, pin_t);
	return val_t;
}

void EC_DigitalIn::pupdr(int _pupd) {
	GPIO_pudr(Port_t, pin_t, _pupd);
}
*/


EC_DigitalOut::EC_DigitalOut(GPIO_TypeDef *Port, int pin) 
{
	uint8_t mode=EC_DOUT;  // mode=1;			
	GPIO_init(Port, pin, mode);
	this->Port_t=Port;
	this->pin_t=pin;
	this->mode_t=mode;	
}
	
		
void EC_DigitalOut::write(int _outVal)
{
	// Exercise
	GPIO_write(Port_t, pin_t, _outVal);
}

void EC_DigitalOut::pupdr(int _pupd){
	// Exercise
	//GPIO_pudr(Port_t, pin_t, _pupd);
}

void EC_DigitalOut::otype(int _type){
	// Exercise
}

void EC_DigitalOut::ospeed(int _speed){
	// Exercise
}

/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-24 by YKKIM
* @bref   Embedded Controller (Arduino): SysTick with buzzer
**********************************
*/
#include "STM32TimerInterrupt.h"
#include "STM32_ISR_Timer.h"

#define HW_TIMER_INTERVAL_MS  100
#define TIMER_INTERVAL        1000L

STM32Timer ITimer(TIM1);    // Init STM32 timer TIM1

STM32_ISR_Timer ISR_Timer;  // Init STM32_ISR_Timer

// constants won't change. They're used here to set pin numbers:
const int ledPin =  13;   // the number of the LED pin
const int buzzPin = 8;    // the number of the buzzer pin

// variables will change:
int buzzState = LOW;
int cnt = 0;

void setup() {
  // initialize the LED pin as an output:
  // your code

  // Interval in microsecs
  // your code

  // Timer interrupt every 1sec
  // your code
}

void loop(){
  if (buzzState == HIGH)
    tone(buzzPin, 100);
    
  else if (buzzState == LOW)
    noTone(buzzPin);
}

// Timer handler controls ISR Timer(timer interrupt).
void TimerHandler(){
  // your code
}

// Whenever ISR Timer interrupts, this function is excuted.
void timerInterrupt(){
  // Use 'cnt' and 'buzzState' variables
  // your code 
  
}

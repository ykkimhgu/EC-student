/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-24 by YKKIM
* @bref   Embedded Controller (Arduino): SysTick (Timer Interrupt)
**********************************
*/
#include "STM32TimerInterrupt.h"
#include "STM32_ISR_Timer.h"

#define HW_TIMER_INTERVAL_MS  100
#define TIMER_INTERVAL        1000L

STM32Timer ITimer(TIM1);    // Init STM32 timer TIM1

STM32_ISR_Timer ISR_Timer;  // Init STM32_ISR_Timer

// constants won't change. They're used here to set pin numbers:
const int ledPin =  13;      // the number of the LED pin

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);

  // Interval in microsecs
  ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler);

  // Timer interrupt every 1sec
  ISR_Timer.setInterval(TIMER_INTERVAL, timerInterrupt);
}

void loop(){
}

// Timer handler controls ISR Timer(timer interrupt).
void TimerHandler(){
  ISR_Timer.run();
}

// Whenever ISR Timer interrupts, this function is excuted.
void timerInterrupt(){
  digitalWrite(ledPin, !digitalRead(ledPin));
}

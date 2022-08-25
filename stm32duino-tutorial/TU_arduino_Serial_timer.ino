/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-24 by YKKIM
* @bref   Embedded Controller (Arduino): Serial communication
**********************************
*/
#include "STM32TimerInterrupt.h"
#include "STM32_ISR_Timer.h"

#define HW_TIMER_INTERVAL_MS  100
#define TIMER1_INTERVAL       1000L
#define TIMER2_INTERVAL       3000L

STM32Timer ITimer(TIM1);    // Init STM32 timer TIM1

STM32_ISR_Timer ISR_Timer1;  // Init STM32_ISR_Timer
STM32_ISR_Timer ISR_Timer2;  // Init STM32_ISR_Timer

const int ledPin = 13;
int bPrint = 0;

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  Serial.print("Hello Nucleo\r\n");

  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  
  // Interval in microsecs
  ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler);

  ISR_Timer1.setInterval(TIMER1_INTERVAL, ledInterrupt);    // Timer interrupt every 1sec
  ISR_Timer2.setInterval(TIMER2_INTERVAL, printInterrupt);  // Timer interrupt every 3sec
}

void loop() {

  // print every 3sec.
  if (bPrint){
    Serial.print("LED = ");
    Serial.println(digitalRead(ledPin));
    bPrint = 0;
  }
}

// Timer handler controls ISR Timers(timer interrupt).
void TimerHandler(){
  ISR_Timer1.run();
  ISR_Timer2.run();
}

// Whenever ISR Timer interrupts, these functions are excuted.
void ledInterrupt(){
  digitalWrite(ledPin, !digitalRead(ledPin));
}

void printInterrupt(){
  bPrint = 1;
}

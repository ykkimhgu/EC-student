/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-24 by YKKIM
* @bref   Embedded Controller (Arduino): PWM with Ultrasonic Sensor
**********************************
*/
#include "STM32TimerInterrupt.h"
#include "STM32_ISR_Timer.h"

#define HW_TIMER_INTERVAL_MS  1
#define TIMER_INTERVAL        1L

STM32Timer ITimer(TIM1);    // Init STM32 timer TIM1

STM32_ISR_Timer ISR_Timer;  // Init STM32_ISR_Timer

const int trigPin = 10;   // Trigger pin : PWM out
const int echoPin = 7;    // Echo pin : Interrupt in

int cnt = 0;
int trig = LOW;
unsigned long duration;
float distance;

void setup() {
  // Initialize the trigger pin as an output
  pinMode(trigPin, OUTPUT);

  // Initialize the echo pin as an input
  pinMode(echoPin, INPUT);

  // Interval in microsecs
  ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler);

  // Timer interrupt every 1sec
  ISR_Timer.setInterval(TIMER_INTERVAL, timerInterrupt);
  
  Serial.begin(9600);
}

void loop() {
  // Generate pwm singal on the trigger pin.
//  digitalWrite(trigPin, LOW);
//  delayMicroseconds(2);
//  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trigPin, LOW);
//  delayMicroseconds(10);

  // Distance is calculated using how much time it takes.
  duration = pulseIn(echoPin, HIGH);
  distance = (float)duration / 58.0;
  
  Serial.printf("duration : %d Distance : ", duration);
  Serial.print(distance);
  Serial.println(" [cm]");
  delay(100);
}

// Timer handler controls ISR Timer(timer interrupt).
void TimerHandler(){
  ISR_Timer.run();
}

// Whenever ISR Timer interrupts, this function is excuted.
void timerInterrupt(){
  cnt++;
  if (cnt > 60)
    cnt = 0;
  else if (cnt < 10)
    trig = HIGH;
  else if (cnt >= 10)
    trig = LOW;

  digitalWrite(trigPin, trig);
}

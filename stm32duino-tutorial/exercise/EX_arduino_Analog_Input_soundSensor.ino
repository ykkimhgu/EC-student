/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-24 by YKKIM
* @bref   Embedded Controller (Arduino): Analog input with Sound Sensor
**********************************
*/

const int ledPin = 13;  // LED pin
const int soundPin = 5; // Sound sensor pin

float measure;          // the value measured from sound sensor.
int ledState = LOW;

void setup() {
  // Initialize the LED pin as an output.
  // your code

  // Initialize the serial port.
  // your code
}

void loop() {
  
  // the measured value needs to be transformed to voltage unit.
  // mapping(0 ~ 3.3V -> 0 ~ 1023)
  // [mV] (0 ~ 1023 -> 0 ~ 3300[mV])
  // your code

  // print measured value
  // your code

  // Change the LED state by measure threshold.
  // your code
  
  digitalWrite(ledPin, ledState);
  delay(200);
}

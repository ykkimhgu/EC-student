/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-24 by YKKIM
* @bref   Embedded Controller (Arduino): Analog input with Photo-register
**********************************
*/

const int ledPin = 13;  // LED pin
const int cdsPin = 0;   // CdS sensor pin

float measure;        // the value measured from CdS sensor.
int ledState = LOW;

void setup() {
  // Initialize the LED pin as an output.
  pinMode(ledPin, OUTPUT);

  // Initialize the serial port.
  Serial.begin(9600);
}

void loop() {

  // the measured value needs to be transformed to voltage unit.
  measure = analogRead(cdsPin);     // mapping(0 ~ 3.3V -> 0 ~ 1023)
  measure = measure * 3300 / 1024;  // [mV] (0 ~ 1023 -> 0 ~ 3300[mV])
  Serial.print("measure = ");
  Serial.print(measure);
  Serial.println(" [mV]");

  if (measure > 2500) ledState = HIGH;
  else                ledState = LOW;

  digitalWrite(ledPin, ledState);
  delay(200);
}

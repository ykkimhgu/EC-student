/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-24 by YKKIM
* @bref   Embedded Controller (Arduino): External Interrupt - Button
**********************************
*/

// constants won't change. They're used here to set pin numbers:
const int btnPin = 3;     // the number of the push button pin
const int ledPin = 13;    // the number of the LED pin

// variables will change:
int btnState = HIGH;         // variable for reading the push button status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  
  // initialize the pushbutton pin as an interrupt input:
  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btnPin), blink, CHANGE);
}

void loop() {
 
}

void blink(){
   // read the state of the pushbutton value:
  btnState = digitalRead(btnPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (btnState == HIGH)
    digitalWrite(ledPin, LOW);  // turn LED on
    
  else     
    digitalWrite(ledPin, HIGH); // turn LED off
}

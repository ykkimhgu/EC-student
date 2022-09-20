/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-24 by YKKIM
* @bref   Embedded Controller (Arduino): PWM with DC Motor
**********************************
*/

const int pwmPin = 11;  // PWM pin
const int btnPin = 3;   // button pin

int btnState = HIGH;

void setup() {
  // Initialize pwm pin as an output
  pinMode(pwmPin, OUTPUT);

  // Initialize button pin as an input
  pinMode(btnPin, INPUT);
}

void loop() {

  btnState = digitalRead(btnPin);

  // when the button is pressed,
  if (btnState == LOW){
    for (int i = 0; i < 10; i++){
      analogWrite(pwmPin, 40 + 10*i);   // Speed up
      delay(100);
    }
  
    for (int i = 10; i > 0; i--){
      analogWrite(pwmPin, 40 + 10*i);   // Speed down
      delay(100);
    }
  }
  // when the button is released,
  else{
    analogWrite(pwmPin, 0);
  }
}

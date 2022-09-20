/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-25 by YKKIM
* @bref   Embedded Controller (Arduino): PWM with DC Motor
**********************************
*/

const int pwmPin = 11;  // PWM pin
const int btnPin = 3;   // button pin
const int dirPin = 9;   // direction pin

int cnt = 0;
int dir = LOW;

void setup() {
  // Initialize PWM pin as an output:
  // your code

  // Initialize the direction pin as an output:
  // your code
  
  // Initialize the push button pin as an interrupt input:
  // your code
  
}

void loop() {
  // Write the direction and speed command to each pins.
  // Hint: speed value can be expressed by 'cnt' variable.
  // speed value : 0 ~ 255
  // your code

  
}

void motorOperation(){
  // Use 'cnt' and 'dir' variables
  // your code
}

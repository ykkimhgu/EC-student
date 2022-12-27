// State definition
#define S0  0
#define S1  1

const int ledPin = 13;
const int pwmPin = 11;
const int btnPin = 3;

int state = S0;
int bPressed = 0;
int ledOn = LOW;

void setup() {
 // [TO-DO] YOUR CODE GOES HERE
}

void loop() {
  nextState();
}

void pressed(){
  bPressed = 1;
}

void nextState(){
  // [TO-DO] YOUR CODE GOES HERE
  
  // Output
  analogWrite(pwmPin, pwm);
  digitalWrite(ledPin, ledState);
}
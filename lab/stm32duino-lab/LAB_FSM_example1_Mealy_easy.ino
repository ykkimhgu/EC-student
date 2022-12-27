// State definition
#define S0  0
#define S1  1

const int ledPin = 13;
const int pwmPin = 11;
const int btnPin = 3;

unsigned char state = S0;
unsigned char nextstate = S0;
unsigned char input = 0;
unsigned char ledOut = LOW;
unsigned char pwmOut = 0;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);

  // Initialize pwm pin as an output:
  pinMode(pwmPin, OUTPUT);
  
  // initialize the pushbutton pin as an interrupt input:
  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btnPin), pressed, FALLING);
}

void loop() {
  // Calculate next state. then update State
  nextState();

  // Output
  analogWrite(pwmPin, pwmOut);
  digitalWrite(ledPin, ledOut);
  
  delay(1000);
}

void pressed(){
  input = 1;
}

void nextState(){
  switch(state){
    case S0:
      if (input){
        nextstate = S1;
        pwmOut = 160;
        ledOut = HIGH;
      }
      else{
        nextstate = S0;
        pwmOut = 0;
        ledOut = LOW;
      }
      break;
    case S1:
      if (input){
        nextstate = S0;
        pwmOut = 0;
        ledOut = LOW;
      }
      else {
        nextstate = S1;
        pwmOut = 160;
        ledOut = HIGH;
      }
      break;
  }

  state = nextstate;
  input = 0;
}

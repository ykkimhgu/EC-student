// State definition
#define S0  0
#define S1  1

// Address number of output in array
#define PWM 0
#define LED 1

typedef struct {
  uint32_t out[2];    // output = FSM[state].out[PWM or LED]
  uint32_t next[2];   // nextstate = FSM[state].next[input]
} State_t;

State_t FSM[2] = {
  {{0   , LOW }, {S0, S1}},
  {{160 , HIGH}, {S1, S0}}
};

const int ledPin = 13;
const int pwmPin = 11;
const int btnPin = 3;

unsigned char state = S0;
unsigned char input = 0;
unsigned char pwmOut = 0;
unsigned char ledOut = LOW;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);

  // Initialize pwm pin as an output:
  pinMode(pwmPin, OUTPUT);
  
  // initialize the pushbutton pin as an interrupt input:
  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btnPin), pressed, FALLING);
  
  Serial.begin(9600);
}

void loop() {
  // Calculate next state. then update State
  nextState();

  // Output of states
  pwmOut = FSM[state].out[PWM];
  ledOut = FSM[state].out[LED];
  analogWrite(pwmPin, pwmOut);
  digitalWrite(ledPin, ledOut);

  delay(1000);
}

void pressed(){
  input = 1;
  nextState();
  input = 0;
}

void nextState(){
  state = FSM[state].next[input];
}

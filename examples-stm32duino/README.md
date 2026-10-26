# STM32duino Reference Examples

STM32duino framework setup: [Getting Started](https://github.com/stm32duino/Arduino_Core_STM32/wiki/Getting-Started).

Genuine, unmodified Arduino/STM32duino API sketches (`pinMode`, `digitalWrite`,
`attachInterrupt`, `analogWrite`, `Serial`, ...), sourced from the course's
[lab pages](https://ykkim.gitbook.io/ec/ec-course/lab) and the
[`ykkimhgu/EC-student`](https://github.com/ykkimhgu/EC-student) repo. They
are kept here **only for side-by-side comparison** against our own
`ecGPIO2`/`ecEXTI2`/... library style in
[`examples/`](../examples/README.md) — not ported, not wired into
`platformio.ini`, and not built as part of this repo.

Two comment-only fixes were made versus the original source (no behavior
change): `ExtInterrupt_Button`'s on/off comments were swapped, and
`Stepper`'s loop-count comment said "200" for a loop that actually runs
1000 times.

---

## Table of Contents

- [GPIO](#gpio)
- [EXTI Interrupt](#exti-interrupt)
- [PWM — DC Motor](#pwm--dc-motor)
- [Stepper Motor](#stepper-motor)
- [FSM Lab: Smart Mini-Fan](#fsm-lab-smart-mini-fan)
  - [Moore (table-driven)](#moore-table-driven)
  - [Mealy (table-driven)](#mealy-table-driven)
  - [Mealy (switch-based)](#mealy-switch-based)
  - [Moore + Ultrasonic (5-state)](#moore--ultrasonic-5-state)

---

## GPIO

[`STM32duino_Digital_IO_Button.ino`](STM32duino_Digital_IO_Button.ino) —
compare with [`Example_GPIO.c`](../examples/Example_GPIO.c)

```cpp
// constants won't change. They're used here to set pin numbers:
const int btnPin = 3;       // the number of the push button pin
const int ledPin = 13;     // the number of the LED pin

// variables will change:
int btnState = HIGH;         // variable for reading the push button status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  
  // initialize the push button pin as an input:
  pinMode(btnPin, INPUT);
}

void loop() {
  // read the state of the push button value:
  btnState = digitalRead(btnPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (btnState == HIGH) 
    digitalWrite(ledPin, LOW);  // turn LED off
  
  else 
    digitalWrite(ledPin, HIGH); // turn LED on
  
}
```

---

## EXTI Interrupt

[`STM32duino_ExtInterrupt_Button.ino`](STM32duino_ExtInterrupt_Button.ino) —
compare with [`Example_EXTI.c`](../examples/Example_EXTI.c)

```cpp
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
    digitalWrite(ledPin, LOW);  // turn LED off

  else
    digitalWrite(ledPin, HIGH); // turn LED on
}
```

---

## PWM — DC Motor

[`STM32duino_PWM_DCMotor.ino`](STM32duino_PWM_DCMotor.ino) —
compare with [`Example_PWM.c`](../examples/Example_PWM.c)

```cpp
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
```

---

## Stepper Motor

[`STM32duino_Stepper.ino`](STM32duino_Stepper.ino) —
compare with [`Example_Stepper.c`](../examples/Example_Stepper.c)

```cpp
// Run Stepper Motor with A4988

int x;
#define BAUD (9600)
const int dirPin = 2;
const int stepPin = 3;
const int enablePin = 6;


void setup()
{
  Serial.begin(BAUD);
  pinMode(enablePin, OUTPUT); // Enable
  pinMode(stepPin, OUTPUT);   // Step
  pinMode(dirPin, OUTPUT);    // Dir
  digitalWrite(enablePin, LOW); // Set Enable low
}

void loop()
{
  digitalWrite(dirPin, HIGH); // Set Dir high (CW)
  Serial.println("Loop 200*5 steps (1*5 rev)");
  for (x = 0; x < 1000; x++) // Loop 200*5 = 1000 times (5 rev)
  {
    digitalWrite(stepPin, HIGH); // Output high
    delay(1); // Wait
    digitalWrite(stepPin, LOW);  // Output low
    delay(1); // Wait
  }
  Serial.println("Pause");
  delay(1000); // pause one second
}
```

---

## FSM Lab: Smart Mini-Fan

Four variants of the same button-controlled fan, showing Moore vs. Mealy
design and table-driven vs. `switch`-based implementation. No `examples/`
counterpart — this is a lab-level FSM design exercise, not a single-library
demo.

### Moore (table-driven)

[`STM32duino_FSM_Fan_Moore.ino`](STM32duino_FSM_Fan_Moore.ino) — output
depends only on the current state.

```cpp
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
```

### Mealy (table-driven)

[`STM32duino_FSM_Fan_Mealy.ino`](STM32duino_FSM_Fan_Mealy.ino) — output
depends on state **and** input.

```cpp
// State definition
#define S0  0
#define S1  1

// Address number of output in array
#define PWM 0
#define LED 1

const int ledPin = 13;
const int pwmPin = 11;
const int btnPin = 3;

unsigned char state = S0;
unsigned char nextstate = S0;
unsigned char input = 0;
unsigned char ledOut = LOW;
unsigned char pwmOut = 0;

// State table definition
typedef struct {
  uint32_t out[2][2];     // output = FSM[state].out[input][PWM or LED]
  uint32_t next[2];       // nextstate = FSM[state].next[input]
} State_t;

State_t FSM[2] = {
  { {{0  , LOW }, {160, HIGH}}, {S0, S1} },
  { {{160, HIGH}, {0  , LOW }}, {S1, S0} } 
};

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

  pwmOut = FSM[state].out[input][PWM];
  ledOut = FSM[state].out[input][LED];
  
  nextstate = FSM[state].next[input];
  
  state = nextstate;
  input = 0;
}
```

### Mealy (switch-based)

[`STM32duino_FSM_Fan_Mealy_Simple.ino`](STM32duino_FSM_Fan_Mealy_Simple.ino)
— same behavior as the table-driven Mealy version above, written with
`switch`/`if` instead of a state table.

```cpp
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
```

### Moore + Ultrasonic (5-state)

[`STM32duino_FSM_Fan_Moore_Ultrasonic.ino`](STM32duino_FSM_Fan_Moore_Ultrasonic.ino)
— richer 5-state version: OFF / 50% / 100%, each ramp step going through a
PAUSE state, driven by a button (interrupt) **and** an HC-SR04 ultrasonic
distance reading (polled in the main loop).

```cpp
// State definition
#define S0    0   // Fan OFF
#define S1    1   // Fan vel = 50%
#define S2    2   // Fan vel = 100%
#define P50   3   // Pause (vel = 50%)
#define P100  4   // Pause (vel = 100%)

// Address number of output in array
#define PWM 0
#define LED 1

// State table definition
typedef struct {
  uint32_t out[2];      // output = FSM[state].out[PWM or LED]
  uint32_t next[2][2];  // nextstate = FSM[state].next[input X][input Y]
} State_t;

State_t FSM[5] = {
  { {0   , LOW }, {{S0  , S0}, {P50 , S1}} },
  { {80  , HIGH}, {{P50 , S1}, {P100, S2}} },
  { {160 , HIGH}, {{P100, S2}, {S0  , S0}} },
  { {0   , HIGH}, {{P50 , S1}, {P100, S2}} },
  { {0   , HIGH}, {{P100, S2}, {S0  , S0}} },
};

// Pin setting
const int ledPin = 13;
const int pwmPin = 11;
const int btnPin = 3;
const int trigPin = 10;
const int echoPin = 7;

unsigned char state = S0;
unsigned char input[2] = {0, 0};
unsigned char pwmOut = 0;
unsigned char ledOut = LOW;

unsigned long duration;
float distance;
int thresh = 5;

void setup() {  
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);

  // Initialize pwm pin as an output:
  pinMode(pwmPin, OUTPUT);
  
  // initialize the pushbutton pin as an interrupt input:
  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btnPin), pressed, FALLING);

  // Initialize the trigger pin as an output
  pinMode(trigPin, OUTPUT);

  // Initialize the echo pin as an input
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // Generate pwm singal on the trigger pin.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);

  // Distance is calculated using how much time it takes.
  duration = pulseIn(echoPin, HIGH);
  distance = (float)duration / 58.0;

  // Calculate next state. then update State
  nextState();

  // Output of states
  pwmOut = FSM[state].out[PWM];
  ledOut= FSM[state].out[LED];  
  analogWrite(pwmPin, pwmOut);
  digitalWrite(ledPin, ledOut);

  Serial.print("distance = ");
  Serial.print(distance);
  Serial.println(" [cm]");
  
  delay(1000);
}

void pressed(){
  input[0] = 1;
  nextState();
  input[0] = 0;
}

void nextState(){
  if (distance < thresh)
    input[1] = 1;
  else
    input[1] = 0;
    
  // get nextState
  state = FSM[state].next[input[0]][input[1]];
}
```

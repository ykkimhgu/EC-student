/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-24 by YKKIM
* @bref   Embedded Controller (Arduino): Serial Key Input
**********************************
*/

char keyIn;

void setup() {
  // Initialize the Serial port
  Serial.begin(9600);
  Serial.print("Hello Nucleo\r\n");
}

void loop() {
  // Check whether some datas come in serial port.
  if (Serial.available() > 0){
    
    keyIn = Serial.read();  // Read data from serial port.
    
    if (keyIn == '\n')
      Serial.print("\r\n");
      
    else if (keyIn)
      Serial.print(keyIn);
  }
}

/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-24 by YKKIM
* @bref   Embedded Controller (Arduino): Time measure
**********************************
*/

int cnt = 0;
unsigned long beginTime, endTime;

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  Serial.print("Program START\r\n");
}


void loop() {
  cnt = 0;
  beginTime = micros();
  
  while(cnt < 100) 
    cnt ++;
    
  endTime = micros();

  // Check how much time counting takes.
  Serial.printf("Counting %d takes %d [us]\r\n", cnt, endTime - beginTime);
  delay(500);
}

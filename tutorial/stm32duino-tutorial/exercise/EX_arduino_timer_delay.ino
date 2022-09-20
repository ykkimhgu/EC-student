/* 
**********************************
* @author SSSLAB
* @Mod    2022-8-24 by YKKIM
* @bref   Embedded Controller (Arduino): Time delay
**********************************
*/

unsigned int cnt = 0;
unsigned long beginTime, endTime;

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println(sizeof(unsigned int));
  Serial.print("Program START\r\n");
}


void loop() {
  cnt = 0;

  // call delaycnt() function
  // your code
  
  // Check how much time counting takes.
  Serial.printf("Counting %d takes %d [us]\r\n", cnt, endTime - beginTime);
  delay(500);
}

void delaycnt(unsigned int delayCnt){
  // check current time [us]
  // your code

  // counting until delayCnt value.
  // your code

  // check current time [us]
  // your code 
}

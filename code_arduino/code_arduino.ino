
#include <SoftwareSerial.h>
#define Rx 11             //Định nghĩa chân 11 là Rx
#define Tx 12             //Định nghĩa chân 12 là Tx  
SoftwareSerial mySerial(Rx, Tx); //Khởi tạo cổng serial mềm

boolean ledstate = 0;    //Trạng thái led
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

}
void loop() {

        mySerial.print("tao la duong");   //Gửi chuỗi ledon ra cổng serial
        delay(1000);
        //mySerial.print("ledoff");

  if(mySerial.available()){
    Serial.print(char(mySerial.read())); //Gửi ký tự nhận được ra cổng serial
    Serial.print("ket noi dc");

  }
}

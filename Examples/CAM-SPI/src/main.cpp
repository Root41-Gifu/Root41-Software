#include <Arduino.h>
#include <SPI.h>
#define SS_PIN_CAM PB4
#define SS_PIN_BALL PB5
#define BALL_RESET PB10
#define BAUD_RATE 19200
#define CHAR_BUF 128

void setup() {
  pinMode(SS_PIN_CAM, OUTPUT);
  Serial.begin(BAUD_RATE);

  pinMode(SS_PIN_BALL, OUTPUT);
  digitalWrite(SS_PIN_BALL, HIGH);
  delay(1000);  // Give the OpenMV Cam time to bootup.
  Serial.println("start");
  pinMode(PA8, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // byte byteRead;
  // if (Serial.available()) {
  //   // Read the most recent byte
  //   byteRead = Serial.read();
  //   // ECHO the value that was read
  //   Serial.write(byteRead);
  // }
  // Serial.println(byteRead);
  // int len = 0;
  // char buff[CHAR_BUF] = {0};
  // char tame[]="ARUNGO";
  // // Serial.println(digitalRead(PA8));
  // digitalWrite(SS_PIN_CAM, LOW);
  // delay(1);

  // if (SPI.transfer(1) == 85) {  // saw sync char?
  //   SPI.transfer(&len, 4);      // get length
  //   if (len) {
  //     SPI.transfer(&buff, min(len, CHAR_BUF));
  //     len -= min(len, CHAR_BUF);
  //   }
  //   while (len--) SPI.transfer(0);
  // }

  // digitalWrite(SS_PIN_CAM, HIGH);
  // if(strcmp(buff,tame)==0){
  //   Serial.println("ある");
  // }
  // int N = int(buff[0]-'0');
  // SPI.beginTransaction(SPISettings(6400, MSBFIRST, SPI_MODE0));
  // digitalWrite(SS_PIN_CAM,LOW);
  // digitalWrite(SS_PIN_BALL,HIGH);
  // digitalWrite(BALL_RESET,HIGH);
  // delay(1);
  // byte value = 1;

  // int32_t len = 0;
  // char buff[CHAR_BUF] = {0};
  // byte buff;
  // digitalWrite(SS_PIN_CAM, LOW);
  // delay(1);  // Give the OpenMV Cam some time to setup to send data.

  // if (SPI.transfer(1) == 85) {  // saw sync char?
  //   SPI.transfer(&len, 4);      // get length
  //   if (len) {
  //     // SPI.transfer(&buff, min(len, CHAR_BUF));
  //     // len -= min(len, CHAR_BUF);
  //   }
  //   while (len--)
  // buff=SPI.transfer(0);  // eat any remaining bytes

  // byte buff;
  // buff=SPI.transfer(0);
  // Serial.println(bufff);
  // digitalWrite(SS_PIN_BALL,HIGH);
  // digitalWrite(SS_PIN_CAM,HIGH);
  // Serial.println(buff);
  // Serial.println(N);
  
  // digitalWrite(SS_PIN_CAM, HIGH);
  // Serial.print(buff);
  // delay(1);  // Don't loop to quickly.
  if(Serial.available()){
    byte byteRead=Serial.read();
    Serial.println(byteRead);
  }
}
#include <Arduino.h>
#include <SPI.h>
#define SS_PIN_CAM PB4
#define SS_PIN_BALL PB5
#define BALL_RESET PB10
#define BAUD_RATE 9600
#define CHAR_BUF 128

void setup() {
  pinMode(SS_PIN_CAM, OUTPUT);
  Serial.begin(BAUD_RATE);

  pinMode(SS_PIN_BALL, OUTPUT);
  digitalWrite(SS_PIN_BALL, HIGH);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  SPI.setDataMode(SPI_MODE0);
  delay(1000);  // Give the OpenMV Cam time to bootup.
  Serial.println("start");
}

void loop() {
  int len = 0;
  char buff[CHAR_BUF] = {0};
  char tame[]="ARUNGO";
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
  SPI.beginTransaction(SPISettings(6400, MSBFIRST, SPI_MODE0));
  digitalWrite(SS_PIN_CAM,HIGH);
  digitalWrite(SS_PIN_BALL,LOW);
  digitalWrite(BALL_RESET,HIGH);
  delay(1);
  byte value = 1;
  byte bufff;
  bufff=SPI.transfer(0);
  Serial.println(bufff);
  // SPI.endTransaction();
  digitalWrite(SS_PIN_BALL,HIGH);
  digitalWrite(SS_PIN_CAM,HIGH);
  // digitalWrite(BALL_RESET,LOW);
  // Serial.println(bufff);
  // Serial.println(N);
  delay(1);
}
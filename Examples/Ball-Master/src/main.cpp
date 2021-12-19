#include <Arduino.h>
#include <SPI.h>

void setup() {
  Serial.begin(9600);

  pinMode(PB10, OUTPUT);
  digitalWrite(PB10, HIGH);  //サブマイコンのリセット端子

  pinMode(PB5, OUTPUT);  //サブマイコンのSS端子

  Serial.println("start");

  SPI.begin();
}

void loop() {
  // delayごりごりで誤魔化してるけどいい感じにやってね
  digitalWrite(PB5, LOW);
  delay(1);
  int data = SPI.transfer(2);
  digitalWrite(PB5, HIGH);
  Serial.println(data);
  delay(10);
}
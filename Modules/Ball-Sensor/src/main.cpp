#include <Arduino.h>

#define BALL_RST PD0

void setup() {
  pinMode(BALL_RST,OUTPUT);
}

void loop() {
  digitalWrite(BALL_RST,HIGH);
  delay(2000);
  digitalWrite(BALL_RST,LOW);
  delay(2000);
}
#include <Arduino.h>
#include <SPI.h>
#include <def.h>

volatile byte receiveData;
volatile unsigned long value;

void setup() {
  SPCR |= bit(SPE);
  pinMode(MISO, OUTPUT);
  pinMode(PD0, OUTPUT);
  digitalWrite(PD0, LOW);

  pinMode(A0, INPUT);

  SPI.attachInterrupt();
}

void loop() {
  // 15足してるのは通信エラーなのか単に反応していないのかを区別するため（実験用）
  value = pulseIn(A0, LOW) + 15;
  SPDR = value;
}

ISR(SPI_STC_vect) {  //頼むから　うまく動けよ SPI
  delay(5);
  receiveData = SPDR;
  SPDR = value;
}
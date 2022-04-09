#include <Wire.h>

#define MY_ADDRESS 4

const int pin[3] = {PB4, PB3, PB1};

bool data[3];

int transmit;
int checkDigit;

void writeData(void) {
  Wire.write(transmit);
}

void setup() {
  Wire.begin(4);
  Wire.onRequest(writeData);

  for (int i = 0; i < 3; i++) {
    pinMode(pin[i], INPUT);
  }
}

void loop() {
  checkDigit = 0;
  for (int i = 0; i < 3; i++) {
    data[i] = constrain(digitalRead(pin[i]),0,1);

    checkDigit += data[i];
  }
  
  transmit = 0B11000000 + data[0] * 16 + data[1] * 8 + data[2] * 4 + checkDigit;
}

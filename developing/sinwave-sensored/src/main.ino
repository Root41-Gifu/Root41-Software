#include <Arduino.h>
#include <FlexiTimer2.h>

int IN[3] = {3, 5, 7};
int SD[3] = {9, 6, 8};

float power = 0.2;

volatile int offset = 0;
volatile int velocityUnit;
volatile int deg;
volatile int oldDeg;
volatile int velocity;

int pwmRaw[360] = {
    237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 246, 247, 248, 248, 249,
    250, 250, 251, 251, 252, 252, 252, 253, 253, 253, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 253, 253, 253, 252, 252, 252, 251, 251, 250,
    250, 249, 248, 248, 247, 246, 246, 245, 244, 243, 242, 241, 240, 239, 238,
    237, 234, 230, 227, 223, 220, 216, 213, 209, 206, 202, 199, 195, 191, 188,
    184, 180, 176, 173, 169, 165, 161, 158, 154, 150, 146, 142, 139, 135, 131,
    127, 123, 119, 115, 112, 108, 104, 100, 96,  93,  89,  85,  81,  78,  74,
    70,  66,  63,  59,  55,  52,  48,  45,  41,  38,  34,  31,  27,  24,  20,
    17,  16,  15,  14,  13,  12,  11,  10,  9,   8,   8,   7,   6,   6,   5,
    4,   4,   3,   3,   2,   2,   2,   1,   1,   1,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   1,   1,   1,   2,   2,   2,   3,   3,   4,
    4,   5,   6,   6,   7,   8,   8,   9,   10,  11,  12,  13,  14,  15,  16,
    17,  16,  15,  14,  13,  12,  11,  10,  9,   8,   8,   7,   6,   6,   5,
    4,   4,   3,   3,   2,   2,   2,   1,   1,   1,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   1,   1,   1,   2,   2,   2,   3,   3,   4,
    4,   5,   6,   6,   7,   8,   8,   9,   10,  11,  12,  13,  14,  15,  16,
    17,  20,  24,  27,  31,  34,  38,  41,  45,  48,  52,  55,  59,  63,  66,
    70,  74,  78,  81,  85,  89,  93,  96,  100, 104, 108, 112, 115, 119, 123,
    127, 131, 135, 139, 142, 146, 150, 154, 158, 161, 165, 169, 173, 176, 180,
    184, 188, 191, 195, 199, 202, 206, 209, 213, 216, 220, 223, 227, 230, 234,
    237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 246, 247, 248, 248, 249,
    250, 250, 251, 251, 252, 252, 252, 253, 253, 253, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 253, 253, 253, 252, 252, 252, 251, 251, 250,
    250, 249, 248, 248, 247, 246, 246, 245, 244, 243, 242, 241, 240, 239, 238,
};

byte pwm[1024];

volatile uint8_t interval = 0;
volatile int voltage;

volatile float gain = 0.114;

void measureAngularVelocity(void) {
  deg = analogRead(A0);

  if (interval == 0) {
    velocity = abs(oldDeg - deg);
    if (velocity >= 512) {
      velocity = 1023 - velocity;
    }
    oldDeg = deg;
  }

  voltage = deg + offset + int(velocity * gain);  // + interval * velocityUnit;

  interval++;
  interval %= 32;
}

void setup() {
  initialize();
  calibration();

  for (int i = 0; i < 10; i++) {
    analogWrite(10, 255);
    delay(50);
    analogWrite(10, 0);
    delay(50);
  }
  delay(50);

  analogWrite(10, 70);
  delay(1000);
  digitalWrite(10, LOW);
  delay(1000);

  digitalWrite(13, HIGH);

  offset += 24;

  for (int i = 0; i < 3; i++) {  // SD端子をHIGHにする（通電させる）
    digitalWrite(SD[i], HIGH);
    analogWrite(IN[i], 1);
  }
  FlexiTimer2::set(1.0, 1.0 / (45 * (10 ^ 3)), measureAngularVelocity);
  FlexiTimer2::start();

  TIMSK0 = 0;
}

void loop() {
  while (1) {
    for (int i = 0; i < 100; i++) {
      OCR3C = pwm[voltage % 1024] * power;
      OCR3A = pwm[(voltage + 98) % 1024] * power;
      OCR4B = pwm[(voltage + 49) % 1024] * power;
    }

    signed char c;
    if ((c = Serial.read()) != -1) {
      power += 0.1;
      if (power > 1.0) {
        power = 0.1;
      }
    }
  }
}

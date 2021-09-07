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

int pwm[1024];

volatile uint8_t interval = 0;
volatile uint8_t interval2 = 0;
volatile int voltage;

void measureAngularVelocity(void) {
  if (interval == 0) {
    deg = analogRead(A0);
  }

  if (interval2 == 0) {
    velocity = abs(oldDeg - deg);
    if (velocity >= 512) {
      velocity = 1023 - velocity;
    }
    velocityUnit = velocity / 60;
    oldDeg = deg;
  }

  voltage = deg + offset + int(velocity * 0.175) + interval * velocityUnit;

  interval++;
  interval %= 12;

  interval2++;
  interval2 %= 60;
}

void setup() {
  // TIMSK0 = 0;
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

  FlexiTimer2::set(1.0, 1.0 / (100 * (10 ^ 3)), measureAngularVelocity);
  FlexiTimer2::start();

  offset += 27;

  for (int i = 0; i < 3; i++) {  // SD端子をHIGHにする（通電させる）
    digitalWrite(SD[i], HIGH);
    analogWrite(IN[i], 1);
  }
}

void loop() {
  static unsigned long buzzerTimer = 0;
  while (1) {
    for (int i = 0; i < 100; i++) {
      OCR3C = byte(pwm[voltage % 1024] * power);
      OCR3A = byte(pwm[(voltage + 98) % 1024] * power);
      OCR4B = byte(pwm[(voltage + 49) % 1024] * power);
    }

    signed char c;
    if ((c = Serial.read()) != -1) {
      power += 0.1;
      if (power > 1.0) {
        power = 0.1;
      }
      buzzerTimer = millis();
    }

    if (millis() - buzzerTimer <= 50) {
      analogWrite(10, 255);
    } else {
      analogWrite(10, 0);
    }
  }
}

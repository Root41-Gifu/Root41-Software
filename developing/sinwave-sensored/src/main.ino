#include <Arduino.h>
#include <FlexiTimer2.h>

int IN[3] = {3, 5, 7};
int SD[3] = {9, 6, 8};

float power = 0.2;

const int buzzer = 255;

volatile int offset = 0;
volatile int deg;
volatile int _deg;
volatile int drive;
volatile int _drive;
volatile int velocity;

int surveyPwm[1024];

volatile uint8_t interval = 0;
volatile uint8_t interval2 = 0;
volatile int current;

void measureAngularVelocity(void) {
  if (interval == 0) {
    drive = analogRead(A0);
  }

  if (interval2 == 0) {
    velocity = abs(_drive - drive);
    if (velocity >= 512) {
      velocity = 1023 - velocity;
    }
    _deg = velocity / 60;
    _drive = drive;
  }

  current = drive + offset + int(velocity * 0.175) + interval * _deg;

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
    analogWrite(10, buzzer);
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
  unsigned long buzzerTimer = 0;
  while (1) {
    for (int i = 0; i < 100; i++) {
      OCR3C = byte(surveyPwm[current % 1024] * power);
      OCR3A = byte(surveyPwm[(current + 98) % 1024] * power);
      OCR4B = byte(surveyPwm[(current + 49) % 1024] * power);
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
      analogWrite(10, buzzer);
    } else {
      analogWrite(10, 0);
    }
  }
}

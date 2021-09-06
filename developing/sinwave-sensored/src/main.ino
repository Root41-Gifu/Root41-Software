#include <Arduino.h>
#include <FlexiTimer2.h>

volatile int IN[3] = {3, 5, 7};
int SD[3] = {9, 6, 8};

float power = 1;

volatile int offset = 0;
volatile int deg;
volatile int _deg;
volatile int drive;
volatile int _drive;
volatile int velocity;

volatile int surveyPwm[1024];
volatile int interval = 0;
volatile int interval2 = 0;
volatile int current;

void measureAngularVelocity(void) {
  if (interval == 0) {
    deg = (analogRead(A0) + offset + 22 + velocity / 7) % 1024;

    if (abs(deg - _deg) <= 150 || abs(deg - _deg) >= 874) {
      drive = deg;
    } else {
      drive = _deg;
    }

    _deg = deg;
  }

  if (interval2 == 0) {
    velocity = abs(_drive - drive);
    if (velocity >= 512) {
      velocity = 1023 - velocity;
    }
    _drive = drive;
  }

  current = (drive + ((interval * velocity) / 85)) % 1024;

  interval++;
  interval %= 12;

  interval2++;
  interval2 %= 85;
}

void setup() {
  initialize();

  calibration();

  delay(1000);

  digitalWrite(13, HIGH);

  FlexiTimer2::set(1.0, 1.0 / (96 * (10 ^ 3)), measureAngularVelocity);
  FlexiTimer2::start();

  for (int i = 0; i < 3; i++) {  // SD端子をHIGHにする（通電させる）
    digitalWrite(SD[i], HIGH);
    analogWrite(IN[i], 1);
  }
}

void loop() {
  OCR3C = byte(constrain(surveyPwm[current] * power, 0, 254));
  OCR3A = byte(constrain(surveyPwm[(current + 98) % 1024] * power, 0, 254));
  OCR4B = byte(constrain(surveyPwm[(current + 49) % 1024] * power, 0, 254));
}

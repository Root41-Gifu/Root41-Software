#include <Arduino.h>
#include <FlexiTimer2.h>

int IN[3] = {3, 5, 7};
int SD[3] = {9, 6, 8};

float power = 1;

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

  current = drive + offset + int(velocity * 0.14) + interval * _deg;

  interval++;
  interval %= 12;

  interval2++;
  interval2 %= 60;
}

void setup() {
  // TIMSK0 = 0;
  initialize();

  calibration();

  delay(1000);

  digitalWrite(13, HIGH);

  FlexiTimer2::set(1.0, 1.0 / (97 * (10 ^ 3)), measureAngularVelocity);
  FlexiTimer2::start();

  offset += 27;

  for (int i = 0; i < 3; i++) {  // SD端子をHIGHにする（通電させる）
    digitalWrite(SD[i], HIGH);
    analogWrite(IN[i], 1);
  }
}

void loop() {
  while (1) {
    OCR3C = byte(constrain(surveyPwm[current % 1024] * power, 0, 254));
    OCR3A = byte(constrain(surveyPwm[(current + 98) % 1024] * power, 0, 254));
    OCR4B = byte(constrain(surveyPwm[(current + 49) % 1024] * power, 0, 254));
  }
}

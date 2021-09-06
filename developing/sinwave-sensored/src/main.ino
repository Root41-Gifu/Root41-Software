#include <Arduino.h>
#include <FlexiTimer2.h>

volatile int IN[3] = {3, 5, 7};
int SD[3] = {9, 6, 8};

float power = 0.2;

int offset = 0;

volatile int surveyPwm[1024];

volatile uint16_t _degPoint = 0;
volatile uint16_t degNow = 0;
volatile uint16_t degPoint = 0;
volatile uint16_t interval = 0;
volatile uint16_t angularVelocity = 0;
volatile int value;

void measureAngularVelocity(void) {
  if (interval == 0) {
    degPoint = value;
    angularVelocity = abs(_degPoint - degPoint);
    if (angularVelocity >= 512) {
      angularVelocity = 1023 - angularVelocity;
    }
    angularVelocity = abs(angularVelocity);
    angularVelocity %= 1024;
    _degPoint = degPoint;
  }

  degNow = round(angularVelocity * 0.43 + 35 + degPoint +
                 interval * (angularVelocity >> 3)) %
           1024;

  OCR3C = byte(surveyPwm[degNow] * power);
  OCR3A = byte(surveyPwm[(degNow + 98) % 1024] * power);
  OCR4B = byte(surveyPwm[(degNow + 49) % 1024] * power);

  interval++;
  interval %= 8;
}

void setup() {
  initialize();

  calibration();

  delay(1000);

  digitalWrite(13, HIGH);

  // FlexiTimer2::set(1.0, 1.0 / (8 * (10 ^ 3)), measureAngularVelocity);
  // FlexiTimer2::start();

  for (int i = 0; i < 3; i++) {  // SD端子をHIGHにする（通電させる）
    digitalWrite(SD[i], HIGH);
    analogWrite(IN[i], 1);
  }
}

void loop() {
  for (int i = 0; i < 1024; i += 1) {
    OCR3C = byte(constrain(surveyPwm[i] * power, 0, 254));
    OCR3A = byte(constrain(surveyPwm[(i + 98) % 1024] * power, 0, 254));
    OCR4B = byte(constrain(surveyPwm[(i + 49) % 1024] * power, 0, 254));
    // delayMicroseconds(700);

    int temp = (analogRead(A0) + offset) % 1024;

    Serial.print(1500);
    Serial.print("\t");
    Serial.print(0);
    Serial.print("\t");
    Serial.print(i);
    Serial.print("\t");
    Serial.print(offset);
    Serial.print("\t");
    Serial.print(analogRead(A0));
    Serial.print("\t");
    Serial.println(temp);
  }
}

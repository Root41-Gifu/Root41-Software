void initialize(void) {
  //初期化
  for (int i = 0; i < 3; i++) {
    pinMode(IN[i], OUTPUT);
    pinMode(SD[i], OUTPUT);
  }
  pinMode(A0, INPUT);
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);

  Serial.begin(115200);

  digitalWrite(13, LOW);

  // PWM周波数を変更
  TCCR3B = (TCCR3B & 0b11111000) | 0x01;
  TCCR4B = (TCCR4B & 0b11111000) | 0x01;

  ADCSRA = ADCSRA & 0xf8;
  ADCSRA = ADCSRA | 0x04;

  for (int i = 0; i < 3; i++) {  // LOWにする
    digitalWrite(IN[i], LOW);
    digitalWrite(SD[i], LOW);
  }

  delay(100);

  for (int i = 0; i < 5; i++) {
    analogWrite(10, 255);
    delay(50);
    analogWrite(10, 0);
    delay(50);
  }
  delay(200);
  analogWrite(10, 50);
  delay(200);
  analogWrite(10, 0);
  delay(200);
}

void calibration(void) {
  for (int i = 0; i < 1024; i++) {
    // pwm[i] = round(127 * sin(radians(map(i, 0, 1024, 0, 360 * 7))) + 127);
    pwm[i] = pwmRaw[round(map(i, 0, 1024, 0, 360 * 7) % 360)];
  }

  for (int i = 0; i < 3; i++) {  // SD端子をHIGHにする（通電させる）
    digitalWrite(SD[i], HIGH);
    analogWrite(IN[i], 1);
  }

  randomSeed(analogRead(A1));

  const int loopTime = 3;
  const float calibrationPower = 0.2;

  int offsetRaw[loopTime];

  for (int j = 0; j < loopTime; j++) {
    long randomNumber = random(1023);
    for (int i = 0; i < 1024; i += 1) {
      OCR3C = byte(constrain(pwm[i] * calibrationPower, 0, 254));
      OCR3A = byte(constrain(pwm[(i + 98) % 1024] * calibrationPower, 0, 254));
      OCR4B = byte(constrain(pwm[(i + 49) % 1024] * calibrationPower, 0, 254));

      int temp = analogRead(A0);

      if (randomNumber == i) {
        offsetRaw[j] = ((i - temp + 1024) % 1024);
      }

      if (i >= 1000) {
        analogWrite(10, 255);
      } else {
        analogWrite(10, 0);
      }

      delayMicroseconds(150);
    }
  }

  // for (int i = 0; i < 360; i++) {
  //   pwmRaw[i] = sin(radians(i)) * 254;
  // }

  // while (true) {
  //   for (int i = 0; i < 1024; i++) {
  //     OCR3C = byte(constrain(pwm[i] * calibrationPower, 0, 254));
  //     OCR3A = byte(constrain(pwm[(i + 49) % 1024] * calibrationPower, 0,
  //     254)); OCR4B = byte(constrain(pwm[(i + 98) % 1024] * calibrationPower,
  //     0, 254));

  //     // OCR3C = byte(constrain(sin(radians(i)) * 254 * calibrationPower, 0,
  //     // 254)); OCR3A = byte(
  //     //     constrain(sin(radians(i + 120)) * 254 * calibrationPower, 0,
  //     254));
  //     // OCR4B = byte(
  //     //     constrain(sin(radians(i + 240)) * 254 * calibrationPower, 0,
  //     254));

  //     delayMicroseconds(90);
  //   }
  // }

  // while (true) {
  //   for (int i = 0; i < 360; i++) {
  //     OCR3C = byte(constrain(pwmRaw[i] * calibrationPower, 0, 254));
  //     OCR3A =
  //         byte(constrain(pwmRaw[(i + 120) % 360] * calibrationPower, 0,
  //         254));
  //     OCR4B =
  //         byte(constrain(pwmRaw[(i + 240) % 360] * calibrationPower, 0,
  //         254));

  //     // OCR3C = byte(constrain(sin(radians(i)) * 254 * calibrationPower, 0,
  //     // 254)); OCR3A = byte(
  //     //     constrain(sin(radians(i + 120)) * 254 * calibrationPower, 0,
  //     254));
  //     // OCR4B = byte(
  //     //     constrain(sin(radians(i + 240)) * 254 * calibrationPower, 0,
  //     254));

  //     delayMicroseconds(10);
  //   }
  // }

  offset = 0;

  for (int i = 1; i < loopTime; i++) {
    offset += offsetRaw[i];
  }
  offset /= loopTime - 1;
}
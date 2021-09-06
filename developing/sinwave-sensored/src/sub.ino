void initialize(void) {
  //初期化
  for (int i = 0; i < 3; i++) {
    pinMode(IN[i], OUTPUT);
    pinMode(SD[i], OUTPUT);
  }
  pinMode(A0, INPUT);
  pinMode(13, OUTPUT);

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
}

void calibration(void) {
  for (int i = 0; i < 1024; i++) {
    surveyPwm[i] = round(127 * sin(radians(map(i, 0, 1024, 0, 360 * 7))) + 127);
  }

  for (int i = 0; i < 3; i++) {  // SD端子をHIGHにする（通電させる）
    digitalWrite(SD[i], HIGH);
    analogWrite(IN[i], 1);
  }

  Serial.println("上限,下限,理論値,offset,実測値,補正値");

  randomSeed(analogRead(A1));
  long randomNumber1 = random(1023);
  long randomNumber2 = random(1023);

  int offset1;
  int offset2;

  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 1024; i += 1) {
      OCR3C = byte(constrain(surveyPwm[i] * 0.15, 0, 254));
      OCR3A = byte(constrain(surveyPwm[(i + 98) % 1024] * 0.15, 0, 254));
      OCR4B = byte(constrain(surveyPwm[(i + 49) % 1024] * 0.15, 0, 254));
      // delayMicroseconds(700);

      int temp = analogRead(A0);

      if (randomNumber1 == i && j == 1) {
        offset1 = ((i - temp + 1024) % 1024);
      }
      if (randomNumber2 == i && j == 2) {
        offset2 = ((i - temp + 1024) % 1024);
      }

      Serial.print(1500);
      Serial.print("\t");
      Serial.print(0);
      Serial.print("\t");
      Serial.print(i);
      Serial.print("\t");
      Serial.print(offset);
      Serial.print("\t");
      Serial.print(temp);
      Serial.print("\t");
      Serial.println(0);
    }
  }

  offset = (offset1 + offset2) / 2;
}
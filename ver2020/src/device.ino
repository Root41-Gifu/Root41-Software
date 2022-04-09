void _device::initialize(void) {
  TCCR0B = (TCCR0B & 0b11111000) | 0x04;
  TCCR1B = (TCCR1B & 0b11111000) | 0x04;
  TCCR2B = (TCCR2B & 0b11111000) | 0x04;
  TCCR3B = (TCCR3B & 0b11111000) | 0x04;
  TCCR4B = (TCCR4B & 0b11111000) | 0x04;

  LED.RED = RGBLED.Color(255, 0, 0);
  LED.BLUE = RGBLED.Color(0, 0, 0);
  LED.GREEN = RGBLED.Color(0, 255, 0);
  LED.YELLOW = RGBLED.Color(0, 255, 0);
  LED.WHITE = RGBLED.Color(255, 255, 255);
  LED.PURPLE = RGBLED.Color(255, 0, 200);
  LED.ORANGE = RGBLED.Color(255, 100, 0);
  LED.MINT = RGBLED.Color(100, 255, 50);
  LED.LIME = RGBLED.Color(190, 255, 0);
  LED.NONE = RGBLED.Color(0, 0, 0);

  for (int i = 0; i <= 359; i++) {
    float s;
    motor.calcVal[0][i] = round(sin(radians(i - 300)) * 100.0);
    motor.calcVal[1][i] = round(sin(radians(i - 60)) * 100.0);
    motor.calcVal[2][i] = round(sin(radians(i - 225)) * 100.0);
    motor.calcVal[3][i] = round(sin(radians(i - 135)) * 100.0);

    int valTemp[4];
    for (int k = 0; k < 4; k++) {
      valTemp[k] = motor.calcVal[k][i];
    }

    for (int k = 0; k < 4; ++k) {
      for (int j = k + 1; j < 4; ++j) {
        if (abs(valTemp[k]) >= abs(valTemp[j])) {
          int temp = valTemp[k];
          valTemp[k] = valTemp[j];
          valTemp[j] = temp;
        }
      }
    }
    s = 255.0 / float(abs(valTemp[3]));
    motor.calcVal[0][i] = round((float)motor.calcVal[0][i] * s);
    motor.calcVal[1][i] = round((float)motor.calcVal[1][i] * s);
    motor.calcVal[2][i] = round((float)motor.calcVal[2][i] * s);
    motor.calcVal[3][i] = round((float)motor.calcVal[3][i] * s);
  }

  Wire.begin();

  TOF.init();
  TOF.setTimeout(1);
  TOF.setMeasurementTimingBudget(10);
  TOF.startContinuous(0);

  RGBLED.begin();
  RGBLED.setBrightness(LED.bright);
  LED.changeAll(LED.BLUE);
  RGBLED.show();

  // for (int i = 0; i <= 15; i++) {
  //   pinMode(BALL[i], INPUT);
  // }
  pinMode(BALL_RESET, OUTPUT);
  digitalWrite(BALL_RESET, HIGH);
  pinMode(BALL_HOLD, INPUT);

  pinMode(LINE_BRIGHT, OUTPUT);
  for (int i = 0; i <= 19; i++) {
    pinMode(LINE[i], INPUT_PULLUP);
  }

  pinMode(SW_1, INPUT_PULLUP);
  pinMode(SW_2, INPUT_PULLUP);
  pinMode(SW_RESET, INPUT_PULLUP);

  pinMode(SOLENOID, 27);

  if (EEPROM[0] == 1) {
    //緑号機
    device.robot = true;

    LED.defaultColor = LED.GREEN;
  } else {
    //赤号機
    device.robot = false;

    LED.defaultColor = LED.RED;
  }

  gyro.eeprom[0] = (EEPROM[1] * 256) + EEPROM[2];
  gyro.eeprom[1] = (EEPROM[3] * 256) + EEPROM[4];
  gyro.eeprom[2] = (EEPROM[5] * 256) + EEPROM[6];
  gyro.eeprom[3] = (EEPROM[7] * 256) + EEPROM[8];
  gyro.eeprom[4] = (EEPROM[9] * 256) + EEPROM[10];
  gyro.eeprom[5] = (EEPROM[11] * 256) + EEPROM[12];
}

void _device::check(void) {
  RGBLED.show();
  RGBLED.begin();
  RGBLED.clear();

  if (!digitalRead(SW_RESET)) {
    device.mode = 0;
  } else if (!digitalRead(SW_1)) {
    device.mode = 1;
    // analogWrite(LINE_BRIGHT, line.bright);
    digitalWrite(LINE_BRIGHT, HIGH);
  } else if (!digitalRead(SW_2)) {
    device.mode = 1;
    digitalWrite(LINE_BRIGHT, HIGH);
    // analogWrite(LINE_BRIGHT, line.bright);
  }
}

void _device::UI(void) {
  if (!digitalRead(SW_RESET)) {
    RGBLED.setBrightness(LED.bright * 0.2);
    RGBLED.show();
    if (!digitalRead(SW_1)) {
      RGBLED.setBrightness(LED.bright);
      RGBLED.show();
      LED.changeAll(LED.BLUE);
      RGBLED.show();

      device.waitTime(20);  //チャッタリング防止
      while (!digitalRead(SW_RESET)) {
      }
      device.waitTime(200);

      while (true) {
        if (!digitalRead(SW_RESET)) {  //戻るボタン
          break;
        }

        if (!digitalRead(SW_1)) {
          LED.changeAll(LED.ORANGE);
          RGBLED.show();
          gyro.offsetRead();
          gyro.offsetRead();
          gyro.offsetRead();
          LED.animation1();
          device.waitTime(500);
          break;
        }

        if (!digitalRead(SW_2)) {
          LED.changeAll(LED.MINT);
          RGBLED.show();

          while (true) {
            LED.changeAll(LED.MINT);
            RGBLED.show();
            if (!digitalRead(SW_2)) {
              LED.changeAll(LED.LIME);
              RGBLED.show();
              digitalWrite(SOLENOID, HIGH);
              device.waitTime(200);
              digitalWrite(SOLENOID, LOW);
              device.waitTime(1200);
            }

            if (!digitalRead(SW_RESET)) {
              break;
            }
          }
          break;
        }
      }

    } else if (!digitalRead(SW_2)) {
      RGBLED.setBrightness(LED.bright);
      RGBLED.show();
      LED.changeAll(LED.YELLOW);
      RGBLED.show();

      device.waitTime(20);  //チャッタリング防止
      while (!digitalRead(SW_RESET)) {
      }
      device.waitTime(200);

      while (true) {
        if (!digitalRead(SW_RESET)) {  //戻るボタン
          break;
        }

        if (!digitalRead(SW_1)) {
          LED.changeAll(LED.PURPLE);
          RGBLED.show();
          gyro.calibrationEEPROM();
          gyro.offsetRead();
          break;
        }

        if (!digitalRead(SW_2)) {
          LED.changeAll(LED.GREEN);
          RGBLED.show();
          gyro.setting();
          gyro.read();
          LED.animation1();
          gyro.offsetRead();
          device.waitTime(500);
          break;
        }
      }
    }
  } else {
    RGBLED.setBrightness(LED.bright);
    RGBLED.show();
  }
}

unsigned long _device::getTime(void) {
  return millis() * 4;
}

void _device::waitTime(unsigned long _time) {
  delay(_time / 4);
}

void _device::discharge(void) {
  if (!digitalRead(SW_RESET) && !digitalRead(SW_2)) {
    while (true) {
      LED.changeAll(LED.YELLOW);
      RGBLED.show();
      for (int i = 0; i <= 3; i++) {
        motor.val[i] = 255;
      }
      motor.directDrive(motor.val);
      device.waitTime(500);

      for (int i = 0; i <= 3; i++) {
        motor.val[i] = -255;
      }
      motor.directDrive(motor.val);
      device.waitTime(500);
    }
  }
}

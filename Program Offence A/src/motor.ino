_Motor::_Motor(void) {
  for (int i = 0; i < 359; i++) {
    sinVal[0][i] = round(sin(radians(i - 55)));  // 40
    sinVal[1][i] = round(sin(radians(i - 125)));
    sinVal[2][i] = round(sin(radians(i - 225)));
    sinVal[3][i] = round(sin(radians(i - 315)));  // 320
  }
}

int serialCounter = 0;

void _Motor::directDrive(int* p) {
  int data[4] = {0, 0, 0, 0};
  for (int i = 0; i < 4; i++) {
    if (i == 1 || i == 3) {
      data[i] += 0B10000000;
    }

    if (*(p + i) < 0) {
      data[i] += 0B01000000;
      data[i] += constrain(abs(*(p + i)), 0, 60);
    } else if (*(p + i) == 0) {
      data[i] += 0B01111111;
    } else {
      data[i] += 0B00000000;
      data[i] += constrain(abs(*(p + i)), 0, 60);
    }
  }

  serialCounter++;

  if (serialCounter % 2 == 0) {
    Serial4.write(data[0]);
    Serial1.write(data[2]);
    // gyro.deg = gyro.read();
    delay(1);
    Serial4.write(data[1]);
    Serial1.write(data[3]);
  } else {
    Serial4.write(data[1]);
    Serial1.write(data[3]);
    // gyro.deg = gyro.read();
    delay(1);
    Serial4.write(data[0]);
    Serial1.write(data[2]);
  }
}

void _Motor::release(void) {
  serialCounter++;

  if (serialCounter % 2 == 0) {
    Serial4.write(0B10000000);
    Serial1.write(0B10000000);
    delay(1);
    Serial4.write(0B00000000);
    Serial1.write(0B00000000);
  } else {
    Serial4.write(0B00000000);
    Serial1.write(0B00000000);
    delay(1);
    Serial4.write(0B10000000);
    Serial1.write(0B10000000);
  }
}

void _Motor::normalBrake(void) {
  serialCounter++;

  if (serialCounter % 2 == 0) {
    Serial4.write((0B01111111));
    Serial1.write((0B01111111));
    // gyro.deg = gyro.read();
    delay(1);
    Serial4.write((0B11111111));
    Serial1.write((0B11111111));
  } else {
    Serial4.write((0B11111111));
    Serial1.write((0B11111111));
    // gyro.deg = gyro.read();
    delay(1);
    Serial4.write((0B01111111));
    Serial1.write((0B01111111));
  }
}

void _Motor::ultraBrake(void) {
  Serial4.write((0B01111111));
  Serial1.write((0B01111111));
  // gyro.deg = gyro.read();
  delay(1);
  Serial4.write((0B11111111));
  Serial1.write((0B11111111));
}

void _Motor::begin(void) {
  Serial4.begin(115200);  //右
  Serial4.setRx(A1);
  Serial4.setTx(A0);
  Serial4.begin(115200);

  Serial1.begin(115200);  //左
  Serial1.setRx(PA10);
  Serial1.setTx(PA9);
  Serial1.begin(115200);

  //安全処理
  delay(1000);
  Serial4.write(0B11111111);
  Serial1.write(0B11111111);
  delay(10);
  Serial4.write(0B00000000);
  Serial1.write(0B00000000);
  delay(10);
  Serial4.write(0B10000000);
  Serial1.write(0B10000000);

  // Serial4.write(0B00100000);
  // Serial4.write(0B10100000);
  // Serial1.write(0B01100000);
  // Serial1.write(0B11100000);
}

void _Motor::drive(int _deg, int _power, bool _stop = false) {
  //ハードウェアが完成してから実装します
  for (int i = 0; i <= 3; i++) {
    val[i] = 0;
  }

  if (!_stop) {
    // gyro.deg = gyro.read();

    int minimum = 0;

    direction = (gyrodeg - referenceAngle + 360) % 360;
    direction = direction > 180 ? direction - 360 : direction;

    if (abs(direction) >= 50) {
      //ずれすぎ
      _deg = NULL;
      _power = NULL;
    }

    //姿勢制御
    if (_deg == NULL && _power == NULL) {
      Kp = 2.6;
      Ki = 0.007;
      Kd = 1;

      minimum = 70;
    } else {
      // Kp = 1.5;    //比例定数
      // Ki = 0.004;  //積分定数
      Kp = 1.4;
      Ki = 0.012;
      Kd = 0.65;
    }

    //角速度系
    // int angularVelocity = dmpgyro.z;  //角速度？
    int angularVelocity;
    if (abs(angularVelocity) <= 2) {
      angularVelocity = 0;
    }
    if (angularVelocity >= 0 && direction <= 0) {
      angularVelocity = 0;
    } else if (angularVelocity <= 0 && direction >= 0) {
      angularVelocity = 0;
    }
    if (abs(direction) <= 2) {
      angularVelocity = 0;
    }

    if (abs(direction) <= 50)
      integral += direction;

    direction *= Kp * -1;               //比例制御
    direction -= integral * Ki;         //積分制御
    direction -= angularVelocity * Kd;  //積分制御
    if (direction >= 0) {
      direction = constrain(direction, 10 + minimum, 355);
    } else {
      direction = constrain(direction, -355, -10 - minimum);
    }

    //機体が前を向いたら積分していたものをクリアする
    if (gyrodeg <= 5 || gyrodeg >= 355) {
      integral = 0;
      direction = 0;
    }

    if (_deg == NULL && _power == NULL) {
      for (int i = 0; i < 4; i++) {
        val[i] = direction;
      }
    } else {
      for (int i = 0; i < 4; i++) {
        val[i] = calcVal[i][_deg];
      }

      for (int i = 0; i < 4; i++) {
        if (direction >= 0) {
          val[i] *= float(255 - direction) / 255.0;
          val[i] *= float(float(_power) / 100.0);
        } else {
          val[i] *= abs(float(-255 - direction)) / 255.0;
          val[i] *= float(float(_power) / 100.0);
        }
        val[i] += direction;
      }
    }
  }

  for (int i = 0; i <= 3; i++) {
    val[i] = constrain(val[i], -255, 255);
  }

  //   directDrive(val);
}

void _Motor::motorCalc(int _deg, int _power, bool _stop, int _referenceAngle) {
  float Motor[4];
  float Max[2];

  _deg = constrain(_deg, 0, 360);
  _power = constrain(_power, 0, 60);
  _referenceAngle = constrain(_referenceAngle, -180, 180);
  if (abs(_referenceAngle) < 20) {
    //数値は変更
    _referenceAngle = 0;
  }
  float Arrange_deg;
  if (_deg + _referenceAngle >= 360) {
    Arrange_deg = (_deg + referenceAngle) - 360;
  } else if (_deg + _referenceAngle < 0) {
    Arrange_deg = 360 - (_deg + referenceAngle);
  } else {
    Arrange_deg = _deg + _referenceAngle;
  }

  if (_stop) {
    //ストップしたい
  } else {
    // Motor[0]=sin(radians(_deg-60+referenceAngle));
    // Motor[1]=sin(radians(_deg-135+referenceAngle));
    // Motor[2]=sin(radians(_deg-225+referenceAngle));
    // Motor[3]=sin(radians(_deg-300+referenceAngle));
    Motor[0] = sinVal[0][(int)Arrange_deg];
    Motor[1] = sinVal[1][(int)Arrange_deg];
    Motor[2] = sinVal[2][(int)Arrange_deg];
    Motor[3] = sinVal[3][(int)Arrange_deg];

    float _Max;
    for (int i = 0; i < 4; i++) {
      if (abs(Motor[i]) > _Max) {
        _Max = Motor[i];
      }
    }
    Motor[0] = Motor[0] * _power / _Max;
    Motor[1] = Motor[1] * _power / _Max;
    Motor[2] = Motor[2] * _power / _Max;
    Motor[3] = Motor[3] * _power / _Max;

    for (int i = 0; i < 4; i++) {
      Kval[i] = Motor[i];
    }
  }
}

void _Motor::motorPID_drive(int motor_speed) {
  float Collection;  //スピード算出値

  //角度オーバー修正
  if (gyro.deg > 180) {
    Collection = gyro.deg - 360;
  } else {
    Collection = gyro.deg;
  }

  // P制御（比例）
  Collection *= -0.53;  // P制御 0.078 Mizunami 0.072(0.9) or 81(09) 67 0.043

  // D制御（微分）
  // Collection -= gyro.differentialRead() * 0.024;  // D制御 64 0.012
  // Collection = constrain(Collection, -30, 30);

  int max = 60 - abs(Collection);

  // motorCalc(_Mdegree, max, false, 0);

  // D制御（微分）
  // Collection -= gyro.differentialRead() * 0.024;  // D制御 64 0.012
  if(_Mdegree!=1000){
  motor.val[0] = max * sin(radians((_Mdegree + 360 - 55) % 360));
  motor.val[1] = max * sin(radians((_Mdegree + 360 - 135) % 360));
  motor.val[2] = max * sin(radians((_Mdegree + 135) % 360));
  motor.val[3] = max * sin(radians((_Mdegree + 55) % 360));
  }else{
    for(int i=0; i<4; i++){
      motor.val[i]=0;
    }
  }
  for (int i = 0; i < 4; i++) {
    //大きすぎるのを防止
    motor.val[i] += Collection * 0.7;
    motor.val[i] = constrain(motor.val[i], -60, 60);
  }

  motor.directDrive(motor.val);
  // motor.release();
}
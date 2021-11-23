_Motor::_Motor(void) {
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
}

void _Motor::directDrive(int* p){

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

    if (abs(direction) <= 50) integral += direction;

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

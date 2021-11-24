_Motor::_Motor(void) {
    for(int i=0; i<359; i++){
        sinVal[0][i]=round(sin(radians(i-60)));
        sinVal[1][i]=round(sin(radians(i-135)));
        sinVal[2][i]=round(sin(radians(i-225)));
        sinVal[3][i]=round(sin(radians(i-300)));
    }
//   for (int i = 0; i <= 359; i++) {
//     float s;
//     motor.calcVal[0][i] = round(sin(radians(i - 300)) * 100.0);
//     motor.calcVal[1][i] = round(sin(radians(i - 60)) * 100.0);
//     motor.calcVal[2][i] = round(sin(radians(i - 225)) * 100.0);
//     motor.calcVal[3][i] = round(sin(radians(i - 135)) * 100.0);

//     int valTemp[4];
//     for (int k = 0; k < 4; k++) {
//       valTemp[k] = motor.calcVal[k][i];
//     }

//     for (int k = 0; k < 4; ++k) {
//       for (int j = k + 1; j < 4; ++j) {
//         if (abs(valTemp[k]) >= abs(valTemp[j])) {
//           int temp = valTemp[k];
//           valTemp[k] = valTemp[j];
//           valTemp[j] = temp;
//         }
//       }
//     }
//     s = 255.0 / float(abs(valTemp[3]));
//     motor.calcVal[0][i] = round((float)motor.calcVal[0][i] * s);
//     motor.calcVal[1][i] = round((float)motor.calcVal[1][i] * s);
//     motor.calcVal[2][i] = round((float)motor.calcVal[2][i] * s);
//     motor.calcVal[3][i] = round((float)motor.calcVal[3][i] * s);
//   }
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

void _Motor::motorCalc(int _deg,int _power,bool _stop,int _referenceAngle){
    float Motor[3];
    float Max[2];

    _deg=constrain(_deg,0,360);
    _power=constrain(_power,0,100);
    _referenceAngle=constrain(_referenceAngle,-180,180);
    if(abs(_referenceAngle)<20){
        //数値は変更
        _referenceAngle=0;
    }
    float Arrange_deg;
    if(_deg+_referenceAngle>=360){
        Arrange_deg=(_deg+referenceAngle)-360;
    }else if(_deg+_referenceAngle<0){
        Arrange_deg=360-(_deg+referenceAngle);
    }else{
        Arrange_deg=_deg+_referenceAngle;
    }

    if(_stop){
        //ストップしたい
    }else{
        // Motor[0]=sin(radians(_deg-60+referenceAngle));
        // Motor[1]=sin(radians(_deg-135+referenceAngle));
        // Motor[2]=sin(radians(_deg-225+referenceAngle));
        // Motor[3]=sin(radians(_deg-300+referenceAngle));
        Motor[0]=sinVal[0][Arrange_deg];
        Motor[1]=sinVal[1][Arrange_deg];
        Motor[2]=sinVal[2][Arrange_deg];
        Motor[3]=sinVal[3][Arrange_deg];
        
        float _Max;
        for(int i=0; i<4; i++){
            if(abs(Motor[i])>_Max){
                _Max=Motor[i];
            }
        }
        Motor[0]=Motor[0]*_power/_Max;
        Motor[1]=Motor[1]*_power/_Max;
        Motor[2]=Motor[2]*_power/_Max;
        Motor[3]=Motor[3]*_power/_Max;
    }
}
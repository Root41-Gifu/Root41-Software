_Ball::_Ball() {
  for (int i = 0; i < BALL_NUM; i++) {
    vectorX[i] = sin(radians(i * 22.5));
    vectorY[i] = cos(radians(i * 22.5));
  }
}

// int _Ball::Max_calc(float _value[16]) {
//     int _Max=100;
//     for(int i=0; i<16; i++){
//         if(_value[i]>_value[_Max]||_Max==100){
//             _Max=i;
//         }
//     }
//     return _Max;
// }

void _Ball::Max_calc(float* _value) {
  int _Max[3];
  _Max[0] = 100;
  _Max[1] = 100;
  _Max[2] = 100;
  for (int i = 0; i < 16; i++) {
    if (_value[i] > _value[_Max[0]] || _Max[0] == 100) {
      _Max[2] = _Max[1];
      _Max[1] = _Max[0];
      _Max[0] = i;
    } else if (_value[i] > _value[_Max[1]] || _Max[1] == 100) {
      _Max[2] = _Max[1];
      _Max[1] = i;
    } else if (_value[i] > _value[_Max[0]] || _Max[0] == 100) {
      _Max[2] = i;
    }
  }
  max[0] = _Max[0];
  max[1] = _Max[1];
  max[2] = _Max[2];
}

void _Ball::SPI_read(void) {
  // // for(int i=0; i<32; i++){
  //     digitalWrite(PB5,LOW);
  //     int data = SPI.transfer(0);
  //     digitalWrite(PB5, HIGH);
  //     // if(data<16){
  //     //     readp=data;
  //     // }else{
  //     //     value[readp]=data;
  //     // }
  // value[0]=data;
  // // }
  SPI.begin();
  int data;
  unsigned long starTime = micros();
  digitalWrite(PB5, LOW);
  // SPI.transfer(-);
  for (int i = 0; i < 33; i++) {
    data = SPI.transfer(0);
    delayMicroseconds(10);
    if (data < 16) {
      readp = data;
    } else if (i != 0) {
      value[readp] = data;
    }
  }
  value[6] = (value[5] + value[8]) / 2;
  value[7] = (value[5] + value[8]) / 2;
  if (value[8] > 100) {
    value[8] * 1.7;
  }
  if (value[9] > 100) {
    value[9] * 1.4;
  }
  digitalWrite(PB5, HIGH);
  unsigned long endTime = micros();
  SPI.end();
  // if(data==0){
  //   readp=0;
  //   readp++;
  // }else{
  //   if(data>10){//誤動作の値削除
  //     value[readp-1]=data;
  //   }
  //   readp++;
  //   if(readp>16){
  //     readp=0;
  //   }
  // }
}

void _Ball::calcDistance(void) {
  for (int i = 0; i < BALL_NUM; i++) {
    dist[i] = LPF_value[i];
  }
  distance = 0;
  distance += dist[max[0]];
  if (max[0] - 1 < 0) {
    distance += dist[15];
  } else {
    distance += dist[max[0] - 1];
  }
  if (max[0] + 1 > 15) {
    distance += dist[0];
  } else {
    distance += dist[max[0] + 1];
  }
  if (LPF_value[max[0]] > 15) {
    int _Level[3];
    if (max[0] < 8) {
      _Level[3] = 140;
      _Level[2] = 230;
    } else if (max[0] == 8) {
      _Level[3] = 140;
      _Level[2] = 180;
    } else if (max[0] < 12) {
      _Level[3] = 160;
      _Level[2] = 230;
    } else {
      _Level[3] = 170;
      _Level[2] = 230;
    }
    // }else if(max[0]>11){
    //   _Level[3]=250;
    //   _Level[2]=290;
    // }else if(max[0]<9){
    //   _Level[3]=110;
    //   _Level[2]=170;
    // }else{
    //   _Level[3]=180;
    //   _Level[2]=260;
    // }
    if (ball.distance < _Level[3]) {
      distanceLevel = 3;
    } else if (ball.distance < _Level[2]) {
      distanceLevel = 2;
    } else {
      distanceLevel = 1;
    }
    if (distance < 110 && ball.distance != 0) {
      distanceLevel = 1;
    }
  } else {
    distanceLevel = 0;
  }
}

void _Ball::calcDirection(void) {
  vectortX = vectorX[max[0]];
  vectortY = vectorY[max[0]];
  if (max[1] != 100) {
    vectortX += vectorX[max[1]] * 0.5;
    vectortY += vectorY[max[1]] * 0.5;
  } else if (max[2] != 100) {
    vectortX += vectorX[max[2]] * 0.2;
    vectortY += vectorY[max[2]] * 0.2;
  }

  degree = degrees(atan2(vectortX, vectortY));
  if (degree < 0) {
    degree += 360;
  }
  //右が（0~180）、左が(-180~0)
}

void _Ball::average(void) {
  if (millis() - averageTimer > 50) {
    int lastmax_average[3];
    lastmax_average[0] = max_average[0];
    lastmax_average[1] = max_average[1];
    lastmax_average[2] = max_average[2];
    max_average[0] = 100;
    max_average[1] = 100;
    max_average[2] = 100;
    for (int i = 0; i < BALL_NUM; i++) {
      if (averageCounter[max_average[0]] < averageCounter[i] ||
          max_average[0] == 100) {
        max_average[2] = max_average[1];
        max_average[1] = max_average[0];
        max_average[0] = i;
      } else if (averageCounter[max_average[1]] < averageCounter[i] ||
                 max_average[1] == 100) {
        max_average[2] = max_average[1];
        max_average[1] = i;
      } else if (averageCounter[max_average[2]] < averageCounter[i] ||
                 max_average[2] == 100) {
        max_average[2] = i;
      }
    }

    if (averageCounter[max_average[0]] < averageCounter[16]) {
      max_average[0] = 100;  //ない判定
    } else if (averageCounter[max_average[0]] == 0) {
      max_average[0] = lastmax_average[0];
    }
    for (int i = 0; i < BALL_NUM + 1; i++) {
      averageCounter[i] = 0;
    }
    averageTimer = millis();
  } else {
    if (max[0] != 100) {
      averageCounter[max[0]] += 4;
      if (max[1] != 100) {
        averageCounter[max[1]] += 2;
        if (max[2] != 100) {
          averageCounter[max[2]] += 1;
        }
      }
    } else {
      averageCounter[16]++;
    }
  }
}

void _Ball::calc(void) {
  //簡単な方向、距離の分割プログラム
  int _degree;
  int move_16[3][16] = {{0, 22, 60, 110, 150, 180, 210, 240, 270, 120, 150, 180,
                         210, 250, 300, 337},
                        {0, 20, 40, 150, 180, 200, 210, 260, 260, 100, 150, 160,
                         180, 210, 320, 340},
                        {0, 20, 40, 150, 180, 200, 210, 260, 260, 100, 150, 160,
                         180, 210, 320, 340}};
  // for(int i=0; i<16; i++){
  //   move_16[0][i]=i*22.5;
  // }
  if (max[0] == 100) {
    _degree = 1000;
  } else {
    float _plusvector[2];
    float gain_constant = 6;
    int max_gain = 140;
    int gain_degree;

    if (vectortX > 0) {
      gain_degree = map(ball.degree, 0, 180, 0, max_gain);
      _plusvector[0] = vectortX + sin_d[degree + gain_degree] * gain_constant;
      _plusvector[1] = vectortY + cos_d[degree + gain_degree] * gain_constant;
      _degree = degrees(atan2(_plusvector[0], _plusvector[1]));
      if (_degree < 0) {
        _degree += 360;
      }
    } else {
      gain_degree = map(ball.degree, 180, 360, max_gain, 0);
      _plusvector[0] = vectortX + sin_d[degree - gain_degree] * gain_constant;
      _plusvector[1] = vectortY + cos_d[degree - gain_degree] * gain_constant;
      _degree = degrees(atan2(_plusvector[0], _plusvector[1]));
      if (_degree < 0) {
        _degree += 360;
      }
    }
    Serial.print(vectortX);
    Serial.print(" ");
    Serial.print(vectortY);
    Serial.print(" ");
    Serial.print(sin_d[degree+gain_degree]);
    Serial.print(" ");
    Serial.print(cos_d[degree+gain_degree]);
    Serial.print(" ");
    Serial.print(degree);
    Serial.print(" ");
    Serial.print(gain_degree);
    Serial.println(" ");
    // _degree = degree;
    if (distanceLevel == 0) {
      _degree = 1000;
    } else {
      if (distance != 1) {
        // _degree = move_16[2][max[0]];
      } else {
        // _degree=move_16[2][max[0]];
        // _degree = degree;
      }
    }
    // _degree = move_16[max[0]][distanceLevel];
  }
  Move_degree = _degree;
  // if (Move_degree < 0) {
  //   Move_degree = 360 + Move_degree;
  // }
}

void _Ball::LPF(void) {
  float k;
  // LPF調整ファイト
  for (int i = 0; i < 16; i++) {
    if (abs(value[i] - LPF_value[i]) > 30) {
      k = 0.06;  // 0.07
      if (value[i] - LPF_value[i] < -30) {
        k = 0.15;  // 0.15
      }
    } else {
      k = 0.03;  // 0.07
    }
    LPF_value[i] += k * (value[i] - LastLPF[i]);
    LastLPF[i] = LPF_value[i];
  }
}

int _Ball::adjustValue(int number, int originalValue) {
  // 0,100~
  // 1,110~
  // 2,120~
  // 3,125~ ときどき80~もある
  // 4,130~
  // 5,140
  // 8,110~
  // 9,110~
  // 10,110~ 真正面80~
  // 11,100~
  // 12.120~
  // 13.120~ 真正面0も
  // 14.120~
  int adjustment;
  switch (number) {
    case 0:
      if (originalValue > 100) {
        adjustment = originalValue + 10;
        break;
      }
    case 8:
      if (originalValue > 80) {
        adjustment = originalValue + 20;
        break;
      }
    case 11:
      if (originalValue < 110 && originalValue > 60) {
        adjustment = originalValue - 20;
        if (value[8] > 120) {
          adjustment -= 15;
        }
        break;
      }
    case 12:
      if (originalValue < 110 && originalValue > 60) {
        adjustment = originalValue - 20;
        if (value[8] > 120) {
          adjustment -= 15;
        }
        break;
      }
    case 14:
      if (originalValue > 90) {
        adjustment = originalValue + 35;
      }
    default:
      adjustment = originalValue;
      break;
  }
  return adjustment;
}
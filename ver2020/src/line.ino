// ISR(timer5Event) {
//   //ラインの記述はここに
//   line.read();
// }

_line::_line(void) {
  for (int i = 0; i <= 19; i++) {
    val[i] = false;
  }
}

int _line::calc(void) {
  float _deg;
  float _x = 0;
  float _y = 0;
  if (flag) {
    for (int i = 0; i <= 11; i++) {  //可変
      if (order[i] != 100) {
        _x += vector[order[i]][0];
        _y += vector[order[i]][1];
      }
    }
    if (_x == 0 || _y == 0) {
      if (_x == 0) {
        if (_y > 0) {
          _deg = 0;
        } else {
          _deg = 180;
        }
      } else if (_y == 0) {
        if (_x > 0) {
          _deg = 90;
        } else {
          _deg = 270;
        }
      }
    } else {
      _deg = atan2(_x, _y);
      _deg = degrees(_deg);
      if (_deg < 180) {
        _deg += 180;
      } else {
        _deg -= 180;
      }
    }
  } else {
    _deg = 1000;
  }
  return _deg;
}

void _line::process(void) {
  speed = 100;
  if (flag) {
    if (deg != 1000) {
      //ラインタッチ方向判定（四方向）
      if (deg <= 35 || deg >= 325) {
        side = 0;  //↑
      } else if (deg >= 145 && deg <= 215) {
        side = 2;  //↓
      } else if (deg < 180) {
        side = -1;  //→
      } else if (deg > 180) {
        side = 1;  //←
      }
    }
    if (_mode == 0) {
      // none
      flag = false;
    } else if (_mode == 1) {
      // stop
      deg = 1000;
      if (!touch) {
        //ストップ中にオーバーしたら緊急でもどる
        overTimer = device.getTime();
        _mode = 3;
      }
      if (device.getTime() - stopTimer >= 300) {
        //通常の動きに移行
        _mode = 2;
      }
      //センサーのタイマーでアプローチするかを判断
      approach = false;
      for (int i = 0; i <= 19; i++) {
        if (stopTime[i] >= 70) {  // 150
          if (abs(side) == 1) {
            if (ball.top >= 2 && ball.top <= 5) {
              if (abs(side) == 1) {
                _mode = 2;
                approachdeg = ball.top * 22.5;
                // approach = true;
              }
            } else if (ball.top >= 11 && ball.top <= 14) {
              if (abs(side) == 1) {
                _mode = 2;
                approachdeg = ball.top * 22.5;
                // approach = true;
              }
            }
          }
        }
      }
    } else if (_mode == 2) {
      // move
      if (approach) {
        _mode = 4;
      }
      if (touch && side == 2) {
        // overrun
        // if (device.getTime() - stopTimer >= 800) {
        //   if (x >= 0) {
        //     deg = 225;
        //   } else {
        //     deg = 135;
        //   }
        // }
      }
      if (!touch) {
        overTimer = device.getTime();
        _mode = 0;
      }
    } else if (_mode == 3) {
      // over
      // if (approachdeg >= 180) {
      //   deg = approachdeg - 180;
      // } else {
      //   deg = approachdeg + 180;
      // }
      if (touch) {
        _mode = 2;
      }
    } else if (_mode == 4) {
      // approach
      if (ball.deg != 1000) {
        if (touch) {
          deg = approachdeg;
          speed = 80;
        } else {
          approach = false;
          _mode = 3;
        }
      } else {
        _mode = 2;
      }
    }
  } else {
    //リセット
    for (int i = 0; i <= 19; i++) {
      val[i] = false;
      order[i] = 100;
      check[i] = 0;
      stopTime[i] = 0;
    }
    s = false;
    approach = false;
    now = 100;
    first = 100;
    whited = 0;
    side = 0;
    _mode = 0;
    error = 0;
  }
}

void _line::brightnessAdjust(void) {
  int lowestBright = 255;  //最低値
  int highestBright = 255;
  int accumulation = 0;
  bool reacted[20] = {};

  for (int i = 0; i <= 255; i++) {
    analogWrite(46, i);
    for (int j = 0; j <= 19; j++) {
      pinMode(LINE[j], INPUT);
      bool val = !digitalRead(LINE[j]);
      if (val) {
        if (lowestBright == 255) {
          lowestBright = i;
        }
        if (!reacted[j]) {
          accumulation++;
        }
        reacted[j] = true;
      }
      //////Serial.print(val);
    }
    //Serial.println("");
    if (accumulation >= 8) {
      highestBright = i;
      // break;
    }
    delay(0);
  }

  bright = (lowestBright + highestBright * 2) / 3;
  bright = constrain(bright, 0, 255);
  EEPROM[13] = bright;

  analogWrite(LINE_BRIGHT, bright);

  //Serial.println(bright);
}

void _line::read(void) {
  //読み込み
  touch = false;
  whiting = 0;
  for (int i = 0; i <= 19; i++) {
    if (!digitalRead(LINE[i])) {
      whiting++;
      just = i;
      if (whited == 0) {
        first = i;
      }
      if (check[i] == 0) {
        now = i;
        order[whited] = now;
        whited++;
        check[i] = 1;
      }
      if (!flag) {
        stopTimer = device.getTime();
        first = now;
        _mode = 1;
      }
      if (!val[i]) {
        stopingTimer[i] = device.getTime();
      }
      stopTime[i] = device.getTime() - stopingTimer[i];
      flag = true;
      val[i] = true;
      touch = true;
    } else {
      val[i] = false;
    }
  }
}

void _line::linetrace(void) {
  if (!keeper.setup) {
    if (touch) {
      keeper.mode = 0;
    } else {
      device.mode = 1;
      //とりあえず
    }
    keeper.setup = true;
  }
  if (keeper.mode == 0) {
    // inline&&moving
    if (line.whiting <= 1) {
      keeper.mode = 3;
      // if (abs(180 - gyro.deg) <= 120) {
      //   line.gyrobreak = true;
      // } else {
      //   line.gyrobreak = false;
      // }
      keeper.offTimer = device.getTime();
    }
    if (!digitalRead(BALL_HOLD)) {
      ball.holdTimer = device.getTime();
      kicker.val = false;
      // position = 0;
    }
    if (digitalRead(BALL_HOLD) && !(ball.top > 3 && ball.top < 13)) {
      if (device.getTime() - ball.holdTimer >= 70) {
        kicker.val = true;
        // if (motor.referenceAngle != 0) {
        //   if (device.getTime() - holdTimer < 300) {
        //     kicker.val = false;
        //   }
        // }
        // if (device.getTime() - speedTimer >= 600 || speedTimer == 0) {
        //   speedTimer = device.getTime();
        // }
      }
    }
    if (keeper.modein >= 5 && keeper.frontmode &&
        device.getTime() - keeper.banTimer >= 3000) {
      keeper.mode = 2;
      keeper.atackTimer = device.getTime();
    }
    // else if (keeper.modein >= 8 && keeper.frontmode &&
    //            device.getTime() - keeper.banTimer >= 3000) {
    //   keeper.mode = 4;
    //   keeper.atackTimer = device.getTime();
    // }
    if (keeper.count >= 50) {
      if (keeper.frontball >= 45) {
        keeper.Front = true;
        keeper.modein++;
      } else {
        keeper.Front = false;
        keeper.modein = 0;
      }
      keeper.frontball = 0;
      keeper.count = 0;
    } else {
      if (ball.top < 2 || ball.top > 14) {
        keeper.frontball++;
      }
      keeper.count++;
    }
    keeper.frontmode = false;
    if (ball.top < 2 || ball.top > 14) {
      line.deg = 1000;
      keeper.frontmode = true;
      // motor.referenceAngle = 0;
    } else if (ball.top < 4 || ball.top > 12) {
      if (ball.top < 8) {
        line.deg = 90;
        // motor.referenceAngle = ball.top * 22.5;
      } else {
        line.deg = 270;
        // motor.referenceAngle = ball.top * 22.5;
      }
    } else if (ball.top < 8) {
      line.deg = 90;
    } else {
      line.deg = 270;
    }
    int checkFront = 0;
    for (int i = 0; i <= 19; i++) {
      if (i < 4 || i > 14) {
        if (line.val[i]) {
          checkFront++;
        }
      }
    }
    if (checkFront < 2 && whiting > 0 && line.deg != 1000) {
      if (line.deg < 180) {
        line.deg += 20;
      } else {
        line.deg -= 20;
      }
    }
  } else if (keeper.mode == 1) {
    // inline&&touching
  } else if (keeper.mode == 2) {
    // offline&&attacking
    if (device.getTime() - keeper.atackTimer <= 100) {
      line.deg = 0;
    } else {
      keeper.mode = 0;
    }
    keeper.banTimer = device.getTime();
  } else if (keeper.mode == 3) {
    // offline&&missing
    if (gyrobreak && whiting <= 3) {
      line.deg = 180;
    } else if (whiting <= 3) {
      line.deg = just * 18;
      if (just >= 4 && just <= 16) {
        if (line.deg < 180) {
          line.deg -= 35;
        } else {
          line.deg += 35;
        }
      }
      if (device.getTime() - keeper.offTimer >= 500) {
        line.deg = 180;
      }
      // if (device.getTime() - keeper.offTimer >= 300) {
      //   if (just >= 3 && just <= 5) {
      //     line.deg = 20;
      //   } else if (just >= 13 && just <= 16) {
      //     line.deg = 340;
      //   }
      // }
    } else {
      line.deg = 1000;
      keeper.mode = 0;
    }
  } else if (keeper.mode == 4) {
    // offline&&going
    if (device.getTime() - keeper.atackTimer <= 1000) {
      line.deg = 0;
    } else {
      keeper.mode = 0;
    }
    keeper.banTimer = device.getTime();
  }
}
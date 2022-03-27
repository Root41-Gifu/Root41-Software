_Line::_Line() {
  for (int i = 0; i < 20; i++) {
    value[i] = false;
  }
  // for (int i = 0; i < 20; i++) {
  //   _vectorX[i] = sin(radians(i * 18));
  //   _vectorY[i] = cos(radians(i * 18));
  // }
  // for (int i = 0; i < 4; i++) {
  //   _vectorX[i] = sin(radians(i * 90));
  //   _vectorY[i] = cos(radians(i * 90));
  // }
  //   block_vectorX[0] = sin(radians(0));
  // block_vectorY[0] = cos(radians(0));
  // block_vectorX[1] = sin(radians(0));
  // block_vectorY[1] = cos(radians(0));
  // block_vectorX[2] = sin(radians(180));
  // block_vectorY[2] = cos(radians(180));
  // block_vectorX[3] = sin(radians(180));
  // block_vectorY[3] = cos(radians(180));
  // block_vectorX[4] = sin(radians(270));
  // block_vectorY[4] = cos(radians(270));
  // block_vectorX[5] = sin(radians(270));
  // block_vectorY[5] = cos(radians(270));
  // block_vectorX[6] = sin(radians(90));
  // block_vectorY[6] = cos(radians(90));
  // block_vectorX[7] = sin(radians(90));
  // block_vectorY[7] = cos(radians(90));
  for (int i = 0; i < LINE_NUM; i++) {
    if (i < LINE_FRONTNUM) {
      Line_Where[i] = 0;
    } else if (i < LINE_FRONTNUM + LINE_REARNUM) {
      Line_Where[i] = 2;
    } else if (i < LINE_FRONTNUM + LINE_REARNUM + LINE_LEFTNUM) {
      Line_Where[i] = 4;
    } else if (i < LINE_FRONTNUM + LINE_REARNUM + LINE_LEFTNUM + LINE_REARNUM) {
      Line_Where[i] = 6;
    }
  }
  for (int i = 1; i <= 9; i++) {
    Line_Where[i] = 0;
  }
  Line_Where[1] = 1;
  Line_Where[4] = 1;
  Line_Where[8] = 1;
  // for (int i = 10; i <= 15; i++) {
  //   Line_Where[i] = 2;
  // }
  // for (int i = 16; i <= 18; i++) {
  //   Line_Where[i] = 3;
  // }
  Line_Where[10] = 2;
  Line_Where[12] = 2;
  Line_Where[13] = 2;
  Line_Where[15] = 2;
  Line_Where[16] = 2;
  Line_Where[18] = 2;
  Line_Where[11] = 3;
  Line_Where[14] = 3;
  Line_Where[17] = 3;
  for (int i = 19; i <= 21; i++) {
    Line_Where[i] = 5;
  }
  Line_Where[25] = 5;
  Line_Where[27] = 5;
  for (int i = 22; i <= 24; i++) {
    Line_Where[i] = 4;
  }
  Line_Where[26] = 4;
  for (int i = 28; i <= 30; i++) {
    Line_Where[i] = 4;
  }
  Line_Where[31] = 5;
  Line_Where[32] = 4;
  for (int i = 33; i <= 34; i++) {
    Line_Where[i] = 6;
  }
  for (int i = 35; i <= 40; i++) {
    Line_Where[i] = 7;
  }
}

int readCounter = 0;

void _Line::read(void) {
  readCounter++;
  readCounter %= 4;
  int bitSelect;

  if (readCounter == 0) {
    Wire.requestFrom(LINE_FRONTADDRESS, 2);  //アドレスは変えてね

    while (Wire.available() >= 2) {
      byte readValue[2];
      readValue[0] = i2cReadWithTimeoutFunction();
      readValue[1] = i2cReadWithTimeoutFunction();
      for (int i = 0; i < 3; i++) {
        value[bitSelect] = readValue[0] & (1 << i + 1);
        bitSelect++;
      }
      for (int i = 0; i < 3; i++) {
        value[bitSelect] = readValue[0] & (1 << i + 5);
        bitSelect++;
      }
      value[bitSelect] = readValue[1] & (1 << 3);
      bitSelect++;
      for (int i = 0; i < 3; i++) {
        value[bitSelect] = readValue[1] & (1 << i + 5);
        bitSelect++;
      }
    }
  } else if (readCounter == 1) {
    Wire.requestFrom(LINE_REARADDRESS, 2);  //アドレスは変えてね

    while (Wire.available() >= 2) {
      byte readValue[2];
      readValue[0] = i2cReadWithTimeoutFunction();
      readValue[1] = i2cReadWithTimeoutFunction();
      for (int i = 0; i < 3; i++) {
        value[bitSelect] = readValue[0] & (1 << i + 1);
        bitSelect++;
      }
      value[bitSelect] = readValue[0] & (1 << 5);
      bitSelect++;
      for (int i = 0; i < 2; i++) {
        value[bitSelect] = readValue[1] & (1 << i + 2);
        bitSelect++;
      }
      for (int i = 0; i < 3; i++) {
        value[bitSelect] = readValue[1] & (1 << i + 5);
        bitSelect++;
      }
    }
  } else if (readCounter == 2) {
    Wire.requestFrom(LINE_LEFTADDRESS, 2);  //アドレスは変えてね
    while (Wire.available() >= 2) {
      byte readValue[2];
      readValue[0] = i2cReadWithTimeoutFunction();
      readValue[1] = i2cReadWithTimeoutFunction();
      for (int i = 0; i < 8; i++) {
        value[bitSelect] = readValue[0] & (1 << i);
        bitSelect++;
      }
      for (int i = 0; i < 6; i++) {
        value[bitSelect] = readValue[1] & (1 << i + 2);
        bitSelect++;
      }
    }
  } else {
    Wire.requestFrom(LINE_RIGHTADDRESS, 2);  //アドレスは変えてね

    while (Wire.available() >= 2) {
      byte readValue[2];
      readValue[0] = i2cReadWithTimeoutFunction();
      readValue[1] = i2cReadWithTimeoutFunction();
      for (int i = 0; i < 3; i++) {
        value[bitSelect] = readValue[0] & (1 << i + 1);
        bitSelect++;
      }
      value[bitSelect] = readValue[0] & (1 << 5);
      bitSelect++;
      value[bitSelect] = readValue[0] & (1 << 7);
      bitSelect++;
      value[bitSelect] = readValue[1] & (1 << 3);
      bitSelect++;
      value[bitSelect] = readValue[1] & (1 << 5);
      bitSelect++;
      value[bitSelect] = readValue[1] & (1 << 7);
      bitSelect++;
    }
  }
  value[41] = true;
  value[42] = true;
}

void _Line::arrange(void) {
  //読み込み後のデータ抽出、整理

  //リセット等
  touch = false;
  whiting = 0;
  for (int i = 0; i < 8; i++) {
    detect_num[i] = 0;
  }

  //角度のずれ
  if (gyro.deg < 180) {
    current_degree = gyro.deg;
  } else {
    current_degree = gyro.deg - 360;
  }

  //センサーごとの整理
  for (int i = 0; i < LINE_NUM; i++) {
    if (!value[i]) {
      //反応してたら
      if (!check[i]) {
        //過去に反応なし
        order[whited] = i;
        whited++;
        check[i] = true;
        passed_num[Line_Where[i]]++;
      }

      if (!checkBlock[Line_Where[i]] && passed_num[Line_Where[i]] > 0) {
        //そのブロックが過去に反応なし
        // 0だと誤反応の可能性あり、増やしてもいいかも
        checkBlock[Line_Where[i]] = true;
        orderBlock[Block] = Line_Where[i];
        Block++;
      }

      if (!flag) {
        InTimer = millis();
        //ラインフラグなし
        //   stopTimer = device.getTime();
        mode = 1;
        flag = true;

        //角度のずれ
        if (gyro.deg < 180) {
          reference_degree = gyro.deg;
        } else {
          reference_degree = gyro.deg - 360;
        }

        //オーバーシュート時にもどる
        if (millis() - OutTimer <= LINEOVERTIME) {
          if (whited > LINEOVERNUM) {
            rdegree = leftdegree;
          } else {
            odegree = leftdegree;
          }
        } else {
          Rflag = false;
        }
      }

      whiting++;
      touch = true;
      flag = true;
      Rflag = false;
      Oflag = false;
      detect_num[Line_Where[i]]++;
      OutTimer = millis();
    }
  }

  if (!touch) {
    flag = false;
  }

  //モード振り分け
  if (flag) {
    if (Block <= 2) {
      mode = 1;
    } else if (abs(reference_degree) < 50) {
      mode = 2;
    } else {
      mode = 3;
    }
  } else {
    mode = 0;
  }

  //ラインオフの時
  if (!flag) {
    if (millis() - OutTimer > LINEOVERTIME && whited >= LINEOVERNUM) {
      Rflag = false;
      Oflag = false;
      flag = false;
      leftdegree = 1000;
      rdegree = 1000;
      odegree = 1000;
    } else if (millis() - OutTimer > LINERETURNTIME && whited < LINEOVERNUM) {
      Rflag = false;
      Oflag = false;
      flag = false;
      leftdegree = 1000;
      rdegree = 1000;
    } else if (whited >= LINEOVERNUM) {
      Rflag = true;
      Oflag = false;
    } else {
      Rflag = false;
      Oflag = true;
    }
    if (!Rflag) {
      for (int i = 0; i < 8; i++) {
        orderBlock[i] = 100;
        checkBlock[i] = 0;
        detect_num[i] = 0;
        passed_num[i] = 0;
      }

      Block = 0;
      whited = 0;
      whiting = 0;
      for (int i = 0; i < LINE_NUM; i++) {
        order[i] = 100;
        check[i] = 0;
      }
      reference_degree = 0;
      current_degree = 0;
    }
  } else {
    Rflag = false;
    Oflag = false;
  }
}

int _Line::calcDirection(void) {
  int _degree;  //ベクトルに範囲
  t_vectorX = 0;
  t_vectorY = 0;
  int count = 0;
  for (int i = 0; i < 8; i++) {
    for (int i = 0; i < 12; i++) {
      if (i < whited) {
        t_vectorX += block_vectorX[Line_Where[order[i]]];
        t_vectorY += block_vectorY[Line_Where[order[i]]];
      }
    }
  }
  _degree = degrees(atan2(t_vectorX, t_vectorY));
  // Serial.print(_degree);
  return _degree;
}

void _Line::calc(void) {
  int _degree = 1000;
  if (flag) {
    t_vectorX = 0;
    t_vectorY = 0;
    current_degree = 0;  // kese
    if (mode == 1) {
      //少数反応
      _degree = calcDirection() - current_degree;
      // _degree=Block_degree[orderBlock[0]];
    } else if (mode == 2) {
      //ずれ少ない多数反応
      _degree = calcDirection() - current_degree;
      // if(abs(orderBlock[0]-orderBlock[1])==4){
      //連番　直線的な可能性
      //角度修正ありにしたい＜
      // _degree=Block_degree[orderBlock[0]]-current_degree;
      // }
      // else if(abs(orderBlock[0]-orderBlock[1])==1){
      //   //横での連続
      //   _degree=(Block_degree[orderBlock[0]]*2+Block_degree[orderBlock[1]])/2-current_degree;
      // }else{
      //   if(orderBlock[0]==3){
      //     if(orderBlock[1]==0){
      //       _degree=(Block_degree[orderBlock[0]]*2+Block_degree[orderBlock[1]])/2-current_degree;
      //     }
      //   }
      //   if(orderBlock[0]==0){
      //     if(orderBlock[1]==3){
      //       _degree=(Block_degree[orderBlock[0]]*2+Block_degree[orderBlock[1]])/2-current_degree;
      //     }
      //   }
      // }
    } else if (mode == 3) {
      //傾き杉
      _degree = calcDirection() - current_degree;
      // _degree=Block_degree[orderBlock[0]]-current_degree;
    } else if (mode == 4) {
      //オーバー　
      _degree = calcDirection() - current_degree;
      // _degree=totaldegree-current_degree;
    }
    // t_vectorX = 0;
    // t_vectorY = 0;
    // for (int i = 0; i < 3; i++) {
    //   if (orderBlock[i] != 100) {
    //     t_vectorX += block_vectorX[orderBlock[i]] * (1 - i * 0.3);
    //     t_vectorY += block_vectorY[orderBlock[i]] * (1 - i * 0.3);
    //   }
    //
    // if (orderBlock[0] != 100) {
    //   _degree = degrees(atan2(t_vectorX, t_vectorY));

    //   if (mode == 1) {
    //     motor.reference_degree = 0;  //角度変えながら
    //     _degree -= current_degree;   //最新のに合わせる
    //   } else if (mode == 2) {
    //     motor.reference_degree = reference_degree;
    //     _degree -= current_degree;
    //   }

    //   if (_degree >= 0) {
    //     _degree += 180;
    //   } else {
    //     _degree += 180;
    //   }
    // } else {
    //   degree = 10000;  //まだ動けフラグ
    // }
  }
  if (Rflag) {
    _degree = rdegree;
    flag = false;  // test
  }
  if (Oflag) {
    _degree = odegree;
    flag = false;
  }
  if (millis() - InTimer <= 10) {
    _degree = 10000;
  }
  Move_degree = _degree;
  leftdegree = _degree;
}

void _Line::vectorCalc(void) {
  block_vectorX[0] = sin(radians(180));
  block_vectorY[0] = cos(radians(180));
  block_vectorX[1] = sin(radians(180));
  block_vectorY[1] = cos(radians(180));
  block_vectorX[2] = sin(radians(0));
  block_vectorY[2] = cos(radians(0));
  block_vectorX[3] = sin(radians(0));
  block_vectorY[3] = cos(radians(0));
  block_vectorX[4] = sin(radians(90));
  block_vectorY[4] = cos(radians(90));
  block_vectorX[5] = sin(radians(90));
  block_vectorY[5] = cos(radians(90));
  block_vectorX[6] = sin(radians(270));
  block_vectorY[6] = cos(radians(270));
  block_vectorX[7] = sin(radians(270));
  block_vectorY[7] = cos(radians(270));
}
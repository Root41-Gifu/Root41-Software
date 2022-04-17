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
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 7; j++) {
      Line_Where[i * 7 + j] = i;
    }
  }
  one_degree[0] = 180;
  one_degree[1] = 180;
  one_degree[2] = 180;
  one_degree[3] = 180;
  one_degree[4] = 180;
  one_degree[5] = 180;
  one_degree[6] = 180;
  one_degree[7] = 0;
  one_degree[8] = 0;
  one_degree[9] = 0;
  one_degree[10] = 0;
  one_degree[11] = 0;
  one_degree[12] = 0;
  one_degree[13] = 0;
  one_degree[14] = 90;
  one_degree[15] = 90;
  one_degree[16] = 90;
  one_degree[17] = 90;
  one_degree[18] = 90;
  one_degree[19] = 90;
  one_degree[20] = 90;
  one_degree[21] = 270;
  one_degree[22] = 270;
  one_degree[23] = 270;
  one_degree[24] = 270;
  one_degree[25] = 270;
  one_degree[26] = 270;
  one_degree[27] = 270;
}

void _Line::read(void) {
  Wire.end();
  Wire.begin();
  Wire.setClock(400000);
  readCounter = !readCounter;

  if (readCounter) {
    Wire.requestFrom(LINE_FRONTADDRESS, 1);
    while (Wire.available() >= 1) {
      byte readValue;
      readValue = i2cReadWithTimeoutFunction();
      for (int i = 0; i < 5; i++) {
        angel_value[i] = readValue & (1 << 1 + i);
      }
      cross_value[0] = readValue & (1 << 6);
      edge_value[0] = readValue & (1 << 7);
      ball.hold = !(readValue & (1));
    }
    Wire.requestFrom(LINE_REARADDRESS, 1);
    while (Wire.available() >= 1) {
      byte readValue;
      readValue = i2cReadWithTimeoutFunction();
      for (int i = 0; i < 5; i++) {
        angel_value[i + 5] = readValue & (1 << 1 + i);
      }
      cross_value[1] = readValue & (1 << 6);
      edge_value[1] = readValue & (1 << 7);
    }
  } else {
    Wire.requestFrom(LINE_LEFTADDRESS, 1);
    while (Wire.available() >= 1) {
      byte readValue;
      readValue = i2cReadWithTimeoutFunction();
      for (int i = 0; i < 5; i++) {
        angel_value[i + 10] = readValue & (1 << 1 + i);
      }
      cross_value[2] = readValue & (1 << 6);
      edge_value[2] = readValue & (1 << 7);
    }
    Wire.requestFrom(LINE_RIGHTADDRESS, 1);
    while (Wire.available() >= 1) {
      byte readValue;
      readValue = i2cReadWithTimeoutFunction();
      for (int i = 0; i < 5; i++) {
        angel_value[i + 15] = readValue & (1 << 1 + i);
      }
      cross_value[3] = readValue & (1 << 6);
      edge_value[3] = readValue & (1 << 7);
    }
  }
  for (int i = 0; i < 4; i++) {
    value[i * 7] = edge_value[i];
    value[i * 7 + 1] = cross_value[i];
    for (int j = 0; j < 5; j++) {
      value[i * 7 + j + 2] = angel_value[i * 5 + j];
    }
  }
  Wire.end();
  Wire.begin();
}

void _Line::arrange(void) {
  //読み込み後のデータ抽出、整理

  //リセット等
  touch = false;
  whiting = 0;
  for (int i = 0; i < 4; i++) {
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
    if (value[i]) {
      //反応してたら
      if (!check[i]) {
        //過去に反応なし
        order[whited] = i;
        whited++;
        check[i] = true;
        passed_num[Line_Where[i]]++;
      }

      if (!checkBlock[Line_Where[i]]) {
        //そのブロックが過去に反応なし
        // 0だと誤反応の可能性あり、増やしてもいいかも
        checkBlock[Line_Where[i]] = true;
        orderBlock[Block] = Line_Where[i];
        Block++;
      }

      if (!flag) {
        in_degree=ball.LPF_degree;
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
        // if (millis() - OutTimer <= LINEOVERTIME) {
        //   if (whited > LINEOVERNUM) {
        //     rdegree = leftdegree;
        //   } else {
        //     odegree = leftdegree;
        //   }
        // } else {
        //   Rflag = false;
        // }
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
    if (Rflag) {
      if (millis() - OutTimer > LINERETURNTIME) {
        Rflag = false;
      }
    } else if (Oflag) {
      if (millis() - OutTimer > LINEOVERTIME) {
        Oflag = false;
      }
    } else if (whited < LINEOVERNUM && millis() - OutTimer <= LINERETURNTIME) {
      Rflag = true;
      rdegree = 1000;
      rdegree = leftdegree;
    } else if (whited >= LINEOVERNUM && millis() - OutTimer <= LINEOVERTIME) {
      Oflag = true;
      odegree = 1000;
      odegree = leftdegree;
    }
    // if (millis() - OutTimer > LINEOVERTIME && whited >= LINEOVERNUM) {
    //   Rflag = false;
    //   Oflag = false;
    //   flag = false;
    //   leftdegree = 1000;
    //   rdegree = 1000;
    //   odegree = 1000;
    // } else if (millis() - OutTimer > LINERETURNTIME && whited < LINEOVERNUM)
    // {
    //   Rflag = false;
    //   Oflag = false;
    //   flag = false;
    //   leftdegree = 1000;
    //   rdegree = 1000;
    // } else if (whited >= LINEOVERNUM) {
    //   Rflag = true;
    //   Oflag = false;
    // } else {
    //   Rflag = false;
    //   Oflag = true;
    // }
    if (!Rflag && !Oflag) {
      for (int i = 0; i < 4; i++) {
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
      rdegree = 0;
      odegree = 0;
      reference_degree = 0;
      current_degree = 0;
      in_degree=1000;
    }
  } else {
    Rflag = false;
    Oflag = false;
  }
}

void _Line::keeper_arrange(void) {
  int touch = false;
  Block = false;
  whiting = 0;
  for (int i = 0; i < 8; i++) {
    detect_num[i] = 0;
    checkBlock[i] = false;
  }
  for (int i = 0; i < LINE_NUM; i++) {
    if (!value[i]) {
      touch = true;
      flag = true;
      detect_num[Line_Where[i]]++;
      if (!checkBlock[Line_Where[i]]) {
        Block++;
      }
      checkBlock[Line_Where[i]] = true;
      whiting++;
      Last_Block = Line_Where[i];
    }
  }
  if (!touch) {
    if (flag) {
      awayFlag = true;
      awayTimer = millis();
      flag = false;
    }
    if (awayFlag) {
      if (millis() - awayTimer > 20) {
        awayFlag = false;
      }
    }
  }
}

int _Line::calcDirection(void) {
  int _degree;  //ベクトルに範囲
  t_vectorX = 0;
  t_vectorY = 0;
  int count = 0;
  for (int i = 0; i < 6; i++) {
    if (i < whited) {
      t_vectorX += sin_d[one_degree[order[i]]];
      t_vectorY += cos_d[one_degree[order[i]]];
    }
  }
  _degree = degrees(atan2(t_vectorX, t_vectorY));

  if (!ball.stop) {
    if (_degree > 150 && _degree < 210) {
      if (ball.LPF_degree > 15 && ball.LPF_degree < 90) {
        _degree = 225;
      } else if (ball.LPF_degree > 270 && ball.LPF_degree < 345) {
        _degree = 135;
      }
    }
  }

  if(millis()-InTimer>=400){
    if (_degree > 150 && _degree < 210) {
      if (ball.LPF_degree < 90) {
        _degree = 225;
      } else if (ball.LPF_degree > 270) {
        _degree = 135;
      }
    }
  }

  // t_vectorX = 0;
  // t_vectorY = 0;
  // int count = 0;
  // for (int i = 0; i < 6; i++) {
  //   if (i < Block) {
  //     t_vectorX += block_vectorX[orderBlock[i]];
  //     t_vectorY += block_vectorY[orderBlock[i]];
  //   }
  // }
  // _degree = degrees(atan2(t_vectorX, t_vectorY));

  return _degree;
}

void _Line::calc(void) {
  int _degree = 1000;
  if (flag) {
    t_vectorX = 0;
    t_vectorY = 0;
    current_degree = gyro.deg;  // kese
    if (mode == 1) {
      //少数反応
      // _degree = calcDirection() - current_degree;
      _degree = Block_degree[orderBlock[0]];
    } else if (mode == 2) {
      //ずれ少ない多数反応
      _degree = calcDirection();
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
      _degree = calcDirection();
      // _degree=Block_degree[orderBlock[0]]-current_degree;
    } else if (mode == 4) {
      //オーバー　
      _degree = calcDirection();
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
  }
  if (Oflag) {
    _degree = odegree;
  }
  // if (millis() - InTimer <= 0) {
  //   _degree = 10000;
  // }
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
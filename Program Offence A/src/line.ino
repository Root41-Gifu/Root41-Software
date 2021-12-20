_Line::_Line() {
  for (int i = 0; i < 20; i++) {
    value[i] = false;
  }
  // for (int i = 0; i < 20; i++) {
  //   _vectorX[i] = sin(radians(i * 18));
  //   _vectorY[i] = cos(radians(i * 18));
  // }
  for (int i = 0; i < 4; i++) {
    _vectorX[i] = sin(radians(i * 90));
    _vectorY[i] = cos(radians(i * 90));
  }
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
  for (int i = 0; i <= 7; i++) {
    Line_Where[i] = 0;
  }
  for (int i = 8; i <= 9; i++) {
    Line_Where[i] = 1;
  }
  for (int i = 10; i <= 15; i++) {
    Line_Where[i] = 2;
  }
  for (int i = 16; i <= 18; i++) {
    Line_Where[i] = 3;
  }
  for (int i = 19; i <= 22; i++) {
    Line_Where[i] = 5;
  }
  for (int i = 23; i <= 26; i++) {
    Line_Where[i] = 4;
  }
  for (int i = 27; i <= 31; i++) {
    Line_Where[i] = 5;
  }
  for (int i = 32; i <= 34; i++) {
    Line_Where[i] = 6;
  }
  for (int i = 35; i <= 40; i++) {
    Line_Where[i] = 7;
  }
}

void _Line::read(void) {
  int bitSelect;
  Wire.requestFrom(LINE_FRONTADDRESS, 2);  //アドレスは変えてね

  while (Wire.available() >= 2) {
    byte readValue[2];
    readValue[0] = Wire.read();
    readValue[1] = Wire.read();
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
  Wire.requestFrom(LINE_REARADDRESS, 2);  //アドレスは変えてね

  while (Wire.available() >= 2) {
    byte readValue[2];
    readValue[0] = Wire.read();
    readValue[1] = Wire.read();
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
  Wire.requestFrom(LINE_LEFTADDRESS, 2);  //アドレスは変えてね
  while (Wire.available() >= 2) {
    byte readValue[2];
    readValue[0] = Wire.read();
    readValue[1] = Wire.read();
    for (int i = 0; i < 8; i++) {
      value[bitSelect] = readValue[0] & (1 << i);
      bitSelect++;
    }
    for (int i = 0; i < 6; i++) {
      value[bitSelect] = readValue[1] & (1 << i + 2);
      bitSelect++;
    }
  }
  Wire.requestFrom(LINE_RIGHTADDRESS, 2);  //アドレスは変えてね

  while (Wire.available() >= 2) {
    byte readValue[2];
    readValue[0] = Wire.read();
    readValue[1] = Wire.read();
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
  value[41]=true;
  value[42]=true;
}

void _Line::arrange(void) {
  touch = false;
  whiting = 0;
  for(int i=0; i<8; i++){
    detect_num[i]=0;
  }
  for (int i = 0; i < LINE_NUM; i++) {
    if (!value[i]) {  //数値逆転
      if (!check[i]) {
        order[whited] = i;
        whited++;
        check[i] = true;
        passed_num[Line_Where[i]]++;
      }
      if (!flag) {
        //   stopTimer = device.getTime();
        mode = 1;
        flag = true;
        if (millis() - OutTimer <= LINEOVERTIME) {
          rdegree = leftdegree;
        } else {
          Rflag = false;
        }
      }
      whiting=true;
      touch = true;
      detect_num[Line_Where[i]]++;
      OutTimer = millis();
    }
  }
  if (!touch) {
    flag = false;
  }
  if (!flag) {
    for (int i = 0; i < 8; i++) {
      orderBlock[i] = 100;
      checkBlock[i] = 0;
      detect_num[i]=0;
      passed_num[i]=0;
    }
    for (int i = 0; i < LINE_NUM; i++) {
      order[i] = 100;
      check[i] = 0;
    }
    if (millis() - OutTimer > LINEOVERTIME) {
      Rflag = false;
      leftdegree = 1000;
      rdegree = 1000;
    } else {
      Rflag = true;
    }
    Block = 0;
  } else {
    
  }
}

void _Line::calcDirection(void) {
  int circleRange = 10;  //ベクトルに範囲
  t_vectorX = 0;
  t_vectorY = 0;
  for (int i = 0; i < circleRange; i++) {
    t_vectorX += _vectorX[order[i]];
    t_vectorY += _vectorY[order[i]];
  }
}

void _Line::calc(void) {
  int _degree = 1000;
  if (Rflag) {
    _degree = rdegree;
  }
  Move_degree = _degree;
  leftdegree = _degree;
}
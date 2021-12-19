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
  for (int i = 32; i <= 40; i++) {
    Line_Where[i] = 6;
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
}

void _Line::arrange(void) {
  touch = false;
  whiting = 0;
  Front = 0;
  Rear = 0;
  Left = 0;
  Right = 0;
  FrontEdge = 0;
  RearEdge = 0;
  LeftEdge = 0;
  RightEdge = 0;
  FrontInside = 0;
  RearInside = 0;
  LeftInside = 0;
  RightInside = 0;
  for (int i = 0; i < LINE_NUM; i++) {
    if (!value[i]) {  //数値逆転
      if (!check[i]) {
        order[whited] = i;
        whited++;
        check[i] = true;
        // if (i >= 20) {
        // if (i < LINE_FRONTNUM) {
        //   Front++;
        // } else if (i < LINE_FRONTNUM + LINE_REARNUM) {
        //   Rear++;
        // } else if (i < LINE_FRONTNUM + LINE_REARNUM + LINE_LEFTNUM) {
        //   Left++;
        // } else if (i < LINE_FRONTNUM + LINE_REARNUM + LINE_LEFTNUM +
        //                    LINE_RIGHTNUM) {
        //   Right++;
        // }
        // if(i<LINE_FRONTEDGENUM){
        //   FrontEdge++;
        // }else if(i<LINE_FRONTEDGENUM+LINE_FRONTINSIDENUM){
        //   FrontInside++;
        // }else if(i<LINE_FRONTEDGENUM+LINE_FRONTINSIDENUM+LINE_REAREDGENUM){
        //   RearEdge++;
        // }else
        // if(i<LINE_FRONTEDGENUM+LINE_FRONTINSIDENUM+LINE_REAREDGENUM+LINE_REARINSIDENUM){
        //   RearInside++;
        // }else
        // if(i<LINE_FRONTEDGENUM+LINE_FRONTINSIDENUM+LINE_REAREDGENUM+LINE_REARINSIDENUM+LINE_LEFTEDGENUM){
        //   LeftEdge++;
        // }else
        // if(i<LINE_FRONTEDGENUM+LINE_FRONTINSIDENUM+LINE_REAREDGENUM+LINE_REARINSIDENUM+LINE_LEFTEDGENUM+LINE_LEFTINSIDENUM){
        //   LeftInside++;
        // }else
        // if(i<LINE_FRONTEDGENUM+LINE_FRONTINSIDENUM+LINE_REAREDGENUM+LINE_REARINSIDENUM+LINE_LEFTEDGENUM+LINE_LEFTINSIDENUM+LINE_RIGHTEDGENUM){
        //   RightEdge++;
        // }else
        // if(i<LINE_FRONTEDGENUM+LINE_FRONTINSIDENUM+LINE_REAREDGENUM+LINE_REARINSIDENUM+LINE_LEFTEDGENUM+LINE_LEFTINSIDENUM+LINE_RIGHTEDGENUM+LINE_RIGHTINSIDENUM){
        //   RightInside++;
        // }
        // }
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
      touch = true;
      if (Line_Where[i] == 0) {
        FrontEdge++;
      } else if (Line_Where[i] == 1) {
        FrontInside++;
      } else if (Line_Where[i] == 2) {
        RearEdge++;
      } else if (Line_Where[i] == 3) {
        RearInside++;
      } else if (Line_Where[i] == 4) {
        LeftEdge++;
      } else if (Line_Where[i] == 5) {
        LeftInside++;
      } else if (Line_Where[i] == 6) {
        RightEdge++;
      } else if (Line_Where[i] == 7) {
        RightInside++;
      }
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
    if (FrontEdge > 0 && !checkBlock[0]) {
      orderBlock[Block] = 0;
      checkBlock[0] = true;
      Block++;
    } else if (FrontInside > 0 && !checkBlock[1]) {
      orderBlock[Block] = 1;
      checkBlock[1] = true;
      Block++;
    } else if (RearEdge > 0 && !checkBlock[2]) {
      orderBlock[Block] = 2;
      checkBlock[2] = true;
      Block++;
    } else if (RearInside > 0 && !checkBlock[3]) {
      orderBlock[Block] = 3;
      checkBlock[3] = true;
      Block++;
    } else if (LeftEdge > 0 && !checkBlock[4]) {
      orderBlock[Block] = 4;
      checkBlock[4] = true;
      Block++;
    } else if (LeftInside > 0 && !checkBlock[5]) {
      orderBlock[Block] = 5;
      checkBlock[5] = true;
      Block++;
    } else if (RightEdge > 0 && !checkBlock[6]) {
      orderBlock[Block] = 6;
      checkBlock[6] = true;
      Block++;
    } else if (RightInside > 0 && !checkBlock[7]) {
      orderBlock[Block] = 7;
      checkBlock[7] = true;
      Block++;
    }
    Edge=int(checkBlock[0]/2);
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
  if (orderBlock[0] == 0 && orderBlock[1] == 4) {
    _degree = 135;
  } else if (orderBlock[0] == 0 && orderBlock[1] == 6) {
    _degree = 225;
  } else if (orderBlock[0] == 4 && orderBlock[1] == 0) {
    _degree = 135;
  } else if (orderBlock[0] == 4 && orderBlock[1] == 2) {
    _degree = 45;
  } else if (orderBlock[0] == 6 && orderBlock[1] == 0) {
    _degree = 225;
  } else if (orderBlock[0] == 6 && orderBlock[1] == 2) {
    _degree = 315;
  } else if (orderBlock[0] == 2 && orderBlock[1] == 4) {
    _degree = 45;
  } else if (orderBlock[0] == 2 && orderBlock[1] == 6) {
    _degree = 315;
  }
  if (_degree == 1000) {
    if (orderBlock[0] == 0) {
      _degree = 180;
    } else if (orderBlock[0] == 2) {
      _degree = 0;
    } else if (orderBlock[0] == 4) {
      _degree = 45;
    } else if (orderBlock[0] == 6) {
      _degree = 270;
    }
  }
  if (Rflag) {
    _degree = rdegree;
  }
  // float _vX;
  // float _vY;
  // for (int i = 0; i < 2; i++) {
  //   if (orderBlock[i] == 0) {
  //     _vX += _vectorX[0];
  //     _vY += _vectorY[0];
  //   } else if (orderBlock[i] == 2) {
  //     _vX += _vectorX[2];
  //     _vY += _vectorY[2];
  //   } else if (orderBlock[i] == 4) {
  //     _vX += _vectorX[3];
  //     _vY += _vectorY[3];
  //   } else if (orderBlock[i] == 6) {
  //     _vX += _vectorX[1];
  //     _vY += _vectorY[1];
  //   }
  // }
  // _degree = degrees(atan2(_vY, _vX));
  Move_degree = _degree;
  leftdegree = _degree;
}
_Line::_Line() {
  for (int i = 0; i < LINE_NUM; i++) {
    value[i] = false;
  }
  for (int i = 0; i < 20; i++) {
    _vectorX[i] = sin(radians(i * 18));
    _vectorY[i] = cos(radians(i * 18));
  }
}

void _Line::read(void) {
  //   Wire.requestFrom(LINE_FRONTADDRESS,2);//アドレスは変えてね
  //   int byteCounter=0;
  //   while (Wire.available()>=2) {
  //       byte readValue[2];
  //       readValue[0]= Wire.read();
  //       readValue[1]= Wire.read();
  //       int bitSelect;
  //       for(int i=0; i<3; i++){
  //           value[bitSelect]=readValue[0]&(1<<i+1);
  //           bitSelect++;
  //       }
  //       for(int i=0; i<3; i++){
  //           value[bitSelect]=readValue[0]&(1<<i+5);
  //           bitSelect++;
  //       }
  //       value[bitSelect]=readValue[1]&(1<<3);
  //       bitSelect++;
  //       for(int i=0; i<3; i++){
  //           value[bitSelect]=readValue[1]&(1<<i+5);
  //           bitSelect++;
  //       }
  //   }
  //   Wire.requestFrom(LINE_REARADDRESS, 2);  //アドレスは変えてね
  //   int byteCounter = 0;
  //   while (Wire.available() >= 2) {
  //     int bitSelect;
  //     byte readValue[2];
  //     readValue[0] = Wire.read();
  //     readValue[1] = Wire.read();
  //     for (int i = 0; i < 3; i++) {
  //       value[bitSelect] = readValue[0] & (1 << i + 1);
  //       bitSelect++;
  //     }
  //     value[bitSelect] = readValue[0] & (1 << 5);
  //     bitSelect++;
  //     for (int i = 0; i < 2; i++) {
  //       value[bitSelect] = readValue[1] & (1 << i + 2);
  //       bitSelect++;
  //     }
  //     for (int i = 0; i < 3; i++) {
  //       value[bitSelect] = readValue[1] & (1 << i + 5);
  //       bitSelect++;
  //     }
  //   }
  //   Wire.requestFrom(LINE_LEFTADDRESS, 2);  //アドレスは変えてね
  //   int byteCounter = 0;
  //   while (Wire.available() >= 2) {
  //     int biSelect;
  //     byte readValue[2];
  //     readValue[0] = Wire.read();
  //     readValue[1] = Wire.read();
  //     for (int i = 0; i < 8; i++) {
  //       value[bitSelect] = readValue[0] & (1 << i);
  //       bitSelect++;
  //     }
  //     for (int i = 0; i < 8; i++) {
  //       value[bitSelect] = readValue[1] & (1 << i + 2);
  //       bitSelect++;
  //     }
  //   }
  Wire.requestFrom(LINE_LEFTADDRESS, 2);  //アドレスは変えてね
  int byteCounter = 0;
  while (Wire.available() >= 2) {
    byte readValue[2];
    readValue[0] = Wire.read();
    readValue[1] = Wire.read();
    for (int i = 0; i < 8; i++) {
      value[i] = readValue[0] & (1 << i);
    }
    for (int i = 0; i < 8; i++) {
      value[i + 8] = readValue[1] & (1 << i);
    }
  }
  //   Wire.requestFrom(LINE_RIGHTADDRESS, 2);  //アドレスは変えてね
  //   int byteCounter = 0;
  //   while (Wire.available() >= 2) {
  //     byte readValue[2];
  //     readValue[0] = Wire.read();
  //     readValue[1] = Wire.read();
  //     for (int i = 0; i < 3; i++) {
  //       value[bitSelect] = readValue[0] & (1 << i + 1);
  //       bitSelect++;
  //     }
  //     value[bitSelect] = readValue[0] & (1 << 5);
  //     bitSelect++;
  //     for (int i = 0; i < 2; i++) {
  //       value[bitSelect] = readValue[1] & (1 << i + 2);
  //       bitSelect++;
  //     }
  //     for(int i=0; i<3; i++){
  //         value[bitSelect]=readValue[1]&(1<<i+5);
  //         bitSelect++;
  //     }
  //   }
}

void _Line::arrange(void) {
  touch = false;
  whiting = 0;
  Front=0;
  Rear=0;
  Left=0;
  Right=0;
  for (int i = 0; i < LINE_NUM; i++) {
    if (value[i]) {
      if (!check[i]) {
        order[whited] = i;
        whited++;
        check[i] = true;
        // if (i >= 20) {
        if (i < LINE_FRONTNUM) {
          Front++;
        } else if (i < LINE_FRONTNUM + LINE_REARNUM) {
          Rear++;
        } else if (i < LINE_FRONTNUM + LINE_REARNUM + LINE_LEFTNUM) {
          Left++;
        } else if (i < LINE_FRONTNUM + LINE_RAERNUM + LINE_LEFTNUM +
                           LINE_RIGHTNUM) {
          Right++;
        }
        // }
      }
      if (!flag) {
        //   stopTimer = device.getTime();
        mode = 1;
        flag = true;
      }
      touch = true;
    }
  }
  if (!touch) {
    flag = false;
  }
  if(!flag){
      for(int i=0; i<4; i++){
        orderBlock[i]=100;
        checkBlock[i]=0;
      }
      Block=0;
  }else{
      if(Front>0&&!checkBlock[0]){
          orderBlock[Block]=0;
          checkBlock[0]=true;
          Block++;
      }else if(Rear>0&&!checkBlock[1]){
          orderBlock[Block]=1;
          checkBlock[1]=true;
          Block++;
      }else if(Left>0&&!checkBlock[2]){
          orderBlock[Block]=2;
          checkBlock[2]=true;
          Block++;
      }else if(Right>0&&!checkBlock[3]){
          orderBlock[Block]=3;
          checkBlock[3]=true;
          Block++;
      }
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

void _Line::calc(void){
    int _degree;
    if(orderBlock[0]==0&&orderBlock[1]==2){
        _degree=135;
    }else if(orderBlock[0]==0&&orderBlock[1]==3){
        _degree=225;
    }else if(orderBlock[0]==2&&orderBlock[1]==0){
        _degree=135;
    }else if(orderBlock[0]==2&&orderBlock[1]==1){
        _degree=45;
    }else if(orderBlock[0]==3&&orderBlock[1]==0){
        _degree=225;
    }else if(orderBlock[0]==3&&orderBlock[1]==1){
        _degree=315;
    }else if(orderBlock[0]==1&&orderBlock[1]==2){
        _degree=45;
    }else if(orderBlock[0]==1&&orderBlock[1]==3){
        _degree=315;
    }else if(orderBlock[0]==0){
        _degree=180;
    }else if(orderBlock[0]==1){
        _degree=90;
    }else if(orderBlock[0]==2){
        _degree=270;
    }else if(orderBlock[0]==3){
        _degree=0;
    }
    Move_degree=_degree;
}
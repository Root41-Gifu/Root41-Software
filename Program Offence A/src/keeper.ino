_Keeper::_Keeper(void) {
  mode = 0;
}

void _Keeper::analyze(void) {
  if (line.flag || line.awayFlag) {
    if (line.checkBlock[0] && line.checkBlock[1]) {
      mode = 6;
    } else {
      mode = 1;
    }
  } else {
    if (line.Last_Block == 1) {
      mode = 3;
    } else if (line.Last_Block == 0) {
      mode = 4;
    } else {
      mode = 5;
    }
  }
  float ball_x = sin(radians(ball.degree));
  // speed = abs(ball_x);
  speed = 1;
  // speed=map(abs(ball_x),0,1,0.3,1))
}

void _Keeper::calc(void) {
  int _degree = 1000;
  x_position = camera.readData;
  if (mode == 1) {
    // if (line.whiting > 0) {
    //   if (line.detect_num[4] + line.detect_num[5] ==
    //   line.whiting&&ball.Move_degree==90) {
    //     _degree = 1000;
    //   } else if (line.detect_num[6] + line.detect_num[7] ==
    //   line.whiting&&ball.Move_degree==270) {
    //     _degree = 1000;
    //   } else {
    _degree = ball.Move_degree;
    frontoverFlag = false;
    if(x_position==3){
      
    }else if(x_position==2){
      if(_degree==270){
        _degree=280;
      }else if(_degree==90){
        _degree=80;
      }
    }else if(x_position==1){
      if(_degree==270){
        _degree=280;
      }else if(_degree==90){
        _degree=80;
      }
    }else if(x_position==5){
      _degree=60;
    }else if(x_position==4){
      _degree=300;
    }
    //   }
    // } else {
    //   _degree = ball.Move_degree;
    // }
    // _degree = 1000;
  } else if (mode == 2) {
    _degree = 1000;
    // } else if (mode == 3) {
    //   _degree = 180;
    // } else if (mode == 4) {
    //   _degree = 0;
  } else if (mode == 6) {
    _degree = 0;
  } else {
    _degree = line.Block_degree[line.Last_Block];
    if (_degree == 0) {
      if (!frontoverFlag) {
        frontoverTimer = millis();
        frontoverFlag = true;
      }
      if (frontoverFlag) {
        if (millis() - frontoverTimer < 0) {
          _degree = ball.Move_degree;
        } else {
          _degree = 0;
        }
      }
    }
    _degree += 540;
    _degree %= 360;
    if(x_position==3&&line.Last_Block!=0){
      _degree=180;
    }else if(x_position==1){
      if(_degree==180){
        _degree=225;
      }
    }else if(x_position==2){
      if(_degree==180){
        _degree=135;
      }
    }else if(x_position==5){
      _degree=60;
    }else if(x_position==4){
      _degree=300;
    }
  }
  if(x_position==1||x_position==2){
    speed=0.7;
  }else if(x_position==4||x_position==5){
    speed=0.5;
  }
  Move_degree = _degree;
}
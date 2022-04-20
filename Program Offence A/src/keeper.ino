_Keeper::_Keeper(void) {
  mode = 0;
}

void _Keeper::analyze(void) {
  if (line.flag || line.awayFlag) {
    if (line.checkBlock[0] &&
        line.checkBlock[1]) {
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
  speed=1;
  // speed=map(abs(ball_x),0,1,0.3,1))
}

void _Keeper::calc(void) {
  int _degree = 1000;
  // x_position=camera.readData;
  if (x_position == 0) {
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
      if (_degree == 90) {
        _degree -= 10;
      } else if (_degree == 270) {
        _degree += 10;
      }
      _degree += 540;
      _degree %= 360;
    }
  } else if (x_position == 3) {
    if (mode == 1) {
      _degree = ball.Move_degree;
    } else if (mode == 6) {
      _degree = 0;
    } else {
      _degree = line.Block_degree[line.Last_Block];
    }
  } else if (x_position == 1) {
    if (mode == 1) {
      _degree = ball.Move_degree;
      if (line.checkBlock[2]&&line.Block==1&&_degree<180) {
        _degree = 1000;
      }else if(_degree>180&&_degree!=1000){
        _degree+=30;
      }
    } else if (mode == 2) {
      _degree = 330;
    } else if (mode == 3) {
      _degree = 200;
    } else if (mode == 4) {
      _degree = 330;
    }else{
      _degree = line.Block_degree[line.Last_Block];
    }
  } else if (x_position == 2) {
    if (mode == 1) {
      _degree = ball.Move_degree;
      if (line.checkBlock[3]&&line.Block==1&&_degree>180) {
        _degree = 1000;
      }else if(_degree<180){
        _degree -=30;
      }
    } else if (mode == 2) {
      _degree = 30;
    } else if (mode == 3) {
      _degree = 160;
    } else if (mode == 4) {
      _degree = 30;
    }else{
      _degree = line.Block_degree[line.Last_Block];
    }
  }
  Move_degree = _degree;
}
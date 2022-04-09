_Keeper::_Keeper(void) {
  mode = 0;
}

void _Keeper::analyze(void) {
  if (line.flag || line.awayFlag) {
    if (line.checkBlock[0] + line.checkBlock[1] > 0 &&
        line.checkBlock[2] + line.checkBlock[3] > 0) {
      mode = 6;
    } else {
      mode = 1;
    }
  } else {
    if (line.Last_Block == 2 || line.Last_Block == 3) {
      mode = 3;
    } else if (line.Last_Block == 0 || line.Last_Block == 1) {
      mode = 4;
    } else {
      mode = 5;
    }
  }
}

void _Keeper::calc(void) {
  int _degree = 1000;
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
    _degree = -0;
  } else {
    _degree = line.Block_degree[line.Last_Block];
    _degree += 540;
    _degree %= 360;
  }
  Move_degree = _degree;
}
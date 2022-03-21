_UI::_UI() {
  mode = 0;
  submode = 0;
  emergency = false;
}

void _UI::read() {
  Wire.requestFrom(UI_ADDRESS, 1);
  while (Wire.available()) {
    byte readValue = Wire.read();
    touch[2] = !(readValue & (1 << 2));
    touch[1] = !(readValue & (1 << 3));
    touch[3] = !(readValue & (1 << 4));
    int touchedNum = touch[1] + touch[2] + touch[3];
    int _touchedNum = readValue & 3;
    if (touchedNum == _touchedNum) {
      Consistency = true;
    } else {
      Consistency = false;
    }
  }
}

void _UI::check(int num) {
  if (!touchFlag[num]) {
    if (touch[num]) {
      switchingFlag[num] = true;
      touchFlag[num] = true;
    }
  } else {
    if (!touch[num]) {
      counter[num] = 0;
      touchFlag[num] = false;
      longpressFlag[num] = false;
    }
    if (longpressFlag[num] > 0) {
      longpressFlag[num] = true;
    }
  }
}

void _UI::refrection(void) {
  if (mode == 6) {
    if (switchingFlag[0]) {
      select = false;
      active = false;
      submode = 0;
    }
    if (!active) {
      if (switchingFlag[1]) {
        if (select) {
          submode--;
        } else {
          mode++;
        }
      }
      if (switchingFlag[2]) {
        // if(select){
        //     submode++;
        // }else{
        //     mode++;
        // }
      }
    }
    if (switchingFlag[3]) {
      if (!select) {
        select = true;
      } else {
        standby = true;
        standbyTimer = millis();
      }
    } else {
      if (standby && !touch[3]) {
        active = true;
        standby = false;
      }
    }
    if (submode < 0) {
      submode = 1;
    } else if (submode > 1) {
      submode = 0;
    }
  } else if (mode == 3) {
    if (switchingFlag[0]) {
      select = false;
      active = false;
      submode = 0;
    }
    if (!active) {
      if (switchingFlag[1]) {
        if (select) {
          submode--;
        } else {
          mode++;
        }
      }
      if (switchingFlag[2]) {
        // if(select){
        //     submode++;
        // }else{
        //     mode++;
        // }
      }
    }
    if (switchingFlag[3]) {
      if (!select) {
        select = true;
      } else {
        standby = true;
        standbyTimer = millis();
      }
    } else {
      if (standby && !touch[3]) {
        active = true;
        standby = false;
      }
    }
    if (submode < 0) {
      submode = 1;
    } else if (submode > 1) {
      submode = 0;
    }
  } else if (mode == 6) {
    if (switchingFlag[0]) {
      select = false;
      active = false;
      submode = 0;
    }
    if (!active) {
      if (switchingFlag[1]) {
        if (select) {
          submode--;
        } else {
          mode++;
        }
      }
      if (switchingFlag[2]) {
        // if(select){
        //     submode++;
        // }else{
        //     mode++;
        // }
      }
    } else {
      if (switchingFlag[1]) {
        MotorPower += 5;
        if (MotorPower > 100) {
          MotorPower = 0;
        }
      }
    }
    if (switchingFlag[3]) {
      if (!select) {
        select = true;
      } else {
        standby = true;
        standbyTimer = millis();
      }
    } else {
      if (standby && !touch[3]) {
        active = true;
        standby = false;
      }
    }
    if (submode < 0) {
      submode = 1;
    } else if (submode > 1) {
      submode = 0;
    }
  } else {
    if (switchingFlag[0]) {
      select = false;
      active = false;
    }
    if (!active) {
      if (switchingFlag[1]) {
        mode++;
        switchScope--;
      }
      if (switchingFlag[2]) {
        // mode++;
        // switchScope++;
        //エラー対策
      }
    }
    if (switchingFlag[3]) {
      standby = true;
      standbyTimer = millis();
    } else {
      if (standby && !touch[3]) {
        active = true;
        standby = false;
      }
    }
  }
  if (switchingFlag[1]) {
    switchScope++;
  }
  for (int i = 0; i <= 3; i++) {
    switchingFlag[i] = false;
  }
  if (mode < 1) {
    mode = 7;
  } else if (mode > 7) {
    mode = 1;
  }
  if (switchScope > LINE_NUM) {
    switchScope = 0;
  } else if (switchScope < 0) {
    switchScope = LINE_NUM - 1;
  }
}

void _UI::NeoPixeldisplay(int _mode) {
  front.clear();
  rear.clear();
  left.clear();
  right.clear();
  strip.clear();
  // reset
  // for (int i = 0; i < 16; i++) {
  //   strip.setPixelColor(i, 0, 0, 0);
  // }
  if (active) {
    // if (mode == 1) {
    //   if (ball.max[0] != 100) {
    //     unsigned long TopPixelColor = strip.Color(255, 0, 0);
    //     strip.setPixelColor(ball.max[0], TopPixelColor);
    //   } else {
    //     unsigned long TopPixelColor = strip.Color(255, 0, 0);
    //     StripFulldisplay(TopPixelColor);
    //   }
    // }
    // if (mode == 1) {
    //   unsigned long BlackDawn_Color = strip.Color(0, 0, 0);
    //   StripFulldisplay(BlackDawn_Color);
    // } else 
    if (mode == 4||mode==1) {
      unsigned long BallDistance_Color1 = strip.Color(255, 0, 0);  // distance1
      unsigned long BallDistance_Color2 =
          strip.Color(125, 0, 125);                                // distance1
      unsigned long BallDistance_Color3 = strip.Color(0, 0, 255);  // distance1
      int _side[2];
      _side[0] = ball.max[0] - 1;
      _side[1] = ball.max[0] + 1;
      if (ball.max[0] - 1 < 0) {
        _side[0] = 15;
      } else if (ball.max[0] + 1 > 15) {
        _side[1] = 0;
      }
      switch (ball.distanceLevel) {
        case 0:
          StripFulldisplay(BallDistance_Color3);
          break;
        case 3:
          strip.setPixelColor(ball.max[0], BallDistance_Color3);
          strip.setPixelColor(_side[0], BallDistance_Color3);
          strip.setPixelColor(_side[1], BallDistance_Color3);
          break;
        case 2:
          strip.setPixelColor(ball.max[0], BallDistance_Color2);
          strip.setPixelColor(_side[0], BallDistance_Color2);
          strip.setPixelColor(_side[1], BallDistance_Color2);
          break;

        case 1:
          strip.setPixelColor(ball.max[0], BallDistance_Color1);
          strip.setPixelColor(_side[0], BallDistance_Color1);
          strip.setPixelColor(_side[1], BallDistance_Color1);
          break;

        default:
          break;
      }
    }
  } else if (standby) {
    //スタンバイ中
    unsigned long NeoPixelColor = strip.Color(255, 255, 255);
    //スタンバイ時のの演出
    if (millis() - standbyTimer < 400) {
      int timerSeparate = millis() - standbyTimer;
      for (int i = 0; i <= (timerSeparate / 50) % 8; i++) {
        strip.setPixelColor(i, NeoPixelColor);
        strip.setPixelColor(15 - i, NeoPixelColor);
      }
    } else {
      StripFulldisplay(NeoPixelColor);
    }
  } else {
    if (mode == 1 || mode == 2) {
      // offence
      unsigned long OffencelColor = strip.Color(255, 0, 0);
      StripFulldisplay(OffencelColor);
    } else if (mode == 3) {
      unsigned long GyroColor = strip.Color(0, 0, 255);
      StripFulldisplay(GyroColor);
    } else if (mode == 4) {
      unsigned long KickerColor = strip.Color(255, 255, 0);
      StripFulldisplay(KickerColor);
    } else if (mode == 5) {
      unsigned long LineColor = strip.Color(255, 255, 255);
      StripFulldisplay(LineColor);
    } else if (mode == 6) {
      unsigned long MotorColor = strip.Color(255, 0, 255);
      StripFulldisplay(MotorColor);
    }
  }

  // line
  if (mode == 1 || mode == 2 || mode == 5) {
    if (active) {
      unsigned long lineNeoPixelColor = front.Color(255, 255, 255);
      unsigned long lineNeoPixelDicline = front.Color(0, 0, 0);
      if (LIGHTLIMIT == 1) {
        for (int i = 0; i < LED_FRONT; i++) {
          if (i >= 0 && i <= 8) {  // 0-7
            front.setPixelColor(i, lineNeoPixelDicline);
          } else {
            front.setPixelColor(i, lineNeoPixelColor);
          }
        }
        for (int i = 0; i < LED_REAR; i++) {
          if (i >= 0 && i <= 8) {  // 0-5
            rear.setPixelColor(i, lineNeoPixelDicline);
          } else {
            rear.setPixelColor(i, lineNeoPixelColor);
          }
        }
        for (int i = 0; i < LED_LEFT; i++) {
          if (i >= 1 && i <= 7) {  // 4-7
            left.setPixelColor(i, lineNeoPixelDicline);
          } else {
            left.setPixelColor(i, lineNeoPixelColor);
          }
        }
        for (int i = 0; i < LED_RIGHT; i++) {
          if (i >= 0 && i <= 8) {  // 4-8
            right.setPixelColor(i, lineNeoPixelDicline);
          } else {
            right.setPixelColor(i, lineNeoPixelColor);
          }
        }
      } else {
        for (int i = 0; i < LED_FRONT; i++) {
          front.setPixelColor(i, lineNeoPixelColor);
        }
        for (int i = 0; i < LED_REAR; i++) {
          rear.setPixelColor(i, lineNeoPixelColor);
        }
        for (int i = 0; i < LED_LEFT; i++) {
          left.setPixelColor(i, lineNeoPixelColor);
        }
        for (int i = 0; i < LED_RIGHT; i++) {
          right.setPixelColor(i, lineNeoPixelColor);
        }
      }

      // if (switchScope < LINE_FRONTNUM) {
      //   front.setPixelColor(switchScope, lineNeoPixelColor);
      // } else if (switchScope < LINE_FRONTNUM + LINE_REARNUM) {
      //   rear.setPixelColor(switchScope - LINE_FRONTNUM, lineNeoPixelColor);
      // } else if (switchScope < LINE_FRONTNUM + LINE_REARNUM + LINE_LEFTNUM) {
      //   left.setPixelColor(switchScope - LINE_FRONTNUM - LINE_REARNUM,
      //                       lineNeoPixelColor);
      // } else if (switchScope <
      //            LINE_FRONTNUM + LINE_REARNUM + LINE_LEFTNUM + LINE_RIGHTNUM)
      //            {
      //   right.setPixelColor(
      //       switchScope - LINE_FRONTNUM - LINE_REARNUM - LINE_LEFTNUM,
      //       lineNeoPixelColor);
      // }
    }
  }
  strip.show();
  front.show();
  rear.show();
  left.show();
  right.show();
}

void _UI::StripFulldisplay(unsigned long color) {
  strip.clear();
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void _UI::LCDdisplay(void) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  if (active) {
    display.print("R:");
  } else {
    display.print("S:");
  }
  if (UI.mode == 0) {
    if (!active) {
      display.println("Starting");
    } else {
      display.println("Starting");
    }
    display.setCursor(0, 17);
    display.setTextSize(1);
  } else if (UI.mode == 1) {
    display.println("Offence");
  } else if (UI.mode == 2) {
    display.println("Defence");
  } else if (UI.mode == 3) {
    if (!select) {
      display.print("Gyro ");
      if ((millis() / LCD_INTERVAL) % 2 == 0) {
        display.println(">");
      } else if ((millis() / 300) % 2 == 1) {
        display.println(" ");
      }
    } else {
      if (!active) {
        if ((millis() / LCD_INTERVAL) % 2 == 0) {
          display.print("G>");
        } else if ((millis() / LCD_INTERVAL) % 2 == 1) {
          display.print("G ");
        }
      }
      if (submode == 0) {
        display.println("Front");
      } else if (submode == 1) {
        display.println("Calb");
      }
    }
    display.setCursor(0, 17);
    display.setTextSize(1);
  } else if (UI.mode == 4) {
    display.println("Ball");
  } else if (UI.mode == 5) {
    display.println("Line");
  } else if (UI.mode == 6) {
    if (!select) {
      display.print("Motor ");
      if ((millis() / LCD_INTERVAL) % 2 == 0) {
        display.println(">");
      } else if ((millis() / LCD_INTERVAL) % 2 == 1) {
        display.println(" ");
      }
    } else {
      if (!active) {
        if ((millis() / LCD_INTERVAL) % 2 == 0) {
          display.print("M>");
        } else if ((millis() / LCD_INTERVAL) % 2 == 1) {
          display.print("M ");
        }
      }
      if (submode == 0) {
        display.println("Test");
      } else if (submode == 1) {
        if (active) {
          if ((millis() / LCD_INTERVAL) % 2 == 0) {
            display.print("P>");
          } else if ((millis() / LCD_INTERVAL) % 2 == 1) {
            display.print("P ");
          }
          display.print(MotorPower);
        } else {
          display.println("Power");
        }
      }
    }
    display.setCursor(0, 17);
    display.setTextSize(1);
  } else if (UI.mode == 7) {
    display.println("Camera");
  }
  display.setCursor(0, 17);
  display.setTextSize(1);
  display.print("Battery: ");
  display.print(Battery);
  display.println(" V");
  display.print(line.flag);
  display.print(" ");
  display.print(ball.max[0]);
  display.print(" ");
  display.print(_Mdegree);
  // display.print(motor.val[3]);
  // display.print(gyro.deg);
  display.drawLine(0, 15, 127, 15, WHITE);
  // 描画バッファの内容を画面に表示
  display.display();
}

void _UI::Errordisplay(int code) {
  if (code != 0) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    if (code == 1) {
      display.println("EMERGENCY!");
      display.setCursor(0, 17);
      display.print("CODE:");
      display.print(code);
      for (int i = 0; i < 16; i++) {
        if ((millis() / 300) % 2 > 0) {
          strip.setPixelColor(i, 255, 0, 0);
        } else {
          strip.setPixelColor(i, 255, 255, 0);
        }
      }
      strip.show();
    }
    display.drawLine(0, 15, 127, 15, WHITE);
  }
  display.display();
}

void _UI::NeoPixelReset(int brightS, int brightL) {
  strip.begin();
  strip.show();
  strip.setBrightness(brightS);
  front.begin();
  front.show();
  front.setBrightness(brightL);
  rear.begin();
  rear.show();
  rear.setBrightness(brightL);
  left.begin();
  left.show();
  left.setBrightness(brightL);
  right.begin();
  right.show();
  right.setBrightness(brightL);
}

void _UI::SerialPrint(bool Display) {
  if (Display) {
    Serial.print(line.Rflag);
    Serial.print(" ");
    Serial.print(line.Rflag);
    Serial.print(" ");
    Serial.print(line.Move_degree);
    Serial.print(" ");
    Serial.print(line.leftdegree);
    Serial.print(" ");
    Serial.print(ball.Move_degree);
    Serial.print(" ");
    Serial.println(_Mdegree);
  }
}
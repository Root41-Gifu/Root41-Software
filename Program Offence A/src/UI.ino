_UI::_UI() {
  mode = 0;
  submode = 0;
  emergency = false;
}

void _UI::read() {
  Wire.requestFrom(UI_ADDRESS, 1);  //アドレスは変えてね
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
          mode--;
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
        active = true;
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
          mode--;
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
        active = true;
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
          mode--;
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
        active = true;
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
        mode--;
        switchScope--;
      }
      if (switchingFlag[2]) {
        // mode++;
        // switchScope++;
        //エラー対策
      }
    }
    if (switchingFlag[3]) {
      active = true;
    }
  }
  for (int i = 0; i <= 3; i++) {
    switchingFlag[i] = false;
  }
  if (mode < 0) {
    mode = 7;
  } else if (mode > 7) {
    mode = 0;
  }
  if (switchScope > 15) {
    switchScope = 0;
  } else if (switchScope < 0) {
    switchScope = 0;
  }
}

void _UI::NeoPixeldisplay(int _mode) {
  front.clear();
  rear.clear();
  left.clear();
  right.clear();
  strip.clear();
  if (_mode == 0) {
    if (ball.max_average[0] == 100) {
      for (int i = 0; i < BALL_NUM; i++) {
        strip.setPixelColor(i, 255, 0, 0);
      }
    } else {
      int i = ball.max_average[0] - 1;
      int k = ball.max_average[0] + 1;
      if (i < 0) {
        i = 16 + i;
      } else if (k > 15) {
        k = 0;
      }
      strip.setPixelColor(ball.max_average[0], 255, 0, 0);
      strip.setPixelColor(i, 255, 0, 0);
      strip.setPixelColor(k, 255, 0, 0);
      // strip.setPixelColor(ball.max_average[2], 0, 0, 255);
    }
  }
  if (_mode == 0) {
    if (ball.max[0] == 100) {
      for (int i = 0; i < BALL_NUM; i++) {
        strip.setPixelColor(i, 255, 0, 0);
      }
    } else {
      int i = ball.max[0] - 1;
      int k = ball.max[0] + 1;
      if (i < 0) {
        i = 16 + i;
      } else if (k > 15) {
        k = 0;
      }
      strip.setPixelColor(ball.max[0], 255, 0, 0);
      strip.setPixelColor(i, 255, 0, 0);
      strip.setPixelColor(k, 255, 0, 0);
      // strip.setPixelColor(ball.max_average[2], 0, 0, 255);
    }
  }
  if (mode == 5 || mode == 1) {
    if (active) {
      unsigned long lineNeoPixelColor = front.Color(255, 0, 0);
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
  }
  strip.show();
  front.show();
  rear.show();
  left.show();
  right.show();
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
    display.println("Kicker");
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
  display.println(line.Move_degree);
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
        strip.show();
      }
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
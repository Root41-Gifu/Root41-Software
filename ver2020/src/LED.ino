void _LED::gyroShow(unsigned long _color = 'hogehoge') {
  // //Serial.println(gyro.deg);
  if (abs(180 - gyro.deg) <= 178) {
    int deg2 = gyro.deg - 11;
    deg2 += 360;
    deg2 %= 360;
    if (deg2 > 180) {
      deg2 -= 360;
    }
    deg2 *= -1;
    deg2 += 360;
    deg2 %= 360;
    int light = 0;
    for (int i = 0; i <= 15; i++) {
      if (deg2 <= (i + 1) * 22.5) {
        light = i;
        break;
      }
    }

    if (LED.white) {
      RGBLED.setPixelColor(light, 255, 255, 255);
      RGBLED.setPixelColor((light + 1) % 16, 255, 255, 255);
      RGBLED.setPixelColor((light + 2) % 16, 255, 255, 255);
      RGBLED.setPixelColor((light + 15) % 16, 255, 255, 255);
      RGBLED.setPixelColor((light + 14) % 16, 255, 255, 255);
      RGBLED.setPixelColor((light + 13) % 16, 255, 255, 255);
    } else {
      if (_color == 'hogehoge') {
        RGBLED.setPixelColor(light, LED.defaultColor);
        RGBLED.setPixelColor((light + 1) % 16, LED.defaultColor);
        RGBLED.setPixelColor((light + 2) % 16, LED.defaultColor);
        RGBLED.setPixelColor((light + 15) % 16, LED.defaultColor);
        RGBLED.setPixelColor((light + 14) % 16, LED.defaultColor);
        RGBLED.setPixelColor((light + 13) % 16, LED.defaultColor);
        if (light == 0) {
          if (gyro.deg <= 180) {
            if (device.robot) {
              RGBLED.setPixelColor((light + 13) % 16, LED.MINT);
            } else {
              RGBLED.setPixelColor((light + 13) % 16, LED.PURPLE);
            }
          } else {
            if (device.robot) {
              RGBLED.setPixelColor((light + 2) % 16, LED.MINT);
            } else {
              RGBLED.setPixelColor((light + 2) % 16, LED.PURPLE);
            }
          }
        }

      } else {
        RGBLED.setPixelColor(light, _color);
        RGBLED.setPixelColor((light + 1) % 16, _color);
        RGBLED.setPixelColor((light + 2) % 16, _color);
        RGBLED.setPixelColor((light + 15) % 16, _color);
        RGBLED.setPixelColor((light + 14) % 16, _color);
        RGBLED.setPixelColor((light + 13) % 16, _color);
      }
    }
  } else {
    if (LED.white) {
      changeAll(WHITE);
    } else {
      if (_color == 'hogehoge') {
        changeAll(defaultColor);
      } else {
        changeAll(_color);
      }
    }
  }
}

void _LED::degShow(int d, unsigned long _color = 'hogehoge') {
  int deg2 = d + 11;

  deg2 += 360;
  deg2 %= 360;
  int light = 0;
  for (int i = 0; i <= 15; i++) {
    if (deg2 <= (i + 1) * 22.5) {
      light = i;
      break;
    }
  }

  if (_color == 'hogehoge') {
    _color = defaultColor;
  }

  int _d;

  if (dist == true) {
    _d = ball.dist;
  } else {
    _d = 1;
  }

  if (LED.white) {
    RGBLED.setPixelColor(light, 255, 255, 255);
    RGBLED.setPixelColor((light + 1) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 2) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 15) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 14) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 13) % 16, 255, 255, 255);
  } else {
    if (d != 1000) {
      RGBLED.setPixelColor(light, _color);
      RGBLED.setPixelColor((light + 1) % 16, _color);
      RGBLED.setPixelColor((light + 15) % 16, _color);
      RGBLED.setPixelColor((light + 14) % 16, _color);

      for (int i = 1; i <= _d; i++) {
        RGBLED.setPixelColor((light + 1 + i) % 16, _color);
        RGBLED.setPixelColor((light + 14 - i) % 16, _color);
      }

    } else {
      changeAll(255, 255, 255);
    }
  }
}

void _LED::changeAll(int red, int green, int blue) {
  if (!LED.white) {
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, red, green, blue);
    }
  } else {
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, 0, 0, 0);
    }
  }
}

void _LED::changeAll(unsigned long _color) {
  if (!LED.white) {
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, _color);
    }
  } else {
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, 0, 0, 0);
    }
  }
}

void _LED::animation1(void) {
  for (int i = 0; i <= 15; i++) {
    RGBLED.begin();
    RGBLED.setBrightness(bright);
    LED.changeAll(0, 0, 0);
    for (int k = 0; k <= i; k++) {
      RGBLED.setPixelColor(k, WHITE);
    }
    RGBLED.show();
    device.waitTime(15);
    gyro.deg = gyro.read();
  }
}

void _LED::animation2(void) {
  if (!digitalRead(SW_RESET) && !digitalRead(SW_1)) {
    device.waitTime(500);
    while (true) {
      changeAll(LED.NONE);
      colorWipe(RGBLED.Color(255, 0, 0), 50);
      colorWipe(RGBLED.Color(0, 255, 0), 50);
      colorWipe(RGBLED.Color(0, 0, 255), 50);
      rainbow(10);
    }
  }
  device.waitTime(500);
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < RGBLED.numPixels(); i++) {
    RGBLED.setPixelColor(i, color);
    RGBLED.show();
    device.waitTime(wait);
  }
}

void rainbow(int wait) {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536;
       firstPixelHue += 256) {
    for (int i = 0; i < RGBLED.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / RGBLED.numPixels());
      // RGBLED.setPixelColor(i, RGBLED.gamma32(RGBLED.ColorHSV(pixelHue)));
    }
    RGBLED.show();
    device.waitTime(wait);
  }
}

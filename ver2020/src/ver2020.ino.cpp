# 1 "/var/folders/jc/vnhnz_29013d_2j1xgjg51m80000gn/T/tmpf8nmdtv5"
#include <Arduino.h>
# 1 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/ver2020.ino"

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <MPU6050_6Axis_MotionApps20.h>

#include <VL53L0X.h>
#include <Wire.h>


int BALL[16] = {A0, A1, A2, A3, A4, A5, A6, A7,
                A8, A9, A10, A11, A12, A13, A14, A15};
#define BALL_RESET 26
#define BALL_HOLD 29

int LINE[20] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                40, 41, 42, 43, 44, 45, 46, 47, 48, 49};

#define LINE_BRIGHT 12

#define SW_1 22
#define SW_2 25
#define SW_RESET 23

#define SOLENOID 27


Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(16, 28, NEO_GRB + NEO_KHZ800);
VL53L0X TOF;

class _ball {
 public:
  void read(int* b);
  void calc(void);
  void readDistance(void);

  bool exist;

  int val[16];
  int top;
  int second;
  int third;
  int deg;
  int offset;

  float dist;

  int speed;

  unsigned long speedTimer;

  int position = 0;
  unsigned long positionTimer;
  unsigned long driftTimer;
  unsigned long holdTimer;

 private:
  float LPF = 0.4;

} ball;

class _line {
 public:
  _line(void);
  void read(void);
  void brightnessAdjust(void);
  void process(void);
  void linetrace(void);
  int calc(void);



  int deg;

  bool flag;
  bool touch;
  bool approach;
  bool gyrobreak;

  float vector[20][2];

  int _mode;

  int bright;
  int dif;
  int speed = 100;
  int side;
  int approachdeg;

  int incount = 100;
  int over;
  int whiting;
  int reflection;
  int just;

 private:


  int deglog;

  int now;
  int first;
  int whited;

  bool stoping;
  bool s;

  int error;

  float x;
  float y;

  bool val[20];
  int order[20];
  int check[20];

  unsigned long stopTimer;
  unsigned long overTimer;
  unsigned long stopingTimer[20];

  unsigned long stopTime[20];
} line;

class _motor {
 public:
  _motor(void);
  void directDrive(int* p);
  void drive(int _deg, int _power, bool _stop = false);

  int val[4];
  int calcVal[4][360];
  int deg;
  int speed;
  int count;
  int time = 5;
  int referenceAngle = 0;

  unsigned long timer;

  int direction = 0;

 private:
  float Kp;
  float Ki;
  float Kd;

  int integral = 0;
  int gyroOld;
} motor;

class _gyro {
 public:
  void setting(void);
  void calibrationEEPROM(void);
  void offsetRead(void);
  int read(void);
  int differentialRead(void);

  int deg;
  int eeprom[6];

  bool isLift = false;

 private:

  int differentialDeg = 0;
  int offsetVal;
} gyro;

class _tof {
 public:
  int read(void);
  int _dist;

 private:
} tof;

class _position {
 public:
  void reflection(void);
  void get(void);

  bool rock;
  int side[4];
  int vertical[4];
  int reliability;
  unsigned long offTimer;

 private:
} position;

class _device {
 public:
  void initialize(void);
  void check(void);
  void UI(void);
  void discharge(void);
  unsigned long getTime(void);
  void waitTime(unsigned long _time);

  unsigned long startTimer;

  bool robot;

  int mode = 0;

 private:

} device;

class _LED {
 public:
  void gyroShow(unsigned long _color = 'hogehoge');
  void changeAll(int red, int green, int blue);
  void changeAll(unsigned long _color);
  void degShow(int d, unsigned long _color = 'hogehoge');
  void animation1(void);
  void animation2(void);

  bool white = false;
  bool dist = false;

  int bright = 150;

  unsigned long defaultColor;
  unsigned long RED;
  unsigned long BLUE;
  unsigned long GREEN;
  unsigned long YELLOW;
  unsigned long WHITE;
  unsigned long PURPLE;
  unsigned long MINT;
  unsigned long LIME;
  unsigned long ORANGE;
  unsigned long NONE;

 private:

} LED;

class _kicker {
 public:
  void kick(bool status);

  bool val;

 private:
  bool _val = false;

  unsigned long protectionTimer = 0;
  unsigned long kickTimer = 0;
} kicker;

class _keeper {
 public:
  int supportdistance;
  int mode;
  int deg;
  int speed;
  bool setup = 0;
  bool Front;
  bool frontmode;
  int modein;
  int count;
  int frontball;
  unsigned long banTimer;
  unsigned long atackTimer;
  unsigned long offTimer;

 private:
} keeper;
void setup(void);
void loop(void);
void colorWipe(uint32_t color, int wait);
void rainbow(int wait);
float myMap(float x, float in_min, float in_max, float out_min, float out_max);
void dmpDataReady();
void meansensors();
void calibration();
#line 272 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/ver2020.ino"
void setup(void) {
  RGBLED.begin();
  RGBLED.show();

  device.initialize();
  TWBR = 12;
  device.mode = 0;

  for (int i = 0; i <= 19; i++) {
    line.vector[i][0] = sin(radians(i * 18));
    line.vector[i][1] = cos(radians(i * 18));
    line.vector[5][1] = 0;
    line.vector[15][1] = 0;
  }

  Serial.begin(115200);
  Serial.println("Root41 2020");
  Serial.print("Robot Number:");
  Serial.println(device.robot + 1);
  Serial.print("GyroOffsetX:");
  Serial.println(gyro.eeprom[0]);
  Serial.print("GyroOffsetY:");
  Serial.println(gyro.eeprom[1]);
  Serial.print("GyroOffsetZ:");
  Serial.println(gyro.eeprom[2]);
  Serial.print("AccelOffsetX:");
  Serial.println(gyro.eeprom[3]);
  Serial.print("AccelOffsetY:");
  Serial.println(gyro.eeprom[4]);
  Serial.print("AccelOffsetZ:");
  Serial.println(gyro.eeprom[5]);

  gyro.setting();


  LED.animation1();
  LED.animation2();


  device.discharge();

  gyro.offsetRead();

  device.mode = 1;
}

void loop(void) {
  unsigned long errorTimer = device.getTime();
  device.check();

  if (device.mode == 0) {
    gyro.deg = gyro.read();
    LED.gyroShow();
    motor.drive(NULL, NULL, true);
    analogWrite(LINE_BRIGHT, 10);


    device.UI();



    if (device.getTime() - device.startTimer <= 1000) {
      gyro.offsetRead();
    }
  } else if (device.mode == 1) {


    if (!line.flag) {
      LED.gyroShow();
      ball.read(ball.val);
      ball.calc();







    }

    line.read();
    line.deg = line.calc();
    line.process();

    position.get();
    position.reflection();


    motor.deg = ball.deg;
    motor.speed = ball.speed;
    bool stop = false;

    if (line.flag) {
      motor.deg = line.deg;
      motor.speed = line.speed;
      LED.changeAll(LED.WHITE);

      if (motor.deg == 1000) {
        stop = true;
      }
    }





    digitalWrite(LINE_BRIGHT, HIGH);



    kicker.kick(kicker.val);

    motor.timer = device.getTime();

    for (motor.count = 0; motor.count < motor.time; motor.count++) {
      line.read();
      line.deg = line.calc();
      line.process();
      gyro.deg = gyro.read();
      motor.drive(motor.deg, motor.speed, stop);
      if (motor.count >= 1) {
        digitalWrite(BALL_RESET, HIGH);
      }

      if (line.flag) {
        break;
      }
    }

  } else if (device.mode == 2) {

    LED.gyroShow();

    ball.read(ball.val);

    ball.calc();
    line.read();
    line.linetrace();
    position.reflection();

    if (ball.top < 3 || ball.top > 13) {
      keeper.speed = 85;
    } else if (ball.top <= 5 || ball.top >= 11) {
      keeper.speed = 95;
    } else {
      keeper.speed = 95;
    }
    keeper.deg = line.deg;
    gyro.deg = gyro.read();


    kicker.kick(kicker.val);

    if (keeper.deg != 1000) {
      if (gyro.deg <= 30 || gyro.deg >= 330) {


        for (motor.count = 0; motor.count < motor.time; motor.count++) {

          gyro.deg = gyro.read();
          motor.drive(keeper.deg, keeper.speed, false);
          if (motor.count >= 3) {
            digitalWrite(BALL_RESET, HIGH);
          }
        }
      } else {
        motor.drive(0, 0, true);
      }
    } else {
      motor.drive(0, 0, true);
    }

  }

  while (Wire.available()) {
    Wire.read();
  }
# 467 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/ver2020.ino"
}
# 1 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/LED.ino"
void _LED::gyroShow(unsigned long _color = 'hogehoge') {

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

    }
    RGBLED.show();
    device.waitTime(wait);
  }
}
# 1 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/ball.ino"
void _ball::read(int* b) {
  digitalWrite(BALL_RESET, HIGH);
  for (int i = 0; i <= 15; i++) {

    *(b + i) = analogRead(BALL[i]);
    if ((i == 6 || i == 13) && !device.robot) {
      *(b + i) = 700;
    }
    Serial.print(*(b + i));
    Serial.print(" ");
  }
  Serial.println(" ");
  digitalWrite(BALL_RESET, LOW);

  *b *= 0.98;
}

void _ball::calc(void) {
  int existCount = 0;
  top = 0;
  dist = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      third = second;
      second = top;
      top = i;
    } else if (val[i] <= val[second]) {
      third = second;
      second = i;
    } else if (val[i] <= val[third]) {
      third = i;
    }

    if (val[i] >= 690) {
      existCount++;
      dist += 690;
    } else {
      dist += val[i];
    }

  }

  dist /= 16;


  if (existCount <= 13) {
    exist = true;
    deg = top * 22.5;
    offset = 180 - abs(180 - deg);

    Serial.println(position);

    offset = offset * 0.3;
    if (dist <= 465) {
      offset *= 1.4;
      offset += 40;
    } else if (dist <= 485 && (top <= 4 || top >= 12)) {
      offset *= 1.23;
      offset += 40;
    } else if (dist <= 525 && (top > 4 && top < 12)) {
      offset *= 1.4;
      offset += 40;
    } else if (dist <= 510) {
      offset *= 1.2;
      offset += 15;
    }
    offset = constrain(offset, 0, 100);




    if (top >= 1 && top <= 15) {
      if (top <= 8) {
        deg += offset;
      } else {
        deg -= offset;
      }
    }


    if (!digitalRead(BALL_HOLD)) {
      holdTimer = device.getTime();
      kicker.val = false;

    }

    if (digitalRead(BALL_HOLD) && !(top > 3 && top < 13)) {
      if (device.getTime() - holdTimer >= 60) {
        kicker.val = true;





        if (device.getTime() - speedTimer >= 600 || speedTimer == 0) {
          speedTimer = device.getTime();
        }
      }
    }

    LED.dist = true;


    speed = 100;


    if (deg >= 50 && deg <= 120) {
      if (position < 0) {
        position = 0;
        positionTimer = device.getTime();
      } else {
        position++;
      }
    } else if (deg >= 200 && deg <= 240) {
      if (position > 0) {
        position = 0;
        positionTimer = device.getTime();
      } else {
        position--;
      }
    }


  } else {
    exist = false;
    speed = NULL;
    deg = NULL;
  }
}

void _ball::readDistance(void) {
  static float tempDist;
  if (true) {
    if (top != 4 && top != 12) {
      val[4] *= 0.98;
      val[12] *= 0.98;
      tempDist += (1 - 0.2) *
                  (min(val[(top + 2) % 16], val[(top + 14) % 16]) - tempDist);
    } else {
      tempDist += (1 - 0.2) *
                  (min(val[(top + 1) % 16], val[(top + 15) % 16]) - tempDist);
    }
  } else {
    if (top != 4 && top != 12 && top != 8 && top != 0) {
      tempDist = min(val[(top + 2) % 16], val[(top + 14) % 16]);
    } else {
      tempDist = min(val[(top + 1) % 16], val[(top + 15) % 16]);
    }
  }

  dist = constrain(myMap(tempDist, 400, 530, 5, 0), 0, 6);
  if (abs(8 - top) >= 6) {
    dist *= 0.9;
  }


}

float myMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
# 1 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/device.ino"
void _device::initialize(void) {
  TCCR0B = (TCCR0B & 0b11111000) | 0x04;
  TCCR1B = (TCCR1B & 0b11111000) | 0x04;
  TCCR2B = (TCCR2B & 0b11111000) | 0x04;
  TCCR3B = (TCCR3B & 0b11111000) | 0x04;
  TCCR4B = (TCCR4B & 0b11111000) | 0x04;

  LED.RED = RGBLED.Color(255, 0, 0);
  LED.BLUE = RGBLED.Color(0, 0, 255);
  LED.GREEN = RGBLED.Color(0, 255, 0);
  LED.YELLOW = RGBLED.Color(255, 255, 0);
  LED.WHITE = RGBLED.Color(255, 255, 255);
  LED.PURPLE = RGBLED.Color(255, 0, 200);
  LED.ORANGE = RGBLED.Color(255, 100, 0);
  LED.MINT = RGBLED.Color(100, 255, 50);
  LED.LIME = RGBLED.Color(190, 255, 0);
  LED.NONE = RGBLED.Color(0, 0, 0);

  for (int i = 0; i <= 359; i++) {
    float s;
    motor.calcVal[0][i] = round(sin(radians(i - 300)) * 100.0);
    motor.calcVal[1][i] = round(sin(radians(i - 60)) * 100.0);
    motor.calcVal[2][i] = round(sin(radians(i - 225)) * 100.0);
    motor.calcVal[3][i] = round(sin(radians(i - 135)) * 100.0);

    int valTemp[4];
    for (int k = 0; k < 4; k++) {
      valTemp[k] = motor.calcVal[k][i];
    }

    for (int k = 0; k < 4; ++k) {
      for (int j = k + 1; j < 4; ++j) {
        if (abs(valTemp[k]) >= abs(valTemp[j])) {
          int temp = valTemp[k];
          valTemp[k] = valTemp[j];
          valTemp[j] = temp;
        }
      }
    }
    s = 255.0 / float(abs(valTemp[3]));
    motor.calcVal[0][i] = round((float)motor.calcVal[0][i] * s);
    motor.calcVal[1][i] = round((float)motor.calcVal[1][i] * s);
    motor.calcVal[2][i] = round((float)motor.calcVal[2][i] * s);
    motor.calcVal[3][i] = round((float)motor.calcVal[3][i] * s);
  }

  Wire.begin();

  TOF.init();
  TOF.setTimeout(1);
  TOF.setMeasurementTimingBudget(10);
  TOF.startContinuous(0);

  RGBLED.begin();
  RGBLED.setBrightness(LED.bright);
  LED.changeAll(LED.BLUE);
  RGBLED.show();




  pinMode(BALL_RESET, OUTPUT);
  pinMode(BALL_HOLD, INPUT);

  pinMode(LINE_BRIGHT, OUTPUT);
  for (int i = 0; i <= 19; i++) {
    pinMode(LINE[i], INPUT_PULLUP);
  }

  pinMode(SW_1, INPUT_PULLUP);
  pinMode(SW_2, INPUT_PULLUP);
  pinMode(SW_RESET, INPUT_PULLUP);

  pinMode(SOLENOID, 27);

  if (EEPROM[0] == 1) {

    device.robot = true;

    LED.defaultColor = LED.GREEN;
  } else {

    device.robot = false;

    LED.defaultColor = LED.RED;
  }

  gyro.eeprom[0] = (EEPROM[1] * 256) + EEPROM[2];
  gyro.eeprom[1] = (EEPROM[3] * 256) + EEPROM[4];
  gyro.eeprom[2] = (EEPROM[5] * 256) + EEPROM[6];
  gyro.eeprom[3] = (EEPROM[7] * 256) + EEPROM[8];
  gyro.eeprom[4] = (EEPROM[9] * 256) + EEPROM[10];
  gyro.eeprom[5] = (EEPROM[11] * 256) + EEPROM[12];
}

void _device::check(void) {
  RGBLED.show();
  RGBLED.begin();
  RGBLED.clear();

  if (!digitalRead(SW_RESET)) {
    device.mode = 0;
  } else if (!digitalRead(SW_1)) {
    device.mode = 1;

    digitalWrite(LINE_BRIGHT, HIGH);
  } else if (!digitalRead(SW_2)) {
    device.mode = 2;
    digitalWrite(LINE_BRIGHT, HIGH);

  }
}

void _device::UI(void) {
  if (!digitalRead(SW_RESET)) {
    RGBLED.setBrightness(LED.bright * 0.2);
    RGBLED.show();
    if (!digitalRead(SW_1)) {
      RGBLED.setBrightness(LED.bright);
      RGBLED.show();
      LED.changeAll(LED.BLUE);
      RGBLED.show();

      device.waitTime(20);
      while (!digitalRead(SW_RESET)) {
      }
      device.waitTime(200);

      while (true) {
        if (!digitalRead(SW_RESET)) {
          break;
        }

        if (!digitalRead(SW_1)) {
          LED.changeAll(LED.ORANGE);
          RGBLED.show();
          gyro.offsetRead();
          gyro.offsetRead();
          gyro.offsetRead();
          LED.animation1();
          device.waitTime(500);
          break;
        }

        if (!digitalRead(SW_2)) {
          LED.changeAll(LED.MINT);
          RGBLED.show();

          while (true) {
            LED.changeAll(LED.MINT);
            RGBLED.show();
            if (!digitalRead(SW_2)) {
              LED.changeAll(LED.LIME);
              RGBLED.show();
              digitalWrite(SOLENOID, HIGH);
              device.waitTime(200);
              digitalWrite(SOLENOID, LOW);
              device.waitTime(1200);
            }

            if (!digitalRead(SW_RESET)) {
              break;
            }
          }
          break;
        }
      }

    } else if (!digitalRead(SW_2)) {
      RGBLED.setBrightness(LED.bright);
      RGBLED.show();
      LED.changeAll(LED.YELLOW);
      RGBLED.show();

      device.waitTime(20);
      while (!digitalRead(SW_RESET)) {
      }
      device.waitTime(200);

      while (true) {
        if (!digitalRead(SW_RESET)) {
          break;
        }

        if (!digitalRead(SW_1)) {
          LED.changeAll(LED.PURPLE);
          RGBLED.show();
          gyro.calibrationEEPROM();
          gyro.offsetRead();
          break;
        }

        if (!digitalRead(SW_2)) {
          LED.changeAll(LED.GREEN);
          RGBLED.show();
          gyro.setting();
          gyro.read();
          LED.animation1();
          gyro.offsetRead();
          device.waitTime(500);
          break;
        }
      }
    }
  } else {
    RGBLED.setBrightness(LED.bright);
    RGBLED.show();
  }
}

unsigned long _device::getTime(void) {
  return millis() * 4;
}

void _device::waitTime(unsigned long _time) {
  delay(_time / 4);
}

void _device::discharge(void) {
  if (!digitalRead(SW_RESET) && !digitalRead(SW_2)) {
    while (true) {
      LED.changeAll(LED.YELLOW);
      RGBLED.show();
      for (int i = 0; i <= 3; i++) {
        motor.val[i] = 255;
      }
      motor.directDrive(motor.val);
      device.waitTime(500);

      for (int i = 0; i <= 3; i++) {
        motor.val[i] = -255;
      }
      motor.directDrive(motor.val);
      device.waitTime(500);
    }
  }
}
# 1 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/gyro.ino"



MPU6050 mpu;
static uint8_t mpuIntStatus;
static bool dmpReady = false;
static uint16_t packetSize;
int16_t Gyro_Now = 0, Gyro = 0, Gyro_Offset = 0;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float ypr[3];

uint8_t devStatus;

VectorInt16 aa;
VectorInt16 aaReal;
VectorInt16 dmpgyro;
float euler[3];

volatile bool mpuInterrupt = false;


void dmpDataReady() {
  mpuInterrupt = true;
}


void _gyro::setting(void) {
RESTART:
  mpu.initialize();
  if (mpu.testConnection() != true) {
    goto RESTART;
  }
  devStatus = mpu.dmpInitialize();
  if (devStatus != 0) {
    goto RESTART;
  }

  mpu.setXGyroOffset(eeprom[0]);
  mpu.setYGyroOffset(eeprom[1]);
  mpu.setZGyroOffset(eeprom[2]);
  mpu.setXAccelOffset(eeprom[3]);
  mpu.setYAccelOffset(eeprom[4]);
  mpu.setZAccelOffset(eeprom[5]);
  mpu.setDMPEnabled(true);


  mpuIntStatus = mpu.getIntStatus();

  dmpReady = true;

  packetSize = mpu.dmpGetFIFOPacketSize();

  offsetVal = 0;
}


int _gyro::read(void) {
  int tempDeg;

  mpuIntStatus = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
  } else if (mpuIntStatus & 0x02) {
    while (fifoCount < packetSize) {
      fifoCount = mpu.getFIFOCount();
    }
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    mpu.dmpGetGyro(&dmpgyro, fifoBuffer);
    Gyro_Now = degrees(ypr[0]);
    Gyro = Gyro_Now;
    while (Gyro <= 0) {
      Gyro += 360;
    }
    Gyro %= 360;
  }
  tempDeg = ((360 - Gyro) % 360 - offsetVal + 720) % 360;
  while (tempDeg < 0) {
    tempDeg += 360;
  }






  if (degrees(ypr[2]) <= 165 && degrees(ypr[2]) >= -165) {
    isLift = true;
  } else if (degrees(ypr[1]) <= 165 && degrees(ypr[1]) >= -165) {
    isLift = true;
  } else {
    isLift = false;
  }
  return tempDeg % 360;
}


int _gyro::differentialRead(void) {
  if (!dmpReady)
    return;

  while (!mpuInterrupt && fifoCount < packetSize) {
  }

  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  fifoCount = mpu.getFIFOCount();

  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
    return;
  }

  while (fifoCount < packetSize) {
    fifoCount = mpu.getFIFOCount();
  }

  mpu.getFIFOBytes(fifoBuffer, packetSize);
  fifoCount -= packetSize;

  mpu.dmpGetGyro(&dmpgyro, fifoBuffer);

  return dmpgyro.z;
}

int buffersize = 1000;
int acel_deadzone = 8;
int giro_deadzone = 1;

MPU6050 accelgyro(0x68);

int16_t ax, ay, az, gx, gy, gz;

int mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz, state = 0;
int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;

void _gyro::calibrationEEPROM(void) {
  accelgyro.initialize();

  accelgyro.setXAccelOffset(0);
  accelgyro.setYAccelOffset(0);
  accelgyro.setZAccelOffset(0);
  accelgyro.setXGyroOffset(0);
  accelgyro.setYGyroOffset(0);
  accelgyro.setZGyroOffset(0);

  if (state == 0) {
    meansensors();
    state++;
    device.waitTime(100);
  }

  if (state == 1) {
    Serial.println("\nCalculating offsets...");
    calibration();
    state++;
    device.waitTime(100);
  }

  if (state == 2) {
    meansensors();
    Serial.println("\nFINISHED!");
    Serial.print("Your offsets:\t");
    Serial.print(ax_offset);
    Serial.print("\t");
    Serial.print(ay_offset);
    Serial.print("\t");
    Serial.print(az_offset);
    Serial.print("\t");
    Serial.print(gx_offset);
    Serial.print("\t");
    Serial.print(gy_offset);
    Serial.print("\t");
    Serial.println(gz_offset);


    EEPROM[1] = highByte(gx_offset);
    EEPROM[2] = lowByte(gx_offset);

    EEPROM[3] = highByte(gy_offset);
    EEPROM[4] = lowByte(gy_offset);

    EEPROM[5] = highByte(gz_offset);
    EEPROM[6] = lowByte(gz_offset);

    EEPROM[7] = highByte(ax_offset);
    EEPROM[8] = lowByte(ax_offset);

    EEPROM[9] = highByte(ay_offset);
    EEPROM[10] = lowByte(ay_offset);

    EEPROM[11] = highByte(az_offset);
    EEPROM[12] = lowByte(az_offset);
    setting();
    LED.animation1();
    device.waitTime(500);
    return;
  }
}

void meansensors() {
  long i = 0, buff_ax = 0, buff_ay = 0, buff_az = 0, buff_gx = 0, buff_gy = 0,
       buff_gz = 0;

  while (i < (buffersize + 101)) {

    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    if (i > 100 &&
        i <= (buffersize + 100)) {
      buff_ax = buff_ax + ax;
      buff_ay = buff_ay + ay;
      buff_az = buff_az + az;
      buff_gx = buff_gx + gx;
      buff_gy = buff_gy + gy;
      buff_gz = buff_gz + gz;
    }
    if (i == (buffersize + 100)) {
      mean_ax = buff_ax / buffersize;
      mean_ay = buff_ay / buffersize;
      mean_az = buff_az / buffersize;
      mean_gx = buff_gx / buffersize;
      mean_gy = buff_gy / buffersize;
      mean_gz = buff_gz / buffersize;
    }
    i++;
    device.waitTime(2);
  }
}

void calibration() {
  ax_offset = -mean_ax / 8;
  ay_offset = -mean_ay / 8;
  az_offset = (16384 - mean_az) / 8;

  gx_offset = -mean_gx / 4;
  gy_offset = -mean_gy / 4;
  gz_offset = -mean_gz / 4;
  while (1) {
    int ready = 0;
    accelgyro.setXAccelOffset(ax_offset);
    accelgyro.setYAccelOffset(ay_offset);
    accelgyro.setZAccelOffset(az_offset);

    accelgyro.setXGyroOffset(gx_offset);
    accelgyro.setYGyroOffset(gy_offset);
    accelgyro.setZGyroOffset(gz_offset);

    meansensors();
    Serial.println("...");

    LED.changeAll(LED.NONE);
    RGBLED.show();
    device.waitTime(300);
    LED.changeAll(LED.RED);
    RGBLED.show();

    if (abs(mean_ax) <= acel_deadzone)
      ready++;
    else
      ax_offset = ax_offset - mean_ax / acel_deadzone;

    if (abs(mean_ay) <= acel_deadzone)
      ready++;
    else
      ay_offset = ay_offset - mean_ay / acel_deadzone;

    if (abs(16384 - mean_az) <= acel_deadzone)
      ready++;
    else
      az_offset = az_offset + (16384 - mean_az) / acel_deadzone;

    if (abs(mean_gx) <= giro_deadzone)
      ready++;
    else
      gx_offset = gx_offset - mean_gx / (giro_deadzone + 1);

    if (abs(mean_gy) <= giro_deadzone)
      ready++;
    else
      gy_offset = gy_offset - mean_gy / (giro_deadzone + 1);

    if (abs(mean_gz) <= giro_deadzone)
      ready++;
    else
      gz_offset = gz_offset - mean_gz / (giro_deadzone + 1);

    if (ready >= 6)
      break;
  }
}

void _gyro::offsetRead(void) {
  offsetVal = 0;




  for (int i = 0; i < 150; i++) {
    deg = gyro.read();
  }

  delay(100);
  deg = gyro.read();

  if (deg != 0) {
    offsetVal += deg;
  }

  while (offsetVal < 0) {
    offsetVal += 360;
  }
  offsetVal %= 360;

  Serial.println(gyro.read());




}
# 1 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/kicker.ino"
void _kicker::kick(bool status) {
  if (status != _val && status && device.getTime() - protectionTimer >= 1000) {
    kickTimer = device.getTime();
    digitalWrite(SOLENOID, HIGH);

    LED.changeAll(LED.YELLOW);

  } else if (device.getTime() - kickTimer <= 200) {
    digitalWrite(SOLENOID, HIGH);
    LED.changeAll(LED.YELLOW);

    protectionTimer = device.getTime();

  } else {
    digitalWrite(SOLENOID, LOW);
  }
  _val = status;
}
# 1 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/line.ino"





_line::_line(void) {
  for (int i = 0; i <= 19; i++) {
    val[i] = false;
  }
}

int _line::calc(void) {
  float _deg;
  float _x = 0;
  float _y = 0;
  if (flag) {
    for (int i = 0; i <= 11; i++) {
      if (order[i] != 100) {
        _x += vector[order[i]][0];
        _y += vector[order[i]][1];
      }
    }
    if (_x == 0 || _y == 0) {
      if (_x == 0) {
        if (_y > 0) {
          _deg = 0;
        } else {
          _deg = 180;
        }
      } else if (_y == 0) {
        if (_x > 0) {
          _deg = 90;
        } else {
          _deg = 270;
        }
      }
    } else {
      _deg = atan2(_x, _y);
      _deg = degrees(_deg);
      if (_deg < 180) {
        _deg += 180;
      } else {
        _deg -= 180;
      }
    }
  } else {
    _deg = 1000;
  }
  return _deg;
}

void _line::process(void) {
  speed = 100;
  if (flag) {
    if (deg != 1000) {

      if (deg <= 35 || deg >= 325) {
        side = 0;
      } else if (deg >= 145 && deg <= 215) {
        side = 2;
      } else if (deg < 180) {
        side = -1;
      } else if (deg > 180) {
        side = 1;
      }
    }
    if (_mode == 0) {

      flag = false;
    } else if (_mode == 1) {

      deg = 1000;
      if (!touch) {

        overTimer = device.getTime();
        _mode = 3;
      }
      if (device.getTime() - stopTimer >= 300) {

        _mode = 2;
      }

      approach = false;
      for (int i = 0; i <= 19; i++) {
        if (stopTime[i] >= 70) {
          if (abs(side) == 1) {
            if (ball.top >= 2 && ball.top <= 5) {
              if (abs(side) == 1) {
                _mode = 2;
                approachdeg = ball.top * 22.5;

              }
            } else if (ball.top >= 11 && ball.top <= 14) {
              if (abs(side) == 1) {
                _mode = 2;
                approachdeg = ball.top * 22.5;

              }
            }
          }
        }
      }
    } else if (_mode == 2) {

      if (approach) {
        _mode = 4;
      }
      if (touch && side == 2) {
# 117 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/line.ino"
      }
      if (!touch) {
        overTimer = device.getTime();
        _mode = 0;
      }
    } else if (_mode == 3) {






      if (touch) {
        _mode = 2;
      }
    } else if (_mode == 4) {

      if (ball.deg != 1000) {
        if (touch) {
          deg = approachdeg;
          speed = 80;
        } else {
          approach = false;
          _mode = 3;
        }
      } else {
        _mode = 2;
      }
    }
  } else {

    for (int i = 0; i <= 19; i++) {
      val[i] = false;
      order[i] = 100;
      check[i] = 0;
      stopTime[i] = 0;
    }
    s = false;
    approach = false;
    now = 100;
    first = 100;
    whited = 0;
    side = 0;
    _mode = 0;
    error = 0;
  }
}

void _line::brightnessAdjust(void) {
  int lowestBright = 255;
  int highestBright = 255;
  int accumulation = 0;
  bool reacted[20] = {};

  for (int i = 0; i <= 255; i++) {
    analogWrite(46, i);
    for (int j = 0; j <= 19; j++) {
      pinMode(LINE[j], INPUT);
      bool val = !digitalRead(LINE[j]);
      if (val) {
        if (lowestBright == 255) {
          lowestBright = i;
        }
        if (!reacted[j]) {
          accumulation++;
        }
        reacted[j] = true;
      }
      Serial.print(val);
    }
    Serial.println("");
    if (accumulation >= 8) {
      highestBright = i;

    }
    delay(0);
  }

  bright = (lowestBright + highestBright * 2) / 3;
  bright = constrain(bright, 0, 255);
  EEPROM[13] = bright;

  analogWrite(LINE_BRIGHT, bright);

  Serial.println(bright);
}

void _line::read(void) {

  touch = false;
  whiting = 0;
  for (int i = 0; i <= 19; i++) {
    if (!digitalRead(LINE[i])) {
      whiting++;
      just = i;
      if (whited == 0) {
        first = i;
      }
      if (check[i] == 0) {
        now = i;
        order[whited] = now;
        whited++;
        check[i] = 1;
      }
      if (!flag) {
        stopTimer = device.getTime();
        first = now;
        _mode = 1;
      }
      if (!val[i]) {
        stopingTimer[i] = device.getTime();
      }
      stopTime[i] = device.getTime() - stopingTimer[i];
      flag = true;
      val[i] = true;
      touch = true;
    } else {
      val[i] = false;
    }
  }
}

void _line::linetrace(void) {
  if (!keeper.setup) {
    if (touch) {
      keeper.mode = 0;
    } else {
      device.mode = 1;

    }
    keeper.setup = true;
  }
  if (keeper.mode == 0) {

    if (line.whiting <= 1) {
      keeper.mode = 3;





      keeper.offTimer = device.getTime();
    }
    if (!digitalRead(BALL_HOLD)) {
      ball.holdTimer = device.getTime();
      kicker.val = false;

    }
    if (digitalRead(BALL_HOLD) && !(ball.top > 3 && ball.top < 13)) {
      if (device.getTime() - ball.holdTimer >= 70) {
        kicker.val = true;
# 276 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/line.ino"
      }
    }
    if (keeper.modein >= 5 && keeper.frontmode &&
        device.getTime() - keeper.banTimer >= 3000) {
      keeper.mode = 2;
      keeper.atackTimer = device.getTime();
    }





    if (keeper.count >= 50) {
      if (keeper.frontball >= 45) {
        keeper.Front = true;
        keeper.modein++;
      } else {
        keeper.Front = false;
        keeper.modein = 0;
      }
      keeper.frontball = 0;
      keeper.count = 0;
    } else {
      if (ball.top < 2 || ball.top > 14) {
        keeper.frontball++;
      }
      keeper.count++;
    }
    keeper.frontmode = false;
    if (ball.top < 2 || ball.top > 14) {
      line.deg = 1000;
      keeper.frontmode = true;

    } else if (ball.top < 4 || ball.top > 12) {
      if (ball.top < 8) {
        line.deg = 90;

      } else {
        line.deg = 270;

      }
    } else if (ball.top < 8) {
      line.deg = 90;
    } else {
      line.deg = 270;
    }
    int checkFront = 0;
    for (int i = 0; i <= 19; i++) {
      if (i < 4 || i > 14) {
        if (line.val[i]) {
          checkFront++;
        }
      }
    }
    if (checkFront < 2 && whiting > 0 && line.deg != 1000) {
      if (line.deg < 180) {
        line.deg += 20;
      } else {
        line.deg -= 20;
      }
    }
  } else if (keeper.mode == 1) {

  } else if (keeper.mode == 2) {

    if (device.getTime() - keeper.atackTimer <= 100) {
      line.deg = 0;
    } else {
      keeper.mode = 0;
    }
    keeper.banTimer = device.getTime();
  } else if (keeper.mode == 3) {

    if (gyrobreak && whiting <= 3) {
      line.deg = 180;
    } else if (whiting <= 3) {
      line.deg = just * 18;
      if (just >= 4 && just <= 16) {
        if (line.deg < 180) {
          line.deg -= 35;
        } else {
          line.deg += 35;
        }
      }
      if (device.getTime() - keeper.offTimer >= 500) {
        line.deg = 180;
      }







    } else {
      line.deg = 1000;
      keeper.mode = 0;
    }
  } else if (keeper.mode == 4) {

    if (device.getTime() - keeper.atackTimer <= 1000) {
      line.deg = 0;
    } else {
      keeper.mode = 0;
    }
    keeper.banTimer = device.getTime();
  }
}
# 1 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/motor.ino"
_motor::_motor(void) {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void _motor::drive(int _deg, int _power, bool _stop = false) {

  for (int i = 0; i <= 3; i++) {
    val[i] = 0;
  }

  if (!_stop) {


    int minimum = 0;

    direction = (gyro.deg - referenceAngle + 360) % 360;
    direction = direction > 180 ? direction - 360 : direction;

    if(abs(direction) >= 50){
      _deg = NULL;
      _power = NULL;
    }


    if (_deg == NULL && _power == NULL) {
      Kp = 2.6;
      Ki = 0.007;
      Kd = 1;

      minimum = 70;
    } else {


      Kp = 1.4;
      Ki = 0.012;
      Kd = 0.65;
    }

    int angularVelocity = dmpgyro.z;
    if (abs(angularVelocity) <= 2) {
      angularVelocity = 0;
    }
    if (angularVelocity >= 0 && direction <= 0) {
      angularVelocity = 0;
    } else if (angularVelocity <= 0 && direction >= 0) {
      angularVelocity = 0;
    }
    if (abs(direction) <= 2) {
      angularVelocity = 0;
    }

    if (abs(direction) <= 50)
      integral += direction;

    direction *= Kp * -1;
    direction -= integral * Ki;
    direction -= angularVelocity * Kd;
    if (direction >= 0) {
      direction = constrain(direction, 10 + minimum, 355);
    } else {
      direction = constrain(direction, -355, -10 - minimum);
    }


    if (gyro.deg <= 5 || gyro.deg >= 355) {
      integral = 0;
      direction = 0;
    }

    if (_deg == NULL && _power == NULL) {
      for (int i = 0; i < 4; i++) {
        val[i] = direction;
      }

      LED.changeAll(LED.BLUE);
    } else {
      for (int i = 0; i < 4; i++) {
        val[i] = calcVal[i][_deg];
      }

      for (int i = 0; i < 4; i++) {
        if (direction >= 0) {
          val[i] *= float(255 - direction) / 255.0;
          val[i] *= float(float(_power) / 100.0);
        } else {
          val[i] *= abs(float(-255 - direction)) / 255.0;
          val[i] *= float(float(_power) / 100.0);
        }
        val[i] += direction;
      }
    }
  }

  for (int i = 0; i <= 3; i++) {
    val[i] = constrain(val[i], -255, 255);
  }

  directDrive(val);
}

void _motor::directDrive(int* p) {
  for (int i = 0; i <= 3; i++) {
    if (*(p + i) == 0) {
      digitalWrite(4 + i * 2, HIGH);
      digitalWrite(5 + i * 2, HIGH);
    } else if (*(p + i) > 0) {
      analogWrite(4 + i * 2, *(p + i));
      digitalWrite(5 + i * 2, LOW);
    } else {
      digitalWrite(4 + i * 2, LOW);
      analogWrite(5 + i * 2, abs(*(p + i)));
    }
  }
}
# 1 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/position.ino"
void _position::reflection(void) {
  if (device.mode == 1) {
    if (rock) {
      if (side[0] == 1) {
        if (ball.top >= 13) {
          if (motor.referenceAngle == 0 || motor.referenceAngle <= 330) {
            motor.referenceAngle = -(360 - ball.top * 22.5);
          }
        }
      } else if (side[0] == -1) {
        if (ball.top <= 3) {
          if (motor.referenceAngle == 0 || motor.referenceAngle >= 30) {
            motor.referenceAngle = ball.top * 22.5;
          }
        }
      } else {
        motor.referenceAngle = 0;
      }
    } else {
      motor.referenceAngle = 0;
      if (ball.top <= 3 || ball.top >= 13) {
        if (device.getTime() - ball.positionTimer <= 700) {
          if (device.getTime() - ball.driftTimer <= 2000) {
            ball.positionTimer = device.getTime();
            if (ball.position > 10) {
              if (digitalRead(BALL_HOLD)) {
                ball.deg = 340;
              }
              motor.referenceAngle = -10;
            } else if (ball.position < -10) {
              if (digitalRead(BALL_HOLD)) {
                ball.deg = 20;
              }
              motor.referenceAngle = 10;
            }
          }
        }
      } else {
        ball.driftTimer = device.getTime();
        motor.referenceAngle = 0;
      }
    }
  } else if (device.mode == 2) {



  }
}

void _position::get(void) {
  if (ball.deg >= 0 && ball.deg <= 165) {
    if (line.deg >= 210 && line.deg <= 330) {
      side[0] = 1;
      offTimer = device.getTime();
      rock = true;
    }
  }
  if (ball.deg >= 250 && ball.deg <= 360) {
    if (line.deg >= 30 && line.deg <= 150) {
      side[0] = -1;
      offTimer = device.getTime();
      rock = true;
    }
  }
  if (device.getTime() - offTimer >= 700) {
    rock = false;
    side[0] = 0;
    offTimer = 0;
  }
}
# 1 "/Users/Shirokuma89/Documents/Root41/Root41-Software/ver2020/src/tof.ino"
int _tof::read(void) {
  int data = TOF.readRangeContinuousMillimeters();
  _dist = data > 1 ? data : _dist;
  return _dist;
}
//ライブラリのインクルード
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <MPU6050_6Axis_MotionApps20.h>
// #include <Timer5.h>
#include <VL53L0X.h>
#include <Wire.h>

//ピン番号定義
int BALL[16] = {A0, A1, A2,  A3,  A4,  A5,  A6,  A7,
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

//インスタンス作成
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

  //配列系

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

  bool val[20];

 private:
  // none

  int deglog;

  int now;
  int first;
  int whited;

  bool stoping;
  bool s;

  int error;

  float x;
  float y;
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
  // none
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
  int reliability;  //信頼度
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
  // none
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

  int bright = 100;

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
  // none
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
  // Serial.println("Root41 2020");
  // Serial.print("Robot Number:");
  // Serial.println(device.robot + 1);
  // Serial.print("GyroOffsetX:");
  // Serial.println(gyro.eeprom[0]);
  // Serial.print("GyroOffsetY:");
  // Serial.println(gyro.eeprom[1]);
  // Serial.print("GyroOffsetZ:");
  // Serial.println(gyro.eeprom[2]);
  // Serial.print("AccelOffsetX:");
  // Serial.println(gyro.eeprom[3]);
  // Serial.print("AccelOffsetY:");
  // Serial.println(gyro.eeprom[4]);
  // Serial.print("AccelOffsetZ:");
  // Serial.println(gyro.eeprom[5]);

  gyro.setting();

  //起動イルミネーション
  LED.animation1();
  LED.animation2();

  //放電モード
  device.discharge();

  gyro.offsetRead();

  device.mode = 0;
}

void loop(void) {
  unsigned long errorTimer = device.getTime();
  device.check();

  if (device.mode == 0) {  //待機中
    gyro.deg = gyro.read();
    LED.gyroShow();
    motor.drive(NULL, NULL, true);
    analogWrite(LINE_BRIGHT, 10);

    //ボタンによるUI処理
    device.UI();

    //ジャイロ補正

    if (device.getTime() - device.startTimer <= 1000) {
      gyro.offsetRead();
    }
  } else if (device.mode == 1) {  //駆動中

    for (int i = 0; i < 3; i++) {
      motor.val[i] = 255;
    }
    motor.directDrive(motor.val);

    // //処理
    if (!line.flag) {
      //   LED.gyroShow();
      ball.read(ball.val);
      ball.calc();
      // LED.degShow(ball.deg);

      if (device.getTime() - ball.speedTimer <= 800 && ball.speedTimer != 0) {
        ball.speed =
            100 - (map(device.getTime() - ball.speedTimer, 0, 800, 10, 30));
      }
    }

    line.read();
    line.deg = line.calc();
    line.process();

    // position.get();
    // position.reflection();

    // //設定
    motor.deg = ball.deg;
    motor.speed = ball.speed;
    bool stop = false;

    if (line.flag) {
      motor.deg = line.deg;
      motor.speed = line.speed;
      //   LED.changeAll(LED.WHITE);

      if (motor.deg == 1000) {
        stop = true;
      }
    }

    // //持ち上げ消灯
    // // if (gyro.isLift && !device.robot) {
    // //   digitalWrite(LINE_BRIGHT, LOW);
    // // } else {
    // digitalWrite(LINE_BRIGHT, HIGH);
    // // }

    // //駆動
    // kicker.kick(kicker.val);

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

  } else if (device.mode == 2) {  //駆動中

    for (int i = 0; i < 3; i++) {
      motor.val[i] = 100;
    }
    motor.directDrive(motor.val);

    // //処理
    // LED.gyroShow();

    ball.read(ball.val);
    ball.readDistance();
    ball.calc();
    line.read();
    // line.linetrace();
    position.reflection();

    // if (ball.top < 3 || ball.top > 13) {
    //   keeper.speed = 85;
    // } else if (ball.top <= 5 || ball.top >= 11) {
    //   keeper.speed = 95;
    // } else {
    //   keeper.speed = 95;
    // }
    // keeper.deg = line.deg;
    gyro.deg = gyro.read();
    // //駆動

    // kicker.kick(kicker.val);

    // if (keeper.deg != 1000) {
    //   if (gyro.deg <= 30 || gyro.deg >= 330) {
    //     //動きのスムーズ
    //     // motor.drive(line.deg, 70, false);
    //     for (motor.count = 0; motor.count < motor.time; motor.count++) {
    //       // line.read();
    //       gyro.deg = gyro.read();
    //       motor.drive(keeper.deg, keeper.speed, false);
    //       if (motor.count >= 3) {
    //         digitalWrite(BALL_RESET, HIGH);
    //       }
    //     }
    //   } else {
    //     motor.drive(0, 0, true);
    //   }
    // } else {
    //   motor.drive(0, 0, true);
    // }
    // motor.drive(NULL, NULL);
  }

  while (Wire.available()) {
    Wire.read();
  }

  // Serial.println(gyro.deg);
  // for (int i = 0; i < 20; i++) {
  //   Serial.print(line.val[i]==true?"●":"○");
  // }
  // Serial.print(gyro.deg);

  // Serial.println("");
}

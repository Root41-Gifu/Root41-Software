

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <SPI.h>
#include <Wire.h>

#define voltage PC0

#define BALL_NUM 16
#define LINE_NUM 41
#define LINE_FRONTNUM 10
#define LINE_REARNUM 9
#define LINE_LEFTNUM 14
#define LINE_RIGHTNUM 8
#define LED_STRIP 16
#define LED_FRONT 13
#define LED_REAR 14
#define LED_LEFT 12
#define LED_RIGHT 12

#define LED_PIN_T PB6
#define LED_PIN_F PB7
#define LED_PIN_L PA15
#define LED_PIN_B PC1
#define LED_PIN_R PB1

#define LCD_INTERVAL 300

#define UI_ADDRESS 0x04
#define LINE_FRONTADDRESS 0x08
#define LINE_REARADDRESS 0x20
#define LINE_LEFTADDRESS 0x10
#define LINE_RIGHTADDRESS 0x40

#define LINE_BRIGHTNESS 180
#define NEOPIXEL_BRIGHTNESS 10

Adafruit_SSD1306 display(-1);
Adafruit_NeoPixel strip(LED_STRIP, LED_PIN_T, NEO_GRB + NEO_KHZ400);
Adafruit_NeoPixel front(LED_FRONT, LED_PIN_F, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rear(LED_REAR, LED_PIN_B, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel left(LED_LEFT, LED_PIN_L, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel right(LED_RIGHT, LED_PIN_R, NEO_GRB + NEO_KHZ800);

//モーターのやつ
HardwareSerial Serial4(A1, A0);
HardwareSerial Serial1(PA10, PA9);

float Battery;
int MotorPower = 100;
int degree;
bool emergency;  //緊急用のフラグ（やばいとき上げて）

class _UI {
 public:
  _UI(void);
  void NeoPixelReset(int, int);
  void read(void);
  void refrection(void);
  void check(int);
  void LCDdisplay(void);
  void Errordisplay(int);
  void NeoPixeldisplay(int);
  void StripFulldisplay(unsigned long);

  int mode;  //メインモード
  int submode;  //サブモード、キャリブレーションとかの時に帰る
  int setting;

  bool active;  //動作中
  bool select;
  bool adjust;

  //スイッチ全般
  bool Consistency;
  bool switchingFlag[4];
  bool touchFlag[4];
  bool longpressFlag[4];
  bool touch[4];
  int counter[4];
  unsigned long longpressTimer[4];  //長押しタイマー
  unsigned long updateTimer;        // UI表示スパン

  int switchScope;  //テスト用の変数、ボタンで切り替えれます。

 private:
} UI;

class _Ball {
 public:
  _Ball(void);
  void read(void);
  void average(void);
  void calcDistance(void);
  void calcDirection(void);
  void calc(void);
  int adjustValue(int, int);
  void LPF(void);
  unsigned long value[16];  //読み込み値
  float LPF_value[16];      // LPF補正値
  float LastLPF[16];        //前回のLPF補正値
  int dist[16];             //距離
  int max[3];               //最大値（のポート番号）
  int max_average[3];       //最大値の平均
  int averageCounter[17];
  int degree;       //ボールの角度
  int Move_degree;  //進行角度

  float vectorX[16];  //ベクトル（ボール位置の定数）
  float vectorY[16];
  float vectortX;  //変数（算出用）
  float vectortY;

 private:
  int move[3][16];
  int readp;
  int averageTimer;

} ball;

class _Line {
 public:
  _Line(void);
  void read(void);
  void arrange(void);
  void calcDirection(void);
  // void brightnessAdjust(void);
  void calc(void);

  int Move_degree;

  int Line_Where[LINE_NUM];
  bool flag;       //ラインセンサーの動きをするか
  bool touch;      //ラインに触れているか
  bool value[47];  //反応値
  bool check[47];  //加算されたか
  bool checkBlock[8];
  int Block;
  int order[47];  //反応した順番
  int orderBlock[8];

  //カウンター
  int whited;   //反応した数
  int whiting;  //反応している数

  //----エンジェルラインセンサー

  //番号記録
  int just;  //今反応してるやつ

  //タイマー
  unsigned long detectTimer[47];

  //----十字ラインセンサー
  int Front;  //フロント縁部分の反応数 ~7
  int Rear;   //リア縁 ~5
  int Left;   //左 ~3
  int Right;  //右 ~3
  int FrontEdge;
  int RearEdge;
  int LeftEdge;
  int RightEdge;
  int RearInside;   //リア内部 ~3
  int LeftInside;   //左 ~3
  int RightInside;  //右 ~3

  //その他
  int mode;
  float t_vectorX;
  float t_vectorY;

 private:
  float _vectorX[47];
  float _vectorY[47];
} line;

class _Motor {
 public:
  _Motor(void);
  void drive(int, int, bool);
  void begin(void);
  void directDrive(int* p);
  void release(void);
  void normalBrake(void);

  void ultraBrake(void);
  void motorCalc(int, int, bool, int);

  int val[4];
  int Kval[4];
  float sinVal[4][360];
  int calcVal[4][360];
  int deg;
  int speed;
  int count;
  int time = 5;
  int referenceAngle = 0;
  int susumu;
  unsigned long timer;

  int direction = 0;
  unsigned long lowBatteryCount = 0;

  unsigned long ultraBrakeLimit = 0;
  unsigned long ultraBrakeTimer = 0;

  int ultraBrakeFlag = 0;

 private:
  float Kp;
  float Ki;
  float Kd;

  int integral = 0;
  int gyroOld;
} motor;

int gyrodeg;

class _Camera {
 public:
  // _Camera(void);

 private:
} camera;

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

SPISettings MAX6675Setting(4000000, MSBFIRST, SPI_MODE0);

void setup() {
  delay(1000);
  pinMode(PB10, OUTPUT);
  digitalWrite(PB10, HIGH);
  pinMode(PA8, INPUT);

  UI.NeoPixelReset(NEOPIXEL_BRIGHTNESS, LINE_BRIGHTNESS);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  SPI.beginTransaction(MAX6675Setting);
  Wire.begin();
  Serial.begin(9600);

  gyro.setting();
  motor.begin();
}

void loop() {
  Battery = analogRead(voltage) * 0.01612;

  // Ball---------------------------------------------
  ball.read();  // SPI読み込み
  for (int i = 0; i < 16; i++) {
    if (ball.value[i] == 16) {
      ball.value[i] = 0;
      ball.dist[i] = 0;
    }
  }
  for (int i = 0; i <= 3; i++) {
    ball.max[0] = 0;
  }
  // 6,7はanalogpinでした..．
  ball.value[6] = 0;
  ball.value[7] = 0;
  // for(int i=0; i<BALL_NUM; i++){

  //   ball.value[i]=ball.adjustValue(i,ball.value[i]);//全値に調整かける(int)で返すのでよろしく。
  // }
  ball.LPF();  // LPFかける。魔法のフィルタ
  // MAX値求める、max[0]は一番反応いいやつ。
  ball.max[0] = 100;
  ball.max[1] = 100;
  ball.max[2] = 100;
  for (int i = 0; i < 16; i++) {
    if (ball.LPF_value[ball.max[0]] < ball.LPF_value[i] || ball.max[0] == 100) {
      ball.max[2] = ball.max[1];
      ball.max[1] = ball.max[0];
      ball.max[0] = i;
    } else if (ball.LPF_value[ball.max[1]] < ball.LPF_value[i] ||
               ball.max[1] == 100) {
      ball.max[2] = ball.max[1];
      ball.max[1] = i;
    } else if (ball.LPF_value[ball.max[2]] < ball.LPF_value[i] ||
               ball.max[2] == 100) {
      ball.max[2] = i;
    }
  }
  for (int i = 0; i < 3; i++) {
    if (ball.LPF_value[ball.max[i]] < 20) {
      ball.max[i] = 100;
    }
  }
  ball.average();        //平均とる。この関数イランかも知らん
  ball.calcDirection();  //ボールの方向算出
  // ball.calc();//動作角度算出

  // line---------------------------------------------
  line.read();
  line.arrange();
  if (line.flag) {
    line.calc();
  } else {
    line.Move_degree = 1000;
  }

  // UI---------------------------------------------
  UI.read();
  UI.touch[0] = !digitalRead(PA8);  //センサー検知

  for (int i = 0; i <= 3; i++) {
    UI.check(i);  //検知の確認
  }
  UI.refrection();  //スイッチの反映
  if (!emergency) {
    if (UI.active) {
      if (millis() - UI.updateTimer > 500) {
        UI.LCDdisplay();  // LCD表示（重いので500msで回す）
        UI.updateTimer = millis();
      }
    } else {
      UI.LCDdisplay();
    }
    UI.NeoPixeldisplay(UI.mode);  // NeoPixel表示
  } else {
    UI.Errordisplay(emergency);  // Error表示用、点滅するンゴ。
  }

  // gyro
  //ジャイロの読みこみ等

  // Motor---------------------------------------------
  if (!emergency) {
    //進行角度の選定
    if (line.flag) {
      degree = line.Move_degree;
    } else {
      degree = ball.Move_degree;
    }
    if (UI.mode == 0) {
      //セットアップ
      //モーターのセットアップがあったらここで（終わったらmode=1にして）
      // UI.mode=1;
    } else if (UI.mode == 1 || UI.mode == 2) {
      //モードオフェンス、ディフェンスの時
      unsigned long processTimer = millis();
      if (UI.active) {
        //モーター駆動（角度はdegree,パワーはMotorPower）
        for (int j = 0; j < 100; j++) {
          /* code */
          float neko;

          // motor.susumu = (millis() / 5) % 360;
          // Serial.println(motor.susumu);

          // // motor.val[0] = sin(radians(motor.susumu - 60)) * 40;
          // // motor.val[1] = sin(radians(motor.susumu - 135)) * 40;
          // // motor.val[2] = sin(radians(motor.susumu - 225)) * 40;
          // // motor.val[3] = sin(radians(motor.susumu - 300)) * 40;
          // motor.motorCalc(0, 30, false, 0);
          // for (int i = 0; i < 4; i++) {
          //   motor.val[i] = motor.Kval[i];
          // }

          // int a = 20;
          // int b = 2;
          // float c = -0.05;

          // motor.val[0] = -30;
          // motor.val[1] = -30;
          // motor.val[2] = 30;
          // motor.val[3] = 30;

          if (gyro.deg > 180) {
            neko = gyro.deg - 360;
          } else {
            neko = gyro.deg;
          }

          neko *= -0.13;//P制御
          neko += gyro.differentialRead() * -0.025;//微分制御
          // neko *= 1.2;

          neko = constrain(neko, -20, 20);
          for (int i = 0; i < 4; i++) {
            motor.val[i] = round(neko);
            motor.val[i] = constrain(motor.val[i], -40, 40);
          }
          
          motor.directDrive(motor.val);

          // if (gyro.deg <= 5 || gyro.deg >= 355) {
          //   // motor.motorCalc(0, 30, 0, 0);
          //   // for (int i = 0; i < 4; i++) {
          //   //   motor.val[i] = motor.Kval[i];
          //   // }
          //   motor.val[0] = 20;
          //   motor.val[1] = 20;

          //   motor.val[2] = -20;
          //   motor.val[3] = -20;
          //   motor.directDrive(motor.val);
          // } else if (gyro.deg < 180) {
          //   for (int i = 0; i < 4; i++) {
          //     motor.val[i] = -1 * map(gyro.deg, 0, 180, b, a);
          //     motor.val[i] += gyro.differentialRead() * c;

          //     motor.val[i] = constrain(motor.val[i], -20, -2);
          //   }

          //   // motor.motorCalc(0, 30, 0, 0);
          //   // for (int i = 0; i < 4; i++) {
          //   //   motor.val[i] = motor.val[i] + motor.Kval[i];
          //   // }
          //   // float _Max;
          //   // for (int i = 0; i < 4; i++) {
          //   //   if (abs(motor.val[i]) > _Max) {
          //   //     _Max = motor.val[i];
          //   //   }
          //   // }
          //   // motor.val[0] = motor.val[0] * 100 / _Max;
          //   // motor.val[1] = motor.val[1] * 100 / _Max;
          //   // motor.val[2] = motor.val[2] * 100 / _Max;
          //   // motor.val[3] = motor.val[3] * 100 / _Max;

          //   motor.directDrive(motor.val);
          // } else {
          //   for (int i = 0; i < 4; i++) {
          //     motor.val[i] = map(gyro.deg, 180, 360, a, b);
          //     motor.val[i] += gyro.differentialRead() * c;

          //     motor.val[i] = constrain(motor.val[i], 2, 20);
          //   }

          //   // motor.motorCalc(0, 30, 0, 0);
          //   // for (int i = 0; i < 4; i++) {
          //   //   motor.val[i] = motor.val[i] + motor.Kval[i];
          //   // }
          //   // float _Max;
          //   // for (int i = 0; i < 4; i++) {
          //   //   if (abs(motor.val[i]) > _Max) {
          //   //     _Max = motor.val[i];
          //   //   }
          //   // }
          //   // motor.val[0] = motor.val[0] * 100 / _Max;
          //   // motor.val[1] = motor.val[1] * 100 / _Max;
          //   // motor.val[2] = motor.val[2] * 100 / _Max;
          //   // motor.val[3] = motor.val[3] * 100 / _Max;

          //   motor.directDrive(motor.val);
          // }

          // Serial.println(gyro.differentialRead());
          Serial.print("neko:");
          Serial.println(gyro.differentialRead());
        }
        // Serial.println(millis() - processTimer);

      } else {
        //モーターが停止する
        motor.release();
      }
    }
  } else {
    //緊急事態時の行動
    motor.release();
  }

  if (Battery < 10.9 || Battery > 12.6) {
  } else {
    motor.lowBatteryCount = millis();
  }

  if (millis() - motor.lowBatteryCount >= 1000) {
    emergency = true;
  }

  // Serial.println(gyro.deg);
}

// int motorPS(int value) {
//   // XY------
//   byte sendValue;
//   bool rotation;
//   if (value > 0) {
//     rotation = true;
//   } else if (value < 0) {
//     rotation = false;
//   }
//   byte absValue = abs(value);
//   if (value <= 61) {
//     sendValue = (rotation << 6) | (absValue);
//   }
//   return sendValue;
// }
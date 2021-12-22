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
#define LINE_FRONTEDGENUM 0
#define LINE_REAREDGENUM 0
#define LINE_LEFTEDGENUM 0
#define LINE_RIGHTEDGENUM 0
#define LINE_FRONTINSIDENUM 0
#define LINE_REARINSIDENUM 0
#define LINE_LEFTINSIDENUM 0
#define LINE_RIGHTINSIDENUM 0
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

#define LINE_BRIGHTNESS 100  // 50
#define NEOPIXEL_BRIGHTNESS 30
#define LIGHTLIMIT 0
#define LINEOVERTIME 200

Adafruit_SSD1306 display(-1);
Adafruit_NeoPixel strip(LED_STRIP, LED_PIN_T, NEO_GRB + NEO_KHZ400);
Adafruit_NeoPixel front(LED_FRONT, LED_PIN_F, NEO_GRB + NEO_KHZ400);
Adafruit_NeoPixel rear(LED_REAR, LED_PIN_B, NEO_GRB + NEO_KHZ400);
Adafruit_NeoPixel left(LED_LEFT, LED_PIN_L, NEO_GRB + NEO_KHZ400);
Adafruit_NeoPixel right(LED_RIGHT, LED_PIN_R, NEO_GRB + NEO_KHZ400);

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
  void SerialPrint(bool);

  int mode;  //メインモード
  int submode;  //サブモード、キャリブレーションとかの時に帰る
  int setting;

  bool active;  //動作中
  bool standby;
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
  unsigned long standbyTimer;

  int switchScope;  //テスト用の変数、ボタンで切り替えれます。

 private:
} UI;

class _Ball {
 public:
  _Ball(void);
  void SPI_read(void);
  void average(void);
  void calcDistance(void);
  void calcDirection(void);
  void calc(void);
  int adjustValue(int, int);
  void Max_calc(float*);
  void LPF(void);
  unsigned long value[16];  //読み込み値
  float LPF_value[16];      // LPF補正値
  float LastLPF[16];        //前回のLPF補正値
  int dist[16];             //距離
  int distance;
  int distanceLevel;
  int max[3];          //最大値（のポート番号）
  int max_average[3];  //最大値の平均
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
  void calc(void);
  void vectorCalc(void);

  int Move_degree;

  int Line_Where[LINE_NUM];

  bool flag;  //ラインセンサーの動きをするか
  bool Rflag;
  bool touch;      //ラインに触れているか
  bool value[47];  //反応値
  bool check[47];  //加算されたか
  bool checkBlock[8];
  int Block;
  int Edge;
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
  unsigned long OutTimer;

  //----十字ラインセンサー
  int detect_num[8];
  int passed_num[8];

  //その他
  int mode;

  int leftdegree;
  int rdegree;
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
  int time = 5;
  int referenceAngle = 0;
  int susumu;
  int count;
  unsigned long timer;

  int direction = 0;
  unsigned long lowBatteryCount = 0;

  unsigned long ultraBrakeLimit = 0;
  unsigned long ultraBrakeTimer = 0;

  int ultraBrakeFlag = 0;

  long gapIntegral = 0;
  unsigned long integralTimer = 0;

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

int _Mdegree;

float block_vectorX[8];
float block_vectorY[8];

void setup() {
  delay(1000);
  pinMode(PB10, OUTPUT);
  digitalWrite(PB10, HIGH);
  pinMode(PA8, INPUT);

  UI.NeoPixelReset(NEOPIXEL_BRIGHTNESS, LINE_BRIGHTNESS);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  SPI.beginTransaction(MAX6675Setting);
  Wire.begin();
  Serial.begin(115200);

  line.vectorCalc();

  gyro.setting();
  motor.begin();
}

void loop() {
  unsigned long STimer = millis();

  // Battery-check---------------------------------------------
  Battery = analogRead(voltage) * 0.01612;

  // Ball---------------------------------------------
  ball.SPI_read();  // SPI読み込み
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
  ball.LPF();                     // LPFかける。魔法のフィルタ
  ball.Max_calc(ball.LPF_value);  //おかしかったらここコメントアウト
  ball.average();  //平均とる。この関数イランかも知らん
  ball.calcDistance();
  ball.calcDirection();  //ボールの方向算出
  ball.calc();           //動作角度算出

  // line---------------------------------------------
  line.read();
  line.arrange();
  if (line.flag) {
    line.calc();
  } else {
    line.Move_degree = 1000;
  }

  // UI---------------------------------------------
  static int uiCounter = 0;

  // if (uiCounter >= 10) {
  UI.read();
  // } else {
  //   uiCounter++;
  // }

  UI.touch[0] = !digitalRead(PA8);  //センサー検知

  for (int i = 0; i <= 3; i++) {
    UI.check(i);  //検知の確認
  }
  UI.refrection();  //スイッチの反映
  if (!emergency) {
    if (UI.active || UI.standby) {
      if (millis() - UI.updateTimer > 1000) {
        UI.LCDdisplay();  // LCD表示（重いので500msで回す）
        UI.updateTimer = millis();
      }
      // if (UI.mode != 1||UI.standby) {             //消灯します
      UI.NeoPixeldisplay(UI.mode);  // NeoPixel表示
      // }
    } else {
      UI.LCDdisplay();  // LCD表示（重いので500msで回す）
      UI.NeoPixeldisplay(UI.mode);
    }
  } else {
    UI.Errordisplay(emergency);  // Error表示用、点滅するンゴ。
  }

  // gyro
  //ジャイロの読みこみ等

  // Motor---------------------------------------------
  // _Mdegree = 1000;
  // if (line.flag) {
  //   _Mdegree = line.Move_degree;
  // } else {
  //   // if (millis() - line.OutTimer <= LINEOVERTIME) {
  //   //   _Mdegree = line.rdegree;
  //   // } else {
  _Mdegree = int(ball.Move_degree);
  //   // }
  // }
  if (!emergency) {
    //進行角度の選定
    if (UI.mode == 0) {
      //セットアップ
      //モーターのセットアップがあったらここで（終わったらmode=1にして）
      // UI.mode=1;
    } else if (UI.mode == 1 || UI.mode == 2) {
      //モードオフェンス、ディフェンスの時
      unsigned long processTimer = millis();
      if (UI.active == true) {
        // if (_Mdegree != 1000) {
        //モーター駆動（角度はdegree,パワーはMotorPower）
        for (int j = 0; j < 1; j++) {
          float Collection;

          if (gyro.deg > 180) {
            Collection = gyro.deg - 360;
          } else {
            Collection = gyro.deg;
          }

          if (motor.integralTimer - millis() > 25) {
            motor.gapIntegral += Collection;
            motor.gapIntegral = constrain(motor.gapIntegral, -1000, 1000);

            motor.integralTimer = millis();
          }

          Collection *= -0.058;  // P制御 0.078 Mizunami 0.072(0.9) or 81(09)
                                 // 0.062(0.7)<比率によって違うから3

          // Collection -= motor.gapIntegral / 400;  // I　上げると弱くなる
          Collection += gyro.differentialRead() * -0.0055;  // D

          // neko *= -0.078;                            // P制御 0.078 Mizunami
          // 0.072(0.9) or 81(09) 0.062(0.7)<比率によって違うから neko +=
          // gyro.differentialRead() * -0.01;

          // Serial.println(motor.gapIntegral);

          for (int i = 0; i < 4; i++) {
            motor.val[i] = round(Collection);
            motor.val[i] = constrain(motor.val[i], -30, 30);
          }

          int powerD;
          powerD = 35;
          if (_Mdegree != 1000) {
            if (gyro.deg <= 180 || gyro.deg >= 180) {
              //   neko = constrain(neko, -100, 100);
              motor.motorCalc(int(_Mdegree), 8, 0, 0);  // 8
              // if (abs(_Gap) < 5) {
              //   for (int i = 0; i < 4; i++) {
              //     motor.val[i] = motor.Kval[i];
              //   }
              // } else {
              int nekoK[4];
              for (int i = 0; i < 4; i++) {
                nekoK[i] = motor.val[i];
                motor.val[i] = (motor.val[i] + motor.Kval[i]) ;  // motorとジャイロの比率//0.9でも
              }
              // }
              int _Max;
              for (int i = 0; i < 4; i++) {
                if (abs(_Max) < abs(motor.val[i])) {
                  _Max = abs(motor.val[i]);
                }
              }
              for (int i = 0; i < 4; i++) {
                motor.val[i] = motor.val[i] * powerD / abs(_Max);
              }
            } else {
              for (int i = 0; i < 4; i++) {
                motor.val[i] = motor.val[i] * 1.2;
              }
            }
            motor.directDrive(motor.val);
          } else {
            motor.directDrive(motor.val);
          }
        }
        // } else {
        // motor.release();
        // }
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

  // if (Battery < 10.5 || Battery > 12.6) {
  //   // emergency=true;
  // } else {
  //   motor.lowBatteryCount = millis();
  // }

  // if (millis() - motor.lowBatteryCount >= 1000) {
  //   emergency = true;
  // }

  UI.SerialPrint(true);
}
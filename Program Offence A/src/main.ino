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

#define LINE_BRIGHTNESS 130  // 50
#define NEOPIXEL_BRIGHTNESS 20
#define LIGHTLIMIT 0
#define LINEOVERTIME 120

Adafruit_SSD1306 display(-1);
Adafruit_NeoPixel strip(LED_STRIP, LED_PIN_T, NEO_GRB + NEO_KHZ400);
Adafruit_NeoPixel front(LED_FRONT, LED_PIN_F, NEO_GRB + NEO_KHZ400);
Adafruit_NeoPixel rear(LED_REAR, LED_PIN_B, NEO_GRB + NEO_KHZ400);
Adafruit_NeoPixel left(LED_LEFT, LED_PIN_L, NEO_GRB + NEO_KHZ400);
Adafruit_NeoPixel right(LED_RIGHT, LED_PIN_R, NEO_GRB + NEO_KHZ400);

//モーターのやつ
HardwareSerial Serial4(A1, A0);
HardwareSerial Serial1(PA10, PA9);

float sin_d[360];
float cos_d[360];
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
  void SPI_read(void);       // SPI読み込み
  void average(void);        //平均換算今使ってない
  void calcDistance(void);   //距離計算
  void calcDirection(void);  //ベクトル位置計算
  void calc(void);           //進行方向算出
  int adjustValue(int, int);
  void Max_calc(float*);
  void LPF(void);           //ローパスフィルタ
  unsigned long value[16];  //読み込み値
  float LPF_value[16];      // LPF補正値
  float LastLPF[16];        //前回のLPF補正値
  int dist[16];             //距離
  int distance;             //距離
  int distanceLevel;        //距離（０～３）
  int max[3];               //最大値（のポート番号）
  int max_average[3];       //最大値の平均
  int averageCounter[17];
  int degree;       //ボールの角度
  int Move_degree;  //進行角度

  float vectorX[16];  //ベクトル（ボール位置の定数）
  float vectorY[16];
  float vectortX;  //変数（算出用）
  float vectortY;

  float vectorMove[2];

 private:
  int move[3][16];
  int readp;
  int averageTimer;

} ball;

class _Line {
 public:
  _Line(void);
  void read(void);           //読み込みI2C
  void arrange(void);        //読み込み値を処理数値に変換
  void calcDirection(void);  //方向（現在はベクトル）を算出
  void calc(void);           //ベクトル数値から進行方向を算出
  void vectorCalc(void);     //センサーごとのベクトル数値計算

  int Move_degree;  //進行方向

  int Line_Where[LINE_NUM];

  int mode;
  /* modeのリスト
  0:ライン踏んでない（!flag）
  1:ライン踏んでまだwhitedが3以下
  2:ライン踏んで4以上（ずれ少ない）
  3:ライン踏んで4以上（ずれあり）
  */

  bool flag;           //ラインセンサーの動きをするか
  bool Rflag;          //飛び出しリターン時のフラグ
  bool touch;          //ラインに触れているか
  bool value[47];      //反応値
  bool check[47];      //計測されたか
  bool checkBlock[8];  //８分割ブロックの計測フラグ
  int Block;           //８分割ブロック
  int Edge;
  int order[47];      //反応した順番
  int orderBlock[8];  //８分割ブロック

  //カウンター
  int whited;   //反応した数
  int whiting;  //反応している数

  //----エンジェルラインセンサー
  int reference_degree;
  int current_degree;

  //番号記録
  int just;  //今反応してるやつ

  //タイマー
  unsigned long detectTimer[47];  //反応時間計測
  unsigned long OutTimer;         //ラインアウト時間計測

  //----十字ラインセンサー
  int detect_num[8];  //８分割ブロックごとの計測数（リアルタイム）
  int passed_num[8];  //８分割ブロックごとの計測数（通過後を含む）
  //その他
  // int mode;  //モード

  int leftdegree;   //ラインアウト時のライン進行方向
  int rdegree;      //ラインアウト時のリターン進行方向
  float t_vectorX;  //ベクトル換算時のベクトルＸ
  float t_vectorY;  //ベクトル換算時のベクトルＹ

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
  void motorPID_drive(float, float, float, int, int);

  int reference_degree;

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

  Serial.begin(115200);

  //ピン設定
  pinMode(PB10, OUTPUT);
  digitalWrite(PB10, HIGH);
  pinMode(PA8, INPUT);

  //ライブラリ設定
  UI.NeoPixelReset(NEOPIXEL_BRIGHTNESS, LINE_BRIGHTNESS);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  SPI.beginTransaction(MAX6675Setting);
  Wire.begin();

  //クラスごとのセットアップ
  line.vectorCalc();  //ラインごとのベクトル計算
  gyro.setting();
  motor.begin();
  UI.mode=1;

  //三角関数
  for(int i=0; i<360; i++){
    sin_d[i]=sin(radians(i));
    cos_d[i]=cos(radians(i));
  }
}

void loop() {
  // Battery-check---------------------------------------------
  Battery = analogRead(voltage) * 0.01612;

  // Ball---------------------------------------------
  ball.SPI_read();  // SPI読み込み
  // SPI不具合のノイズ除去
  for (int i = 0; i < 16; i++) {
    if (ball.value[i] == 16) {
      ball.value[i] = 0;
      ball.dist[i] = 0;
    }
  }
  //データ初期化
  for (int i = 0; i <= 3; i++) {
    ball.max[0] = 0;
  }
  ball.value[6] = 0;
  ball.value[7] = 0;
  ball.LPF();  // LPFかける。魔法のフィルタ

  ball.Max_calc(ball.LPF_value);  //最大値求める

  ball.average();  //平均とる。現在使っていない

  ball.calcDistance();  //距離を求める

  ball.calcDirection();  //ボールの方向算出

  ball.calc();  //動作角度算出

  // line---------------------------------------------
  line.read();  // I2Cでライン読み込み
  line.arrange();  //ラインの順番、ブロック分け、タイムなどの算出
  if (line.flag) {
    line.calc();  //ライン戻る方向の処理(ラインのルーチン時のみ)
  } else {
    line.Move_degree = 1000;
  }

  // UI---------------------------------------------
  UI.read();  // UIの読み込み

  for (int i = 0; i <= 3; i++) {
    UI.check(i);  //検知の押す離すの確認
  }

  UI.refrection();  //スイッチのアルゴリズムへの反映

  //緊急事態時と平常時の処理
  if (!emergency) {
    //平常時
    if (UI.active || UI.standby) {
      //動作時
      if (millis() - UI.updateTimer > 1000) {
        UI.LCDdisplay();  // LCD表示（重いので500msで回す）
        UI.updateTimer = millis();
      }
      // if (UI.mode != 1||UI.standby) {             //消灯します
      UI.NeoPixeldisplay(UI.mode);  // NeoPixel表示
      // }
    } else {
      //停止時
      UI.LCDdisplay();  // LCD表示
      UI.NeoPixeldisplay(UI.mode);
    }
  } else {
    //緊急時
    UI.Errordisplay(emergency);  // Error表示用、点滅するンゴ。
  }

  // gyro

  //ジャイロの読みこみ等
  // gyro.deg=gyro.read();//<これはモーター処理で読んでるからコメントアウトのままで良し

  // Motor---------------------------------------------

  _Mdegree = 1000;
  // line.reference_degree=0//これは角度のずれを考慮したいときにコメントアウトにして

  if (line.Move_degree == 10000) {
    //ラインなし、ボール反応時
    _Mdegree = ball.Move_degree;
  } else if (line.flag) {
    //ラインあり、ライン検知時
    _Mdegree = line.Move_degree - line.reference_degree;
  } else if (line.Rflag && millis() - line.OutTimer < 200) {
    //ラインあり、ラインオーバー時
    _Mdegree = line.leftdegree - line.reference_degree;
  } else {
    //ラインあり、ラインから距離をとる
    if (millis() - line.OutTimer <= 40) {
      _Mdegree = line.rdegree - line.reference_degree;
    } else {
      // _Mdegree = int(ball.Move_degree);
      _Mdegree = ball.Move_degree;
    }
  }

  //角度オーバーの修正
  if (_Mdegree > 360) {
    _Mdegree = _Mdegree - 360;
  } else if (_Mdegree < 0) {
    _Mdegree = _Mdegree + 360;
  }

  if (!emergency) {
    //進行角度の選定
    if (UI.mode == 0) {
      //セットアップ
      //モーターのセットアップがあったらここで（終わったらmode=1にして）
      // UI.mode=1;
    } else if (UI.mode == 1 || UI.mode == 2) {
      //モードオフェンス、ディフェンスの時
      if (UI.active == true) {
        //動作中
        motor.motorPID_drive(
            0.043, 1, 0.014, 40,
            7);  //比例定数,積分定数,微分定数,モーターS,ジャイロS
      } else {
        //停止中
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

  UI.SerialPrint(false);  //引数で通信切り替え
}
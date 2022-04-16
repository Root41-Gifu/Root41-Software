#include <Wire.h>
int i2cReadWithTimeoutFunction(void);

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <SPI.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

#define ROBOT_NUMBER 1

#define voltage PC0

#define LINE_EFFECT 1

#define BALL_NUM 16
#define LINE_NUM 28
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

#define CAMERA_PIN 0x0A

#define UI_ADDRESS 0x04
#define LINE_FRONTADDRESS 0x08
#define LINE_REARADDRESS 0x40
#define LINE_LEFTADDRESS 0x10
#define LINE_RIGHTADDRESS 0x20
const int lineAddress[] = {0x08, 0x20, 0x40, 0x10};

#define LINE_BRIGHTNESS 22  // 50
#define NEOPIXEL_BRIGHTNESS 30
#define LIGHTLIMIT 0
#define LINEOVERNUM 18
#define LINEOVERTIME 30
#define LINERETURNTIME 10

Adafruit_SSD1306 display(-1);
Adafruit_NeoPixel strip(LED_STRIP, LED_PIN_T, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel front(LED_FRONT, LED_PIN_F, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rear(LED_REAR, LED_PIN_B, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel left(LED_LEFT, LED_PIN_L, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel right(LED_RIGHT, LED_PIN_R, NEO_GRB + NEO_KHZ800);
Adafruit_BNO055 bno = Adafruit_BNO055(55);

//モーターのやつ
HardwareSerial Serial4(A1, A0);
HardwareSerial Serial1(PA10, PA9);

float sin_d[360];
float cos_d[360];
float Battery;
int MotorPower = 100;
int degree;
bool emergency;  //緊急用のフラグ（やばいとき上げて）

unsigned long gyroReset_Timer;
bool reset_flag;

bool readCounter = 0;

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
  int frash_mode = 0;  //ネオピクセルのモード
  int setting;
  int errorCode = 0;

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
  bool bottomUIFlag = false;
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
  void calc(int);            //進行方向算出
  int adjustValue(int, int);
  void Max_calc(float*);
  void LPF(void);           //ローパスフィルタ
  unsigned long value[16];  //読み込み値
  float LPF_value[16];      // LPF補正値
  float LastLPF[16];        //前回のLPF補正値
  float dist[16];           //距離
  float LPF_dist[16];
  float Last_disLPF[16];
  int distance;       //距離
  int distanceLevel;  //距離（０～３）
  int LevelCounter[4];
  int max[3];          //最大値（のポート番号）
  int max_average[3];  //最大値の平均
  int averageCounter[17];
  int degree;       //ボールの角度
  int Move_degree;  //進行角度

  bool hold;
  int holdcounter;

  float vectorX[16];  //ベクトル（ボール位置の定数）
  float vectorY[16];
  float vectortX;  //変数（算出用）
  float vectortY;

  float vectorMove[2];

  unsigned long distTimer;

 private:
  int move[3][16];
  int readp;
  int averageTimer;

} ball;

class _Line {
 public:
  _Line(void);
  void read(void);     //読み込みI2C
  void arrange(void);  //読み込み値を処理数値に変換
  void keeper_arrange(void);
  int calcDirection(void);  //方向（現在はベクトル）を算出
  void calc(void);          //ベクトル数値から進行方向を算出
  void vectorCalc(void);    //センサーごとのベクトル数値計算

  int Move_degree;  //進行方向

  int Line_Where[LINE_NUM];

  int mode;
  /* modeのリスト
  0:ライン踏んでない（!flag）
  1:ライン踏んでまだwhitedが3以下
  2:ライン踏んで4以上（ずれ少ない）
  3:ライン踏んで4以上（ずれあり）
  */

  bool flag;       //ラインセンサーの動きをするか
  bool Rflag;      //飛び出しリターン時のフラグ
  bool Oflag;      //オーバーリターンのフラグ
  bool touch;      //ラインに触れているか
  bool value[28];  //反応値
  // bool value_stock[47];  //反応値
  bool edge_value[4];
  bool cross_value[4];
  bool angel_value[20];
  bool check[28];      //計測されたか
  bool checkBlock[4];  // 4分割ブロックの計測フラグ
  int Block;           // 4分割ブロック
  int Block_degree[4] = {180, 0, 90, 270};
  int one_degree[28];
  int Edge;
  int order[28];      //反応した順番
  int orderBlock[4];  // 4分割ブロック

  int MoveLock;

  int bitSelect;

  //カウンター
  int whited;   //反応した数
  int whiting;  //反応している数

  //----エンジェルラインセンサー
  int reference_degree;
  int current_degree;

  //番号記録
  int just;  //今反応してるやつ

  //タイマー
  unsigned long detectTimer[28];  //反応時間計測
  unsigned long OutTimer;         //ラインアウト時間計測
  unsigned long InTimer;
  unsigned long MovelockTimer;

  //----十字ラインセンサー
  int detect_num[4];  //８分割ブロックごとの計測数（リアルタイム）
  int passed_num[4];  //８分割ブロックごとの計測数（通過後を含む）
  //その他
  // int mode;  //モード

  int totaldegree;
  int leftdegree;   //ラインアウト時のライン進行方向
  int rdegree;      //ラインアウト時のリターン進行方向
  int odegree;      //ラインオーバーリターン時の進行方向
  float t_vectorX;  //ベクトル換算時のベクトルＸ
  float t_vectorY;  //ベクトル換算時のベクトルＹ

  //キーパー関係
  int Last_Block;
  int Last_second_Block;
  bool awayFlag;
  unsigned long awayTimer;

 private:
  float _vectorX[47];
  float _vectorY[47];
} line;

class _Keeper {
 public:
  _Keeper(void);
  void analyze(void);
  void calc(void);

  int mode;
  /*
  0:デフォルト
  1:正常に入ってる（前）
  2:正常に入ってる（横）
  3:飛び出している（前）
  4:飛び出している（後）
  5:飛び出している（横）*/
  int x_position;
  int y_position;
  int line_position;
  int Move_degree;
  int line_Lock;

  float speed = 1;

  bool frontoverFlag;

  unsigned long lockTimer;
  unsigned long frontoverTimer;

 private:
} keeper;

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
  void motorPID_drive(int);

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
  _Camera(void);
  void read();
  void calc();

  byte goal_x;

  int mode;
  int o_goal_X[2];
  bool right_found;
  bool left_found;

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
  int reference_deg;
  int gain_deg;
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
  // delay(1000);

  Serial.begin(115200);

  //ピン設定
  pinMode(PB10, OUTPUT);
  digitalWrite(PB10, HIGH);
  pinMode(PA8, INPUT);
  pinMode(CAMERA_PIN, OUTPUT);
  // LINESENSOR_INITIALIZE:
  Wire.begin();
  // Wire.setClock(400000);
  // for (int i = 0; i < 5; i++) {
  delay(1000);
  gyro.setting();
  // }

  // UI setting
  UI.NeoPixelReset(NEOPIXEL_BRIGHTNESS, LINE_BRIGHTNESS);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  SPI.beginTransaction(MAX6675Setting);

  for (int i = 0; i < 4; i++) {
    Wire.beginTransmission(lineAddress[i]);
    Wire.write(9);  // high
    Wire.write(5);  // low

    int result = Wire.endTransmission();
    Serial.println(result);
    if (result != 0) {
      // goto LINESENSOR_INITIALIZE;
    }
    delay(50);
  }
  // Wire.setClock(400000);
  // for (int i = 0; i < 5; i++) {
  delay(100);
  gyro.setting();
  // }

  UI.NeoPixelReset(NEOPIXEL_BRIGHTNESS, LINE_BRIGHTNESS);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  SPI.beginTransaction(MAX6675Setting);

  Serial.begin(115200);

  line.vectorCalc();
  //クラスごとのセットアップ
  line.vectorCalc();  //ラインごとのベクトル計算

  motor.begin();
  UI.mode = 1;

  //三角関数
  for (int i = 0; i < 360; i++) {
    sin_d[i] = sin(radians(i));
    cos_d[i] = cos(radians(i));
  }

  motor.lowBatteryCount = millis();
}

void loop() {
  long loopTimerA = micros();

  // if (!line.flag && !line.Rflag && !line.Oflag) {
  //   if (millis() - gyroReset_Timer > 1000) {
  //     if (UI.active) {
  //       gyroReset_Timer = millis();
  //       UI.active = false;
  //       reset_flag = true;
  //     }
  //   }
  // }
  // if (reset_flag) {
  //   UI.active = true;
  //   reset_flag = false;
  // }

  // Battery-check---------------------------------------------
  Battery = analogRead(voltage) * 0.01469231;

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

  ball.calc(ball.distance);  //動作角度算出

  // line---------------------------------------------
  if (LINE_EFFECT) {
    // Wire.setClock(400000);
    line.read();  // I2Cでライン読み込み
    if (UI.mode == 1 || UI.mode == 5) {
      line.arrange();  //ラインの順番、ブロック分け、タイムなどの算出
    } else if (UI.mode == 2) {
      line.keeper_arrange();
    }
    if (line.flag) {
      if (UI.mode == 1) {
        line.calc();  //ライン戻る方向の処理(ラインのルーチン時のみ)
      } else if (UI.mode == 2) {
      }
    } else {
      line.Move_degree = 1000;
    }
    if (line.flag) {
      if (line.Move_degree != 10000 && line.Move_degree != 1000) {
        if (line.Move_degree > 30 && line.Move_degree < 150) {
          line.MoveLock = 4;
        } else if (line.Move_degree > 210 && line.Move_degree < 330) {
          line.MoveLock = 2;
        }
      }
      line.MovelockTimer = millis();
    }
    if (millis() - line.MovelockTimer > 800) {
      line.MoveLock = 0;
    }
  }

  // camera
  //  if(camera.mode==0){
  //    camera.o_goal_X[0]=1000;
  //  }else if(camera.mode==1){
  //    camera.read();
  //  }

  // UI---------------------------------------------
  UI.read();  // UIの読み込み

  for (int i = 0; i <= 3; i++) {
    UI.check(i);  //検知の押す離すの確認
  }

  UI.refrection();  //スイッチのアルゴリズムへの反映

  if (line.flag || line.Rflag || line.Oflag || UI.mode == 2) {
    UI.frash_mode = 1;
  } else {
    UI.frash_mode = 0;
  }

  // //緊急事態時と平常時の処理
  if (!emergency) {
    //平常時
    if (UI.active || UI.standby) {
      //動作時
      // if (millis() - UI.updateTimer > 1000) {
      //   UI.LCDdisplay();  // LCD表示（重いので500msで回す）
      //   UI.updateTimer = millis();
      // }
      // if (UI.mode != 1 || UI.standby) {  //消灯します
      UI.NeoPixeldisplay(UI.mode);  // NeoPixel表示
      // }
    } else {
      //停止時
      UI.LCDdisplay();  // LCD表示
      UI.NeoPixeldisplay(UI.mode);
    }
  } else {
    //緊急時
    UI.Errordisplay(UI.errorCode);  // Error表示用、点滅するンゴ。
  }

  // keeper
  if (UI.mode == 2) {
    keeper.analyze();
    keeper.calc();
  }

  // gyro

  //ジャイロの読みこみ等
  // if(!UI.active&&!reset_flag){

  // }
  gyro.deg = 360;           // 360
  gyro.deg += gyro.read();  //
  gyro.deg -= gyro.reference_deg;
  gyro.deg -= gyro.gain_deg;
  gyro.deg %= 360;

  // Motor-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -s

  _Mdegree = 1000;
  motor.reference_degree = 0;
  motor.referenceAngle = 0;
  //
  line.reference_degree =
      0;  //これは角度のずれを考慮したいときにコメントアウトにして
  if (UI.mode == 1) {
    if (line.flag) {
      //ラインあり、ライン検知時
      // _Mdegree = line.Move_degree - line.reference_degree;
      _Mdegree = line.Move_degree;
    } else if (line.Rflag) {
      //ラインあり、ラインオーバー時
      // _Mdegree = line.leftdegree - line.reference_degree;]
      if (millis() - line.OutTimer < 60) {
        _Mdegree = line.rdegree;
      } else {
        _Mdegree = ball.Move_degree;
      }
    } else if (line.Oflag) {
      _Mdegree = line.odegree;
    } else {
      //ラインあり、ラインから距離をとる
      // _Mdegree = int(ball.Move_degree);
      _Mdegree = ball.Move_degree;
      if (line.MoveLock == 2) {
        // if(_Mdegree<45){
        //   _Mdegree=0;
        // }else if(_Mdegree<135){
        //   _Mdegree=1000;
        // }else
        if (_Mdegree < 180) {
          _Mdegree = 1000;
        }
      } else if (line.MoveLock == 4) {
        // if(_Mdegree>315){
        //   _Mdegree=0;
        // }else if(_Mdegree>225){
        //   _Mdegree=1000;
        // }else
        if (_Mdegree > 180) {
          _Mdegree = 1000;
        }
      }
    }
  } else if (UI.mode == 2) {
    _Mdegree = keeper.Move_degree;
  } else {
    _Mdegree = 1000;
  }
  // _Mdegree = ball.Move_degree;
  // _Mdegree = ball.Move_degree;

  //角度オーバーの修正
  if (_Mdegree > 360 && _Mdegree != 1000 && _Mdegree != 10000) {
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
        if (_Mdegree != 10000) {
          motor.motorPID_drive(60);

          // motor.release();
          UI.bottomUIFlag = true;
        } else {
          motor.normalBrake();
        }

        //比例定数,積分定数,微分定数,モーターS,ジャイロS
      } else {
        //停止中
        motor.release();
        UI.bottomUIFlag = false;
      }
    }
  } else {
    //緊急事態時の行動
    UI.bottomUIFlag = false;
    motor.release();
  }

  // Battery = 10.3;

  if (Battery < 10.4 || Battery > 12.7) {
    // emergency = true;
  } else {
    motor.lowBatteryCount = millis();
  }

  // camera.read();
  int udata;

  if (true) {
    // 0
    // for (int i = 0; i < 16; i++) {
    //   Serial.print(ball.LPF_value[i]);
    //   Serial.print(" ");
    // }
    Serial.print(ball.max[0]);
    Serial.print(" ");
    Serial.print(" ");

    Serial.println("");
  }
}

int i2cReadWithTimeoutFunction(void) {
  // int cnt_to = 0;

  // while (1) {
  //   if (digitalRead(SDA) == HIGH || digitalRead(SCL) == HIGH) {
  //     break;
  //   } else {
  //     delay(1);
  //     cnt_to++;

  //     if (cnt_to == 10) {
  //       break;
  //     }
  //   }
  // }

  int returnValue = 0;
  // if (cnt_to != 10) {
  returnValue = Wire.read();
  // }

  return returnValue;
}
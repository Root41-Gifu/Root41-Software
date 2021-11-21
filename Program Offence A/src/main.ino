#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>

#define PI 3.14159265359
#define BALL_NUM 16
#define LINE_NUM 40
#define LED_COUNT 16
#define LED_PIN_T PB6
#define LED_PIN_F PB7
#define LED_PIN_L PA15
#define LED_PIN_B PC1
#define LED_PIN_R PB1

Adafruit_SSD1306 display(-1);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN_T, NEO_GRB + NEO_KHZ800);

int IN[3] = {3, 5, 7};
int SD[3] = {9, 6, 8};

float power = 0.2;

volatile int offset = 0;
volatile int velocityUnit;
volatile int deg;
volatile int oldDeg;
volatile int velocity;

int pwmRaw[360] = {
    237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 246, 247, 248, 248, 249,
    250, 250, 251, 251, 252, 252, 252, 253, 253, 253, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 253, 253, 253, 252, 252, 252, 251, 251, 250,
    250, 249, 248, 248, 247, 246, 246, 245, 244, 243, 242, 241, 240, 239, 238,
    237, 234, 230, 227, 223, 220, 216, 213, 209, 206, 202, 199, 195, 191, 188,
    184, 180, 176, 173, 169, 165, 161, 158, 154, 150, 146, 142, 139, 135, 131,
    127, 123, 119, 115, 112, 108, 104, 100, 96,  93,  89,  85,  81,  78,  74,
    70,  66,  63,  59,  55,  52,  48,  45,  41,  38,  34,  31,  27,  24,  20,
    17,  16,  15,  14,  13,  12,  11,  10,  9,   8,   8,   7,   6,   6,   5,
    4,   4,   3,   3,   2,   2,   2,   1,   1,   1,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   1,   1,   1,   2,   2,   2,   3,   3,   4,
    4,   5,   6,   6,   7,   8,   8,   9,   10,  11,  12,  13,  14,  15,  16,
    17,  16,  15,  14,  13,  12,  11,  10,  9,   8,   8,   7,   6,   6,   5,
    4,   4,   3,   3,   2,   2,   2,   1,   1,   1,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   1,   1,   1,   2,   2,   2,   3,   3,   4,
    4,   5,   6,   6,   7,   8,   8,   9,   10,  11,  12,  13,  14,  15,  16,
    17,  20,  24,  27,  31,  34,  38,  41,  45,  48,  52,  55,  59,  63,  66,
    70,  74,  78,  81,  85,  89,  93,  96,  100, 104, 108, 112, 115, 119, 123,
    127, 131, 135, 139, 142, 146, 150, 154, 158, 161, 165, 169, 173, 176, 180,
    184, 188, 191, 195, 199, 202, 206, 209, 213, 216, 220, 223, 227, 230, 234,
    237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 246, 247, 248, 248, 249,
    250, 250, 251, 251, 252, 252, 252, 253, 253, 253, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 253, 253, 253, 252, 252, 252, 251, 251, 250,
    250, 249, 248, 248, 247, 246, 246, 245, 244, 243, 242, 241, 240, 239, 238,
};

byte pwm[1024];

volatile uint8_t interval = 0;
volatile int voltage;

volatile float gain = 0.114;

float Battery=11.1;
bool emergency;//緊急用のフラグ（やばいとき上げて）

class _UI{
  public:
    _UI(void);
    void refrection(void);
    void check(int);
    void LCDdisplay(void);
    void Errordisplay(int);
    void NeoPixeldisplay(int);

    int mode;//メインモード
    int submode;//サブモード、キャリブレーションとかの時に帰る
    bool active;//動作中
    bool select;
    //スイッチ全般
    bool switchingFlag[4];
    bool touchFlag[4];
    bool longpressFlag[4];
    bool touch[4];
    int counter[4];
    unsigned long longpressTimer[4];//長押しタイマー
    unsigned long updateTimer;//UI表示スパン

    int switchScope;//テスト用の変数、ボタンで切り替えれます。

  private:
    
}UI;

class _Ball{
  public:
    _Ball(void);
    void read(void);
    void average(void);
    void calcDistance(void);
    void calcDirection(void);
    void calc(void);
    int adjustValue(int,int);
    void LPF(void);
    unsigned long value[16];//読み込み値
    float LPF_value[16];//LPF補正値
    float LastLPF[16];//前回のLPF補正値
    int dist[16];//距離
    int max[3];//最大値（のポート番号）
    int max_average[3];//最大値の平均
    int averageCounter[17];
    int degree;//ボールの角度
    int move_degree;//進行角度
    
    float vectorX[16];//ベクトル（ボール位置の定数）
    float vectorY[16];
    float vectortX;//変数（算出用）
    float vectortY;

  private:

    int move[3][16];
    int readp;
    int averageTimer;
    
    
}ball;

class _Line{
  public:
    _Line(void);
    void read(void);
    // void brightnessAdjust(void);
    void calc(void);

    bool flag;
    bool value[40];
    

  private:
}line;

class _Camera{
  public:
    // _Camera(void);

  private:
}camera;

// void measureAngularVelocity(void) {
//   deg = analogRead(A0);

SPISettings MAX6675Setting (4000000, MSBFIRST, SPI_MODE0);

void setup() {

  pinMode(PB10,OUTPUT);
  digitalWrite(PB10,HIGH);
  pinMode(PA8,INPUT);

  strip.begin();
  strip.show();
  strip.setBrightness(30);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  SPI.beginTransaction(MAX6675Setting);
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  //ボール処理
  ball.read();//SPI読み込み
  for(int i=0; i<16; i++){
    if(ball.value[i]==16){
      ball.value[i]=0;
      ball.dist[i]=0;
    }
  }
  for(int i=0; i<=3; i++){
    ball.max[0]=0;
  }
  //6,7はanalogpinでした..．
  ball.value[6]=0;
  ball.value[7]=0;
  // for(int i=0; i<BALL_NUM; i++){
  //   ball.value[i]=ball.adjustValue(i,ball.value[i]);//全値に調整かける(int)で返すのでよろしく。    
  // }
  ball.LPF();//LPFかける。魔法のフィルタ
  //MAX値求める、max[0]は一番反応いいやつ。
  ball.max[0]=100;
  ball.max[1]=100;
  ball.max[2]=100;
  for(int i=0; i<16; i++){
    if(ball.LPF_value[ball.max[0]]<ball.LPF_value[i]||ball.max[0]==100){
      ball.max[2]=ball.max[1];
      ball.max[1]=ball.max[0];
      ball.max[0]=i;
    }else if(ball.LPF_value[ball.max[1]]<ball.LPF_value[i]||ball.max[1]==100){
      ball.max[2]=ball.max[1];
      ball.max[1]=i;
    }else if(ball.LPF_value[ball.max[2]]<ball.LPF_value[i]||ball.max[2]==100){
      ball.max[2]=i;
    }
  }
  for(int i=0; i<3; i++){
    if(ball.LPF_value[ball.max[i]]<20){
      ball.max[i]=100;
    }
  }
  ball.average();//平均とる。この関数イランかも知らん
  ball.calcDirection();//ボールの方向算出
  // ball.calc();//動作角度算出

  //line
  line.read();


  //UI
  UI.touch[0]=!digitalRead(PA8);//センサー検知
  for(int i=0; i<=3; i++){
    UI.check(i);//検知の確認
  }
  UI.refrection();//スイッチの反映
  if(!emergency){
    if(millis()-UI.updateTimer>500){
      UI.LCDdisplay();//LCD表示（重いので500msで回す）
      UI.updateTimer=millis();
    }
    UI.NeoPixeldisplay(UI.mode);//NeoPixel表示
  }else{
    UI.Errordisplay(emergency);//Error表示用、点滅するンゴ。
  }
  Serial.print(line.value[0]);
  Serial.print(line.value[1]);
  Serial.print(UI.switchScope);
  Serial.print(" ");
  Serial.print(ball.max_average[0]);
  Serial.print(" ");
  Serial.print(ball.value[UI.switchScope]);
  Serial.print(" ");
  Serial.print(ball.LPF_value[UI.switchScope]);
  Serial.print(" ");
  Serial.print(ball.LPF_value[ball.max_average[0]]);
  Serial.println(" ");
}
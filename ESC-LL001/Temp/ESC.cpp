//---------------よく使う数学的関数やマクロ---------------

//NOTE:Arduinoからの引用
#define constrain(amt, low, high) \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

extern long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//---------------動的パラメータ。キャリブレーション処理などで決定する。FLASH領域にあらかじめ書き込んでおくのが理想。---------------
int mechanicalAngleOffset = 0;

//---------------固定パラメータ。メンテナンスを容易にするために定数として定義しておく。---------------
const int outputVoltageLimit = 200;  //出力電圧のリミット。0-255の間で指定。
const int interval = 1000;  //割り込み時間間隔の設定。単位はμs
const int intervalCounterLimit = 10;  //間引き用実行処理間隔指定
const char
    sinWave[128];  // TODO:ここを頑張って求める。Excelとかでいいんじゃね？

//---------------モーターとは直接関わりのない変数。---------------
int intervalCounter = 0;  //間引いて実行したい時用のインターバルカウンタ。

//---------------変数とか---------------
int realAngularVelocity = 0;  //符号ありであることに気をつける
int targetAngularVelocity = 0;
int deviation;

int mechanicalAngle = 0;
int electricalAngle = 0;

//---------------関数---------------

void INTERVAL(void) {  // ここはinterval[μs]秒ごとに実行される
  mechanicalAngle = getMechanicalAngle();  //機械角の取得
  electricalAngle = convertToElectricalAngle(mechanicalAngle);  //電気角への変換

  intervalCounter++;  //間引き実行用カウンタ
  intervalCounter %= intervalCounterLimit;
  if (intervalCounter == 0) {
    //---------------この中に間引き実行したいプログラムを記述しろ---------------
    realAngularVelocity = getAngularVelocity();
    deviation = realAngularVelocity = targetAngularVelocity;
    //---------------ここまで---------------
  }

  //CHECK:次回ここから実装する
  
}

int getAngularVelocity(void) {  // 角速度を求める。Δtが一定であることが動作条件
  return 0;  // TODO:ちゃんと実装しろ
}

int getMechanicalAngle(void) {  //現在ロータ位置の取得 0-4095でreturnされる
  int _mechanicalAngle = 1234;  // LLでアナログ値の取得を行う。1234は仮の値

  //オフセット処理（検出用磁石が電気角0度に向いているとは限らない）
  _mechanicalAngle += 4096;
  _mechanicalAngle += _mechanicalAngleOffset;
  _mechanicalAngle %= 4096;

  return _mechanicalAngle;
}

int convertToElectricalAngle(const int _mechanicalAngle) {  //電気角に変換 1byteで返す
  int _electricalAngle = constrain(_mechanicalAngle, 0, 4095) % 585;
  _electricalAngle = constrain(map(_electricalAngle, 0, 585, 0, 127), 0, 127);

  return _electricalAngle;
}

void drive(int _electricalAngle, int power) {  // TODO:ちゃんと実装しろ
}
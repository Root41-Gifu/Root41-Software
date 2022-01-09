//---------------よく使う数学的関数やマクロ---------------
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

//---------------モーターとは直接関わりのない変数。---------------
int intervalCounter = 0;  //間引いて実行したい時用のインターバルカウンタ。

//---------------変数とか---------------
int realAngularVelocity = 0;  //符号ありであることに気をつける
int targetAngularVelocity = 0;
int deviation;

//---------------関数---------------

void INTERVAL(void) {  // ここはinterval[μs]秒ごとに実行される
  intervalCounter++;   //間引き実行用カウンタ
  intervalCounter %= intervalCounterLimit;

  if (intervalCounter == 0) {
    //---------------この中に間引き実行したいプログラムを記述しろ---------------
    realAngularVelocity = getAngularVelocity();
    deviation = realAngularVelocity = targetAngularVelocity;
    //---------------ここまで---------------
  }
  
}

int getAngularVelocity(void) {  // 角速度を求める。Δtが一定であることが動作条件
}

int getMechanicalAngle(void) {  //現在ロータ位置の取得 0-4095でreturnされる
  int mechanicalAngle = 1234;  // LLでアナログ値の取得を行う。1234は仮の値

  //オフセット処理（検出用磁石が電気角0度に向いているとは限らない）
  mechanicalAngle += 4096;
  mechanicalAngle += mechanicalAngleOffset;
  mechanicalAngle %= 4096;

  return mechanicalAngle;
}

int convertToElectricalAngle(const int _mechanicalAngle) {  //電気角に変換
  int electricalAngle = constrain(_mechanicalAngle, 0, 4095) % 585;
  electricalAngle = constrain(map(electricalAngle, 0, 585, 0, 127), 0, 127);

  return electricalAngle;
}

void drive(int _electricalAngle, int power) {}
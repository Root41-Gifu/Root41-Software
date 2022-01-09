//よく使う数学的関数やマクロ
#define constrain(amt, low, high) \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

extern long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// 割り込み関連の変数。インターバル時間何度を監視する。
int interval = 1000;
int intervalCounter = 0;

//パラメータなど。キャリブレーション処理をして測定する。とりあえずダミーで0を入力しておく。

int mechanicalAngleOffset = 0;

// ここはinterval[μs]秒ごとに実行される
void INTERVAL(void) {}

int getAngularVelocity(void) {}

//現在ロータ位置の取得 0-4095を電気角に変換して0-127でreturnする。
int getAngle(void) {
  int mechanicalAngle = 1234;  // LLでアナログ値の取得を行う。1234は仮の値

  //オフセット処理
  mechanicalAngle += 4096;
  mechanicalAngle += mechanicalAngleOffset;
  mechanicalAngle %= 4096;

  //電気角に変換
  int electricalAngle = constrain(mechanicalAngle, 0, 4095) % 585;
  electricalAngle = constrain(map(electricalAngle, 0, 585, 0, 127), 0, 127);

  return electricalAngle;
}
void _motor::begin(void) {
  Serial4.begin(115200);  //右
  Serial4.setRx(A1);
  Serial4.setTx(A0);
  Serial4.begin(115200);

  Serial1.begin(115200);  //左
  Serial1.setRx(PA10);
  Serial1.setTx(PA9);
  Serial1.begin(115200);

  //安全処理
  delay(1000);
  Serial4.write(0B11111111);
  Serial1.write(0B11111111);
  delay(10);
  Serial4.write(0B00000000);
  Serial1.write(0B00000000);
  delay(10);
  Serial4.write(0B10000000);
  Serial1.write(0B10000000);

  Serial4.write(0B00100000);
  Serial4.write(0B10100000);
  Serial1.write(0B01100000);
  Serial1.write(0B11100000);
}

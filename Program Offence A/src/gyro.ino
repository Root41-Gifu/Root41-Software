
MPU6050 mpu;
static uint8_t mpuIntStatus;
static bool dmpReady = false;
static uint16_t packetSize;
int16_t Gyro_Now = 0, Gyro = 0, Gyro_Offset = 0;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float ypr[3];

uint8_t devStatus;

VectorInt16 aa;
VectorInt16 aaReal;
VectorInt16 dmpgyro;
float euler[3];

volatile bool mpuInterrupt = false;

//割り込み
void dmpDataReady() { mpuInterrupt = true; }

//初期化
void _gyro::setting(void) {
RESTART:

  // gyro.eeprom[0] = (EEPROM[1] * 256) + EEPROM[2];
  // gyro.eeprom[1] = (EEPROM[3] * 256) + EEPROM[4];
  // gyro.eeprom[2] = (EEPROM[5] * 256) + EEPROM[6];
  // gyro.eeprom[3] = (EEPROM[7] * 256) + EEPROM[8];
  // gyro.eeprom[4] = (EEPROM[9] * 256) + EEPROM[10];
  // gyro.eeprom[5] = (EEPROM[11] * 256) + EEPROM[12];

  mpu.initialize();
  if (mpu.testConnection() != true) {
    goto RESTART;  //接続失敗
  }
  devStatus = mpu.dmpInitialize();
  if (devStatus != 0) {
    goto RESTART;  //初期化失敗
  }

  // -4781	-2019	5343	-41	5	30
  mpu.setXGyroOffset(-41);
  mpu.setYGyroOffset(5);
  mpu.setZGyroOffset(30);
  mpu.setXAccelOffset(-4781);
  mpu.setYAccelOffset(-2019);
  mpu.setZAccelOffset(5343);
  // mpu.setXGyroOffset(eeprom[0]);
  // mpu.setYGyroOffset(eeprom[1]);
  // mpu.setZGyroOffset(eeprom[2]);
  // mpu.setXAccelOffset(eeprom[3]);
  // mpu.setYAccelOffset(eeprom[4]);
  // mpu.setZAccelOffset(eeprom[5]);
  mpu.setDMPEnabled(true);

  // attachInterrupt(0, dmpDataReady, RISING);
  mpuIntStatus = mpu.getIntStatus();

  dmpReady = true;

  packetSize = mpu.dmpGetFIFOPacketSize();

  offsetVal = 0;
}

//角度取得
int _gyro::read(void) {
  int tempDeg;

  mpuIntStatus = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
  } else if (mpuIntStatus & 0x02) {
    while (fifoCount < packetSize) {
      fifoCount = mpu.getFIFOCount();
    }
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    // if (!line.flag)
    mpu.dmpGetGyro(&dmpgyro, fifoBuffer);
    Gyro_Now = degrees(ypr[0]);  // + 180;
    Gyro = Gyro_Now;
    while (Gyro <= 0) {
      Gyro += 360;
    }
    Gyro %= 360;
  }
  tempDeg = ((360 - Gyro) % 360 - offsetVal + 720) % 360;
  while (tempDeg < 0) {
    tempDeg += 360;
  }
  // while (Wire.available()) {
  //   i2cReadWithTimeoutFunction();
  // }
  // Serial.print(degrees(ypr[1]));
  // Serial.print("\t");
  // Serial.println(degrees(ypr[2]));
  if (degrees(ypr[2]) <= 165 && degrees(ypr[2]) >= -165) {
    isLift = true;
  } else if (degrees(ypr[1]) <= 165 && degrees(ypr[1]) >= -165) {
    isLift = true;
  } else {
    isLift = false;
  }

  while (char s = Wire.read() != -1) {
  }
  return tempDeg % 360;
}

//角速度取得
int _gyro::differentialRead(void) {
  return dmpgyro.z;
  // if (!dmpReady) {
  //   return 0;
  // }

  // while (!mpuInterrupt && fifoCount < packetSize) {
  // }

  // mpuInterrupt = false;
  // mpuIntStatus = mpu.getIntStatus();

  // fifoCount = mpu.getFIFOCount();

  // if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
  //   mpu.resetFIFO();
  //   return 0;  // 処理終了
  // }

  // while (fifoCount < packetSize) {
  //   fifoCount = mpu.getFIFOCount();
  // }

  // mpu.getFIFOBytes(fifoBuffer, packetSize);
  // fifoCount -= packetSize;

  // mpu.dmpGetGyro(&dmpgyro, fifoBuffer);

  // return dmpgyro.z;
}

int buffersize = 1000;
int acel_deadzone = 8;
int giro_deadzone = 1;

MPU6050 accelgyro(0x68);

int16_t ax, ay, az, gx, gy, gz;

int mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz, state = 0;
int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;

void _gyro::calibrationEEPROM(void) {
  accelgyro.initialize();

  accelgyro.setXAccelOffset(0);
  accelgyro.setYAccelOffset(0);
  accelgyro.setZAccelOffset(0);
  accelgyro.setXGyroOffset(0);
  accelgyro.setYGyroOffset(0);
  accelgyro.setZGyroOffset(0);

  if (state == 0) {
    meansensors();
    state++;
    delay(100);
  }

  if (state == 1) {
    Serial.println("\nCalculating offsets...");
    calibration();
    state++;
    delay(100);
  }

  if (state == 2) {
    meansensors();
    Serial.println("\nFINISHED!");
    Serial.print("Your offsets:\t");
    Serial.print(ax_offset);
    Serial.print("\t");
    Serial.print(ay_offset);
    Serial.print("\t");
    Serial.print(az_offset);
    Serial.print("\t");
    Serial.print(gx_offset);
    Serial.print("\t");
    Serial.print(gy_offset);
    Serial.print("\t");
    Serial.println(gz_offset);

    // EEPROMに洗脳する
    EEPROM[1] = highByte(gx_offset);
    EEPROM[2] = lowByte(gx_offset);

    EEPROM[3] = highByte(gy_offset);
    EEPROM[4] = lowByte(gy_offset);

    EEPROM[5] = highByte(gz_offset);
    EEPROM[6] = lowByte(gz_offset);

    EEPROM[7] = highByte(ax_offset);
    EEPROM[8] = lowByte(ax_offset);

    EEPROM[9] = highByte(ay_offset);
    EEPROM[10] = lowByte(ay_offset);

    EEPROM[11] = highByte(az_offset);
    EEPROM[12] = lowByte(az_offset);
    setting();
    delay(500);
    return;
  }
}

void meansensors() {
  long i = 0, buff_ax = 0, buff_ay = 0, buff_az = 0, buff_gx = 0, buff_gy = 0,
       buff_gz = 0;

  while (i < (buffersize + 101)) {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    if (i > 100 &&
        i <= (buffersize + 100)) {  // First 100 measures are discarded
      buff_ax = buff_ax + ax;
      buff_ay = buff_ay + ay;
      buff_az = buff_az + az;
      buff_gx = buff_gx + gx;
      buff_gy = buff_gy + gy;
      buff_gz = buff_gz + gz;
    }
    if (i == (buffersize + 100)) {
      mean_ax = buff_ax / buffersize;
      mean_ay = buff_ay / buffersize;
      mean_az = buff_az / buffersize;
      mean_gx = buff_gx / buffersize;
      mean_gy = buff_gy / buffersize;
      mean_gz = buff_gz / buffersize;
    }
    i++;
    delay(2);  // Needed so we don't get repeated measures
  }
}

void calibration() {
  ax_offset = -mean_ax / 8;
  ay_offset = -mean_ay / 8;
  az_offset = (16384 - mean_az) / 8;

  gx_offset = -mean_gx / 4;
  gy_offset = -mean_gy / 4;
  gz_offset = -mean_gz / 4;
  while (1) {
    int ready = 0;
    accelgyro.setXAccelOffset(ax_offset);
    accelgyro.setYAccelOffset(ay_offset);
    accelgyro.setZAccelOffset(az_offset);

    accelgyro.setXGyroOffset(gx_offset);
    accelgyro.setYGyroOffset(gy_offset);
    accelgyro.setZGyroOffset(gz_offset);

    meansensors();
    Serial.println("...");

    if (abs(mean_ax) <= acel_deadzone)
      ready++;
    else
      ax_offset = ax_offset - mean_ax / acel_deadzone;

    if (abs(mean_ay) <= acel_deadzone)
      ready++;
    else
      ay_offset = ay_offset - mean_ay / acel_deadzone;

    if (abs(16384 - mean_az) <= acel_deadzone)
      ready++;
    else
      az_offset = az_offset + (16384 - mean_az) / acel_deadzone;

    if (abs(mean_gx) <= giro_deadzone)
      ready++;
    else
      gx_offset = gx_offset - mean_gx / (giro_deadzone + 1);

    if (abs(mean_gy) <= giro_deadzone)
      ready++;
    else
      gy_offset = gy_offset - mean_gy / (giro_deadzone + 1);

    if (abs(mean_gz) <= giro_deadzone)
      ready++;
    else
      gz_offset = gz_offset - mean_gz / (giro_deadzone + 1);

    if (ready >= 6) break;
  }
}

void _gyro::offsetRead(void) {
  offsetVal = 0;
  // Serial.println(gyro.read());
  // offsetVal = deg;
  // Serial.println(offsetVal);

  for (int i = 0; i < 150; i++) {
    deg = gyro.read();
  }

  delay(100);
  deg = gyro.read();

  if (deg != 0) {
    offsetVal += deg;
  }

  while (offsetVal < 0) {
    offsetVal += 360;
  }
  offsetVal %= 360;

  Serial.println(gyro.read());

  // if (device.getTime() - device.startTimer >= 1200) {
  //   device.startTimer = device.getTime();
  // }
}

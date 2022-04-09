_Camera::_Camera() {
  mode = 1;
}

void _Camera::read() {
  // int32_t len = 0;
  // char buff[CHAR_BUF] = {0};
  // digitalWrite(CAMERA_PIN, LOW);
  // delay(1);  // Give the OpenMV Cam some time to setup to send data.

  // if (SPI.transfer(1) == 85) {  // saw sync char?
  //   SPI.transfer(&len, 4);      // get length
  //   if (len) {
  //     SPI.transfer(&buff, min(len, CHAR_BUF));
  //     len -= min(len, CHAR_BUF);
  //   }
  //   while (len--)
  //     SPI.transfer(0);  // eat any remaining bytes
  // }

  // digitalWrite(SS_PIN, HIGH);
  // delay(1);  // Don't loop to quickly.

  SPI.begin();
  int data;
  unsigned long startTime = micros();
  digitalWrite(CAMERA_PIN, LOW);

  data = SPI.transfer(0);
  delayMicroseconds(10);

  digitalWrite(CAMERA_PIN, HIGH);
  unsigned long endTime = micros();
  Serial.print(data);
  Serial.print(" ");
  Serial.println(startTime - endTime);
  SPI.end();
}

void _Camera::calc() {}
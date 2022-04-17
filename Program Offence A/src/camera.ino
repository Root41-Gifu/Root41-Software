_Camera::_Camera() {
  mode = 1;
}

void _Camera::read() {
  byte readData;
  if (Serial6.available()) {
    readData = Serial6.read();
  }
  Serial.println(readData);
  mode = readData >> 6;
  goal_x = readData &= B00111111;
}

void _Camera::calc() {}
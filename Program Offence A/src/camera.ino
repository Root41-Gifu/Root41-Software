_Camera::_Camera() {
  mode = 1;
}

void _Camera::read() {
  byte readData;
  if (Serial.available()) {
    readData = Serial.read();
  }
  mode = readData >> 6;
  goal_x = readData &= B00111111;
}

void _Camera::calc() {}
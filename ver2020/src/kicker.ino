void _kicker::kick(bool status) {
  if (status != _val && status && device.getTime() - protectionTimer >= 1000) {
    kickTimer = device.getTime();
    digitalWrite(SOLENOID, HIGH);

    LED.changeAll(LED.YELLOW);

  } else if (device.getTime() - kickTimer <= 200) {
    digitalWrite(SOLENOID, HIGH);
    LED.changeAll(LED.YELLOW);

    protectionTimer = device.getTime();

  } else {
    digitalWrite(SOLENOID, LOW);
  }
  _val = status;
}

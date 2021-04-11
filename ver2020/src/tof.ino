int _tof::read(void) {
  int data = TOF.readRangeContinuousMillimeters();
  _dist = data > 1 ? data : _dist;
  return _dist;
}

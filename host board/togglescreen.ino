void togglescreenfunction() {
  // for testing without esp now
  reading = 0;
  avgReading = 0;
  for (int n = 0; n < 10; n++)
  {
    reading = reading + analogRead(joystick);
  }
  avgReading = reading / 10;
  // Serial.println(String("average reading:") + avgReading);
  if (avgReading >= MIDDLE_JOYSTICK_THRESHOLD_MIN && avgReading <= MIDDLE_JOYSTICK_THRESHOLD_MAX && togglescreen == 1)
  {
    togglescreen = 2;
  }
  else if (avgReading >= MIDDLE_JOYSTICK_THRESHOLD_MIN && avgReading <= MIDDLE_JOYSTICK_THRESHOLD_MAX && togglescreen == 2)
  {
    togglescreen = 1;
  }
  delay(200);
}
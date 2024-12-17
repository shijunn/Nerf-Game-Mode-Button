void readstick() {
  // for testing without esp now
  reading = 0;
  avgReading = 0;
  for (int n = 0; n < 10; n++)
  {
    reading = reading + analogRead(joystick);
  }
  avgReading = reading / 10;
  // Serial.println(String("average reading:") + avgReading);
  if (avgReading >= LEFT_JOYSTICK_THRESHOLD_MIN && avgReading <= LEFT_JOYSTICK_THRESHOLD_MAX)  // 7 decrease to 1, 1 wrap to 7
  {
    menu = (menu > CAPTURE_THE_FLAG) ? (menu - 1) : (NUM_MODES - 1);
    // 2 is more than 1, true, 2 - 1 = 1
    //1 is not less than 1, false, 8-1 = 7
  }
  else if (avgReading >= RIGHT_JOYSTICK_THRESHOLD_MIN && avgReading <= RIGHT_JOYSTICK_THRESHOLD_MAX)  // 1 increase to 7, 7 wrap to 1
  {
    menu = (menu < NUM_MODES - 1) ? (menu + 1) : (CAPTURE_THE_FLAG);
    // 6 is less than 7, true, 6 + 1 = 7
    // 7 is not less than 7, false, capture the flag
  }
  else if (avgReading >= MIDDLE_JOYSTICK_THRESHOLD_MIN && avgReading <= MIDDLE_JOYSTICK_THRESHOLD_MAX && laststate == HIGH)
  {
    pressedTime = millis();
    laststate = LOW;
  }
  else if(avgReading >= MIDDLE_JOYSTICK_THRESHOLD_MIN && avgReading <= MIDDLE_JOYSTICK_THRESHOLD_MAX && laststate == LOW)
  {
    longpressduration = millis() - pressedTime;
    if( longpressduration > 2000)
    {
      middlelongpressed = true;
      laststate = HIGH;
    }
  }
  else
  {
    laststate = HIGH; // when middle isnt pressed, time will restart to prevent accidental press
  }
  delay(200);
}
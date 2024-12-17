void setting()
{
  middlelongpressed = false;
  
  while (middlelongpressed == false)
  {
    reading = 0;
    avgReading = 0;
    for (int n = 0; n < 10; n++)
    {
      reading = reading + analogRead(joystick);
    }
    avgReading = reading / 10;
    // Serial.println(String("average reading:") + avgReading);
    if (avgReading >= LEFT_JOYSTICK_THRESHOLD_MIN && avgReading <= LEFT_JOYSTICK_THRESHOLD_MAX && timesetting > 0)
    {
      timesetting--;
      gamedata.gamemodetime = timesetting * 60000; // convert to milliseconds
      sendall();
    }
    else if (avgReading >= RIGHT_JOYSTICK_THRESHOLD_MIN && avgReading <= RIGHT_JOYSTICK_THRESHOLD_MAX)
    {
      timesetting++;
      gamedata.gamemodetime = timesetting * 60000; // convert to milliseconds
      sendall();
    }
    else if (avgReading >= MIDDLE_JOYSTICK_THRESHOLD_MIN && avgReading <= MIDDLE_JOYSTICK_THRESHOLD_MAX && laststate == HIGH)
    {
      pressedTime = millis();
      laststate = LOW;
    }
    else if(avgReading >= MIDDLE_JOYSTICK_THRESHOLD_MIN && avgReading <= MIDDLE_JOYSTICK_THRESHOLD_MAX && laststate == LOW)
    {
      longpress = millis() - pressedTime;
      if (longpress > 2000)
      {
        middlelongpressed = true;
        laststate = HIGH;
        gamedata.gamemodetime = timesetting * 60000; // convert to milliseconds
        //gamemodetime = preferences.getULong("gamemodetime", gamemodetime);
        #ifdef devdebug
          Serial.println(String("Game time set to: ") + timesetting + String(" mins"));
          Serial.println(gamedata.gamemodetime + String("ms"));
        #endif
        sendall();
      }
    }
    else
    {
      laststate = HIGH; // when middle isnt pressed, time will restart to prevent accidental press
    }
    //gamemodetime = preferences.getULong("gamemodetime");
    oledDisplayStrInt(gamedata.gamemodetime/60000, " mins"); // live display update
    //oledDisplayInt(timesetting);
    delay(100);
  }
}
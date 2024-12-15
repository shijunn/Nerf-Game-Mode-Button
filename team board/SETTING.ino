/*
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
      gamemodetime = timesetting * 60000; // convert to milliseconds
    }
    else if (avgReading >= RIGHT_JOYSTICK_THRESHOLD_MIN && avgReading <= RIGHT_JOYSTICK_THRESHOLD_MAX)
    {
      timesetting++;
      gamemodetime = timesetting * 60000; // convert to milliseconds
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
        gamemodetime = timesetting * 60000; // convert to milliseconds
        //gamemodetime = preferences.getULong("gamemodetime", gamemodetime);
        #ifdef devdebug
          Serial.println(String("Game time set to: ") + timesetting + String(" mins"));
          Serial.println(gamemodetime + String("ms"));
        #endif
        esp_err_t result = esp_now_send(0, (unint8_t *) &gamedata, sizeof(game_struct));
        
        if (result == ESP_OK)
        {
          Serial.println("Sent successfully");
        }
        else
        {
          Serial.println("Error sending the data");
        }
      }
    }
    else
    {
      laststate = HIGH; // when middle isnt pressed, time will restart to prevent accidental press
    }
    //gamemodetime = preferences.getULong("gamemodetime");
    oledDisplayStrInt(gamemodetime/60000, " mins"); // live display update
    //oledDisplayInt(timesetting);
    delay(100);
  }
}
*/
/*
void IRAM_ATTR watchDogInterrupt() 
{
  Serial.println("watchDogInterrupt reboot");
  watchDogActive = true;
  ESP.restart();
}

void watchDogRefresh()
{
  timerAlarmDisable(watchDogTimer);  
  timerWrite(watchDogTimer, 0); //reset timer (feed watchdog)
  timerAlarmEnable(watchDogTimer);
  Serial.println("Watchdog restarted");
}
*/
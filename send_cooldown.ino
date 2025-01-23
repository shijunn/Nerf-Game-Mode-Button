void sendcooldown()
{
  if (((millis() - timesent) > SENDCOOLDOWN) && !sentbefore)
  { // update host board every 5 seconds to prevent crashing
    sendscoretimeendgame();
    sentbefore = true;
    timesent = millis();
  }
  else if (((millis() - timesent) > SENDCOOLDOWN) && sentbefore)
  {
    sendscoretimeendgame();
    sentbefore = false;
    timesent = millis();
  }
}
void resetvariables()
{
  incomingmodenum = CAPTURE_THE_FLAG; // mode = 0
  startedrecording = false;
  prevTime = 0;
  totalTime = 0;
  startPressTime = 0;
  gamedata.recordedTime = 0;
  pressed = false;
  storedPrev = false;
  gamedata.endgame = false;
  gamedata.score = 0;
  sentbefore = false;
  pixels.clear();
  pixels.show();
}
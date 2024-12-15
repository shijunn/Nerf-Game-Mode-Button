void resetvariables()
{
  //lastmenu = 0;
  //menu = CAPTURE_THE_FLAG; // menu = 1
  incomingmodenum = CAPTURE_THE_FLAG; // mode = 1
  //middlelongpressed = false;
  //externalbuttonpressed = false;
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
/*
void resetscore()
{
  score = 0;
}
*/
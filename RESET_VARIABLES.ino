void resetvariables()
{
  lastmenu = 0;
  menu = CAPTURE_THE_FLAG; // menu = 1
  modenum = CAPTURE_THE_FLAG; // mode = 1
  middlelongpressed = false;
  //externalbuttonpressed = false;
  startedrecording = false;
  pressedTime = 0;
  prevTime = 0;
  totalTime = 0;
  longpressduration = 0;
  startPressTime = 0;
  recordedTime = 0;
  pressed = false;
  storedPrev = false;
  endgame = false;
  pixels.clear();
  pixels.show();
}

void resetscore()
{
  score = 0;
}
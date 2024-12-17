void resetvariables()
{
  lastmenu = 0;
  menu = CAPTURE_THE_FLAG; // menu = 1
  gamedata.modenum = 1; // mode = 1
  middlelongpressed = false;
  longpressduration = 0;
  pixels.clear();
  pixels.show();
  display.setTextSize(2);
  gamedata.confirm = false;
  sendall(); // reset variables on team board
}
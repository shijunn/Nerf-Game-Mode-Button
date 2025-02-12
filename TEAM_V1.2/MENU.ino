void updatedisplay() {
  if (incomingmodenum != lastincomingmodenum || incominggoback)
  {
    colorWipe(pixels.Color(255, 255, 255), 5);
    lastincomingmodenum = incomingmodenum;
    pixels.clear();
    pixels.show();
  }
  switch (incomingmodenum)
  {
    case CAPTURE_THE_FLAG:
      oledDisplayCenter("1.FLAG");
      break;

    case DOMINATION:
      oledDisplayCenter("2.DOMINATION");
      break;

    case KING_OF_THE_HILL:
      oledDisplayCenter("3.KOTH");
      break;

    case CLICKER_DOMINATION:
      oledDisplayCenter("4.CLICKER");
      break;

    case CHESS_CLOCK:
      oledDisplayCenter("5.CHESS CLOCK");
      break;

    case SETTING:
      oledDisplayCenter("6.SETTINGS");
      break;

    case BOARD_NUMBER:
      oledDisplayStrInt("Board: ", gamedata.id);
      break;

    default:
      oledDisplayCenter("just booted up");
      break;
  }
}
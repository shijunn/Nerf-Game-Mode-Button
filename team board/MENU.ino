/*
void menudisplay() {
  // for testing without esp now
  if (menu != lastmenu) {
    lastmenu = menu;
    //display.clearDisplay();
    //display.display();
    //display.setCursor(0,10);
    switch (menu) {
      case 1:
        //display.print("FLAG"); // capture the flag
        oledDisplayCenter("1.FLAG");
        modenum = CAPTURE_THE_FLAG;
        break;

      case 2:
        //display.print("DOMINATION"); //domination
        oledDisplayCenter("2.DOMINATION");
        modenum = DOMINATION;
        break;

      case 3:
        //display.print("KOTH"); // king of the hill
        oledDisplayCenter("3.KOTH");
        modenum = KING_OF_THE_HILL;
        break;

      case 4:
        //display.print("CLICKER"); // clicker domination
        oledDisplayCenter("4.CLICKER");
        modenum = CLICKER_DOMINATION;
        break;

      case 5:
        oledDisplayCenter("5.CHESS CLOCK");
        modenum = CHESS_CLOCK;
        break;

      case 6:
        oledDisplayCenter("Settings");
        modenum = SETTING;
        break;
      
      // add in settings page for time limit
      // same time set for all games
      // basically changing the global variable
      // save in preference

      default:
        Serial.println("ERROR AT SWITCH CASE");
        break;
    }
    display.display();
  }
}
*/

void updatedisplay()
{

    switch(incomingmodenum)
    {
      case 1:
        oledDisplayCenter("1.FLAG");
        break;

      case 2:
        oledDisplayCenter("2.DOMINATION");
        break;
      
      case 3:
        oledDisplayCenter("3.KOTH");
        break;

      case 4:
        oledDisplayCenter("4.CLICKER");
        break;

      case 5:
        oledDisplayCenter("5.CHESS CLOCK");
        break;

      case 6:
        oledDisplayStrInt("Minutes: ", (incominggamemodetime/60000));
        break;

      case 7:
        oledDisplayStrInt("Board: ", gamedata.id);
        break;
        
      default:
        oledDisplayCenter("just booted up");
        break;
  }
}
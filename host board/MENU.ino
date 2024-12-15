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
        gamedata.modenum = 1;
        sendall(); // updates live display on team board to show that it is synced
        break;

      case 2:
        //display.print("DOMINATION"); //domination
        oledDisplayCenter("2.DOMINATION");
        gamedata.modenum = 2;
        sendall();
        break;

      case 3:
        //display.print("KOTH"); // king of the hill
        oledDisplayCenter("3.KOTH");
        gamedata.modenum = 3;
        sendall();
        break;

      case 4:
        //display.print("CLICKER"); // clicker domination
        oledDisplayCenter("4.CLICKER");
        gamedata.modenum = 4;
        sendall();
        break;

      case 5:
        oledDisplayCenter("5.CHESS CLOCK");
        gamedata.modenum = 5;
        sendall();
        break;

      case 6:
        oledDisplayCenter("Settings");
        gamedata.modenum = 6;
        sendall();
        break;
      
      // add in settings page for time limit
      // same time set for all games
      // basically changing the global variable
      // save in preference

      case 7:
        oledDisplayCenter("Board number");
        gamedata.modenum = 7;
        sendall();
        break;

      default:
        Serial.println("ERROR AT SWITCH CASE");
        break;
    }
    display.display();
  }
}
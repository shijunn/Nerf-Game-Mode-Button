void checkendgame()
{
  unsigned long movingtime = 0;
  while (true) //&& second team's incomingendgame)
  {
    // displays timer
    movingtime = gamedata.gamemodetime-millis();
    oledDisplayULong(movingtime);
    if (board1_endgame && !board2_endgame)
    {
      #ifdef devdebug
        Serial.println("board1 won");
      #endif
      oledDisplayCenter("Blue wins!");
      colorWipe(pixels.Color(0,0,255), 5); // sets neopixels on host to blue
      break;
    }
    else if (!board1_endgame && board2_endgame)
    {
      #ifdef devdebug
        Serial.println("board2 won");
      #endif
      oledDisplayCenter("Red wins");
      colorWipe(pixels.Color(255,0,0), 5); // sets neopixels on host to red
      break;
    }
  }
  
}

void checkscore()
{
  bool over = false;
  int previncomingscore = 0;
  while (true)
  {
    oledDisplayScore("1.score", board1_score, "2.score", board2_score);
    /*
    if (previncomingscore < incomingscore)
    {
      previncomingscore = incomingscore;
    }
    */
    if (board1_endgame || board2_endgame)
    {
      #ifdef devdebug
        Serial.println("time ended");
      #endif
      over = true;
    }
    if (board1_endgame && !board2_endgame && over || !board1_endgame && board2_endgame && over)
    { // check who won
      if (gamedata.modenum == 2) // least clicks wins
      {
        if (board1_score < board2_score)
        {
          #ifdef devdebug
            Serial.println("board1 won");
          #endif
          oledDisplayCenter("Blue wins!");
          colorWipe(pixels.Color(0,0,255), 5); // sets neopixels on host to blue
          break;
        }
        else if (board2_score < board1_score)
        {
          #ifdef devdebug
            Serial.println("board2 won");
          #endif
          oledDisplayCenter("Red wins");
          colorWipe(pixels.Color(255,0,0), 5); // sets neopixels on host to red
          break;
        }
        else
        {
          #ifdef devdebug
            Serial.println("DRAW");
          #endif
          oledDisplayCenter("Draw");
          colorWipe(pixels.Color(200,200,200), 5); // sets neopixels on host to white
          break;
        }
      }
      else if (gamedata.modenum == 4) // most clicks wins
      {
        if (board1_score > board2_score)
        {
          #ifdef devdebug
            Serial.println("board1 won");
          #endif
          oledDisplayCenter("Blue wins!");
          colorWipe(pixels.Color(0,0,255), 5); // sets neopixels on host to blue
          break;
        }
        else if (board2_score > board1_score)
        {
          #ifdef devdebug
            Serial.println("board2 won");
          #endif
          oledDisplayCenter("Red wins");
          colorWipe(pixels.Color(255,0,0), 5); // sets neopixels on host to red
          break;
        }
        else
        {
          #ifdef devdebug
            Serial.println("DRAW");
          #endif
          oledDisplayCenter("Draw");
          colorWipe(pixels.Color(200,200,200), 5); // sets neopixels on host to white
          break;
        }
      }
    }
  }
}
/*
void checkprogress()
{
// check endgame and check time
// team board sends live time recorded
// host display updates with new time
  while (true)
  {
    oledDisplayTime(incomingtesttime, "test ", incomingrecordedTime, "rec ");
  
    if (incomingendgame)
    {
      #ifdef devdebug
        Serial.println("game ended by obj");
      #endif
      break;
    }
  }
}
*/
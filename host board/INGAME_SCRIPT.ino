void checkendgame()
{
  unsigned long movingtime = 0;
  while (true) //&& second team's incomingendgame)
  {
    // displays timer
    movingtime = gamedata.gamemodetime-millis();
    oledDisplayULong(movingtime);
    if (board1_endgame && !board2_endgame && modenum == 1)
    {
      delay(500);
      mp3.playTrack(3);
      delay(100);
      mp3.playTrack(3);
      #ifdef devdebug
        Serial.println("board1 won");
      #endif
      oledDisplayCenter("Blue wins!");
      colorWipe(pixels.Color(0,0,255), 5); // sets neopixels on host to blue
      delay(3000);
      break;
    }
    else if (!board1_endgame && board2_endgame && modenum == 1)
    {
      delay(500);
      mp3.playTrack(4);
      delay(100);
      mp3.playTrack(4);
      #ifdef devdebug
        Serial.println("board2 won");
      #endif
      oledDisplayCenter("Red wins");
      colorWipe(pixels.Color(255,0,0), 5); // sets neopixels on host to red
      delay(3000);
      break;
    }
    if (board1_endgame && modenum == 3 || board2_endgame && modenum == 3)
    {
      if (board1_testtime > board2_testtime)
      {
        delay(500);
        mp3.playTrack(3);
        delay(100);
        mp3.playTrack(3);
        #ifdef devdebug
          Serial.println("board1 won");
        #endif
        oledDisplayCenter("Blue wins!");
        colorWipe(pixels.Color(0,0,255), 5); // sets neopixels on host to blue
        delay(3000);
        break;
      }
      else if (board1_testtime < board2_testtime)
      {
        delay(500);
        mp3.playTrack(4);
        delay(100);
        mp3.playTrack(4);
        #ifdef devdebug
          Serial.println("board2 won");
        #endif
        oledDisplayCenter("Red wins");
        colorWipe(pixels.Color(255,0,0), 5); // sets neopixels on host to red
        delay(3000);
        break;
      }
    }
  }
  
}

void checkscore() // doesnt reset variables on team boards
{
  bool over = false;
  int previncomingscore = 0;
  while (true)
  {
    oledDisplayScore("B score ", board1_score, "R score ", board2_score);
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
          mp3.playTrack(3);
          delay(500);
          mp3.playTrack(3);
          #ifdef devdebug
            Serial.println("board1 won");
          #endif
          oledDisplayCenter("Blue wins!");
          colorWipe(pixels.Color(0,0,255), 5); // sets neopixels on host to blue
          delay(3000);
          break;
        }
        else if (board2_score < board1_score)
        {
          mp3.playTrack(4);
          delay(500);
          mp3.playTrack(4);
          #ifdef devdebug
            Serial.println("board2 won");
          #endif
          oledDisplayCenter("Red wins");
          colorWipe(pixels.Color(255,0,0), 5); // sets neopixels on host to red
          delay(3000);
          break;
        }
        else
        {
          #ifdef devdebug
            Serial.println("DRAW");
          #endif
          oledDisplayCenter("Draw");
          colorWipe(pixels.Color(200,200,200), 5); // sets neopixels on host to white
          delay(3000);
          break;
        }
      }
      else if (gamedata.modenum == 4) // most clicks wins
      {
        if (board1_score > board2_score)
        {
          mp3.playTrack(3);
          delay(500);
          mp3.playTrack(3);
          #ifdef devdebug
            Serial.println("board1 won");
          #endif
          oledDisplayCenter("Blue wins!");
          colorWipe(pixels.Color(0,0,255), 5); // sets neopixels on host to blue
          delay(3000);
          break;
        }
        else if (board2_score > board1_score)
        {
          mp3.playTrack(4);
          delay(500);
          mp3.playTrack(4);
          #ifdef devdebug
            Serial.println("board2 won");
          #endif
          oledDisplayCenter("Red wins");
          colorWipe(pixels.Color(255,0,0), 5); // sets neopixels on host to red
          delay(3000);
          break;
        }
        else
        {
          #ifdef devdebug
            Serial.println("DRAW");
          #endif
          oledDisplayCenter("Draw");
          colorWipe(pixels.Color(200,200,200), 5); // sets neopixels on host to white
          delay(3000);
          break;
        }
      }
    }
  }
}

void checkprogress()
{
// check endgame and check time
// team board sends live time recorded
// host display updates with new time
  while (true)
  {
    // incomingtesttime stores current recordedtime + prevtime and check if it meets the threshold
    // incomingrecordedtime stores only the current time being recorded
    togglescreenfunction();
    if (togglescreen == 1)
    {
      oledDisplayTime(board1_testtime, "B test ", board1_recordedTime, "B rec ");
    }
    else if (togglescreen == 2)
    {
      oledDisplayTime(board2_testtime, "R test ", board2_recordedTime, "R rec ");
    }
    if (board1_endgame || board2_endgame)
    {
      #ifdef devdebug
        Serial.println("game ended by obj");
      #endif
      break;
    }
  }
}

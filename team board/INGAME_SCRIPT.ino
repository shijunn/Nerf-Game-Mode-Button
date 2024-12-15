// while loop condition
// while current time - time when game started = less than 3 mins -> continue loop doing stuff in the loop
// domination and clicker domination is using the same code


// COMPLETED
void captureTheFlag()  // one hit to win, 3 mins, completed
{
  Serial.println("Starting capture the flag");
  oledDisplayCountdown(incomingcountdowntime); // countdown time set by host

  startGameTime = millis();
  while (millis() - startGameTime < incominggamemodetime)  // if ____ mins not over yet
  {
    colorWipe(pixels.Color(0, 0, 255), 5);  // blue
    if (digitalRead(extButton) == LOW)
    {
      oledDisplayCenter("Winner");
      gamedata.endgame = true;
      sendscoretimeendgame(); // sets endgame flag to true to host
      // host will receive and see who sent the flag first
      // displays winner accordingly
      // resets variables
      delay(2000);
      break;
    }
  }
  oledDisplayCenter("OVER");
  delay(2000);  // to display
}


// COMPLETED
void domination()  // least number of clicks wins, start from 0, 3mins
{
  #ifdef devdebug
    Serial.println("Starting domination");
  #endif

  // HIGH -> LOW -> HIGH = one press
  

  oledDisplayCountdown(incomingcountdowntime);
  startGameTime = millis();
  while (millis() - startGameTime < incominggamemodetime)
  {
    colorWipe(pixels.Color(200, 0, 150), 5);
    int reading = digitalRead(extButton);

    if (!pressed && reading == LOW)
    { // need to use flag, if not score will go to 1 when game starts
      pressed = true;
    }

    if (reading != defaultbuttonstate) // if button is pressed
    {
      lastdebouncetime = millis();
    }

    if (((millis() - lastdebouncetime) > debouncedelay) && pressed) // if time since last press is > 25ms
    {
      if (reading != buttonstate) // check if button has been let go (HIGH)
      {
        buttonstate = reading;
        if (buttonstate == HIGH) // only register press if the button has gone from LOW to HIGH (pressed -> not pressed)
        {
          gamedata.score++;
          #ifdef devdebug
            Serial.println(gamedata.score);
          #endif
          // sendscoretimeendgame();
          // sentbefore = true;
          // timesent = millis();
          sendcooldown();
        }
      }
    }

    sendcooldown(); // still continue to update host even after button is not pressed
    defaultbuttonstate = reading;
  }
  oledDisplayCenter("OVER");
  gamedata.endgame = true;
  sendscoretimeendgame(); // ends game on host board
  delay(2000);  // to display
  // send scores to host
}


// COMPLETED
void kingOfTheHill()  // press and hold, start from 0, 5 mins
{
  #ifdef devdebug
    Serial.println("Starting king of the hill");
  #endif
  //unsigned long testtime = 0;
  

  oledDisplayCountdown(incomingcountdowntime);
  startGameTime = millis();
  while ((millis() - startGameTime < incominggamemodetime) && !endgame) {
    colorWipe(pixels.Color(255, 128, 0), 5);
    // add recorded time with previous time
    if (digitalRead(extButton) == LOW && !startedrecording)  // while pressed
    {
      startPressTime = millis();
      startedrecording = true;
      #ifdef devdebug
        Serial.println("started recording");
      #endif
    }
    else if (digitalRead(extButton) == LOW && startedrecording)// startedrecording == true
    {
      gamedata.recordedTime = millis() - startPressTime;
      #ifdef devdebug
        Serial.println(gamedata.recordedTime); // pressed duration for this press
      #endif
      sendscoretimeendgame();
      gamedata.testtime = gamedata.recordedTime + prevTime; // checks if it reached threshold while button IS pressed
      // press duration for this press + previous press
    }
    
    else // button not pressed
    {
      // checks if it has reached threshold while button is NOT pressed
      startedrecording = false;  // when not pressed, reset flag
      prevTime += gamedata.recordedTime; // assigns recorded time to another variable so when user lets go of button and presses it again, it will continue from the previous time saved when the person stoppped pressing the button instead of starting from 0
      #ifdef devdebug
        Serial.println("button not pressed");
        Serial.println(String("Time pressed (ms) = ") + prevTime);
      #endif
      gamedata.recordedTime = 0; // reset recordedTime back to 0
      
    }
    if (gamedata.recordedTime >= incominggamemodetime || gamedata.testtime >= incominggamemodetime) 
    // sets the flag to true to exit while loop while button is pressed or let go
    // recordedTime means the person didnt let go at all for the entire game
    // testtime means the person let go once and the score was saved, then the person press again and the score reached the threshold while the button is pressed
    {
      gamedata.endgame = true;
      sendscoretimeendgame();
    }
  }
  gamedata.endgame = true;
  gamedata.testtime = 0;
  gamedata.recordedTime = 0;
  sendscoretimeendgame(); // resets display on host
  Serial.println("game ended");
  oledDisplayCenter("OVER");
  delay(2000);  // to display
}

// COMPLETED
void clickerDomination()
{
  #ifdef devdebug
    Serial.println("Starting clicker domination");
  #endif

  // HIGH -> LOW -> HIGH = one press
  

  oledDisplayCountdown(5);
  startGameTime = millis();
  while (millis() - startGameTime < incominggamemodetime)
  {
    colorWipe(pixels.Color(200, 0, 150), 5);
    int reading = digitalRead(extButton);

    if (!pressed && reading == LOW)
    { // need to use flag, if not score will go to 1 when game starts
      pressed = true;
    }

    if (reading != defaultbuttonstate) // if button is pressed
    {
      lastdebouncetime = millis();
    }

    if (((millis() - lastdebouncetime) > debouncedelay) && pressed) // if time since last press is > 25ms
    {
      if (reading != buttonstate) // check if button has been let go (HIGH)
      {
        buttonstate = reading;
        if (buttonstate == HIGH) // only register press if the button has gone from LOW to HIGH (pressed -> not pressed)
        {
          score++;
          #ifdef devdebug
            Serial.println(score);
          #endif
        }
      }
    }
    defaultbuttonstate = reading;
  }
  oledDisplayCenter("OVER");
  delay(2000);  // to display
  // send scores to host
}


void chessClock() // press and let go, go down from time limit, first team to reach 0 wins
{  // assume this board is blue team
  #ifdef devdebug
    Serial.println("Starting chess clock");
  #endif

  // unsigned long newTime = incominggamemodetime;
  // unsigned long checktime = incominggamemodetime;
  

  oledDisplayCountdown(5);
  colorWipe(pixels.Color(200, 200, 150), 5);
  while(endgame) //game ends only when timer on one team reaches 0
  {
    
    if (digitalRead(extButton) == LOW && digitalRead(cancelButton) == HIGH)
    {                           // only blue team button pressed
      pressed = true;
      startPressTime = millis(); // start recording
      // minus startPressTime from incominggamemodetime
    }
    
    else if (digitalRead(extButton) == HIGH && digitalRead(cancelButton) == LOW)
    {                           // only red team button pressed
      if (pressed == true) // if pressed previously
      {
        storedPrev = true;
      }
        pressed = false; // reset flag
        // start recording on other team board
        prevTime = newTime;
    }
    
    if (pressed) // continue calculating without the button being held down
    { // if blue team pressed
      if (storedPrev) // prevTime has been stored before
      {
        newTime = prevTime - (millis()-startPressTime);
        #ifdef devdebug
          Serial.println(newTime + String("     if"));
        #endif
        // when opponent press button, time recorded here is stored as newTime
        // newTime is stored into prevTime
        // when button is pressed again, game continues from time that was stored here
        checktime = incominggamemodetime - newTime;
      }
      else{
        newTime = incominggamemodetime - (millis() - startPressTime);
        #ifdef devdebug
          Serial.println(newTime + String("      else"));
        #endif
        // when button is pressed again, it continues from this number
        // only goes through this else condition for the first time the button is pressed in the game
      }
    }
    if (checktime <= 0 || newTime <= 0)
    {
      endgame = true;
    }
  }
  oledDisplayCenter("OVER");
  delay(2000);
}

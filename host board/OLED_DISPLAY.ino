void oledDisplayCenter(String text) 
{
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

  // display on horizontal and vertical center
  display.clearDisplay(); // clear display
  display.setTextColor(SSD1306_WHITE);
  display.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);
  display.println(text); // text to display
  display.display();
}

void oledDisplayInt(int number)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  display.println(number);
  display.display();
}

void oledDisplayCountdown(int seconds)
{
  for (seconds; seconds>0; seconds--)
  {
    oledDisplayInt(seconds);
    delay(1000); // 1 second pause
  }
  oledDisplayCenter("START!");
}

void oledDisplayStrInt(int number, String text)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(number + text);
  display.display();
}

void oledDisplayULong(unsigned long number)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(number);
  display.display();
}

void oledDisplayTime(unsigned long number1, String text1, unsigned number2, String text2)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(text1 + number1);
  display.setCursor(0,18);
  display.println(text2 + number2);
  display.display();
}

void oledDisplayScore(String text1, int score1, String text2, int score2)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(text1 + score1);
  display.setCursor(0,18);
  display.println(text2 + score2);
  display.display();
}
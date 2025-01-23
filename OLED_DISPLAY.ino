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
    pixels.clear();
    pixels.show();
    oledDisplayInt(seconds);
    showwhite();
    delay(1000); // 1 second pause
  }
  oledDisplayCenter("START!");
}

void oledDisplayStrInt(String text, int number)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(text + number);
  display.display();
}
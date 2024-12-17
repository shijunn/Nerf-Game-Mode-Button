void colorWipe(uint32_t c, uint8_t wait)
{
  for(uint16_t i=0; i<pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}
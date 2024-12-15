/*
void sendgamemode()
{
  esp_err_t result = esp_now_send(0, (uint8_t *) &gamedata.modenum, sizeof(gamedata.modenum)); // sends just the time setting
        
  if (result == ESP_OK)
  {
    Serial.println("Sent game mode successfully");
  }
  else
  {
  Serial.println("Error sending game mode");
  }
}
*/

/*
void confirmmode()
{
  esp_err_t result = esp_now_send(0, (uint8_t *) &gamedata.confirm, sizeof(gamedata.confirm)); // sends just the time setting
        
  if (result == ESP_OK)
  {
    Serial.println("Sent game mode successfully");
  }
  else
  {
  Serial.println("Error sending game mode");
  }
}
*/

void sendall()
{
  esp_err_t result = esp_now_send(0, (uint8_t *) &gamedata, sizeof(game_struct)); // sends just the time setting
        
  if (result == ESP_OK)
  {
    Serial.println("Sent game mode successfully");
  }
  else
  {
  Serial.println("Error sending game mode");
  }
}
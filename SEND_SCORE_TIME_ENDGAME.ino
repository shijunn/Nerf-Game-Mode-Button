void sendscoretimeendgame()
{
  esp_err_t result = esp_now_send(0, (uint8_t *) &gamedata, sizeof(game_struct));
        
  if (result == ESP_OK)
  {
    Serial.println("Sent game score/time/endgame status successfully");
  }
  else
  {
  Serial.println("Failed to send game score/time/endgame status");
  }
}
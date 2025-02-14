// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  char macStr[18];
  Serial.print("Packet to: ");
  // copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery to HOST Success" : "Delivery to HOST Fail");
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&incominggamedata, incomingData, sizeof(incominggamedata));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Game time set to: ");
  Serial.println((incominggamedata.gamemodetime/60000) + String(" mins"));
  Serial.print("Game mode: ");
  Serial.println(incominggamedata.modenum);
  incominggamemodetime = incominggamedata.gamemodetime;
  incomingmodenum = incominggamedata.modenum;
  incomingconfirm = incominggamedata.confirm;
  incomingcountdowntime = incominggamedata.countdowntime/1000;
  incomingtogglescreen = incominggamedata.togglescreen;
  incomingmiddlelongpressed = incominggamedata.middlelongpressed;
  incominggoback = incominggamedata.goback;
  incomingbutton = incominggamedata.button;
}
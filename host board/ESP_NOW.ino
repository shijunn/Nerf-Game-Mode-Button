// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  #ifdef devdebug
    char macStr[18];
    Serial.print("Packet received from: ");
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.println(macStr);
  #endif

  memcpy(&incominggamedata, incomingData, sizeof(incominggamedata)); // copy incoming data from team boards onto host board and store inside a variable specifically for incoming data
  // Serial.printf("Board ID %u: %u bytes\n", incominggamedata.id, len);
  // Update the structures with the new incoming data
  boards[incominggamedata.id-1].endgame = incominggamedata.endgame;
  boards[incominggamedata.id-1].score = incominggamedata.score;
  boards[incominggamedata.id-1].testtime = incominggamedata.testtime;
  boards[incominggamedata.id-1].recordedTime = incominggamedata.recordedTime;
  // Serial.printf("endgame: %d \n", boards[incominggamedata.id-1].endgame);
  // Serial.printf("score: %d \n", boards[incominggamedata.id-1].score);
  board1_endgame = boards[0].endgame;
  board2_endgame = boards[1].endgame;
  board1_score = boards[0].score;
  board2_score = boards[1].score;
  board1_testtime = boards[0].testtime;
  board2_testtime = boards[1].testtime;
  board1_recordedTime = boards[0].recordedTime;
  board2_recordedTime = boards[1].recordedTime;
  // Serial.println();
  /*
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Endgame flag: ");
  Serial.println(incominggamedata.endgame);
  incomingendgame = incominggamedata.endgame;
  incomingscore = incominggamedata.score;
  incomingtesttime = incominggamedata.testtime;
  incomingrecordedTime = incominggamedata.recordedTime;
  */
}
#if !defined( ESP32 )
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <Preferences.h>
#include <esp_now.h>  // https://randomnerdtutorials.com/esp-now-one-to-many-esp32-esp8266/
#include <WiFi.h>

#define NUM_PIXELS 32
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3c
#define devdebug
#define RW_MODE false
#define RO_MODE true
#define SENDCOOLDOWN 5000

const int neopixelpin = 19;
const int otaButton = 4;  // OTA button
const int extButton = 5;     // external button

/*
why is enum used 
https://www.reddit.com/r/learnprogramming/comments/yk8d84/comment/iurxecv/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
*/
enum gamemodes {
  CAPTURE_THE_FLAG = 0,  // enum automatically adds the next integer
  DOMINATION,
  KING_OF_THE_HILL,
  CLICKER_DOMINATION,
  CHESS_CLOCK,
  SETTING,
  BOARD_NUMBER,
  NUM_MODES              // if game modes are added or removed down the line, enum will automatically change the integer of NUM_MODES
};

int reading = 0;
int avgReading = 0;
int lastmenu = -1;
int menu = CAPTURE_THE_FLAG;     // menu = 0
int laststate = HIGH;
int buttonstate;
int defaultbuttonstate = HIGH;
bool startedrecording = false;
bool pressed = false;
bool storedPrev = false;
bool sentbefore = false; // for cooldown
bool firstconfirm = true;

Preferences preferences;

uint8_t broadcastAddressHost[] = {0xe4, 0xb3, 0x23, 0x94, 0x3b, 0x28};
esp_now_peer_info_t peerInfo;
//https://randomnerdtutorials.com/esp-now-many-to-one-esp32/#more-96216
unsigned long startGameTime;
unsigned long startPressTime;
unsigned long lastdebouncetime = 0;
unsigned long debouncedelay = 20;
unsigned long timesetting = 0;
unsigned long longpress = 0;
unsigned long timesent = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  //declaration for SSD1306 display connected using I2C
Adafruit_NeoPixel pixels(NUM_PIXELS, neopixelpin, NEO_GRB + NEO_KHZ800);

// temporarily put here for ease of use
int score = 0;
bool endgame = false;
unsigned long checktime;
unsigned long newTime;
unsigned long prevTime;
unsigned long totalTime;

typedef struct game_struct
{
  int score;
  unsigned long gamemodetime;
  bool endgame;
  unsigned long checktime;
  unsigned long newTime;
  // unsigned long prevTime;
  // unsigned long totalTime;
  unsigned long testtime;
  unsigned long recordedTime;
  int modenum;
  bool confirm;
  unsigned long countdowntime;
  int id;
  int togglescreen;
  bool chessbuttonpress1;
  bool chessbuttonpress2;
  bool middlelongpressed;
  bool goback;
} game_struct;

game_struct gamedata;

game_struct incominggamedata; // for team or host board



// delcaring variables for storing incoming values from other boards
int incomingmodenum;
int incomingtogglescreen;
int lastincomingmodenum = 0;
bool incomingconfirm = false;
bool incomingchessbuttonpress1;
bool incomingchessbuttonpress2;
unsigned long incominggamemodetime;
unsigned long incomingcountdowntime;
bool incomingmiddlelongpressed;
bool incominggoback;




void oledDisplayCenter();
void oledDisplayInt();
void oledDisplayCountdown();
void oledDisplayStrInt();
void menudisplay();
void resetvariables();
void resetscore();
void colorWipe(uint32_t c, uint8_t wait);
void captureTheFlag();
void domination();
void kingOfTheHill();
void clickerDomination();
void chessClock();
void updatedisplay();
void sendscoretimeendgame();
void sendcooldown();



void setup() {
  Serial.begin(115200);
  Wire.begin(6, 7);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // register host as a peer
  memcpy(peerInfo.peer_addr, broadcastAddressHost, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();

  pixels.begin();
  pixels.clear();
  pixels.show();
  pixels.setBrightness(30);

  pinMode(otaButton, INPUT);
  pinMode(extButton, INPUT);

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  gamedata.score = 0;
  gamedata.gamemodetime = 0;
  gamedata.endgame = false;
  //gamedata.checktime = gamedata.gamemodetime;
  //gamedata.newTime = gamedata.gamemodetime;
  // gamedata.prevTime = 0;
  // gamedata.totalTime = 0;
  gamedata.testtime = 0;
  gamedata.recordedTime = 0;
  gamedata.id = 1; //default blue team

  // id = 1 blue team
  // id = 2 red team
  sendscoretimeendgame();
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  
}

void loop() {
  if (digitalRead(otaButton) == LOW) // change to red team
  {
    gamedata.id = 2;
    sendcooldown();
  }
  updatedisplay();
  while (incomingconfirm)
  { // only receives confirm flag when host long pressed on a game mode not setting
    if (firstconfirm)
    {
      colorWipe(pixels.Color(255, 255, 255), 5);
      pixels.clear();
      pixels.show();
      firstconfirm = false;
    }
    switch (incomingmodenum)
    {
      case CAPTURE_THE_FLAG:
        captureTheFlag();
        break;

      case DOMINATION:
        domination();
        break;

      case KING_OF_THE_HILL:
        kingOfTheHill();
        break;

      case CLICKER_DOMINATION:
        clickerDomination();
        break;

      case CHESS_CLOCK:
        chessClock();
        break;

      case SETTING:
        //while (incomingconfirm == true){
          if (incomingtogglescreen == 0)
          {
            oledDisplayStrInt("Mins: ", (incominggamemodetime / 60000));
          }
          else if (incomingtogglescreen == 1)
          {
            oledDisplayStrInt("Sec: ", (incomingcountdowntime / 1000));
          }
        //}
        break;

      case BOARD_NUMBER:
        sendscoretimeendgame();
        break;


    }
    // replace these 3 lines with resetvariables()
    gamedata.score = 0; // reset score on host board and team board
    gamedata.endgame = false; // reset endgame flag on team board and send it to host board
    sentbefore = false;
    sendscoretimeendgame();
  }
  firstconfirm = true;
  pixels.clear();
  pixels.show();
}
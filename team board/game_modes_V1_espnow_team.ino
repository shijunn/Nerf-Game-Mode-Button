#if !defined( ESP32 )
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <Preferences.h>
#include <esp_now.h>  // https://randomnerdtutorials.com/esp-now-one-to-many-esp32-esp8266/
#include <WiFi.h>

#define NUM_PIXELS 6
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3c
#define devdebug
#define WATCHDOG_TIMEOUT_S 30// Final=30sec,180=3minutes, 90=1.5miuntes, 60=1minute, 1500=25 minutes, 1140=19 minutes
#define RW_MODE false
#define RO_MODE true
#define SENDCOOLDOWN 5000

const int neopixelpin = 4;
const int joystick = 34;      // joystick
const int cancelButton = 39;  // OTA button
const int extButton = 35;     // external button
// joystick threshold is based on bottom left and top right joystick values
/*
const int LEFT_JOYSTICK_THRESHOLD_MIN = 1740;
const int LEFT_JOYSTICK_THRESHOLD_MAX = 1880;
const int RIGHT_JOYSTICK_THRESHOLD_MIN = 280;
const int RIGHT_JOYSTICK_THRESHOLD_MAX = 315;
const int MIDDLE_JOYSTICK_THRESHOLD_MIN = 2240;
const int MIDDLE_JOYSTICK_THRESHOLD_MAX = 2295;
*/

// average reading threshold
// values vary on different boards
/*
const int LEFT_JOYSTICK_THRESHOLD_MIN = 1748;
const int LEFT_JOYSTICK_THRESHOLD_MAX = 2000;
const int RIGHT_JOYSTICK_THRESHOLD_MIN = 287;
const int RIGHT_JOYSTICK_THRESHOLD_MAX = 300;
const int MIDDLE_JOYSTICK_THRESHOLD_MIN = 2240;
const int MIDDLE_JOYSTICK_THRESHOLD_MAX = 2252;
*/



/*
why is enum used 
https://www.reddit.com/r/learnprogramming/comments/yk8d84/comment/iurxecv/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
*/
enum gamemodes {
  CAPTURE_THE_FLAG = 1,  // enum automatically adds the next integer so
  DOMINATION,            // 2
  KING_OF_THE_HILL,      // 3
  CLICKER_DOMINATION,    // 4
  CHESS_CLOCK,           // 5
  SETTING,               // 6
  BOARD_NUMBER,          // 7
  NUM_MODES              // 8, if game modes are added or removed down the line, enum will automatically change the integer of NUM_MODES
};

int reading = 0;
int avgReading = 0;
int lastmenu = 0;
int menu = CAPTURE_THE_FLAG;     // menu = 1
//int modenum = CAPTURE_THE_FLAG;  // mode = 1
//int score = 0;
int laststate = HIGH;
int buttonstate;
int defaultbuttonstate = HIGH;
bool middlelongpressed = false;
// volatile bool cancel = false;
bool watchDogActive = false;
bool startedrecording = false;
bool pressed = false;
//bool endgame = false;
bool storedPrev = false;
bool sentbefore = false; // for cooldown

Preferences preferences;

uint8_t broadcastAddressHost[] = {0xa0, 0xa3, 0xb3, 0x79, 0x60, 0x98};
// uint8_t broadcastAddressBlue[] = {0xa0, 0xa3, 0xb3, 0x79, 0x60, 0x80};
// uint8_t broadcastAddressRed[] = {0xa0, 0xa3, 0xb3, 0x79, 0x60, 0xbc};
esp_now_peer_info_t peerInfo;
//https://randomnerdtutorials.com/esp-now-many-to-one-esp32/#more-96216
unsigned long startGameTime;
unsigned long startPressTime;
//unsigned long recordedTime;
// unsigned long prevTime;
// unsigned long totalTime;
unsigned long lastdebouncetime = 0;
unsigned long debouncedelay = 25;
unsigned long timesetting = 0;
// unsigned long gamemodetime = preferences.getULong("gamemodetime");
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
//unsigned long testtime = 0; is in gamescript

typedef struct game_struct
{
  int score;
  unsigned long gamemodetime;
  bool endgame;
  //unsigned long checktime;
  //unsigned long newTime;
  // unsigned long prevTime;
  // unsigned long totalTime;
  unsigned long testtime;
  unsigned long recordedTime;
  int modenum;
  bool confirm;
  int countdowntime;
  int id;
} game_struct;

game_struct gamedata;

game_struct incominggamedata; // for team or host board



// delcaring variables for storing incoming values from other boards
unsigned long incominggamemodetime;
int incomingmodenum;
bool incomingconfirm = false;

int lastincomingmodenum = 0;
int incomingcountdowntime = 0;


void oledDisplayCenter();
void oledDisplayInt();
void oledDisplayCountdown();
void oledDisplayStrInt();
void menudisplay();
void IRAM_ATTR watchDogInterrupt(void);
void IRAM_ATTR cancelMode();
//void IRAM_ATTR buttonhit();
void watchDogRefresh();
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

/*
void logger(String message)
{
  #ifdef devdebug
    //String time = String(millis());
    //String millisecs = time.substring(time.length()-3);
    //String secs = time.substring(0,time.length()-3);
    Serial.println("logger: " + message);
  #endif
}
*/



void setup() {
  Serial.begin(115200);

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

  pinMode(joystick, INPUT);
  pinMode(cancelButton, INPUT);
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
  gamedata.id = 1;

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  
}

void loop() {
  if (digitalRead(cancelButton) == LOW)
  {
    gamedata.id = 2; // default without pressing is 1
    sendcooldown();
  }
  updatedisplay();
  if (incominggamedata.confirm == true)
  { // only receives confirm flag when host long pressed on a game mode not setting
    switch (incomingmodenum)
    {
      // modenum is in int so need to use number for now
      // will make it more readable once everything works
      case 1:
        captureTheFlag();
        break;

      case 2:
        domination();
        break;

      case 3:
        kingOfTheHill();
        break;

      case 4:
        clickerDomination();
        break;

      case 5:
        chessClock();
        break;

      case 7:
        sendscoretimeendgame();
        break;


    }
    // replace these 3 lines with resetvariables()
    gamedata.score = 0; // reset score on host board and team board
    gamedata.endgame = false; // reset endgame flag on team board and send it to host board
    sentbefore = false;
    sendscoretimeendgame();
  }
  pixels.clear();
  pixels.show();
  /*if (incominggamedata.confirm == true)
  {
    switch (incomingmodenum) {

      // COMPLETED
      case CAPTURE_THE_FLAG:  // 3 mins, press to stop
        captureTheFlag();
        break;

      // COMPLETED
      case DOMINATION:
        domination();
        break;

      // COMPLETED
      case KING_OF_THE_HILL:
        kingOfTheHill();
        break;

      // COMPLETED
      case CLICKER_DOMINATION:
        clickerDomination();
        break;
      
      // COMPLETED
      case CHESS_CLOCK:
        chessClock();
        break;

      
      // ADDING PREFERENCES
      // case SETTING:
      //   setting();
      //   break;
      

      default:
        Serial.println("ERROR AT STARTING GAME SWITCH CASE");
        break;
    }
    resetvariables();
    resetscore();
  }
  */
}
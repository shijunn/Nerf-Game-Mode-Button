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
const int LEFT_JOYSTICK_THRESHOLD_MIN = 1748;
const int LEFT_JOYSTICK_THRESHOLD_MAX = 2000;
const int RIGHT_JOYSTICK_THRESHOLD_MIN = 285;
const int RIGHT_JOYSTICK_THRESHOLD_MAX = 300;
const int MIDDLE_JOYSTICK_THRESHOLD_MIN = 2240;
const int MIDDLE_JOYSTICK_THRESHOLD_MAX = 2252;




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
// int score = 0;
int laststate = HIGH;
int buttonstate;
int defaultbuttonstate = HIGH;
bool middlelongpressed = false;
// volatile bool cancel = false;
bool watchDogActive = false;
bool startedrecording = false;
bool pressed = false;
// bool endgame = false;
bool storedPrev = false;

Preferences preferences;

uint8_t broadcastAddressBlue[] = {0xa0, 0xa3, 0xb3, 0x79, 0x60, 0x80};
uint8_t broadcastAddressRed[] = {0xa0, 0xa3, 0xb3, 0x79, 0x60, 0xbc};
esp_now_peer_info_t peerInfo;

unsigned long pressedTime;
unsigned long longpressduration;
unsigned long startGameTime;
unsigned long startPressTime;
//unsigned long recordedTime;
//unsigned long prevTime;
//unsigned long totalTime;
unsigned long lastdebouncetime = 0;
unsigned long debouncedelay = 25;
unsigned long timesetting = 0;
// unsigned long gamemodetime = preferences.getULong("gamemodetime");
//unsigned long gamemodetime = preferences.isKey("gamemodetime");
unsigned long longpress = 0;
uint32_t sendmodetime;

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

game_struct board1;
game_struct board2;

game_struct boards[2] = {board1, board2};

// bool incomingendgame = false;
// int incomingscore = 0;
// unsigned long incomingtesttime = 0;
// unsigned long incomingrecordedTime = 0;

// delcaring variables for storing incoming values from other boards
// unsigned long incominggamemodetime;

// was wondering why all the functions like checkendgame() did not go into the if else conditions even though endgame is set to true on the team board, forgot that im supposed to assign the values in the callback function for it to actually save somewhere
bool board1_endgame;
bool board2_endgame;
int board1_score;
int board2_score;
unsigned long board1_testtime;
unsigned long board2_testtime;
unsigned long board1_recordedTime;
unsigned long board2_recordedTime;

/*
bool board1_endgame = boards[0].endgame;
bool board2_endgame = boards[1].endgame;
int board1_score = boards[0].score;
int board2_score = boards[1].score;
unsigned long board1_testtime = boards[0].testtime;
unsigned long board2_testtime = boards[1].testtime;
unsigned long board1_recordedTime = boards[0].recordedTime;
unsigned long board2_recordedTime = boards[1].recordedTime;
*/




void readstick();
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
/*
void captureTheFlag();
void domination();
void kingOfTheHill();
void clickerDomination();
void chessClock();
*/
void setting();
void checkendgame();
void checkscore();
//void checkprogress();

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

void sendgamemode();
void sendall();

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
  // register blue team as a peer
  memcpy(peerInfo.peer_addr, broadcastAddressBlue, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  //register red team as a peer
  memcpy(peerInfo.peer_addr, broadcastAddressRed, 6);
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

  // attachInterrupt(cancelButton, cancelMode, FALLING);

  preferences.begin("settings", RW_MODE);
  //preferences.getULong("gamemodetime", gamemodetime);
  // if (gamemodetime == 0)
  // {
  //   preferences.putULong("gamemodetime", 60000);
  // }


  // setting defaults for variables to be sent to team boards
  gamedata.score = 0;
  gamedata.gamemodetime = 0;
  // gamedata.endgame = false;
  //gamedata.checktime = gamedata.gamemodetime;
  //gamedata.newTime = gamedata.gamemodetime;
  // gamedata.prevTime = 0;
  // gamedata.totalTime = 0;
  // gamedata.testtime = 0;
  gamedata.modenum = CAPTURE_THE_FLAG;
  gamedata.confirm = false;
  gamedata.endgame = false;
  gamedata.countdowntime = 0;

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  sendall();
}

void loop() {
  readstick();
  menudisplay(); // update display on team board with gamedata.modenum

  if (middlelongpressed) {

    display.clearDisplay();
    display.display();

    /*
    GAME MODES
    1. capture the flag     -      3 mins, press to stop
    2. domination           -      3 mins, least number of clicks wins, start from 0
    3. king of the hill     -      5 mins, press and hold, start from 0
    4. clicker domination   -      3 mins, start from 0, most clicks wins
    5. chess clock          -      start from time limit, press once, timer goes down

    countdown for a few seconds before starting
    */
    
    //confirmmode();
    switch (gamedata.modenum) {
      
      // COMPLETED
      case CAPTURE_THE_FLAG:  // 3 mins, press to stop
        //captureTheFlag();
        gamedata.confirm = true;
        gamedata.countdowntime = 6; // can adjust in settings later
        sendall(); // send a bool to team board to start countdown
        oledDisplayCountdown(gamedata.countdowntime); // countdown on host screen before checking whether the team board set endgame flag to true
        checkendgame(); // recieves updates from teams and displays scores on oled
        break;

      // COMPLETED
      case DOMINATION:
        //domination();
        gamedata.confirm = true;
        gamedata.countdowntime = 6;
        sendall();
        oledDisplayCountdown(gamedata.countdowntime);
        checkscore(); // receives updates from teams and displays time on oled
        break;


      case KING_OF_THE_HILL:
        //kingOfTheHill();
        gamedata.confirm = true;
        gamedata.countdowntime = 6;
        sendall();
        oledDisplayCountdown(gamedata.countdowntime);
        //checkprogress();
        break;


      case CLICKER_DOMINATION:
        //clickerDomination();
        gamedata.confirm = true;
        sendall();
        //checkscore();
        break;
      

      case CHESS_CLOCK:
        //chessClock();
        gamedata.confirm = true;
        sendall();
        //checkprogress();
        break;

      // ADDING PREFERENCES
      case SETTING:
        setting();
        break;

      case BOARD_NUMBER:
        gamedata.confirm = true;
        sendall();
        break;

      default:
        Serial.println("ERROR AT STARTING GAME SWITCH CASE");
        break;
    }
    resetvariables();
    resetscore();
  }
}
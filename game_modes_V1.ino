#if !defined( ESP32 )
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <Preferences.h>

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
const int RIGHT_JOYSTICK_THRESHOLD_MIN = 287;
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
  NUM_MODES              // 7, if game modes are added or removed down the line, enum will automatically change the integer of NUM_MODES
};

int reading = 0;
int avgReading = 0;
int lastmenu = 0;
int menu = CAPTURE_THE_FLAG;     // menu = 1
int modenum = CAPTURE_THE_FLAG;  // mode = 1
int score = 0;
int laststate = HIGH;
int buttonstate;
int defaultbuttonstate = HIGH;
bool middlelongpressed = false;
// volatile bool cancel = false;
bool watchDogActive = false;
bool startedrecording = false;
bool pressed = false;
bool endgame = false;
bool storedPrev = false;

Preferences preferences;

unsigned long pressedTime;
unsigned long longpressduration;
unsigned long startGameTime;
unsigned long startPressTime;
unsigned long recordedTime;
unsigned long prevTime;
unsigned long totalTime;
unsigned long lastdebouncetime = 0;
unsigned long debouncedelay = 25;
unsigned long timesetting = 0;
// unsigned long gamemodetime = preferences.getULong("gamemodetime");
unsigned long gamemodetime = preferences.isKey("gamemodetime");
unsigned long longpress = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  //declaration for SSD1306 display connected using I2C
Adafruit_NeoPixel pixels(NUM_PIXELS, neopixelpin, NEO_GRB + NEO_KHZ800);

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
void captureTheFlag();
void domination();
void kingOfTheHill();
void clickerDomination();
void chessClock();
void setting();

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
  preferences.getULong("gamemodetime", gamemodetime);
  if (gamemodetime == 0)
  {
    preferences.putULong("gamemodetime", 60000);
  }
  
}

void loop() {
  readstick();
  menudisplay();

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

    switch (modenum) {

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
      case SETTING:
        setting();
        break;

      default:
        Serial.println("ERROR AT STARTING GAME SWITCH CASE");
        break;
    }
    resetvariables();
    resetscore();
  }
}
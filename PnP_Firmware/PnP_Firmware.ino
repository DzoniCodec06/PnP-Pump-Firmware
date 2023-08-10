#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define RST_PIN -1

#define BUTTON_PRESS_DELAY 250

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RST_PIN);

const int button_down = 5;
const int button_up = 4;
const int button_select = 3;

int down;
int up;
int select;

int screen = 1;

bool main_menu = true;
bool suck_menu = false;
bool power_menu = false;


void bootMenu() {
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.setCursor(20, 0);
  oled.print("WELCOME");
  oled.setCursor(10, 20);
  oled.print("PnP Pump");
  oled.display();
  delay(2000);
  oled.clearDisplay();
  oled.display();
}

void suckMenu(int sc) {
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(40, 0);
  oled.print("Suck");
  oled.setTextSize(2);
  oled.setCursor(20, 30);
  oled.print("Suck:");
  
  switch (sc) {
    case 1:
      oled.setCursor(80, 30);
      oled.print("Yes");
      break;
    case 2:
      oled.setCursor(80, 30);
      oled.print("No");
      break;
    default:
      screen = 2;
  }

  oled.display();
}

void menuSelect(int sc) {
  switch (sc) {
    case 1:
      Serial.println("Suck Menu");
      main_menu = false;
      suck_menu = true;
      suckMenu(screen);
      break;
    case 2:
      Serial.println("Blow Menu");
      break;
    case 3:
      Serial.println("Power Menu");
      break;
  }
}

void mainMenu(int sc) {
  oled.clearDisplay();

  oled.setCursor(10, 0);
  oled.setTextSize(2);
  oled.print("Main Menu");

  switch (sc) {
    case 1:
      oled.setTextSize(1);
      oled.setCursor(30, 30);
      oled.print("-> ");
      break;
    case 2:
      oled.setTextSize(1);
      oled.setCursor(30, 40);
      oled.print("-> ");
      break;
    case 3:
      oled.setTextSize(1);
      oled.setCursor(30, 50);
      oled.print("-> ");
      break;
  }

  oled.setCursor(50, 30);
  oled.print("Suck");

  oled.setCursor(50, 40);
  oled.print("Blow");

  oled.setCursor(50, 50);
  oled.print("Power");

  oled.display();
}

void setup() {
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) for (;;);
  Serial.begin(9600);
  bootMenu();
  pinMode(button_down, INPUT_PULLUP);
  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_select, INPUT_PULLUP);
}

void loop() {
  down = digitalRead(button_down);
  up = digitalRead(button_up);
  select = digitalRead(button_select);

  if (main_menu) mainMenu(screen);
  else if (suck_menu) suckMenu(screen);

  if (down == LOW) {
    if (screen > 1) screen--;
    delay(BUTTON_PRESS_DELAY);
  } else if (up == LOW) {
    if (screen < 3) screen++;
    delay(BUTTON_PRESS_DELAY);
  } else if (select == LOW) {
    menuSelect(screen);
    delay(BUTTON_PRESS_DELAY);
  }
}

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define RST_PIN -1

#define BUTTON_PRESS_DELAY 250

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RST_PIN);

const int button_down = 3;
const int button_up = 5;
const int button_select = 4;

int down;
int up;
int select;

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

void goDown() {
  Serial.println("Down");
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("Down");
  oled.display();
}

void goUp() {
  Serial.println("Up");
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("Up");
  oled.display();
}

void menuSelect() {
  Serial.println("Select");
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("Select");
  oled.display();
}

void mainMenu() {
  oled.clearDisplay();
  oled.setCursor(10, 0);
  oled.setTextSize(2);
  oled.print("Main Menu");
  
  oled.setTextSize(1);
  
  oled.setCursor(30, 30);
  oled.print("-> Suck");

  oled.setCursor(30, 40);
  oled.print("-> Blow");

  oled.setCursor(30, 50);
  oled.print("-> Power");
  
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

  if (down == LOW) {
    goDown();
    delay(BUTTON_PRESS_DELAY);
  } else if (up == LOW) {
    goUp();
    delay(BUTTON_PRESS_DELAY);
  } else if (select == LOW) {
    menuSelect();
    delay(BUTTON_PRESS_DELAY);
  }

  mainMenu();
}

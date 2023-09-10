#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define RST_PIN -1

#define BUTTON_PRESS_DELAY 250

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RST_PIN);

const int button_down = 4;
const int button_up = 5;
const int button_select = 3;
const int pump = 6; //Vacuum pump, activates when HIGH is applied!
const int pedal = 7; //Foot pedal, input pullup mode!
const int led_pin = 2; //ws2812b 5050 led diodes 

int down;
int up;
int select;
int pedalval;

int pumpvalue;

int screen = 1;

bool main_menu = true;
bool suck_menu = false;
bool blow_menu = false;
bool power_menu = false;

bool suck = false;
bool blow = false;

int power_level = 1;

const int SHORT_PRESS_TIME = 500; // 500 milliseconds

int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin

unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;

bool start = true;

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
      oled.print("On");
      break;
    case 2:
      oled.setCursor(80, 30);
      oled.print("Off");
      break;
    default:
      screen = 2;
  }

  oled.display();
}

void blowMenu(int sc) {
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(40, 0);
  oled.print("Blow");
  oled.setTextSize(2);
  oled.setCursor(20, 30);
  oled.print("Blow:");

  switch (sc) {
    case 1:
      oled.setCursor(80, 30);
      oled.print("On");
      break;
    case 2:
      oled.setCursor(80, 30);
      oled.print("Off");
      break;
    default:
      screen = 2;
  }

  oled.display();
}

void powerMenu(int pwr) {
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(40, 0);
  oled.print("Power");
  oled.setTextSize(2);
  oled.setCursor(1, 30);
  oled.print("-");
  oled.setCursor(113, 30);
  oled.print("+");

  switch (pwr) {
    case 1:
      oled.drawRect(15, 30, 15, 15, WHITE);
      oled.drawRect(35, 30, 15, 15, WHITE);
      oled.drawRect(55, 30, 15, 15, WHITE);
      oled.drawRect(75, 30, 15, 15, WHITE);
      oled.drawRect(95, 30, 15, 15, WHITE);
      analogWrite(pump, 0);
      break;
    case 2:
      oled.fillRect(15, 30, 15, 15, WHITE);
      oled.drawRect(35, 30, 15, 15, WHITE);
      oled.drawRect(55, 30, 15, 15, WHITE);
      oled.drawRect(75, 30, 15, 15, WHITE);
      oled.drawRect(95, 30, 15, 15, WHITE);
      if (digitalRead(pedal) == LOW) {
        analogWrite(pump, 80);
        return;
      }
      else {
        analogWrite(pump, 0);
      }
      break;
    case 3:
      oled.fillRect(15, 30, 15, 15, WHITE);
      oled.fillRect(35, 30, 15, 15, WHITE);
      oled.drawRect(55, 30, 15, 15, WHITE);
      oled.drawRect(75, 30, 15, 15, WHITE);
      oled.drawRect(95, 30, 15, 15, WHITE);
      if (digitalRead(pedal) == LOW) {
        analogWrite(pump, 135);
        return;
      }
      else {
        analogWrite(pump, 0);
      }
      break;
    case 4:
      oled.fillRect(15, 30, 15, 15, WHITE);
      oled.fillRect(35, 30, 15, 15, WHITE);
      oled.fillRect(55, 30, 15, 15, WHITE);
      oled.drawRect(75, 30, 15, 15, WHITE);
      oled.drawRect(95, 30, 15, 15, WHITE);
      if (digitalRead(pedal) == LOW) {
        analogWrite(pump, 175);
        return;
      }
      else {
        analogWrite(pump, 0);
      }
      break;
    case 5:
      oled.fillRect(15, 30, 15, 15, WHITE);
      oled.fillRect(35, 30, 15, 15, WHITE);
      oled.fillRect(55, 30, 15, 15, WHITE);
      oled.fillRect(75, 30, 15, 15, WHITE);
      oled.drawRect(95, 30, 15, 15, WHITE);
      if (digitalRead(pedal) == LOW) {
        analogWrite(pump, 225);
        return;
      }
      else {
        analogWrite(pump, 0);
      }
      break;
    case 6:
      oled.fillRect(15, 30, 15, 15, WHITE);
      oled.fillRect(35, 30, 15, 15, WHITE);
      oled.fillRect(55, 30, 15, 15, WHITE);
      oled.fillRect(75, 30, 15, 15, WHITE);
      oled.fillRect(95, 30, 15, 15, WHITE);
      if (digitalRead(pedal) == LOW) {
        analogWrite(pump, 255);
        return;
      }
      else {
        analogWrite(pump, 0);
      }
      break;
  }

  oled.display();
}

void menuSelect(int sc) {
  if (main_menu) {
    switch (sc) {
      case 1:
        Serial.println("Suck Menu");
        main_menu = false;
        suck_menu = true;
        suckMenu(screen);
        break;
      case 2:
        Serial.println("Blow Menu");
        main_menu = false;
        blow_menu = true;
        blowMenu(screen);
        break;
      case 3:
        Serial.println("Power Menu");
        main_menu = false;
        power_menu = true;
        powerMenu(screen);
        break;
    }
  } else if (suck_menu) {
    switch (sc) {
      case 1:
        Serial.println("On");
        suck = true;
        suck_menu = false;
        main_menu = true;
        mainMenu(screen);
        digitalWrite(pump, HIGH);
        break;
      case 2:
        Serial.println("Off");
        suck = false;
        suck_menu = false;
        main_menu = true;
        mainMenu(screen);
        digitalWrite(pump, LOW);
        break;
    }
  } else if (blow_menu) {
    switch (sc) {
      case 1:
        Serial.println("On");
        blow = true;
        blow_menu = false;
        main_menu = true;
        mainMenu(screen);
        break;
      case 2:
        Serial.println("Off");
        blow = false;
        blow_menu = false;
        main_menu = true;
        mainMenu(screen);
        break;
    }
  } else if (power_menu) {
    Serial.println("Power: ");
    Serial.print(power_level);
    power_menu = false;
    main_menu = true;
    mainMenu(screen);
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

void powerOff(int sc) {
  if (start) {
    start = false;
    return;
  } else {
    oled.clearDisplay();
    oled.setCursor(20, 20);
    oled.setTextSize(2);
    oled.print("goodbye");
    oled.display();
    delay(3000);
    oled.clearDisplay();
    oled.display();
    main_menu = false;
  }
}

void setup() {
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) for (;;);
  Serial.begin(9600);
  bootMenu();
  pinMode(button_down, INPUT_PULLUP);
  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_select, INPUT_PULLUP);
  pinMode(pedal, INPUT_PULLUP);
  pinMode(pump, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  down = digitalRead(button_down);
  up = digitalRead(button_up);
  select = digitalRead(button_select);
  pedalval = digitalRead(pedal);

  if (main_menu) mainMenu(screen);
  else if (suck_menu) suckMenu(screen);
  else if (blow_menu) blowMenu(screen);
  else if (power_menu) powerMenu(power_level);

  if (down == LOW) {
    if (screen > 1) screen--;
    if (power_level > 1 && power_menu) power_level--;
    delay(BUTTON_PRESS_DELAY);
  } else if (up == LOW) {
    if (screen < 3 && !power_menu) screen++;
    if (power_level < 6 && power_menu) power_level++;
    delay(BUTTON_PRESS_DELAY);
  } /*else if (select == LOW) {
    menuSelect(screen);
    delay(BUTTON_PRESS_DELAY);
  } */

  if (lastState == HIGH && select == LOW) pressedTime = millis();

  else if (lastState == LOW && select == HIGH) {                   // button is released
    releasedTime = millis();
    long pressDuration = releasedTime - pressedTime;

    if (pressDuration < SHORT_PRESS_TIME) menuSelect(screen);
    else powerOff(screen);
  }

  if (suck) digitalWrite(13, HIGH);
  else if (!suck) digitalWrite(13, LOW);

  lastState = select;
}

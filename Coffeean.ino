#include "TimerThree.h"
#include <SPI.h>
#include <Wire.h>
/* #include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> */
#include <OneWire.h>
#include <EEPROM.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* PINS - output (5 TOTAL) */
#define PIN_RELAY_PUMP 2
#define PIN_RELAY_VALVE 3

#define PIN_SSR_HEAT 5
#define PIN_SSR_GND 6
//#define PIN_SPEAKER 7
//#define PIN_LED 7

/* PINS - input (7 TOTAL) */
#define PIN_BUTTON_MODE 50
#define PIN_BUTTON_SET 48
#define PIN_BUTTON_UP 52
#define PIN_BUTTON_DOWN 46

#define PIN_SWITCH_BREW 42
#define PIN_SWITCH_STEAM 38
#define PIN_SWITCH_WATER 34
#define PIN_SWITCH_GND 30


#define PIN_DISPLAY_GND 19


// 7 INPUTS + 5 OUTPUTS + VCC + GND + 2x PT1000 = 16 WIRES

/* DISPLAY MODES */
#define DMODE_HOME 0
#define DMODE_COFFEE_TEMP 1
#define DMODE_STEAM_TEMP 2
// #define DMODE_BACKFLUSH 3
#define DMODE_BTN_DEBUG 3
int dModeCount = 4;

int dModeCurrent = DMODE_HOME;
int dModePrev = DMODE_HOME;

/* WORK MODES */
#define WMODE_SLEEP 0
#define WMODE_COFFEE 1
#define WMODE_STEAM 2
#define WMODE_BACKFLUSH 3
#define WMODE_BREWING 4
#define WMODE_BREWING_FINISHED 5
int wModeCurrent = WMODE_COFFEE;
int wModePrev = WMODE_COFFEE;

/* display */
// #define OLED_RESET 5 //
// Adafruit_SSD1306 display(OLED_RESET); //128x64, yellow 16 rows

/* VARIABLES */
double currentTemperature = 100;
double coffeeTemperature = 100;
double steamTemperature = 140;

#define T_MAX 180
#define T_MIN 15

/* SETUP */
long lastTemperatureMeasurement = 0;
long temperatureMeasurementInterval = 1000;
double rawTemperature = 0;

const int tempArraySize = 6;
double tempArray [tempArraySize];
int tempArrayPointer = 0;
double tempArrayAvg = 0;
double tempArraySD = 0;

long brewStartTime = 0;
long brewDuration = 2 * 1000;


long lastLcdRefresh = 0;

void setup() {
  pinMode(PIN_RELAY_PUMP, OUTPUT);
  pinMode(PIN_RELAY_VALVE, OUTPUT);

  //buttons
  pinMode(PIN_BUTTON_MODE, INPUT);
  pinMode(PIN_BUTTON_SET, INPUT);
  pinMode(PIN_BUTTON_UP, INPUT);
  pinMode(PIN_BUTTON_DOWN, INPUT);

  pinMode(PIN_SWITCH_STEAM, INPUT);
  pinMode(PIN_SWITCH_BREW, INPUT);
  pinMode(PIN_SWITCH_WATER, INPUT);

  //display
  pinMode(PIN_DISPLAY_GND, OUTPUT);
  digitalWrite(PIN_DISPLAY_GND, LOW);

  pinMode(PIN_SSR_GND, OUTPUT);
  digitalWrite(PIN_SSR_GND, LOW);

  pinMode(PIN_SWITCH_GND, OUTPUT);
  digitalWrite(PIN_SWITCH_GND, LOW);



  digitalWrite(PIN_BUTTON_MODE, HIGH);
  digitalWrite(PIN_BUTTON_SET, HIGH);
  digitalWrite(PIN_BUTTON_UP, HIGH);
  digitalWrite(PIN_BUTTON_DOWN, HIGH);
  digitalWrite(PIN_SWITCH_STEAM, HIGH);
  digitalWrite(PIN_SWITCH_BREW, HIGH);
  digitalWrite(PIN_SWITCH_WATER, HIGH);

  //turn off relays
  digitalWrite(PIN_RELAY_PUMP, HIGH);
  digitalWrite(PIN_RELAY_VALVE, HIGH);
  /*
    pinMode(PIN_SSR_HEAT, OUTPUT);
    digitalWrite(PIN_SSR_HEAT, HIGH);
    */
  Timer3.initialize(500000);

  Serial.begin(9600);

  //  InitDisplay();

  //SetDS3231time(0, 10, 14, 6, 7, 2, 2016);

  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.print("COFFEAN PID!");
  lcd.clear();

  lastLcdRefresh = millis();
}

const int backFlushCycles = 3;
int backFlushCurrentCycle = 0;
const int backFlushCycleDuration = 1 * 3000;
long backFlushCurrentCycleStart = 0;


void TurnOffRelay(int pin) {
  digitalWrite(pin, HIGH);
}

void TurnOnRelay(int pin) {
  digitalWrite(pin, LOW);
}

void ManageState() {

  if (wModeCurrent == WMODE_COFFEE) {
    TurnOffRelay(PIN_RELAY_PUMP);
    TurnOffRelay(PIN_RELAY_VALVE);

    if (IsSwitchOn(PIN_SWITCH_STEAM)) {
      wModeCurrent = WMODE_STEAM;
    }
    if (IsSwitchOn(PIN_SWITCH_BREW)) {
      wModeCurrent = WMODE_BREWING;
      brewStartTime = millis();
    }
  }

  if (wModeCurrent == WMODE_STEAM) {
    TurnOffRelay(PIN_RELAY_PUMP);
    TurnOffRelay(PIN_RELAY_VALVE);

    if (!IsSwitchOn(PIN_SWITCH_STEAM)) {
      wModeCurrent = WMODE_COFFEE;
    }
  }

  if (wModeCurrent == WMODE_BREWING) {
    TurnOnRelay(PIN_RELAY_PUMP);
    TurnOnRelay(PIN_RELAY_VALVE);

    if (!IsSwitchOn(PIN_SWITCH_BREW)) {
      wModeCurrent = WMODE_COFFEE;
    }

    if (brewStartTime + brewDuration < millis()) {
      wModeCurrent = WMODE_BREWING_FINISHED;
    }
  }

  if (wModeCurrent == WMODE_BACKFLUSH) {
    if (backFlushCycleDuration + backFlushCurrentCycleStart < millis()) {
      backFlushCurrentCycle++;
      backFlushCurrentCycleStart = millis();
    }

    if (backFlushCurrentCycle % 2 == 0) {
      TurnOnRelay(PIN_RELAY_PUMP);
      TurnOnRelay(PIN_RELAY_VALVE);
    } else {
      TurnOffRelay(PIN_RELAY_PUMP);
      TurnOffRelay(PIN_RELAY_VALVE);
    }
    if (IsButtonPushed(PIN_BUTTON_SET) || backFlushCurrentCycle >= backFlushCycles * 2 - 1) {
      wModeCurrent = WMODE_COFFEE;
      backFlushCurrentCycle = 0;
    }
  }

  if (wModeCurrent == WMODE_BREWING_FINISHED) {
    TurnOffRelay(PIN_RELAY_PUMP);
    TurnOffRelay(PIN_RELAY_VALVE);

    if (!IsSwitchOn(PIN_SWITCH_BREW)) {
      wModeCurrent = WMODE_COFFEE;
    }
  }
}



// the loop function runs over and over again forever
void loop() {
  SetHeater(currentTemperature);

  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  ReadDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  /*
  if (second % 2 == 0) {
      digitalWrite(PIN_SSR_HEAT, HIGH);
  } else {
    digitalWrite(PIN_SSR_HEAT, LOW);
  }
  */
  ProcessButtons();

  RenderDisplay();

  ManageState();
  ReadTemperature();
  /*
    if (millis() / 1000 % 2 == 0) {
      lcd.backlight();
    } else {
      lcd.noBacklight();
    }
    */
}

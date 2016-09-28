#include "TimerThree.h"
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <EEPROM.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* PINS - output (5 TOTAL) */
#define PIN_RELAY_PUMP 6
#define PIN_RELAY_VALVE 7

#define PIN_SSR_HEAT 5
//#define PIN_SSR_GND 6
//#define PIN_SPEAKER 7
//#define PIN_LED 7

/* PINS - input (7 TOTAL) */
#define PIN_BUTTON_MODE 50
#define PIN_BUTTON_SET 48
#define PIN_BUTTON_UP 52
#define PIN_BUTTON_DOWN 46

#define PIN_SWITCH_BREW 38
#define PIN_SWITCH_STEAM 34
#define PIN_SWITCH_WATER 42
#define PIN_SWITCH_GND 30

#define HMODE_CONST 0
#define HMODE_FULL_HEAT 0
#define HMODE_COOLING 0
#define HMODE_HEATING 0
#define HMODE_EMERGENCY 0
#define HMODE_MAX_COOLING 0

#define PIN_DISPLAY_GND 19


// 7 INPUTS + 5 OUTPUTS + VCC + GND + 2x PT1000 = 16 WIRES

/* DISPLAY MODES */
#define DMODE_HOME 0
#define DMODE_COFFEE_TEMP 1
#define DMODE_STEAM_TEMP 2
#define DMODE_BTN_DEBUG 3
#define DMODE_TMP_DEBUG 4
#define DMODE_BACKFLUSH 5

int dModeCount = 6;

int dModeCurrent = DMODE_TMP_DEBUG;
int dModePrev = DMODE_TMP_DEBUG;

/* WORK MODES */
#define WMODE_SLEEP 0
#define WMODE_COFFEE 1
#define WMODE_STEAM 2
#define WMODE_BACKFLUSH 3
#define WMODE_BREWING 4
#define WMODE_BREWING_FINISHED 5
int wModeCurrent = WMODE_COFFEE;
int wModePrev = WMODE_COFFEE;

/* VARIABLES */
double currentTemperature = 100;
double coffeeTemperature = 100;
double steamTemperature = 140;
double targetTemperature = 0;

int hModeCurrent = 0;
int pwmInterval = 0;

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
int brewDuration = 30000;

void setup() {
  //relays
  pinMode(PIN_RELAY_PUMP, OUTPUT);
  pinMode(PIN_RELAY_VALVE, OUTPUT);
  Timer3.initialize(500000);

  //buttons
  pinMode(PIN_BUTTON_MODE, INPUT);
  pinMode(PIN_BUTTON_SET, INPUT);
  pinMode(PIN_BUTTON_UP, INPUT);
  pinMode(PIN_BUTTON_DOWN, INPUT);
  digitalWrite(PIN_BUTTON_MODE, HIGH);
  digitalWrite(PIN_BUTTON_SET, HIGH);
  digitalWrite(PIN_BUTTON_UP, HIGH);
  digitalWrite(PIN_BUTTON_DOWN, HIGH);

  //switches
  pinMode(PIN_SWITCH_STEAM, INPUT);
  pinMode(PIN_SWITCH_BREW, INPUT);
  pinMode(PIN_SWITCH_WATER, INPUT);
  pinMode(PIN_SWITCH_GND, OUTPUT);
  digitalWrite(PIN_SWITCH_STEAM, HIGH);
  digitalWrite(PIN_SWITCH_BREW, HIGH);
  digitalWrite(PIN_SWITCH_WATER, HIGH);
  digitalWrite(PIN_SWITCH_GND, LOW);

  //display
  pinMode(PIN_DISPLAY_GND, OUTPUT);
  digitalWrite(PIN_DISPLAY_GND, LOW);

  //turn off relays
  digitalWrite(PIN_RELAY_PUMP, LOW);
  digitalWrite(PIN_RELAY_VALVE, LOW);

  //display
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.print("COFFEAN PID!");
  lcd.clear();

  //first time setup
  //SetDS3231time(0, 10, 14, 6, 7, 2, 2016);

  //serial init
  Serial.begin(9600);
}

const int backFlushCycles = 3;
int backFlushCurrentCycle = 0;
const int backFlushCycleDuration = 10000;
long backFlushCurrentCycleStart = 0;


void ManageState() {

  if (wModeCurrent == WMODE_COFFEE) {
    targetTemperature = coffeeTemperature;
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
    targetTemperature = steamTemperature;
    TurnOffRelay(PIN_RELAY_PUMP);
    TurnOffRelay(PIN_RELAY_VALVE);

    if (!IsSwitchOn(PIN_SWITCH_STEAM)) {
      wModeCurrent = WMODE_COFFEE;
    }
  }

  if (wModeCurrent == WMODE_BREWING) {
    targetTemperature = coffeeTemperature;
    TurnOnRelay(PIN_RELAY_PUMP);
    TurnOnRelay(PIN_RELAY_VALVE);

    if (!IsSwitchOn(PIN_SWITCH_BREW)) {
      wModeCurrent = WMODE_COFFEE;
    }

    if (brewStartTime + brewDuration < millis()) {
      wModeCurrent = WMODE_BREWING_FINISHED;
      lcd.clear();
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
    targetTemperature = coffeeTemperature;
    TurnOffRelay(PIN_RELAY_PUMP);
    TurnOffRelay(PIN_RELAY_VALVE);

    if (!IsSwitchOn(PIN_SWITCH_BREW)) {
      wModeCurrent = WMODE_COFFEE;
    }
  }
}

// the loop function runs over and over again forever
void loop() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  ReadDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  ProcessButtons();
  RenderDisplay();
  ManageState();
  ReadTemperature();
  SetHeater(currentTemperature);
}

bool IsButtonPushed(int pin) {
  if (digitalRead(pin) == LOW) {
    while (digitalRead(pin) == LOW) {}
    return true;
  }
  return false;
}

bool IsSwitchOn(int pin) {
  if (digitalRead(pin) == LOW) {
    //while (digitalRead(pin) == LOW) {}
    return true;
  }
  return false;
}

void ProcessButtons() {

  if (IsButtonPushed(PIN_BUTTON_MODE)) {
    dModeCurrent = ++dModeCurrent % dModeCount;
    lcd.clear();
  }

  if (dModeCurrent == DMODE_BACKFLUSH) {
    if (IsButtonPushed(PIN_BUTTON_SET)) {
      wModeCurrent = WMODE_BACKFLUSH;
      backFlushCurrentCycle = 0;
      backFlushCurrentCycleStart = millis();
    }
  }

  if (dModeCurrent == DMODE_COFFEE_TEMP) {
    if (IsButtonPushed(PIN_BUTTON_UP)) {
      coffeeTemperature++;
      lcd.clear();
    }
    if (IsButtonPushed(PIN_BUTTON_DOWN)) {
      coffeeTemperature--;
      lcd.clear();
    }
    if (IsButtonPushed(PIN_BUTTON_SET)) {
      coffeeTemperature = 100;
      lcd.clear();
    }
  }

  if (dModeCurrent == DMODE_STEAM_TEMP) {
    if (IsButtonPushed(PIN_BUTTON_UP)) {
      steamTemperature++;
      lcd.clear();
    }
    if (IsButtonPushed(PIN_BUTTON_DOWN)) {
      steamTemperature--;
      lcd.clear();
    }
    if (IsButtonPushed(PIN_BUTTON_SET)) {
      steamTemperature = 140;
      lcd.clear();
    }
  }
}

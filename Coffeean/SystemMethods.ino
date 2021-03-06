bool IsButtonPushed(int pin) {
  if (digitalRead(pin) == LOW) {
    while (digitalRead(pin) == LOW) {}
    lastKeyHitTime = millis();
    return true;
  }
  return false;
}

bool IsSwitchOn(int pin) {
  if (digitalRead(pin) == LOW) {
    //while (digitalRead(pin) == LOW) {}
    lastKeyHitTime = millis();
    return true;
  }
  return false;
}

void LoadEEPROMData() {
  coffeeTemperature = EEPROM.read(EEPROM_COFFEE);
  if (coffeeTemperature < 50 || coffeeTemperature > 150) {
    coffeeTemperature = 95;
  }
  steamTemperature = EEPROM.read(EEPROM_STEAM);
  if (steamTemperature < 50 || steamTemperature > 170) {
    steamTemperature = 140;
  }
}

void SaveEEPROMData(int address, int value) {
  EEPROM.write(address, value);
}

void ManageState() {
  if (wModeCurrent == WMODE_COFFEE) {
    targetTemperature = coffeeTemperature;
    TurnOffRelay(PIN_SSR_PUMP);
    TurnOffRelay(PIN_SSR_VALVE);

    if (IsSwitchOn(PIN_SWITCH_STEAM)) {
      wModeCurrent = WMODE_STEAM;
      lcd.clear();
      lastDisplayRefresh = 0;
    }
    if (IsSwitchOn(PIN_SWITCH_BREW)) {
      wModeCurrent = WMODE_BREWING;
      lcd.clear();
      brewStartTime = millis();
      lastDisplayRefresh = 0;
    }
    if (IsSwitchOn(PIN_SWITCH_WATER)) {
      wModeCurrent = WMODE_WATER;
      lcd.clear();
      lastDisplayRefresh = 0;
    }
  }

  if (wModeCurrent == WMODE_WATER) {
    targetTemperature = coffeeTemperature;
    TurnOnRelay(PIN_SSR_PUMP);
    TurnOffRelay(PIN_SSR_VALVE);

    if (!IsSwitchOn(PIN_SWITCH_WATER)) {
      wModeCurrent = WMODE_COFFEE;
    }
  }

  if (wModeCurrent == WMODE_STEAM) {
    targetTemperature = steamTemperature;
    TurnOffRelay(PIN_SSR_PUMP);
    TurnOffRelay(PIN_SSR_VALVE);

    if (!IsSwitchOn(PIN_SWITCH_STEAM)) {
      wModeCurrent = WMODE_COFFEE;
    }
  }

  if (wModeCurrent == WMODE_BREWING) {
    targetTemperature = coffeeTemperature;
    TurnOnRelay(PIN_SSR_PUMP);
    TurnOnRelay(PIN_SSR_VALVE);

    if (!IsSwitchOn(PIN_SWITCH_BREW)) {
      wModeCurrent = WMODE_COFFEE;
      lcd.clear();
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
      TurnOnRelay(PIN_SSR_PUMP);
      TurnOnRelay(PIN_SSR_VALVE);
    } else {
      TurnOffRelay(PIN_SSR_PUMP);
      TurnOffRelay(PIN_SSR_VALVE);
    }
    if (IsButtonPushed(PIN_BUTTON_SET) || backFlushCurrentCycle >= backFlushCycles * 2 - 1) {
      wModeCurrent = WMODE_COFFEE;
      backFlushCurrentCycle = 0;
    }
  }

  if (wModeCurrent == WMODE_BREWING_FINISHED) {
    targetTemperature = coffeeTemperature;
    TurnOffRelay(PIN_SSR_PUMP);
    TurnOffRelay(PIN_SSR_VALVE);
    
    if (millis() % 500 > 250 ) {
      lcd.noBacklight();
    } else {
      lcd.backlight();
    }
    
    if (!IsSwitchOn(PIN_SWITCH_BREW)) {
      wModeCurrent = WMODE_COFFEE;
      lcd.backlight();
    }
  }
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
      SaveEEPROMData(EEPROM_COFFEE, coffeeTemperature);
      lcd.clear();
    }
    if (IsButtonPushed(PIN_BUTTON_DOWN)) {
      coffeeTemperature--;
      SaveEEPROMData(EEPROM_COFFEE, coffeeTemperature);
      lcd.clear();
    }
    if (IsButtonPushed(PIN_BUTTON_SET)) {
      coffeeTemperature = 100;
      SaveEEPROMData(EEPROM_COFFEE, coffeeTemperature);
      lcd.clear();
    }
  }

  if (dModeCurrent == DMODE_STEAM_TEMP) {
    if (IsButtonPushed(PIN_BUTTON_UP)) {
      steamTemperature++;
      SaveEEPROMData(EEPROM_STEAM, steamTemperature);
      lcd.clear();
    }
    if (IsButtonPushed(PIN_BUTTON_DOWN)) {
      steamTemperature--;
      SaveEEPROMData(EEPROM_STEAM, steamTemperature);
      lcd.clear();
    }
    if (IsButtonPushed(PIN_BUTTON_SET)) {
      steamTemperature = 140;
      SaveEEPROMData(EEPROM_STEAM, steamTemperature);
      lcd.clear();
    }
  }
}

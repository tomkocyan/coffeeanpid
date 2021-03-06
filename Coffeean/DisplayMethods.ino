void RenderDisplay() {
  lcd.setCursor(0, 0);
  
  if (wModeCurrent != WMODE_BREWING_FINISHED) {
    if (millis() > displaySwitchOffTime + lastKeyHitTime) {
      lcd.noBacklight();
    } else {
      lcd.backlight();
    }
  }
  
  switch (dModeCurrent) {
    case DMODE_HOME:
      RenderTemperature();
      lcd.setCursor(0, 1);
      RenderWorkMode();
      break;

    case DMODE_COFFEE_TEMP:
      RenderMenuTitle();
      lcd.setCursor(0, 1);
      RenderCoffeeTemperature();
      break;

    case DMODE_STEAM_TEMP:
      RenderMenuTitle();
      lcd.setCursor(0, 1);
      RenderSteamTemperature();
      break;

    case DMODE_BACKFLUSH:
      RenderMenuTitle();
      lcd.setCursor(0, 1);
      RenderBackflush();
      break;

    case DMODE_BTN_DEBUG:
      RenderBtnDebug();
      break;

    case DMODE_TMP_DEBUG:
      RenderTmpDebug();
      break;
  }
}

void RenderTemperature() {
  if (millis() - lastDisplayRefresh < 1000) return;
  
  if (currentTemperature < 100) lcd.print(" ");
  lcd.print(currentTemperature);
  lcd.print(" => ");
  lcd.print((int)targetTemperature);
  if (targetTemperature < 100) lcd.print(" ");

  if (wModeCurrent == WMODE_BREWING) {
    lcd.setCursor(10, 0);
    lcd.print(" ");
    lcd.print( (int)((1.0 * millis() - brewStartTime) / 1000) );
  }
  
  lastDisplayRefresh = millis();
}

void RenderMenuTitle() {
  lcd.print("SETTINGS");
}

void RenderCoffeeTemperature() {
  lcd.print("> Coffee: ");
  lcd.print((int)coffeeTemperature);
}

void RenderSteamTemperature() {
  lcd.print("> Steam: ");
  lcd.print((int)steamTemperature);
}

void RenderBtnDebug() {
  lcd.print("SWITCH TEST");
  lcd.setCursor(0, 1);

  if (IsSwitchOn(PIN_SWITCH_BREW)) {
    lcd.print("BREW ");
  } else
  {
    lcd.print("---- ");
  }

  if (IsSwitchOn(PIN_SWITCH_WATER)) {
    lcd.print("WATER ");
  } else
  {
    lcd.print("----- ");
  }

  if (IsSwitchOn(PIN_SWITCH_STEAM)) {
    lcd.print("STEAM ");
  } else
  {
    lcd.print("----- ");
  }
}

void RenderTmpDebug() {
  lcd.print("TEMPER. DEBUG");
  lcd.setCursor(0, 1);
  lcd.print("P/");
  lcd.print(pwmInterval);
  lcd.print("  T/");
  lcd.print(currentTemperature);
  lcd.print("   ");
}

void RenderBackflush() {
  lcd.print("BACKFLUSH ");

  if (wModeCurrent == WMODE_COFFEE) {
    lcd.print("START?");
    return;
  }

  lcd.print((backFlushCurrentCycle / 2) + 1);
  lcd.print("/");
  lcd.print(backFlushCycles);
  lcd.print("   ");
}

void RenderTime() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  ReadDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  lcd.print(hour);
  lcd.print(":");
  if (minute < 10) lcd.print("0");
  lcd.print(minute);
  lcd.print(":");
  if (second < 10) lcd.print("0");
  lcd.print(second);
}

void RenderWorkMode() {
  switch (wModeCurrent) {
    case WMODE_COFFEE:
      lcd.print("ESPRESSO   ");
      break;
    case WMODE_STEAM:
      lcd.print("HORKA PARA ");
      break;
    case WMODE_SLEEP:
      lcd.print("SLEEP      ");
      break;
    case WMODE_BACKFLUSH:
      lcd.print("PROPLACH   ");
      break;
    case WMODE_BREWING:
      lcd.print("ESPRESSO   ");
      break;
    case WMODE_BREWING_FINISHED:
      lcd.print("HOTOVO     ");
      break;
    case WMODE_WATER:
      lcd.print("HORKA VODA ");
      break;
  }
}

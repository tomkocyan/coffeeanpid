void RenderDisplay() {

  lcd.setCursor(0, 0);

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
      RenderMenuTitle();
      lcd.setCursor(0, 1);
      RenderBtnDebug();
      break;

    case DMODE_TMP_DEBUG:
      RenderTmpDebug();
      break;
  }
}

void RenderTemperature() {
  if (currentTemperature < 100) lcd.print(" ");
  lcd.print(currentTemperature);
  lcd.print(" => ");
  lcd.print((int)targetTemperature);
  if (targetTemperature < 100) lcd.print(" ");
  lcd.setCursor(10, 0);

  if (wModeCurrent == WMODE_BREWING) {
    lcd.print(" ");
    lcd.print( (1.0 * millis() - brewStartTime) / 1000 );
  }
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
      lcd.print("COFFEE ");
      break;
    case WMODE_STEAM:
      lcd.print("STEAM  ");
      break;
    case WMODE_SLEEP:
      lcd.print("SLEEP  ");
      break;
    case WMODE_BACKFLUSH:
      lcd.print("FLUSH  ");
      break;
    case WMODE_BREWING:
      lcd.print("BREW   ");
      break;
    case WMODE_BREWING_FINISHED:
      lcd.print("B.FIN.");
      break;
  }
}

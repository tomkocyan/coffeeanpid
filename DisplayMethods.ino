//https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library/blob/master/LiquidCrystal_I2C.h

void RenderDisplay() {
  switch (dModeCurrent) {
    case DMODE_HOME:
      RenderDebug();
      break;

    case DMODE_COFFEE_TEMP:
      RenderMenuTitle();
      RenderCoffeeTemperature();
      break;

    case DMODE_STEAM_TEMP:
      RenderMenuTitle();
      RenderSteamTemperature();
      break;
    /*
            case DMODE_BACKFLUSH:
              RenderBackflush();
              break; */

    case DMODE_BTN_DEBUG:
      RenderBtnDebug();
      break;
  }

  //RenderTime();
  //RenderTemperature();
  //RenderWorkMode();

  //display.display();
}

void RenderDebug() {
  lcd.setCursor(0, 0);
  lcd.print("Home");

  lcd.print(" ");
  lcd.print(millis());
  
  lcd.setCursor(0, 1);
  if (currentTemperature < 100) lcd.print(" ");
  lcd.print(currentTemperature);
}


void RenderMenuTitle() {
  lcd.setCursor(0, 0);
  lcd.print("SETTINGS");
}

void RenderCoffeeTemperature() {
  lcd.setCursor(0, 1);
  lcd.print("> Coffee: ");
  lcd.print((int)coffeeTemperature);
}

void RenderSteamTemperature() {
  lcd.setCursor(0, 1);
  lcd.print("> Steam: ");
  lcd.print((int)steamTemperature);
}


void RenderProgressBar(float percent) {
  /*
  display.drawRect(0, 45, 128, 10, WHITE);
  display.fillRect(0, 45, (int)(128 * percent / 100), 10, WHITE);*/
}

void RenderBtnDebug() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SWITCH TEST");
  lcd.setCursor(0, 1);

  if (IsSwitchOn(PIN_SWITCH_BREW)) {
    lcd.print("BREW ");
  } else
  {
    lcd.print("---- ");
  }

  if (IsSwitchOn(PIN_SWITCH_STEAM)) {
    lcd.print("STEAM ");
  } else
  {
    lcd.print("----- ");
  }

  if (IsSwitchOn(PIN_SWITCH_WATER)) {
    lcd.print("WATER ");
  } else
  {
    lcd.print("----- ");
  }
}

void RenderBackflush() {
  /*
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Backflush:");
  display.setTextSize(3);
  display.setCursor(0, 20);

  if (wModeCurrent == WMODE_COFFEE) {
    display.print("START?");
    return;
  }

  display.print((backFlushCurrentCycle / 2) + 1);
  display.print("/");
  display.println(backFlushCycles);

  display.setTextSize(1);
  display.setCursor(70, 20);
  display.println(millis());
  display.setCursor(70, 30);
  display.print("S: ");
  display.println(backFlushCurrentCycleStart);
  display.setCursor(70, 40);
  display.print("E: ");
  display.println(backFlushCurrentCycleStart + backFlushCycleDuration);

  float perc = 100.0 * (millis() - backFlushCurrentCycleStart) / backFlushCycleDuration;
  RenderProgressBar(perc);
  */
}


void RenderButton(int pin, char* text) {
  /*
  if (IsSwitchOn(pin)) {
    display.setTextColor(BLACK, WHITE);
  }
  display.print(text);
  display.setTextColor(WHITE, BLACK);
  */
}

void RenderTime() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  ReadDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);


  lcd.setCursor(0, 0);
  lcd.print(hour);
  lcd.print(":");
  if (minute < 10) lcd.print("0");
  lcd.print(minute);
  lcd.print(":");
  if (second < 10) lcd.print("0");
  lcd.print(second);

  /*
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 57);
  display.print(hour);
  display.print(":");
  if (minute < 10) display.print("0");
  display.print(minute);
  display.print(":");
  if (second < 10) display.print("0");
  display.print(second);
  */
}

void RenderWorkMode() {
  /*
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(52, 57);
  */
  lcd.setCursor(0, 0);
  switch (wModeCurrent) {
    case WMODE_COFFEE:
      lcd.print("COFFEE");
      break;
    case WMODE_STEAM:
      lcd.print("STEAM");
      break;
    case WMODE_SLEEP:
      lcd.print("SLEEP");
      break;
    case WMODE_BACKFLUSH:
      lcd.print("FLUSH");
      break;
    case WMODE_BREWING:
      lcd.print("BREW");
      break;
    case WMODE_BREWING_FINISHED:
      lcd.print("B.FIN.");
      break;
  }

}

void RenderTemperature() {


  if (millis() - lastLcdRefresh > 500) {
    //lcd.clear();

    lcd.setCursor(10, 0);
    lcd.print("     ");
    lcd.setCursor(10, 0);
    lcd.print("98.6*C");

    //RenderDisplay();
    lastLcdRefresh = millis();
  }


  //lcd.setCursor(10, 0);
  //lcd.print(currentTemperature);
  //lcd.print(millis());
}

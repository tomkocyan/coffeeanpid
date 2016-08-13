

void InitDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.display();
}

void RenderDisplay() {
  display.clearDisplay();

  switch(dModeCurrent) {
    case DMODE_DEBUG:
    RenderDebug();
    break;
    
    case DMODE_COFFEE_TEMP:
    RenderCoffeeTemperature();
    break;

    case DMODE_STEAM_TEMP:
    RenderSteamTemperature();
    break;

    case DMODE_BACKFLUSH:
    RenderBackflush();
    break;

    case DMODE_TEMP_DEBUG:
    RenderTempDebug();
    break;
  }
  
  RenderTime();
  RenderTemperature();
  RenderWorkMode();

  display.display();
}

void RenderCoffeeTemperature() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Coffee:");
  display.setTextSize(3);
  display.setCursor(40, 20);
  display.println((int)coffeeTemperature);
}


void RenderProgressBar(float percent) {
  display.drawRect(0,45,128,10,WHITE);
  display.fillRect(0,45,(int)(128*percent/100),10,WHITE);
}

void RenderTempDebug() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Temperat.:");
  
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(currentTemperature);
display.println(rawTemperature);

int displaySize = 3;
  for (int i = 0; i < tempArraySize; i++) {
  display.setCursor(40 + (i / displaySize) * 40, (i % displaySize)*10 + 20);
    if (i == tempArrayPointer)  {
      display.print(">");
    } else {
      display.print(" ");
    }
    display.println(tempArray[i]);
  }

}

void RenderBackflush() {
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
}

void RenderSteamTemperature() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Steam:");
  display.setTextSize(3);
  display.setCursor(40, 20);
  display.println((int)steamTemperature);
}

void RenderDebug() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Time:");
  display.setCursor(20, 20);
  display.println(millis());
  int pos = (millis() / 10) % 128;
  display.drawLine(pos, 50, pos, 60, WHITE);

  RenderButton(PIN_SWITCH_BREW, "BREW");
  display.print(" ");
  RenderButton(PIN_SWITCH_STEAM, "STEAM");
}

void RenderButton(int pin, char* text) {
  if (IsSwitchOn(pin)) {
    display.setTextColor(BLACK, WHITE);
  } 
  display.print(text);
  display.setTextColor(WHITE, BLACK);
}

void RenderTime() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  ReadDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
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
}

void RenderWorkMode() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(52, 57);

  switch(wModeCurrent) {
    case WMODE_COFFEE:
    display.print("COFFEE");
    break;
    case WMODE_STEAM:
    display.print("STEAM");
    break;
    case WMODE_SLEEP:
    display.print("SLEEP");
    break;
    case WMODE_BACKFLUSH:
    display.print("FLUSH");
    break;
    case WMODE_BREWING:
    display.print("BREW");
    break;
    case WMODE_BREWING_FINISHED:
    display.print("B.FIN.");
    break;
  }
  
}

void RenderTemperature() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(90, 57);
  display.print(currentTemperature);
}

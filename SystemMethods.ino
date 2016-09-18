/*
void Beep() {
  tone(PIN_SPEAKER, 1200, 100);
  delay(100);
}
*/
/*
void DoHeartbeat() {
  if ( ((int)(millis() / 1000)) % 2 == 0) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);
  }
}
*/
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

  Serial.println(IsSwitchOn(PIN_SWITCH_BREW));

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

}

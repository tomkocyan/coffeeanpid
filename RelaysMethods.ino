void TurnOffRelay(int pin) {
  digitalWrite(pin, LOW);
}

void TurnOnRelay(int pin) {
  digitalWrite(pin, HIGH);
}

void SetHeater(double currentTemperature) {
  int fullHeatTemp = targetTemperature - 10;

  if (currentTemperature ==  coffeeTemperature) {
    pwmInterval = 35;
    hModeCurrent = HMODE_CONST;
  }

  else if (currentTemperature <  fullHeatTemp) {
    pwmInterval = 1024;
    hModeCurrent = HMODE_FULL_HEAT;
  }
  else if (currentTemperature > coffeeTemperature + 1) { //max cool
    pwmInterval = 0;
    hModeCurrent = HMODE_MAX_COOLING;
  }
  else if (currentTemperature > coffeeTemperature) { //cooling
    double diff = currentTemperature - coffeeTemperature;
    double ratio = diff / coffeeTemperature;
    pwmInterval = (int)(1024 * ratio / 3) + 10;
    //pwmInterval = 0;
    hModeCurrent = HMODE_COOLING;
  }
  else { //heating
    double diff = coffeeTemperature - currentTemperature - 1;
    double ratio = diff / coffeeTemperature;
    pwmInterval = (int)(1024 * ratio * 5) + 40;

    if (pwmInterval < 70) pwmInterval = 70;

    hModeCurrent = HMODE_HEATING;
  }
  if (currentTemperature > 150) {
    pwmInterval = 0;
    hModeCurrent = HMODE_EMERGENCY;
  }

  if (pwmInterval > 1024) {
    pwmInterval = 1024;
  }

  Timer3.pwm(PIN_SSR_HEAT, pwmInterval);
}

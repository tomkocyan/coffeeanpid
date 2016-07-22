void SetHeater(double currentTemperature) {
  Timer1.pwm(PIN_SSR_HEAT, 512);
/*
  if (mode == MODE_COFFEE) {

    //Serial.println("COFFEE");

    int fullHeatTemp = coffeeTemp - 10;

    if (actTemp ==  coffeeTemp) {
      pwmInterval = 35;
      hmode = HMODE_CONST;
    }

    else if (actTemp <  fullHeatTemp) {
      pwmInterval = 1024;
      hmode = HMODE_FULL_HEAT;
    }
    else if (actTemp > coffeeTemp + 1) { //max cool
      pwmInterval = 0;
      hmode = HMODE_MAX_COOLING;
    }
    else if (actTemp > coffeeTemp) { //cooling
      double diff = actTemp - coffeeTemp;
      double ratio = diff / coffeeTemp;
      pwmInterval = (int)(1024 * ratio / 3) + 10;
      //pwmInterval = 0;
      hmode = HMODE_COOLING;
    }
    else { //heating
      double diff = coffeeTemp - actTemp - 1;
      double ratio = diff / coffeeTemp;
      pwmInterval = (int)(1024 * ratio * 5) + 40;

      if (pwmInterval < 70) pwmInterval = 70;

      hmode = HMODE_HEATING;
    }
    if (actTemp > 150) {
      pwmInterval = 0;
      hmode = HMODE_EMERGENCY;
    }

    if (brewInProgress && millis() < brewFullHeatStop) {
      pwmInterval = 1024;
    }
  } else if (mode == MODE_STEAM) {
    //Serial.println("STEAM");
    if (actTemp < steamTemp) {
      pwmInterval = 1024;
      hmode = HMODE_FULL_HEAT;
    } else {
      pwmInterval = 0;
      hmode = HMODE_COOLING;
    }
  } else {
    //Serial.println("OTHER COOL");
    pwmInterval = 0;
    hmode = HMODE_COOLING;
  }

  if (pwmInterval > 1024) {
    pwmInterval = 1024;
  }

  //Serial.print("PWM : ");
  //Serial.println(pwmInterval);
  for (int i = 10; i <= 13; i++) {
    Timer1.pwm(i, pwmInterval);                // setup pwm on pin 9, 50% duty cycle
  }
  */
}

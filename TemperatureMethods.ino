/*float GetPlatinumRTD(float R, float R0) {
  float A = 3.9083E-3;
  float B = -5.775E-7;
  float T;

  R = R / R0;

  //T = (0.0-A + sqrt((A*A) - 4.0 * B * (1.0 - R))) / 2.0 * B;
  T = 0.0 - A;
  T += sqrt((A * A) - 4.0 * B * (1.0 - R));
  T /= (2.0 * B);

  if (T > 0 && T < 200) {
    return T;
  }
  else {
    //T=  (0.0-A - sqrt((A*A) - 4.0 * B * (1.0 - R))) / 2.0 * B;
    T = 0.0 - A;
    T -= sqrt((A * A) - 4.0 * B * (1.0 - R));
    T /= (2.0 * B);
    return T;
  }
}*/

float GetPlatinumRTD(float R, float R0) {
  float A = 3.9083E-3;
  float B = -5.775E-7;
  float T;

  R = R / R0;

  //T = (0.0-A + sqrt((A*A) - 4.0 * B * (1.0 - R))) / 2.0 * B;
  T = 0.0 - A;
  T += sqrt((A * A) - 4.0 * B * (1.0 - R));
  T /= (2.0 * B);

  if (T > 0 && T < 200) {
    return T;
  }
  else {
    //T=  (0.0-A - sqrt((A*A) - 4.0 * B * (1.0 - R))) / 2.0 * B;
    T = 0.0 - A;
    T -= sqrt((A * A) - 4.0 * B * (1.0 - R));
    T /= (2.0 * B);
    return T;
  }
}

void ReadTemperature() {
  if (millis() - lastTemperatureMeasurement < temperatureMeasurementInterval) return;
  lastTemperatureMeasurement = millis();

  /*    O +5V
   *    |
   *   | |
   *   | |  Rc = 1200 OHM
   *   | |
   *    |
   *    O===========> A0 VSTUP
   *    |
   *   | |
   *   | |  PT1000 => Uv = napeti na PT1000
   *   | |
   *    |
   *    O GND
   */

  double U = 5; //vstupni napeti (z USB)
  double Rc = 1200; //odpor rezistoru v delici
  double analogSum = 0;
  int cycles = 10;
  double Uv = (U * analogSum) / 1024; //napeti na PT1000
  double Rv = (Uv * Rc) / (U - Uv); //impedance PT1000
  
  //rawTemperature = GetPlatinumRTD(Rv, 1000); //prectena hodnota
  rawTemperature = 90.0 + 0.1 * (millis() % 10);

  //vypocet prumeru poslednich hodnot a smerodatne odchylky
  tempArrayPointer = (tempArrayPointer + 1) % tempArraySize;
  tempArray[tempArrayPointer] = rawTemperature;

  double sum = 0;
  for (int i = 0; i < tempArraySize; i++) {
    sum += tempArray[i];
  }
  currentTemperature = sum / tempArraySize;

  tempArraySD = 0;
  for (int i = 0; i < tempArraySize; i++) {
    double diff  = currentTemperature - tempArray[i];
    tempArraySD += diff * diff;
  }
  tempArraySD = sqrt(tempArraySD);
}


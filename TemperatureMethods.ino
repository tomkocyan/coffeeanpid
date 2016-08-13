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
  //  actTemp = thermocouple.readCelsius();
  //actTemp = 20;
  lastTemperatureMeasurement = millis();

  double U = 5;
  double val = analogRead(A0);    // read the input pin
  //  lcdMono.print(val);             // debug value
  double Rc = 1200; //5 * 220; //10000;
  double Uv = (U * val) / 1024;
  double Rv = (Uv * Rc) / (U - Uv);
  rawTemperature = GetPlatinumRTD(Rv, 1000);

  tempArrayPointer = (tempArrayPointer + 1) % tempArraySize;

  tempArray[tempArrayPointer] = rawTemperature;

  double sum = 0;

  for (int i = 0; i < tempArraySize; i++) {
    sum += tempArray[i];
  }

  currentTemperature = sum / tempArraySize;


}


#define DS3231_I2C_ADDRESS 0x68

// Convert normal decimal numbers to binary coded decimal
byte DecToBcd(byte val) {
  return ( (val / 10 * 16) + (val % 10) );
}

// Convert binary coded decimal to normal decimal numbers
byte BcdToDec(byte val) {
  return ( (val / 16 * 10) + (val % 16) );
}

void SetDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year) {
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(DecToBcd(second)); // set seconds
  Wire.write(DecToBcd(minute)); // set minutes
  Wire.write(DecToBcd(hour)); // set hours
  Wire.write(DecToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(DecToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(DecToBcd(month)); // set month
  Wire.write(DecToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void ReadDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year) {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = BcdToDec(Wire.read() & 0x7f);
  *minute = BcdToDec(Wire.read());
  *hour = BcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = BcdToDec(Wire.read());
  *dayOfMonth = BcdToDec(Wire.read());
  *month = BcdToDec(Wire.read());
  *year = BcdToDec(Wire.read());
}

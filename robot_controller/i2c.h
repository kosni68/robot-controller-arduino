
void scann_i2c() {
  byte error, address;
  int nDevices;
  Serial.println(F("Scanning..."));
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print(F("I2C device found at address 0x"));
      if (address<16) {
        Serial.print(F("0"));
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print(F("Unknow error at address 0x"));
      if (address<16) {
        Serial.print(F("0"));
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println(F("No I2C devices found\n"));
  }
  else {
    Serial.println(F("done\n"));
  }        
}

bool i2c_force_endTransmission(byte adresse)
{
  Wire.beginTransmission(ADDRESS_I2C_LCD);
  int error = Wire.endTransmission(false);
  Serial.print(F("erreur apres endTransmission false : Nr"));
  Serial.println(error);
}

bool i2c_test_connection(byte adresse)
{
  Serial.print(F("i2c_test_connection adresse : 0x"));
  Serial.println(adresse, HEX);

  Wire.beginTransmission(adresse);
  int error = Wire.endTransmission();

  if (error == 0)
  {
    Serial.println(F("I2C ok :"));
    return HIGH;
  }
  else if (error == 1)
  {
    Serial.println(F("données trop longues pour tenir dans le tampon de transmission"));
    return LOW;
  }
  else if (error == 2)
  {
    Serial.println(F("NACK reçu lors de la transmission de l'adresse"));
    return LOW;
  }
  else if (error == 3)
  {
    Serial.println(F("NACK reçu lors de la transmission des données"));
    i2c_force_endTransmission(adresse);
    return LOW;
  }
  else
  {
    Serial.print(F("erreur : "));
    Serial.println(String(error));
    //i2c_force_endTransmission(adresse);
    return LOW;
  }
}

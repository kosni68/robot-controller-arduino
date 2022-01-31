// ***********************************************************************
// ***************************    LCD    *********************************
// ***********************************************************************

bool init_lcd()
{
  if (i2c_test_connection(ADDRESS_I2C_LCD))
  {
    if (connection_lcd == LOW)
    {
      lcd.init();
      connection_lcd = HIGH;
      Serial.print(F("Lcd_i2c_initialiser !"));
    }
    return HIGH;
  }
  else
  {
    Serial.print(F("connection_lcd = LOW"));
    connection_lcd = LOW;
    return LOW;
  }
}

void setup_Lcd()
{
  Serial.print(F("\n\t --- setup_Lcd --- \n"));
  if (!i2c_test_connection(ADDRESS_I2C_LCD))
  {
    connection_lcd = LOW;
    return;
  }
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Controller");
  lcd.setCursor(0, 1);
  lcd.print("init.");
}

void print_lcd()
{

  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Controller");
  lcd.setCursor(0, 1);
  lcd.print(millis() / 100);
}
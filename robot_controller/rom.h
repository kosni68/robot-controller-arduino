
// ***********************************************************************
// **************************    EEPROM    *******************************
// ***********************************************************************

void variable_default()
{
  PIN_joystick_speed = 15;
  PIN_joystick_steer = 14;
  PIN_speed_inverse = 0;
  PIN_steer_inverse = 0;
  PIN_buzzer = 3;
  PIN_weapon_enable = 5;
  PIN_weapon_speed = 17;
  PIN_weapon_enable_2 = 6;
  PIN_lcd_scroll = 4;

  inverse_speed = LOW;
  inverse_steer = LOW;
  correction_scale = HIGH;

  joystick_deadzone = 30;

  joystick_speed_min = 0;
  joystick_speed_middle = 512;
  joystick_speed_max = 1023;

  joystick_steer_min = 0;
  joystick_steer_middle = 512;
  joystick_steer_max = 1023;

  x_speed[0] = 0;
  x_speed[1] = 700;
  x_speed[2] = 1000;

  y_speed[0] = 120;
  y_speed[1] = 300;
  y_speed[2] = 1000;

  x_steer[0] = 0;
  x_steer[1] = 700;
  x_steer[2] = 1000;

  y_steer[0] = 120;
  y_steer[1] = 300;
  y_steer[2] = 1000;

  weapon_speed_min = 0;
  weapon_speed_max = 1023;
}

void save_eeprom()
{
  byte byte_position = 1;

  EEPROM.put(byte_position, end_address[0]);
  byte_position += sizeof(end_address[0]);

  EEPROM.put(byte_position, end_address[1]);
  byte_position += sizeof(end_address[1]);

  EEPROM.put(byte_position, PIN_joystick_speed);
  byte_position += sizeof(PIN_joystick_speed);

  EEPROM.put(byte_position, PIN_joystick_steer);
  byte_position += sizeof(PIN_joystick_steer);

  EEPROM.put(byte_position, PIN_speed_inverse);
  byte_position += sizeof(PIN_speed_inverse);

  EEPROM.put(byte_position, PIN_steer_inverse);
  byte_position += sizeof(PIN_steer_inverse);

  EEPROM.put(byte_position, PIN_buzzer);
  byte_position += sizeof(PIN_buzzer);

  EEPROM.put(byte_position, PIN_weapon_enable);
  byte_position += sizeof(PIN_weapon_enable);

  EEPROM.put(byte_position, PIN_weapon_speed);
  byte_position += sizeof(PIN_weapon_speed);

  EEPROM.put(byte_position, PIN_weapon_enable_2);
  byte_position += sizeof(PIN_weapon_enable_2);

  EEPROM.put(byte_position, PIN_lcd_scroll);
  byte_position += sizeof(PIN_lcd_scroll);

  EEPROM.put(byte_position, inverse_speed);
  byte_position += sizeof(inverse_speed);

  EEPROM.put(byte_position, inverse_steer);
  byte_position += sizeof(inverse_steer);

  EEPROM.put(byte_position, correction_scale);
  byte_position += sizeof(correction_scale);


  EEPROM.put(byte_position, joystick_deadzone);
  byte_position += sizeof(joystick_deadzone);

  EEPROM.put(byte_position, joystick_speed_min);
  byte_position += sizeof(joystick_speed_min);

  EEPROM.put(byte_position, joystick_speed_middle);
  byte_position += sizeof(joystick_speed_middle);

  EEPROM.put(byte_position, joystick_speed_max);
  byte_position += sizeof(joystick_speed_max);

  EEPROM.put(byte_position, joystick_steer_min);
  byte_position += sizeof(joystick_steer_min);

  EEPROM.put(byte_position, joystick_steer_middle);
  byte_position += sizeof(joystick_steer_middle);

  EEPROM.put(byte_position, joystick_steer_max);
  byte_position += sizeof(joystick_steer_max);

  EEPROM.put(byte_position, x_speed[0]);
  byte_position += sizeof(x_speed[0]);

  EEPROM.put(byte_position, x_speed[1]);
  byte_position += sizeof(x_speed[1]);

  EEPROM.put(byte_position, x_speed[2]);
  byte_position += sizeof(x_speed[2]);

  EEPROM.put(byte_position, y_speed[0]);
  byte_position += sizeof(y_speed[0]);

  EEPROM.put(byte_position, y_speed[1]);
  byte_position += sizeof(y_speed[1]);

  EEPROM.put(byte_position, y_speed[2]);
  byte_position += sizeof(y_speed[2]);

  EEPROM.put(byte_position, x_steer[0]);
  byte_position += sizeof(x_steer[0]);

  EEPROM.put(byte_position, x_steer[1]);
  byte_position += sizeof(x_steer[1]);

  EEPROM.put(byte_position, x_steer[2]);
  byte_position += sizeof(x_steer[2]);

  EEPROM.put(byte_position, y_steer[0]);
  byte_position += sizeof(y_steer[0]);

  EEPROM.put(byte_position, y_steer[1]);
  byte_position += sizeof(y_steer[1]);

  EEPROM.put(byte_position, y_steer[2]);
  byte_position += sizeof(y_steer[2]);
  
  EEPROM.put(byte_position, weapon_speed_min);
  byte_position += sizeof(weapon_speed_min);

  EEPROM.put(byte_position, weapon_speed_max);
  byte_position += sizeof(weapon_speed_max);
}

void read_eeprom()
{
  byte byte_position = 1;

  EEPROM.get(byte_position, end_address[0]);
  byte_position += sizeof(end_address[0]);

  EEPROM.get(byte_position, end_address[1]);
  byte_position += sizeof(end_address[1]);

  EEPROM.get(byte_position, PIN_joystick_speed);
  byte_position += sizeof(PIN_joystick_speed);

  EEPROM.get(byte_position, PIN_joystick_steer);
  byte_position += sizeof(PIN_joystick_steer);

  EEPROM.get(byte_position, PIN_speed_inverse);
  byte_position += sizeof(PIN_speed_inverse);

  EEPROM.get(byte_position, PIN_steer_inverse);
  byte_position += sizeof(PIN_steer_inverse);

  EEPROM.get(byte_position, PIN_buzzer);
  byte_position += sizeof(PIN_buzzer);

  EEPROM.get(byte_position, PIN_weapon_enable);
  byte_position += sizeof(PIN_weapon_enable);

  EEPROM.get(byte_position, PIN_weapon_speed);
  byte_position += sizeof(PIN_weapon_speed);

  EEPROM.get(byte_position, PIN_weapon_enable_2);
  byte_position += sizeof(PIN_weapon_enable_2);

  EEPROM.get(byte_position, PIN_lcd_scroll);
  byte_position += sizeof(PIN_lcd_scroll);

  EEPROM.get(byte_position, inverse_speed);
  byte_position += sizeof(inverse_speed);

  EEPROM.get(byte_position, inverse_steer);
  byte_position += sizeof(inverse_steer);

  EEPROM.get(byte_position, correction_scale);
  byte_position += sizeof(correction_scale);

  EEPROM.get(byte_position, joystick_speed_min);
  byte_position += sizeof(joystick_speed_min);

  EEPROM.get(byte_position, joystick_deadzone);
  byte_position += sizeof(joystick_deadzone);

  EEPROM.get(byte_position, joystick_speed_middle);
  byte_position += sizeof(joystick_speed_middle);

  EEPROM.get(byte_position, joystick_speed_max);
  byte_position += sizeof(joystick_speed_max);

  EEPROM.get(byte_position, joystick_steer_min);
  byte_position += sizeof(joystick_steer_min);

  EEPROM.get(byte_position, joystick_steer_middle);
  byte_position += sizeof(joystick_steer_middle);

  EEPROM.get(byte_position, joystick_steer_max);
  byte_position += sizeof(joystick_steer_max);

  EEPROM.get(byte_position, x_speed[0]);
  byte_position += sizeof(x_speed[0]);

  EEPROM.get(byte_position, x_speed[1]);
  byte_position += sizeof(x_speed[1]);

  EEPROM.get(byte_position, x_speed[2]);
  byte_position += sizeof(x_speed[2]);

  EEPROM.get(byte_position, y_speed[0]);
  byte_position += sizeof(y_speed[0]);

  EEPROM.get(byte_position, y_speed[1]);
  byte_position += sizeof(y_speed[1]);

  EEPROM.get(byte_position, y_speed[2]);
  byte_position += sizeof(y_speed[2]);

  EEPROM.get(byte_position, x_steer[0]);
  byte_position += sizeof(x_steer[0]);

  EEPROM.get(byte_position, x_steer[1]);
  byte_position += sizeof(x_steer[1]);

  EEPROM.get(byte_position, x_steer[2]);
  byte_position += sizeof(x_steer[2]);

  EEPROM.get(byte_position, y_steer[0]);
  byte_position += sizeof(y_steer[0]);

  EEPROM.get(byte_position, y_steer[1]);
  byte_position += sizeof(y_steer[1]);

  EEPROM.get(byte_position, y_steer[2]);
  byte_position += sizeof(y_steer[2]);

  EEPROM.get(byte_position, weapon_speed_min);
  byte_position += sizeof(weapon_speed_min);

  EEPROM.get(byte_position, weapon_speed_max);
  byte_position += sizeof(weapon_speed_max);
}

void init_eeprom()
{
  if (EEPROM.read(0) != value_to_init_eeprom)
  {
    Serial.println(F("\n\t --- variable_default --- \n"));
    variable_default();
    save_eeprom();
    EEPROM.write(0, value_to_init_eeprom);
  }
  else
  {
    Serial.println(F("\n\t --- read_eeprom --- \n"));
    read_eeprom();
  }
}
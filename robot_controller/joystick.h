// ***********************************************************************
// *********************    SCALING FALSE CURVE   ************************
// ***********************************************************************

int scaling_false_curve(int value, int x_value_min, int x_value_middle, int x_value_max, int y_value_min, int y_value_middle, int y_value_max)
{
  // x = abscisse , y = ordonnée

  if (value >= x_value_min && value <= x_value_middle)
    return map(value, x_value_min, x_value_middle, y_value_min, y_value_middle);
  else
    return map(value, x_value_middle, x_value_max, y_value_middle, y_value_max);
}

// ***********************************************************************
// ***************************    SCALING    *****************************
// ***********************************************************************

int scaling(int analogread, int analogread_value_min, int analogread_value_middle, int analogread_value_max, int min, int maximum)
{
  int return_value;
  int middle = 0;
  int deadzone = 30;

  return_value = middle;

  if (analogread > analogread_value_middle + deadzone)
  {
    return_value = map(analogread, analogread_value_middle + deadzone, analogread_value_max, middle, maximum);
    if (return_value > maximum)
      return maximum;
  }
  else if (analogread < analogread_value_middle - deadzone)
  {
    return_value = map(analogread, analogread_value_middle - deadzone, analogread_value_min, middle, min);
    if (return_value < min)
      return min;
  }
  return return_value;
}

// ***********************************************************************
// ***********************    READ JOYSTICK    ***************************
// ***********************************************************************

void read_joystick()
{

  speed_read = analogRead(PIN_joystick_speed);
  steer_read = analogRead(PIN_joystick_steer);

  if (correction_scale)
  {
    speed_scale = scaling(speed_read, joystick_speed_min, joystick_speed_middle, joystick_speed_max, send_value_speed_min, send_value_speed_max);

    if (speed_scale > 0)
      joystate.speed_send = scaling_false_curve(speed_scale, x_speed[0], x_speed[1], x_speed[2], y_speed[0], y_speed[1], y_speed[2]);
    else
      joystate.speed_send = -scaling_false_curve(-speed_scale, x_speed[0], x_speed[1], x_speed[2], y_speed[0], y_speed[1], y_speed[2]);

    steer_scale = scaling(steer_read, joystick_steer_min, joystick_steer_middle, joystick_steer_max, send_value_steer_min, send_value_steer_max);

    if (steer_scale > 0)
      joystate.steer_send = scaling_false_curve(steer_scale, x_steer[0], x_steer[1], x_steer[2], y_steer[0], y_steer[1], y_steer[2]);
    else
      joystate.steer_send = -scaling_false_curve(-steer_scale, x_steer[0], x_steer[1], x_steer[2], y_steer[0], y_steer[1], y_steer[2]);

    if (joystate.speed_send <= y_speed[0] && joystate.speed_send >= -y_speed[0])
      joystate.speed_send = 0;
    if (joystate.steer_send <= y_steer[0] && joystate.steer_send >= -y_steer[0])
      joystate.steer_send = 0;
  }
  else
  {
    joystate.speed_send = speed_read;
    joystate.steer_send = steer_read;
  }

  if (inverse_speed)
    joystate.speed_send = -joystate.speed_send;
  if (inverse_steer)
    joystate.steer_send = -joystate.steer_send;

  if (SERIAL_DEBUG)
  {
    Serial.print(F("\t\t speed_read = "));
    Serial.print(speed_read);
    Serial.print(F("\t steer_read = "));
    Serial.println(steer_read);
    Serial.print(F("\t\t speed_send = "));
    Serial.print(joystate.speed_send);
    Serial.print(F("\t steer_send = "));
    Serial.println(joystate.steer_send);
  }

  if (inverse_send_speed_steer)
  {
    int speed = joystate.speed_send;
    joystate.speed_send = joystate.steer_send;
    joystate.steer_send = speed;
  }
}

// ***********************************************************************
// ***********************    CALIBRATION AUTO    ************************
// ***********************************************************************

void calibration_joystick(byte PIN_joystick, int &value_read)
{
  unsigned long currentMillis = millis();
  unsigned long previousMillis = millis();

  const long interval = 3000;
  long value_read_moyenne = 0;
  int nb_value = 0;

  while (currentMillis - previousMillis < interval)
  {
    value_read = analogRead(PIN_joystick);
    value_read_moyenne += value_read;
    nb_value++;
    currentMillis = millis();
  }

  value_read = value_read_moyenne / nb_value;
}

void calibration_joystick(byte PIN_joystick, int &value_read_min, int &value_read_max)
{
  unsigned long currentMillis = millis();
  unsigned long previousMillis = millis();

  const long interval = 8000;
  int value_read = 0;
  value_read_min = 999999;
  value_read_max = 0;

  while (currentMillis - previousMillis < interval)
  {
    value_read = analogRead(PIN_joystick);

    if (value_read < value_read_min)
      value_read_min = value_read;

    if (value_read > value_read_max)
      value_read_max = value_read;

    currentMillis = millis();
  }
}

int calibration_auto()
{

  print_lcd_cal_no_move();

  Serial.println(F("\n **** keep the speed joystick in the central position ****"));
  calibration_joystick(PIN_joystick_speed, joystick_speed_middle);

  Serial.println(F("\n **** keep the steer joystick in the central position ****"));
  calibration_joystick(PIN_joystick_steer, joystick_steer_middle);

  Serial.println(F("\n **** move the speed joystick up and down ****"));
  print_lcd_cal_move_speed();
  calibration_joystick(PIN_joystick_speed, joystick_speed_min, joystick_speed_max);

  Serial.println(F("\n **** move the steer joystick from right to left ****"));
  print_lcd_cal_move_steer();
  calibration_joystick(PIN_joystick_steer, joystick_steer_min, joystick_steer_max);

  correction_scale = HIGH;
  save_eeprom();
}

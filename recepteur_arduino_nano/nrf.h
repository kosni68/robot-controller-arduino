RF24 nRF(nRF_CE, nRF_CSn);

void nrf_send_data()
{
  nRF.stopListening();

  if (!nRF.write(&Feedback, sizeof(SerialFeedback)))
  {
      Serial.println(F("(Not Ack'd from controller)"));
  }
  else
  {
      Serial.println(F("Ack controller"));
  }
}

void nrf_receive_data()
{
  nRF.startListening();

  if (nRF.available())
  {
    nRF.read(&joystate, sizeof(struct joystick_state));

    if (joystate.checksum != joystate.buttons + joystate.axis_x + joystate.axis_y)
    {
      Serial.print("error checksum nrf");
      return;
    }

    Serial.print("steer = ");
    Serial.print(joystate.axis_x);
    Serial.print("\t\tspeed = ");
    Serial.println(joystate.axis_y);

    last_joystick_time = millis();

    int steering = deadZone(joystate.axis_x - 512, 1);
    int throttle = deadZone(joystate.axis_y - 512, 1); //anc 20

    hoverserial_send(steering, throttle);
    Serial.print("steering = ");
    Serial.print(steering);
    Serial.print("\t\tthrottle = ");
    Serial.println(throttle);

    digitalWrite(A0, joystate.buttons & button_mask_up);
    digitalWrite(A1, joystate.buttons & button_mask_right);
    digitalWrite(A2, joystate.buttons & button_mask_down);
    digitalWrite(A3, joystate.buttons & button_mask_left);
  }
  else
    Serial.println(F("(no available)"));
}

void init_nrf(const byte robot_address[6], const byte joystick_address[6])
{
  Serial.println(F(" init nrf"));

  printf_begin();
  if (!nRF.begin())
    Serial.println(F("radio hardware is not responding!!"));

  nRF.openWritingPipe(robot_address);
  nRF.openReadingPipe(1, joystick_address);
  nRF.printDetails();
}
RF24 nRF(nRF_CE, nRF_CSn);

void nrf_send_data()
{
  nRF.stopListening();

  if (!nRF.write(&joystate, sizeof(struct joystick_state)))
  {
    if (serial_print)
      Serial.println(F("(Not Ack'd from bot)"));
  }
  else
  {
    if (serial_print)
      Serial.println(F("(OK)"));
  }
}

void nrf_receive_data()
{
  nRF.startListening();

  if (nRF.available())
  {
    nRF.read(&robot_feedback, sizeof(struct feedback_data));
    Serial.println(F("(available)"));
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
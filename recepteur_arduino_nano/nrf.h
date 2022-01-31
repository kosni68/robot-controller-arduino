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
  }
  else
    Serial.println(F("nrf no available"));
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
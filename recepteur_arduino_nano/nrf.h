RF24 nRF(nRF_CE, nRF_CSn);
unsigned long last_send_data_time = 0;

void nrf_send_data()
{

  if (millis() - last_send_data_time > 800)
  {
    nRF.stopListening();

    if (!nRF.write(&Feedback, sizeof(SerialFeedback)))
    {
      Serial.println(F("Not Ack'd from controller"));
    }
    else
    {
      Serial.println(F("Ack controller"));
    }
    last_send_data_time = millis();
  }

}

bool nrf_receive_data()
{
  nRF.startListening();

  if (nRF.available())
  {
    while (nRF.available())
    {
      nRF.read(&joystate, sizeof(struct joystick_state));
      // Serial.println(F("nRF available"));
      return HIGH;
    }
  }
  else
  {
    // Serial.println(F("nrf no available"));
    return LOW;
  }
}

void init_nrf(const byte tx_address[6], const byte rx_address[6])
{
  Serial.println(F(" init nrf"));

  printf_begin();
  if (!nRF.begin())
    Serial.println(F("radio hardware is not responding!!"));

  /*
  le WiFi couvre les fréquences allant de 2,401 GHz (canal 1) à 2,483 GHz (canal 13).
  le Bluetooth, il en va de même. En effet, les fréquences comprises entre 2400 MHz et 2483,5 MHz peuvent être utilisées,
  nous laissant donc « le champ libre » entre 2484 MHz et 2525 MHz.
   */

  nRF.setChannel(104);         // valeur comprise entre 84 et 125
  nRF.setPALevel(RF24_PA_MAX); // RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  nRF.setDataRate(RF24_1MBPS); // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS

  nRF.openWritingPipe(tx_address);
  nRF.openReadingPipe(1, rx_address);
  nRF.printDetails();
}
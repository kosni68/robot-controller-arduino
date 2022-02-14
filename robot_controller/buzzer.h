unsigned long last_bip_buzzer_time = 0;

void init_buzzer()
{
   pinMode(PIN_buzzer, OUTPUT);

   tone(PIN_buzzer, 600); // allume le buzzer actif arduino
   delay(300);
   tone(PIN_buzzer, 900); // allume le buzzer actif arduino
   delay(300);
   tone(PIN_buzzer, 600); // allume le buzzer actif arduino
   delay(300);

   noTone(PIN_buzzer); // désactiver le buzzer actif arduino
}

void bip_buzzer(unsigned int interval_off,unsigned int interval_on)
{

   //Serial.print(millis() - last_bip_buzzer_time);

   if (millis() - last_bip_buzzer_time > interval_off && millis() - last_bip_buzzer_time < interval_off+interval_on)
      tone(PIN_buzzer, 200);
   else if (millis() - last_bip_buzzer_time > interval_off+interval_on)
   {
    noTone(PIN_buzzer);
    last_bip_buzzer_time = millis();
   }
   else
   {
    noTone(PIN_buzzer);
   }
   
}
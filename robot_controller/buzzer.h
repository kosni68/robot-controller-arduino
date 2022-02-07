unsigned long last_bip_buzzer_time = 0;

void init_buzzer()
{
   pinMode(PIN_buzzer, OUTPUT);

   tone(PIN_buzzer, 600); // allume le buzzer actif arduino
   delay(500);
   tone(PIN_buzzer, 900); // allume le buzzer actif arduino
   delay(500);
   tone(PIN_buzzer, 600); // allume le buzzer actif arduino
   delay(500);

   noTone(PIN_buzzer); // désactiver le buzzer actif arduino
}

void bip_buzzer(unsigned int delay)
{
   if (millis() - last_bip_buzzer_time > 1000 && millis() - last_bip_buzzer_time < 1200)
      tone(PIN_buzzer, 600);
   else
   {
    noTone(PIN_buzzer);
    last_bip_buzzer_time = millis();
   }
   
}
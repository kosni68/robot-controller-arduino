
void init_buzzer()
{
   pinMode(PIN_buzzer, OUTPUT);

   tone (PIN_buzzer, 600); // allume le buzzer actif arduino
   delay(500);
   tone(PIN_buzzer, 900); // allume le buzzer actif arduino
   delay(500);
   tone (PIN_buzzer, 600); // allume le buzzer actif arduino
   delay(500);

   noTone(PIN_buzzer);  // désactiver le buzzer actif arduino
}

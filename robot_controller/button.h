/*
Le PORTD correspond aux broches numériques 0 à 7 de la carte Arduino et est contrôlé par les 3 registres suivants :

DDRD - Le registre de direction du port D - écriture/lecture
PORTD - Le registre de données du port D - écriture/lecture
PIND - Le registre des broches numériques en entrée du port D - lecture seulement

Le PORTB correspond aux broches numériques 8 à 13 de la carte Arduino.
Les 2 broches 6 et 7 du port sont connectées au quartz et ne sont pas utilisables. 
Le PORTB est contrôlé par les registres suivants :

DDRB - Le registre de direction du port B - écriture/lecture
PORTB - Le registre de données du port B - écriture/lecture
PINB - Le registre des broches numériques en entrée du port B - lecture seulement
*/

#define button_mask_up 0x01       //B00000001
#define button_mask_right 0x02    //B00000010
#define button_mask_down 0x04     //B00000100
#define button_mask_left 0x08     //B00001000
#define button_mask_start 0x10    //B00010000
#define button_mask_select 0x20   //B00100000
#define button_mask_joystick 0x40 //B01000000

unsigned long last_button_time = 0;

void init_button()
{
    /* Initialisation des ports du joystick
   *  Ports en entrées, activer les pull-ups
   */
    DDRD &= 3;
    PORTD |= 0xFC; //B1111 1100
    DDRB &= 0xFE;  //B1111 1110
    PORTB |= 0x01; //B00000001
}

void read_button()
{
    joystate.buttons = (~PIND >> 2) & B01111111;
    if (PINB & B00000001)
        joystate.buttons &= ~button_mask_joystick;

    if (joystate.buttons & button_mask_up)
        Serial.println(F("UP "));
    if (joystate.buttons & button_mask_right)
        //Serial.println(F("RIGHT "));
    if (joystate.buttons & button_mask_down)
        Serial.println(F("DOWN "));
    if (joystate.buttons & button_mask_left)
    {
        Serial.println(F("LEFT "));

        if (millis() - last_button_time > 400)
        {
            mode_print_lcd++;
            // overflow(mode_print_lcd, 0, sizeof(enum item_mode_lcd) - 1);
            overflow(mode_print_lcd, 0, 3);
            last_button_time = millis();
        }
    }
    if (joystate.buttons & button_mask_start)
        Serial.println(F("START "));
    if (joystate.buttons & button_mask_select)
        Serial.println(F("SELECT "));
    if (joystate.buttons & button_mask_joystick)
        Serial.println(F("JOYSTICK "));
}

#define btn_weapon_enable 0x01    // B00000001
#define btn_weapon_enable_2 0x02  // B00000010
#define button_mask_down 0x04     // B00000100
#define button_mask_left 0x08     // B00001000
#define button_mask_start 0x10    // B00010000
#define button_mask_select 0x20   // B00100000
#define button_mask_joystick 0x40 // B01000000

unsigned long last_button_time = 0;

void init_button()
{
    pinMode(PIN_buzzer, OUTPUT);
    pinMode(PIN_weapon_enable, INPUT_PULLUP);
    pinMode(PIN_weapon_enable_2, INPUT_PULLUP);
    pinMode(PIN_lcd_scroll, INPUT_PULLUP);
    pinMode(PIN_speed_inverse, INPUT_PULLUP);
    pinMode(PIN_steer_inverse, INPUT_PULLUP);
}

void read_button()
{
    joystate.buttons = 0;

    if (!digitalRead(PIN_lcd_scroll))
    {
        Serial.println(F("PIN_lcd_scroll "));

        if (millis() - last_button_time > 400)
        {
            mode_print_lcd++;
            // overflow(mode_print_lcd, 0, sizeof(enum item_mode_lcd) - 1);
            overflow(mode_print_lcd, 0, 3);
            last_button_time = millis();
        }
    }

    if (!digitalRead(PIN_weapon_enable))
    {
        Serial.println(F("PIN_weapon_enable "));
        joystate.buttons = joystate.buttons | btn_weapon_enable;
    }

    if (!digitalRead(PIN_weapon_enable_2))
    {
        Serial.println(F("PIN_weapon_enable_2 "));
        joystate.buttons = joystate.buttons | btn_weapon_enable_2;
    }

    if (PIN_speed_inverse > 0)
    {
        if (!digitalRead(PIN_speed_inverse))
        {
            Serial.println(F("PIN_speed_inverse "));
            joystate.speed_send = -joystate.speed_send;
        }
    }

    if (PIN_steer_inverse > 0)
    {
        if (!digitalRead(PIN_steer_inverse))
        {
            Serial.println(F("PIN_steer_inverse "));
            joystate.steer_send = -joystate.steer_send;
        }
    }
}
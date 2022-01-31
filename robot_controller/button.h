
#define button_mask_up 0x01
#define button_mask_right 0x02
#define button_mask_down 0x04
#define button_mask_left 0x08
#define button_mask_start 0x10
#define button_mask_select 0x20
#define button_mask_joystick 0x40

void read_button()
{
    if (joystate.buttons & button_mask_up)
        Serial.print(F("UP "));
    if (joystate.buttons & button_mask_right)
        Serial.print(F("RIGHT "));
    if (joystate.buttons & button_mask_down)
        Serial.print(F("DOWN "));
    if (joystate.buttons & button_mask_left)
        Serial.print(F("LEFT "));
    if (joystate.buttons & button_mask_start)
        Serial.print(F("START "));
    if (joystate.buttons & button_mask_select)
        Serial.print(F("SELECT "));
    if (joystate.buttons & button_mask_joystick)
        Serial.print(F("JOYSTICK "));
}
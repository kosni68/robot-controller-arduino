// ***********************************************************************
// *****************************   LIBRARY   *****************************
// ***********************************************************************

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "RF24.h"
#include "printf.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

// ***********************************************************************
// ************************     CONSTANTES    ****************************
// ***********************************************************************

#define VERSION "2.0"
#define value_to_init_eeprom 274 //change this value to erase default eeprom
#define ADDRESS_I2C_LCD 0x26     //0x3F

#define nRF_CE 9
#define nRF_CSn 10

const byte nRF_robot_address[6] = "ABcd0";
const byte nRF_joystick_address[6] = "EFgh1";

#define SERIAL_DEBUG LOW

// ***********************************************************************
// ********************     VARIABLES GLOBALES     ***********************
// ***********************************************************************

byte PIN_joystick_speed;
byte PIN_joystick_steer;
byte PIN_buzzer;
byte PIN_weapon_enable;
byte PIN_weapon_speed;
byte PIN_lcd_scroll;

bool serial_print = HIGH;
unsigned long millis_serial_pause = 0;
unsigned long last_ack_send_data_time = 0;

bool inverse_speed = LOW;
bool inverse_steer = LOW;
bool inverse_send_speed_steer = LOW;

int speed_scale;
int steer_scale;

int joystick_deadzone;

int joystick_speed_min;
int joystick_speed_middle;
int joystick_speed_max;

int joystick_steer_min;
int joystick_steer_middle;
int joystick_steer_max;

bool correction_scale = LOW;

struct joystick_state
{
  byte buttons;
  int steer_send;
  int speed_send;
  byte speed_weapon_send;
} joystate;

int steer_read;
int speed_read;

struct feedback_data
{
  uint16_t start;
  int16_t cmd1;
  int16_t cmd2;
  int16_t speedR_meas;
  int16_t speedL_meas;
  int16_t batVoltage;
  int16_t boardTemp;
  uint16_t cmdLed;
  uint16_t checksum;
} robot_feedback;

LiquidCrystal_I2C lcd(ADDRESS_I2C_LCD, 20, 4);

enum item_mode_lcd
{
  JOYSTICK,
  FEEDBACK,
  PINOUT,
};

byte mode_print_lcd = JOYSTICK;
int last_mode_print_lcd = -1;
static int32_t last_print_lcd_time = 0;

bool connection_lcd = LOW;

// coordonee abscisse et ordonnee

int x_speed[3];
int y_speed[3];

int x_steer[3];
int y_steer[3];


// ***********************************************************************
// ****************   Inclusion des sous programmes   ********************
// ***********************************************************************

#include "i2c.h"
#include "util.h"
#include "rom.h"
#include "button.h"
#include "lcd.h"
#include "joystick.h"
#include "config.h"
#include "nrf.h"
#include "buzzer.h"

// ***********************************************************************
// ***********************     FUNCTION SETUP     ************************
// ***********************************************************************

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  Serial.print(F("nRF24L01+ Joystick\nVersion : "));
  Serial.println(String(VERSION));

  scann_i2c();

  init_eeprom();

  setup_Lcd();

  init_button();

  init_nrf(nRF_robot_address, nRF_joystick_address);

  init_buzzer();
  
}

// ***********************************************************************
// ***********************     FUNCTION LOOP     *************************
// ***********************************************************************

void loop()
{

  unsigned long start_millis_loop = millis();

  read_serial();
  serial_print_pause();
  read_joystick();
  joystate.speed_weapon_send = map(analogRead(PIN_weapon_speed),0,1023,0,255);
  read_button();
  print_lcd();

  if (!nrf_send_data())
  {
    if (millis() - last_ack_send_data_time > 200)
    {
      bip_buzzer(1200,200);
    }
  }
  else
  {
    noTone(PIN_buzzer);
    last_ack_send_data_time = millis();
  }

  nrf_receive_data();

  delay(5);

  /*Serial.print(F("temp loop ="));
  Serial.println(String(millis() - start_millis_loop));*/
}

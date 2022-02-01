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

#define VERSION "1.0.4"
#define value_to_init_eeprom 144 //change this value to erase default eeprom
#define ADDRESS_I2C_LCD 0x26

#define nRF_CE 9
#define nRF_CSn 10

const byte nRF_robot_address[6] = "ABcd0";
const byte nRF_joystick_address[6] = "EFgh1";

// ***********************************************************************
// ********************     VARIABLES GLOBALES     ***********************
// ***********************************************************************

byte PIN_joystick_speed;
byte PIN_joystick_steer;

bool serial_print = HIGH;
unsigned long millis_serial_pause = 0;

bool inverse_speed = LOW;
bool inverse_steer = LOW;
bool inverse_send_speed_steer = LOW;

int speed_min;
int speed_middle;
int speed_max;

int steer_min;
int steer_middle;
int steer_max;

bool correction_scale = LOW;

struct joystick_state
{
  byte buttons;
  int steer_send;
  int speed_send;
  int checksum;
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
}; 

byte mode_print_lcd = JOYSTICK;
byte last_mode_print_lcd=-1;

bool connection_lcd = LOW;

// ***********************************************************************
// ****************   Inclusion des sous programmes   ********************
// ***********************************************************************

#include "i2c.h"
#include "util.h"
#include "rom.h"
#include "button.h"
#include "joystick.h"
#include "config.h"
#include "lcd.h"
#include "nrf.h"

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

  Serial.println(F(" init port"));
  /* Initialisation des ports du joystick
   *  Ports en entrées, activer les pull-ups
   */
  DDRD &= 3;
  PORTD |= 0xFC;
  DDRB &= 0xFE;
  PORTB |= 0x01;

  init_nrf(nRF_robot_address,nRF_joystick_address);

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
  read_button();
  print_lcd();

  joystate.checksum = joystate.buttons + joystate.steer_send + joystate.speed_send;

  nrf_send_data();
  nrf_receive_data();

  Serial.print(F("temp loop ="));
  Serial.println(String(millis() - start_millis_loop));
}


#define VERSION "1.0.3"
#define value_to_init_eeprom 144 //change this value to erase default eeprom
#define ADDRESS_I2C_LCD 0x26

// ***********************************************************************
// ****************   Inclusion des bibliothèques   **********************
// ***********************************************************************

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "RF24.h"
#include "printf.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

// ***********************************************************************
// ********************     VARIABLES GLOBALES     ***********************
// ***********************************************************************

const byte nRF_robot_address[6] = "lutin";
const byte nRF_joystick_address[6] = "baball";

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

const byte nRF_CE = 9;
const byte nRF_CSn = 10;
RF24 nRF(nRF_CE, nRF_CSn);

struct joystick_state
{
  byte buttons;
  int steer_send;
  int speed_send;
  int checksum;
} joystate;

int steer_read;
int speed_read;


LiquidCrystal_I2C lcd(ADDRESS_I2C_LCD, 20, 4);
bool connection_lcd = LOW;

// ***********************************************************************
// ****************   Inclusion des sous programmes   ********************
// ***********************************************************************

#include "i2c.h"
#include "rom.h"
#include "button.h"
#include "joystick.h"
#include "config.h"
#include "lcd.h"

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

  Serial.println(F(" init nrf"));
  /* Initialisation de la radio nRF24L01 */
  printf_begin();
  nRF.begin();
  nRF.openWritingPipe(nRF_robot_address);
  nRF.openReadingPipe(1, nRF_joystick_address);
  nRF.printDetails();

  Serial.print(F(" end setup"));
}

// ***********************************************************************
// ***********************     FUNCTION LOOP     *************************
// ***********************************************************************

void loop()
{

  unsigned int temps_loop_depart = millis();

  read_serial();
  print_serial_pause();
  read_joystick();
  read_button();
  print_lcd();


  /* Envoie l'état du joystick au robot
   *  Si le robot ne répond pas dans le délai imparti,
   *  affiche un message d'erreur dans la console
   */

  joystate.checksum = joystate.buttons + joystate.steer_send + joystate.speed_send;

  if (!nRF.write(&joystate, sizeof(struct joystick_state)))
  {
    if (serial_print)
    {
      Serial.println(F("(Not Ack'd from bot)"));
    }
  }
  else
  {
    if (serial_print)
    {
      Serial.println(F("(OK)"));
    }
  }

  /* Attente de 20ms avant le prochain envoi */
  delay(20);

  Serial.print(F("temp loop ="));
  Serial.println(String(millis() - temps_loop_depart));
}

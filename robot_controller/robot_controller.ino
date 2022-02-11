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
#include "Polynomial.h"

// ***********************************************************************
// ************************     CONSTANTES    ****************************
// ***********************************************************************

#define VERSION "1.0.6"
#define value_to_init_eeprom 151 //change this value to erase default eeprom
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

bool serial_print = HIGH;
unsigned long millis_serial_pause = 0;
unsigned long last_ack_send_data_time = 0;

bool inverse_speed = LOW;
bool inverse_steer = LOW;
bool inverse_send_speed_steer = LOW;

int speed;
int steer;

int joystick_speed_min;
int joystick_speed_middle;
int joystick_speed_max;

int joystick_steer_min;
int joystick_steer_middle;
int joystick_steer_max;

int send_value_steer_min;
int send_value_steer_max;

int send_value_speed_min;
int send_value_speed_max;

bool correction_scale = LOW;

struct joystick_state
{
  byte buttons;
  int steer_send;
  int speed_send;
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
  JOYSTICK_CURVE,
  FEEDBACK,
  PINOUT,
};

byte mode_print_lcd = JOYSTICK;
int last_mode_print_lcd = -1;
static int32_t last_print_lcd_time = 0;

bool connection_lcd = LOW;

Polynomial curve_positiv_speed(2, 3); //Polynome du second degrée , nombre de points
Polynomial curve_negativ_speed(2, 3); //Polynome du second degrée , nombre de points

Polynomial curve_positiv_steer(2, 3); //Polynome du second degrée , nombre de points
Polynomial curve_negativ_steer(2, 3); //Polynome du second degrée , nombre de points

double coordonee_X_positiv_speed[] = {0, 500, 1000};
double coordonee_Y_positiv_speed[] = {120, 400, 1000};

double coordonee_X_negativ_speed[] = {0, -500, -1000};
double coordonee_Y_negativ_speed[] = {-120, -400, -1000};


double coordonee_X_positiv_steer[] = {0, 500, 1000};
double coordonee_Y_positiv_steer[] = {0, 250, 1000};

double coordonee_X_negativ_steer[] = {0, -500, -1000};
double coordonee_Y_negativ_steer[] = {0, -250, -1000};


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

  //init_buzzer();

  curve_positiv_speed.calcul_coef(coordonee_X_positiv_speed, coordonee_Y_positiv_speed);
  curve_negativ_speed.calcul_coef(coordonee_X_negativ_speed, coordonee_Y_negativ_speed);

  curve_positiv_steer.calcul_coef(coordonee_X_positiv_steer, coordonee_Y_positiv_steer);
  curve_negativ_steer.calcul_coef(coordonee_X_negativ_steer, coordonee_Y_negativ_steer);
  
  Serial.print("a = ");
  Serial.println(curve_positiv_speed.Get_coeficient(2), 15);
  Serial.print("b = ");
  Serial.println(curve_positiv_speed.Get_coeficient(1), 15);
  Serial.print("c = ");
  Serial.println(curve_positiv_speed.Get_coeficient(0), 15);

  Serial.print("a = ");
  Serial.println(curve_negativ_speed.Get_coeficient(2), 15);
  Serial.print("b = ");
  Serial.println(curve_negativ_speed.Get_coeficient(1), 15);
  Serial.print("c = ");
  Serial.println(curve_negativ_speed.Get_coeficient(0), 15);


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

  if (!nrf_send_data())
  {
    if (millis() - last_ack_send_data_time > 200)
    {
      //bip_buzzer(1200,200);
    }
  }
  else
  {
    noTone(PIN_buzzer);
    last_ack_send_data_time = millis();
  }

  nrf_receive_data();

  /*Serial.print(F("temp loop ="));
  Serial.println(String(millis() - start_millis_loop));*/
}

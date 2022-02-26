// ***********************************************************************
// *****************************   LIBRARY   *****************************
// ***********************************************************************

#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "RF24.h"
#include "printf.h"

// ***********************************************************************
// ************************     CONSTANTES    ****************************
// ***********************************************************************

#define VERSION "1.0.2"
#define nRF_CE 9
#define nRF_CSn 10

const byte nRF_robot_address[6] = "ABcdSC";
const byte nRF_joystick_address[6] = "EFghSC";

#define HOVER_SERIAL_BAUD 115200 // [-] Baud rate for HoverSerial (used to communicate with the hoverboard)
#define SERIAL_BAUD 115200       // [-] Baud rate for built-in Serial (used for the Serial Monitor)
#define START_FRAME 0xABCD       // [-] Start frme definition for reliable serial communication
#define DEBUG_RX 1

#define HOVER_SERIAL_RX_PIN A3 // ancienement 2
#define HOVER_SERIAL_TX_PIN 4

#define WEAPON_SPEED_PIN 5
#define WEAPON_DIR_PIN 6

// https://github.com/EmanuelFeru/hoverboard-firmware-hack-FOC

// ***********************************************************************
// ********************     VARIABLES GLOBALES     ***********************
// ***********************************************************************

int throttle;
int steering;

struct joystick_state
{
  byte buttons;
  int steer;
  int speed;
  byte speed_weapon;
} joystate;

#define btn_weapon_enable 0x01    // B00000001
#define button_mask_right 0x02    // B00000010
#define button_mask_down 0x04     // B00000100
#define button_mask_left 0x08     // B00001000
#define button_mask_start 0x10    // B00010000
#define button_mask_select 0x20   // B00100000
#define button_mask_joystick 0x40 // B01000000

SoftwareSerial HoverSerial(HOVER_SERIAL_RX_PIN, HOVER_SERIAL_TX_PIN);

// Global variables
uint8_t idx = 0;        // Index for new data pointer
uint16_t bufStartFrame; // Buffer Start Frame
byte *p;                // Pointer declaration for the new received data
byte incomingByte;
byte incomingBytePrev;

typedef struct
{
  uint16_t start;
  int16_t steer;
  int16_t speed;
  uint16_t checksum;
} SerialCommand;
SerialCommand Command;

typedef struct
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
} SerialFeedback;
SerialFeedback Feedback;
SerialFeedback NewFeedback;

static int32_t last_joystick_time = 0;

// ***********************************************************************
// ****************   Inclusion des sous programmes   ********************
// ***********************************************************************

#include "nrf.h"
#include "hover_serial.h"

// ***********************************************************************
// ***********************     FUNCTION SETUP     ************************
// ***********************************************************************

void setup()
{
  pinMode(WEAPON_SPEED_PIN, OUTPUT);
  pinMode(WEAPON_DIR_PIN, OUTPUT);

  Serial.begin(SERIAL_BAUD);
  Serial.print(F("nRF24L01+ Robot\nVersion : "));

  init_nrf(nRF_joystick_address, nRF_robot_address);

  HoverSerial.begin(HOVER_SERIAL_BAUD);

  pinMode(LED_BUILTIN, OUTPUT);
}

// ***********************************************************************
// ***********************     FUNCTION LOOP     *************************
// ***********************************************************************

void loop()
{
  unsigned long start_millis_loop = millis();

  nrf_send_data();

  if (nrf_receive_data())
  {
    /*Serial.print("steer = ");
    Serial.print(joystate.steer);
    Serial.print("\t\tspeed = ");
    Serial.println(joystate.speed);*/

    hoverserial_send(joystate.steer, joystate.speed);

    if (joystate.buttons & btn_weapon_enable)
    {
      analogWrite(WEAPON_SPEED_PIN, joystate.speed_weapon);
    }
    else
    {
      analogWrite(WEAPON_SPEED_PIN, 0);
    }

    last_joystick_time = millis();
  }

  /* Si l'on n'a pas reçu de message de la télécommande depuis plus de 500 ms */
  if (millis() - last_joystick_time > 500)
  {
    // Serial.println("No joystick data !");
    analogWrite(WEAPON_SPEED_PIN, 0);
    hoverserial_send(0, 0);
  }
  hoverserial_receive();

  /*Serial.print(F("temp loop ="));
  Serial.println(String(millis() - start_millis_loop));*/
}

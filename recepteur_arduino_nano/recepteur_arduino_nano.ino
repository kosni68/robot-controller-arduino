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

#define VERSION "1.0.1"
#define nRF_CE 9
#define nRF_CSn 10

const byte nRF_robot_address[6] = "ABcd01";
const byte nRF_joystick_address[6] = "EFgh23";

#define HOVER_SERIAL_BAUD 38400 // [-] Baud rate for HoverSerial (used to communicate with the hoverboard)
#define SERIAL_BAUD 115200      // [-] Baud rate for built-in Serial (used for the Serial Monitor)
#define START_FRAME 0xABCD      // [-] Start frme definition for reliable serial communication
#define TIME_SEND 100           // [ms] Sending time interval
#define SPEED_MAX_TEST 300      // [-] Maximum speed for testing//
#define DEBUG_RX 0

// https://github.com/EmanuelFeru/hoverboard-firmware-hack-FOC

// ***********************************************************************
// ********************     VARIABLES GLOBALES     ***********************
// ***********************************************************************

int throttle;
int steering;

/* Valeur des axes et boutons de la manette */
struct joystick_state
{
  /* <buttons> stocke l'état des 7 boutons de la manette dans 1 octet.
   *  chaque bit de <buttons> est mis à 1 si le bouton correspondant est pressé
   *  Par exemple, pour vérifier si le bouton du haut est pressé, on utilise :
   *  if (joystate.buttons & button_mask_up)
   *  {
   *    ...
   *  }
   */
  byte buttons;
  /* <axis_x> est la valeur analogique de l'axe horizontal du joystick
   *  La valeur est comprise entre 0 et 1023
   *  0    : stick à gauche
   *  1023 : stick à gauche
   *  ~512 : stick au centre
   */

  int axis_x;
  int axis_y;
  int checksum;
} joystate;

const byte button_mask_up = 0x01;       /* Bouton haut ou A */
const byte button_mask_right = 0x02;    /* Bouton droit ou B */
const byte button_mask_down = 0x04;     /* Bouton bas ou C */
const byte button_mask_left = 0x08;     /* Bouton gauche ou D */
const byte button_mask_start = 0x10;    /* Bouton start ou E */
const byte button_mask_select = 0x20;   /* Bouton select ou F */
const byte button_mask_joystick = 0x40; /* Bouton sous le stick */

SoftwareSerial HoverSerial(2, 4); // RX, TX

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
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  Serial.begin(SERIAL_BAUD);
  Serial.print(F("nRF24L01+ Robot\nVersion : "));

  init_nrf(nRF_robot_address, nRF_joystick_address);

  HoverSerial.begin(HOVER_SERIAL_BAUD);

  pinMode(LED_BUILTIN, OUTPUT);
}

int deadZone(int inval, int thres)
{
  if (inval > thres)
  {
    return inval - thres;
  }
  else if (inval < -thres)
  {
    return inval + thres;
  }
  else
  {
    return 0;
  }
}

// ***********************************************************************
// ***********************     FUNCTION LOOP     *************************
// ***********************************************************************

unsigned long iTimeSend = 0;
int iTestMax = SPEED_MAX_TEST;
int iTest = 0;

void loop()
{

  static int32_t last_joystick_time = 0;

  nrf_send_data();
  nrf_receive_data();

  /* Si l'on n'a pas reçu de message de la télécommande depuis plus de 500 ms */
  if (millis() - last_joystick_time > 500)
  {
    Serial.println("No joystick data !");
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
    hoverserial_send(0, 0);
    delay(100);
  }
  // hoverserial_receive();
}

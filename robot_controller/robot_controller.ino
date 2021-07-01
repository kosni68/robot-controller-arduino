#include <SPI.h>
#include "RF24.h"
#include "printf.h"
#include <EEPROM.h>

#define VERSION "1.0.2"
#define value_to_init_eeprom 144 //change this value to erase default eeprom

// ***********************************************************************
// ********************     VARIABLES GLOBALES     ***********************
// ***********************************************************************

const byte nRF_robot_address[6] = "lutin";
const byte nRF_joystick_address[6] = "baballe";

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

const byte button_mask_up = 0x01;
const byte button_mask_right = 0x02;
const byte button_mask_down = 0x04;
const byte button_mask_left = 0x08;
const byte button_mask_start = 0x10;
const byte button_mask_select = 0x20;
const byte button_mask_joystick = 0x40;

// ***********************************************************************
// ***********************     FUNCTION SETUP     ************************
// ***********************************************************************

void setup()
{
  Serial.begin(115200);
  Serial.println("nRF24L01+ Joystick");
  Serial.println("Version : " + String(VERSION));
  printf_begin();
  init_eeprom();

  /* Initialisation des ports du joystick
   *  Ports en entrées, activer les pull-ups
   */
  DDRD &= 3;
  PORTD |= 0xFC;
  DDRB &= 0xFE;
  PORTB |= 0x01;

  /* Initialisation de la radio nRF24L01 */
  nRF.begin();
  nRF.openWritingPipe(nRF_robot_address);
  nRF.openReadingPipe(1, nRF_joystick_address);
  nRF.printDetails();
}

// ***********************************************************************
// ***********************     FUNCTION LOOP     *************************
// ***********************************************************************

void loop()
{
  read_serial();
  print_serial_pause();

  read_joystick();

  if (joystate.buttons & button_mask_up)
    Serial.print("UP ");
  if (joystate.buttons & button_mask_right)
    Serial.print("RIGHT ");
  if (joystate.buttons & button_mask_down)
    Serial.print("DOWN ");
  if (joystate.buttons & button_mask_left)
    Serial.print("LEFT ");
  if (joystate.buttons & button_mask_start)
    Serial.print("START ");
  if (joystate.buttons & button_mask_select)
    Serial.print("SELECT ");
  if (joystate.buttons & button_mask_joystick)
    Serial.print("JOYSTICK ");

  /* Envoie l'état du joystick au robot
   *  Si le robot ne répond pas dans le délai imparti,
   *  affiche un message d'erreur dans la console
   */

  joystate.checksum = joystate.buttons+joystate.steer_send+joystate.speed_send;

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
      Serial.println("(OK)");
    }
  }

  /* Attente de 20ms avant le prochain envoi */
  delay(20);
}

// ***********************************************************************
// ***********************    READ JOYSTICK    ***************************
// ***********************************************************************

void read_joystick()
{
  joystate.buttons = (~PIND >> 2) & 0x7F;
  if (PINB & 0x01)
    joystate.buttons &= ~button_mask_joystick;

  speed_read = analogRead(PIN_joystick_speed);
  steer_read = analogRead(PIN_joystick_steer);

  if (correction_scale)
  {
    joystate.speed_send = scaling(speed_read, speed_min, speed_middle, speed_max);
    joystate.steer_send = scaling(steer_read, steer_min, steer_middle, steer_max);
  }
  else
  {
    joystate.speed_send = speed_read;
    joystate.steer_send = steer_read;
  }

  if (inverse_speed)
    joystate.speed_send = 1023 - joystate.speed_send;
  if (inverse_steer)
    joystate.steer_send = 1023 - joystate.steer_send;

  if (serial_print)
  {
    Serial.print("\t\t speed_read = ");
    Serial.print(speed_read);
    Serial.print("\t steer_read = ");
    Serial.println(steer_read);
    Serial.print("\t\t speed_send = ");
    Serial.print(joystate.speed_send);
    Serial.print("\t steer_send = ");
    Serial.println(joystate.steer_send);
  }

  if (inverse_send_speed_steer)
  {
    int speed = joystate.speed_send;
    joystate.speed_send = joystate.steer_send;
    joystate.steer_send = speed;
  }
}

// ***********************************************************************
// ***********************    CALIBRATION AUTO    ************************
// ***********************************************************************

void calibration_joystick(byte PIN_joystick, int &value_read)
{
  unsigned long currentMillis = millis();
  unsigned long previousMillis = millis();

  const long interval = 3000;
  long value_read_moyenne = 0;
  int nb_value = 0;

  while (currentMillis - previousMillis < interval)
  {
    value_read = analogRead(PIN_joystick);
    value_read_moyenne += value_read;
    nb_value++;
    currentMillis = millis();
  }

  value_read = value_read_moyenne / nb_value;
}

void calibration_joystick(byte PIN_joystick, int &value_read_min, int &value_read_max)
{
  unsigned long currentMillis = millis();
  unsigned long previousMillis = millis();

  const long interval = 8000;
  int value_read = 0;
  value_read_min = 999999;
  value_read_max = 0;

  while (currentMillis - previousMillis < interval)
  {
    value_read = analogRead(PIN_joystick);

    if (value_read < value_read_min)
      value_read_min = value_read;

    if (value_read > value_read_max)
      value_read_max = value_read;

    currentMillis = millis();
  }
}

int calibration_auto()
{
  Serial.println("\n **** keep the speed joystick in the central position ****");
  calibration_joystick(PIN_joystick_speed, speed_middle);

  Serial.println("\n **** keep the steer joystick in the central position ****");
  calibration_joystick(PIN_joystick_steer, steer_middle);

  Serial.println("\n **** move the speed joystick up and down ****");
  calibration_joystick(PIN_joystick_speed, speed_min, speed_max);

  Serial.println("\n **** move the steer joystick from right to left ****");
  calibration_joystick(PIN_joystick_steer, steer_min, steer_max);

  correction_scale = HIGH;
  save_eeprom();
}

// ***********************************************************************
// ***************************    SCALING    *****************************
// ***********************************************************************

int scaling(int analogread, int analogread_value_min, int analogread_value_middle, int analogread_value_max)
{
  int valeur_retournee;
  int zero = 0;
  int moyen = 512;
  int maximum = 1023;
  int deadzone = 30;

  if (analogread > analogread_value_middle - deadzone && analogread < analogread_value_middle + deadzone)
    valeur_retournee = moyen;

  else if (analogread > analogread_value_middle + deadzone)
  {
    valeur_retournee = map(analogread, analogread_value_middle + deadzone, analogread_value_max, moyen, maximum);
    if (valeur_retournee > maximum)
      return maximum;
  }
  else if (analogread < analogread_value_middle - deadzone)
  {
    valeur_retournee = map(analogread, analogread_value_middle - deadzone, analogread_value_min, moyen, zero);
    if (valeur_retournee < zero)
      return zero;
  }
  return valeur_retournee;
}

// ***********************************************************************
// ***********************    MONITEUR SERIE    **************************
// ***********************************************************************

void read_serial()
{
  // variable containing the character to read
  char carlu = 0;
  String mot_recu = "";

  int cardispo = Serial.available();
  if (cardispo > 0)
  {

    while (cardispo > 0) // tant qu'il y a des caractères à lire
    {
      carlu = Serial.read();         // on lit le caractère
      cardispo = Serial.available(); // on relit le nombre de caractères dispo
      if (byte(carlu) != 10)
      {
        if (carlu != '\r' && carlu != '\n')
          mot_recu += carlu;
      }
    }

    Serial.print("\n\n mot_recu : ");
    Serial.println(mot_recu);
    serial_print = LOW;

    if (mot_recu == "$$")
    {
      Serial.println("$# : Calibration automatique ");
      Serial.println("$1 : PIN speed = " + String(PIN_joystick_speed));
      Serial.println("$2 : PIN steer = " + String(PIN_joystick_steer));
      Serial.println("$3 : inverse speed = " + String(inverse_speed));
      Serial.println("$4 : inverse steer = " + String(inverse_steer));
      Serial.println("$5 : inverse send speed_steer = " + String(inverse_send_speed_steer));
      Serial.println("$6 : enable correction scale = " + String(correction_scale));
      Serial.println("Reference speed :");
      Serial.println("$7 : speed value min = " + String(speed_min));
      Serial.println("$8 : speed value middle = " + String(speed_middle));
      Serial.println("$9 : speed value max = " + String(speed_max));
      Serial.println("Reference steer :");
      Serial.println("$10 : steer value min = " + String(steer_min));
      Serial.println("$11 : steer value middle = " + String(steer_middle));
      Serial.println("$12 : steer value max = " + String(steer_max));
      Serial.println("\n");
    }
    else if (mot_recu == "$#")
    {
      Serial.println("\n \t\t **** Calibration automatique ****");
      calibration_auto();
    }
    else if (mot_recu == "$3=1")
    {
      Serial.println("inverse_speed HIGH\n\n");
      inverse_speed = HIGH;
    }
    else if (mot_recu == "$3=0")
    {
      Serial.println("inverse_speed LOW\n\n");
      inverse_speed = LOW;
    }
    else if (mot_recu == "$4=1")
    {
      Serial.println("inverse_steer HIGH\n\n");
      inverse_steer = HIGH;
    }
    else if (mot_recu == "$4=0")
    {
      Serial.println("inverse_steer LOW\n\n");
      inverse_steer = LOW;
    }
    else if (mot_recu == "$5=1")
    {
      Serial.println("inverse_send_speed_steer HIGH\n\n");
      inverse_send_speed_steer = HIGH;
    }
    else if (mot_recu == "$5=0")
    {
      Serial.println("inverse_send_speed_steer LOW\n\n");
      inverse_send_speed_steer = LOW;
    }
    else if (mot_recu == "$6=1")
    {
      Serial.println("correction_scale HIGH\n\n");
      correction_scale = HIGH;
    }
    else if (mot_recu == "$6=0")
    {
      Serial.println("correction_scale LOW\n\n");
      correction_scale = LOW;
    }
    else
    {
      if (mot_recu.indexOf("$1=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        PIN_joystick_speed = mot_recu.toInt();
        Serial.print("PIN_joystick_speed :");
        Serial.println(PIN_joystick_speed);
      }
      else if (mot_recu.indexOf("$2=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        PIN_joystick_steer = mot_recu.toInt();
        Serial.print("PIN_joystick_steer :");
        Serial.println(PIN_joystick_steer);
      }
      else if (mot_recu.indexOf("$7=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        speed_min = mot_recu.toInt();
        Serial.print("speed value min :");
        Serial.println(speed_min);
      }
      else if (mot_recu.indexOf("$8=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        speed_middle = mot_recu.toInt();
        Serial.print("speed value middle :");
        Serial.println(speed_middle);
      }
      else if (mot_recu.indexOf("$9=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        speed_max = mot_recu.toInt();
        Serial.print("speed value max :");
        Serial.println(speed_max);
      }
      else if (mot_recu.indexOf("$10=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        steer_min = mot_recu.toInt();
        Serial.print("steer value min :");
        Serial.println(steer_min);
      }
      else if (mot_recu.indexOf("$11=", 0) == 0)
      {
        mot_recu.remove(0, 4);
        steer_middle = mot_recu.toInt();
        Serial.print("steer value middle :");
        Serial.println(steer_middle);
      }
      else if (mot_recu.indexOf("$12=", 0) == 0)
      {
        mot_recu.remove(0, 4);
        steer_max = mot_recu.toInt();
        Serial.print("steer value max :");
        Serial.println(steer_max);
      }
    }
    save_eeprom();
  }
}

void print_serial_pause()
{
  if (serial_print)
  {
    millis_serial_pause = millis();
  }
  else
  {
    if (millis() - millis_serial_pause > 6000)
    {
      serial_print = HIGH;
    }
  }
}

// ***********************************************************************
// **************************    EEPROM    *******************************
// ***********************************************************************

void init_eeprom()
{
  if (EEPROM.read(0) != value_to_init_eeprom)
  {
    Serial.println(F("\n\t --- variable_default --- \n"));
    variable_default();
    save_eeprom();
    EEPROM.write(0, value_to_init_eeprom);
  }
  else
  {
    Serial.println(F("\n\t --- read_eeprom --- \n"));
    read_eeprom();
  }
}

void variable_default()
{
  PIN_joystick_speed = 15;
  PIN_joystick_steer = 16;
  inverse_speed = 0;
  inverse_steer = 0;
  inverse_send_speed_steer = 0;
  correction_scale = 0;
  speed_min = 0;
  speed_middle = 512;
  speed_max = 1023;
  steer_min = 0;
  steer_middle = 512;
  steer_max = 1023;
}

void save_eeprom()
{
  byte byte_position = 1;

  EEPROM.put(byte_position, PIN_joystick_speed);
  byte_position+=sizeof(PIN_joystick_speed);

  EEPROM.put(byte_position, PIN_joystick_steer);
  byte_position+=sizeof(PIN_joystick_steer);

  EEPROM.put(byte_position, inverse_speed);
  byte_position+=sizeof(inverse_speed);

  EEPROM.put(byte_position, inverse_steer);
  byte_position+=sizeof(inverse_steer);

  EEPROM.put(byte_position, correction_scale);
  byte_position+=sizeof(correction_scale);

  EEPROM.put(byte_position, inverse_send_speed_steer);
  byte_position+=sizeof(inverse_send_speed_steer);

  EEPROM.put(byte_position, speed_min);
  byte_position+=sizeof(speed_min);

  EEPROM.put(byte_position, speed_middle);
  byte_position+=sizeof(speed_middle);

  EEPROM.put(byte_position, speed_max);
  byte_position+=sizeof(speed_max);

  EEPROM.put(byte_position, steer_min);
  byte_position+=sizeof(steer_min);

  EEPROM.put(byte_position, steer_middle);
  byte_position+=sizeof(steer_middle);

  EEPROM.put(byte_position, steer_max);
  byte_position+=sizeof(steer_max);
}

void read_eeprom()
{
  byte byte_position = 1;

  EEPROM.get(byte_position, PIN_joystick_speed);
  byte_position+=sizeof(PIN_joystick_speed);

  EEPROM.get(byte_position, PIN_joystick_steer);
  byte_position+=sizeof(PIN_joystick_steer);
  
  EEPROM.get(byte_position, inverse_speed);
  byte_position+=sizeof(inverse_speed);
  
  EEPROM.get(byte_position, inverse_steer);
  byte_position+=sizeof(inverse_steer);
  
  EEPROM.get(byte_position, correction_scale);
  byte_position+=sizeof(correction_scale);
  
  EEPROM.get(byte_position, inverse_send_speed_steer);
  byte_position+=sizeof(inverse_send_speed_steer);
  
  EEPROM.get(byte_position, speed_min);
  byte_position+=sizeof(speed_min);
  
  EEPROM.get(byte_position, speed_middle);
  byte_position+=sizeof(speed_middle);
  
  EEPROM.get(byte_position, speed_max);
  byte_position+=sizeof(speed_max);
  
  EEPROM.get(byte_position, steer_min);
  byte_position+=sizeof(steer_min);
  
  EEPROM.get(byte_position, steer_middle);
  byte_position+=sizeof(steer_middle);
  
  EEPROM.get(byte_position, steer_max);
  byte_position+=sizeof(steer_max);
  
}

// ***********************************************************************
// ****************    CONFIG VIA MONITEUR SERIE    **********************
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

    Serial.print(F("\n\n mot_recu : "));
    Serial.println(mot_recu);
    serial_print = LOW;

    if (mot_recu == "$$")
    {
      Serial.println(F("$# : Calibration automatique "));

      Serial.println(F("PIN :"));
      Serial.print(F("$1 : PIN speed = "));
      Serial.println(PIN_joystick_speed);
      Serial.print(F("$2 : PIN steer = "));
      Serial.println(PIN_joystick_steer);
      Serial.print(F("$3 : PIN buzzer = "));
      Serial.println(PIN_buzzer);

      Serial.println(F("Enable/disable :"));
      Serial.print(F("$10 : inverse speed = "));
      Serial.println(inverse_speed);
      Serial.print(F("$11 : inverse steer = "));
      Serial.println(inverse_steer);
      Serial.print(F("$12 : inverse send speed_steer = " ));
      Serial.println(inverse_send_speed_steer);
      Serial.print(F("$13 : enable correction scale = "));
      Serial.println(correction_scale);

      Serial.println(F("Analog read joystick speed :"));
      Serial.print(F("$21 : speed value min = "));
      Serial.println(joystick_speed_min);
      Serial.print(F("$22 : speed value middle = "));
      Serial.println(joystick_speed_middle);
      Serial.print(F("$23 : speed value max = "));
      Serial.println(joystick_speed_max);

      Serial.println(F("Analog read joystick steer :"));
      Serial.print(F("$24 : steer value min = "));
      Serial.println(joystick_steer_min);
      Serial.print(F("$25 : steer value middle = "));
      Serial.println(joystick_steer_middle);
      Serial.print(F("$26 : steer value max = "));
      Serial.println(joystick_steer_max);

      Serial.println(F("Send value Speed :"));
      Serial.print(F("$31 : send speed min = "));
      Serial.println(send_value_steer_min);
      Serial.print(F("$32 : send speed max = "));
      Serial.println(send_value_steer_max);

      Serial.println(F("Send value Steer :"));
      Serial.print(F("$33 : send steer min = "));
      Serial.println(send_value_speed_min);
      Serial.print(F("$34 : send steer max = "));
      Serial.println(send_value_speed_max);

      Serial.println(F("\n"));
    }
    else if (mot_recu == "$#")
    {
      Serial.println(F("\n \t\t **** Calibration automatique ****"));
      calibration_auto();
    }
    else if (mot_recu == "$10=1")
    {
      Serial.println(F("inverse_speed HIGH\n\n"));
      inverse_speed = HIGH;
    }
    else if (mot_recu == "$10=0")
    {
      Serial.println(F("inverse_speed LOW\n\n"));
      inverse_speed = LOW;
    }
    else if (mot_recu == "$11=1")
    {
      Serial.println(F("inverse_steer HIGH\n\n"));
      inverse_steer = HIGH;
    }
    else if (mot_recu == "$11=0")
    {
      Serial.println(F("inverse_steer LOW\n\n"));
      inverse_steer = LOW;
    }
    else if (mot_recu == "$12=1")
    {
      Serial.println(F("inverse_send_speed_steer HIGH\n\n"));
      inverse_send_speed_steer = HIGH;
    }
    else if (mot_recu == "$12=0")
    {
      Serial.println(F("inverse_send_speed_steer LOW\n\n"));
      inverse_send_speed_steer = LOW;
    }
    else if (mot_recu == "$13=1")
    {
      Serial.println(F("correction_scale HIGH\n\n"));
      correction_scale = HIGH;
    }
    else if (mot_recu == "$13=0")
    {
      Serial.println(F("correction_scale LOW\n\n"));
      correction_scale = LOW;
    }
    else
    {
      if (mot_recu.indexOf("$1=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        PIN_joystick_speed = mot_recu.toInt();
        Serial.print(F("PIN_joystick_speed :"));
        Serial.println(PIN_joystick_speed);
      }
      else if (mot_recu.indexOf("$2=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        PIN_joystick_steer = mot_recu.toInt();
        Serial.print(F("PIN_joystick_steer :"));
        Serial.println(PIN_joystick_steer);
      }
      else if (mot_recu.indexOf("$3=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        PIN_buzzer = mot_recu.toInt();
        Serial.print(F("PIN_buzzer :"));
        Serial.println(PIN_buzzer);
      }
      else if (mot_recu.indexOf("$21=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        joystick_speed_min = mot_recu.toInt();
        Serial.print(F("speed value min :"));
        Serial.println(joystick_speed_min);
      }
      else if (mot_recu.indexOf("$22=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        joystick_speed_middle = mot_recu.toInt();
        Serial.print(F("speed value middle :"));
        Serial.println(joystick_speed_middle);
      }
      else if (mot_recu.indexOf("$23=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        joystick_speed_max = mot_recu.toInt();
        Serial.print(F("speed value max :"));
        Serial.println(joystick_speed_max);
      }
      else if (mot_recu.indexOf("$24=", 0) == 0)
      {
        mot_recu.remove(0, 3);
        joystick_steer_min = mot_recu.toInt();
        Serial.print(F("steer value min :"));
        Serial.println(joystick_steer_min);
      }
      else if (mot_recu.indexOf("$25=", 0) == 0)
      {
        mot_recu.remove(0, 4);
        joystick_steer_middle = mot_recu.toInt();
        Serial.print(F("steer value middle :"));
        Serial.println(joystick_steer_middle);
      }
      else if (mot_recu.indexOf("$26=", 0) == 0)
      {
        mot_recu.remove(0, 4);
        joystick_steer_max = mot_recu.toInt();
        Serial.print(F("steer value max :"));
        Serial.println(joystick_steer_max);
      }
      else if (mot_recu.indexOf("$31=", 0) == 0)
      {
        mot_recu.remove(0, 4);
        send_value_speed_min = mot_recu.toInt();
        Serial.print(F("send value speed min :"));
        Serial.println(send_value_speed_min);
      }
      else if (mot_recu.indexOf("$32=", 0) == 0)
      {
        mot_recu.remove(0, 4);
        send_value_speed_max = mot_recu.toInt();
        Serial.print(F("send value speed max :"));
        Serial.println(send_value_speed_max);
      }
      else if (mot_recu.indexOf("$33=", 0) == 0)
      {
        mot_recu.remove(0, 4);
        send_value_steer_min = mot_recu.toInt();
        Serial.print(F("send value steer min :"));
        Serial.println(send_value_steer_min);
      }
      else if (mot_recu.indexOf("$34=", 0) == 0)
      {
        mot_recu.remove(0, 4);
        send_value_steer_max = mot_recu.toInt();
        Serial.print(F("send value steer max :"));
        Serial.println(send_value_steer_max);
      }
    }
    save_eeprom();
  }
}

void serial_print_pause()
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

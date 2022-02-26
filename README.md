# robot-controller-arduino

two-way communication between a controller and a robot.

• The controller based on arduino communicates with an nrf 24 module.
the controller uses an i2c lcd screen for print joystick value and robot feedback.

• The robot based on arduino communicates with the joystick via an nrf 24 module. arduino communicates in serial link with a hacked hoverboard card for wheel control.

• Firmware hack the hoverboard find this Link : https://github.com/EFeru/hoverboard-firmware-hack-FOC

• This variant Usart must be used to control the board : https://github.com/EFeru/hoverboard-firmware-hack-FOC/wiki/Variant-USART

# TODO liste

• compte a rebour calibration
• inverse steer speed a supprimer
• enable weapon variation parametre
• weapon speed a parametrer
Créer une animation décorative au démarrage aussi

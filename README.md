# robot-controller-arduino

two-way communication between a controller and a robot.

the controller based on arduino communicates with an nrf 24 module.
the controller uses an i2c lcd screen for print joystick value and robot feedback.

the robot based on arduino communicates with the joystick via an nrf 24 module. arduino communicates in serial link with a hacked hoverboard card for wheel control.
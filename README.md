# Simple Leather Cutter Machine
A simple arduino based leather strip cutting machine.
The menu system uses the amazing Magic Designs' MD_Menu library for 2 line displays. The library puts as much as possible in FLASH reducing RAM consumption. Settings are stored in EEPROM.
## Hardware
- Nano CNC Shield v4
- Arduino Nano
- 2 x 8825 Stepstick Driver
- i2c 16x2 LCD Display
- Rotary encoder with button select
- 12VDC 2A power supply
## Libraries required
- MD_Menu (https://github.com/MajicDesigns/MD_Menu)
- MD_REncoder (https://github.com/MajicDesigns/MD_REncoder)
- LiquidCrystal_I2C (https://github.com/johnrickman/LiquidCrystal_I2C)
- SpeedyStepper (https://github.com/Stan-Reifel/SpeedyStepper)
- EEPROM (Included in Arduino IDE)
## Connections
- The Y axis is used to pull the leather and the Z axis for cutting
- Note that the pinout of my Nano CNC Shield v4 was different from documentation found online. The step and direction pins were reversed.
- The rotary encoder module is connected to pins A0, A1 with A2 being the select button (see Nav.cpp)
- The i2c LCD display is on address 0x27 (see Disp.cpp)
### Nano CNC Shield v4 pin definitions
Function | Pin
--- | ---
Enable | 8
X Direction | 2
Y Direction | 3
Z Direction | 4
X Step | 5
Y Step | 6
Z Step | 7
Home | 11 (Z-)
Stop | 12
EncA | A0 (Abort)
EncB | A1 (Hold)
Select | A2 (Resume)
## Menu Control
Rotate changes the menu options, single click selects.
Long click returns to previous menu.
After 5 seconds the menu returns to the main menu.
### Settings
#### Speed (steps/sec)
Max speed of both steppers
#### Acceleration (steps/sec<sup>2</sup>)
Acceleration of both steppers in steps per second<sup>2</sup>
#### Length
Target of length of strip in mm
#### Quantity
Quantity of strips to cut
#### Steps/mm
The quantity of steps required to extrude 1 mm
#### Retract
The quantity of steps to retract after cutting (prevents sticking)
#### Stroke
The length of the cut stroke in steps
#### Save to EEPROM
Save settings to EEPROM
### Job
#### Start
Start cutting using the current settings

 
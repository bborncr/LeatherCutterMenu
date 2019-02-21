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
- Note that the pinout of my Nano CNC Shield v4 was different from documentation I found online. I had to manually test the pins.
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
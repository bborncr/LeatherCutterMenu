#include "Menu.h"

// Routines for specific display hardware, defining
// - Appropriate header files
// - Hardware definitions
// - Global library objects
// - Display callback function

#if DISPLAY_SERIAL
// Display the output to the Serial Monitor
// This is useful for debugging and troubleshooting the structure of the
// menu definitions without using the final output device.

bool display(MD_Menu::userDisplayAction_t action, char *msg)
{
  switch (action)
  {
    case MD_Menu::DISP_INIT:
      Serial.begin(BAUD_RATE);
      break;

    case MD_Menu::DISP_CLEAR:
      Serial.print("\n-> CLS");
      break;

    case MD_Menu::DISP_L0:
      Serial.print("\n0> ");
      Serial.print(msg);
      break;

    case MD_Menu::DISP_L1:
      Serial.print("\n1> ");
      Serial.print(msg);
      break;
  }

  return (true);
}
#endif

#if DISPLAY_LCDSHIELD
// Output display to a one of 2 line LCD display.
// For a one line display, comment out the L0 handling code.
// The output display line is cleared with spaces before the
// requested message is shown.
//#include <LiquidCrystal.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// LCD display definitions
#define  LCD_ROWS  2
#define  LCD_COLS  16

static LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

bool display(MD_Menu::userDisplayAction_t action, char *msg)
{
  static char szLine[LCD_COLS + 1] = { '\0' };

  switch (action)
  {
    case MD_Menu::DISP_INIT:
      lcd.init();
      lcd.backlight();
      lcd.clear();
      lcd.noCursor();
      memset(szLine, ' ', LCD_COLS);
      lcd.setCursor(0, 0);
      lcd.print("CRCibernetica");
      lcd.setCursor(0, 1);
      lcd.print("Click for Menu");
      break;

    case MD_Menu::DISP_CLEAR:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CRCibernetica");
      lcd.setCursor(0, 1);
      lcd.print("Click for Menu");
      break;

    case MD_Menu::DISP_L0:
      lcd.setCursor(0, 0);
      lcd.print(szLine);
      lcd.setCursor(0, 0);
      lcd.print(msg);
      break;

    case MD_Menu::DISP_L1:
      lcd.setCursor(0, 1);
      lcd.print(szLine);
      lcd.setCursor(0, 1);
      lcd.print(msg);
      break;
  }

  return (true);
}
#endif

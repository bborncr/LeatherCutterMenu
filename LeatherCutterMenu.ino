#include "Menu.h"
#include <SpeedyStepper.h>
#include <EEPROM.h>

#define EN 8 // stepper motor enable, low level effective
#define X_DIR 2 //X axis, stepper motor direction control
#define Y_DIR 3 //y axis, stepper motor direction control
#define Z_DIR 4 //zaxis, stepper motor direction control
#define X_STP 5 //x axis, stepper motor control
#define Y_STP 6 //y axis, stepper motor control
#define Z_STP 7 //z axis, stepper motor control
#define STOP_BUTTON_PIN 12 //emergency stop

SpeedyStepper stepperY;
SpeedyStepper stepperZ;

// ID of the settings block
#define CONFIG_VERSION "v01"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

// Settings structure
struct StoreStruct {
  // This is for mere detection if they are your settings
  char version[4];
  // The variables of your settings (eg. settings.spd, settings.accel)
  int16_t spd, accel;
  int8_t lngth, quantity;
} settings = {
  CONFIG_VERSION,
  // The default values
  1000, 1000,
  15,
  10
};

void loadConfig() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2])
    for (unsigned int t = 0; t < sizeof(settings); t++)
      *((char*)&settings + t) = EEPROM.read(CONFIG_START + t);
}

void saveConfig() {
  for (unsigned int t = 0; t < sizeof(settings); t++)
    EEPROM.write(CONFIG_START + t, *((char*)&settings + t));
}

// Global menu data and definitions
int16_t spd = 1000;
int16_t accel = 1000;
uint8_t lngth = 20;
uint8_t quantity = 10;
uint8_t JOB_STAGE = 0; // Completed = 0
uint8_t JOB_COUNT = 0;

MD_Menu::value_t vBuf;  // interface buffer for values

// Menu Headers --------
const PROGMEM MD_Menu::mnuHeader_t mnuHdr[] =
{
  { 10, "Main Menu",      10, 12, 0 },
  { 11, "Settings",   20, 24, 0 },
  { 12, "Job Menu",     40, 40, 0 },

};

// Menu Items ----------
const PROGMEM MD_Menu::mnuItem_t mnuItm[] =
{
  // Starting (Root) menu
  { 10, "Settings",  MD_Menu::MNU_MENU, 11 },
  { 11, "Job",      MD_Menu::MNU_MENU, 12 },

  // Settings Submenu
  { 20, "Speed", MD_Menu::MNU_INPUT, 10 },
  { 21, "Acceleration",    MD_Menu::MNU_INPUT, 11 },
  { 22, "Length", MD_Menu::MNU_INPUT, 12 },
  { 23, "Quantity",    MD_Menu::MNU_INPUT, 13 },
  { 24, "Save to EEPROM",    MD_Menu::MNU_INPUT, 14 },

  // Job
  { 40, "Start", MD_Menu::MNU_INPUT, 40 },
  //  { 41, "Start Z",  MD_Menu::MNU_INPUT, 41 },

};

const PROGMEM MD_Menu::mnuInput_t mnuInp[] =
{
  { 10, "Steps",    MD_Menu::INP_INT,   mnuIValueRqst, 4,   -32768, 0, 32767, 0, 10, nullptr },
  { 11, "Steps",    MD_Menu::INP_INT,   mnuIValueRqst, 4,   -32768, 0, 32767, 0, 10, nullptr },
  { 12, "CM",       MD_Menu::INP_INT,   mnuIValueRqst, 4,   0, 0, 255, 0, 10, nullptr },
  { 13, "QTY",      MD_Menu::INP_INT,   mnuIValueRqst, 4,   0, 0, 255, 0, 10, nullptr },
  { 14, "Confirm",  MD_Menu::INP_RUN,   saveSettings,  0,   0, 0, 0, 0, 0, nullptr },

  { 40, "Confirm", MD_Menu::INP_RUN, myMotorCode, 0, 0, 0, 0, 0, 0, nullptr },
  //  { 41, "Start Z", MD_Menu::INP_RUN, myMotorCode, 0, 0, 0, 0, 0, 0, nullptr },

};

// bring it all together in the global menu object
MD_Menu M(navigation, display,        // user navigation and display
          mnuHdr, ARRAY_SIZE(mnuHdr), // menu header data
          mnuItm, ARRAY_SIZE(mnuItm), // menu item data
          mnuInp, ARRAY_SIZE(mnuInp));// menu input data

MD_Menu::value_t *mnuIValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for integers variables
{
  MD_Menu::value_t *r = &vBuf;

  switch (id)
  {
    case 10:
      if (bGet)
        vBuf.value = spd;
      else
      {
        spd = vBuf.value;
        settings.spd = spd;
        Serial.print(F("\nSpeed changed to "));
        Serial.print(spd);
      }
      break;

    case 11:
      if (bGet)
        vBuf.value = accel;
      else
      {
        accel = vBuf.value;
        settings.accel = accel;
        Serial.print(F("\nAcceleration changed to "));
        Serial.print(accel);
      }
      break;
    case 12:
      if (bGet)
        vBuf.value = lngth;
      else
      {
        lngth = vBuf.value;
        settings.lngth = lngth;
        Serial.print(F("\nLength changed to "));
        Serial.print(lngth);
      }
      break;
    case 13:
      if (bGet)
        vBuf.value = quantity;
      else
      {
        quantity = vBuf.value;
        settings.quantity = quantity;
        Serial.print(F("\nQuantity changed to "));
        Serial.print(quantity);
      }
      break;

    default:
      r = nullptr;
      break;
  }

  return (r);
}

MD_Menu::value_t *myMotorCode(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for run code input
// Only use the index here
{
  if (id == 40 && bGet == 0) {
    executeJob();
  }
  return (nullptr);
}

MD_Menu::value_t *saveSettings(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for run code input
// Only use the index here
{
  if (id == 14 && bGet == 0) {

    Serial.print(F("\nSaving settings to EEPROM"));
    saveConfig();

  }

  return (nullptr);
}

void executeJob() {
  digitalWrite(EN, LOW); // Enable steppers
  JOB_COUNT = 1;
  JOB_STAGE = 1;
  bool stopFlag = false;
  while (JOB_STAGE == 1 && JOB_COUNT <= quantity) {
    stepperY.setCurrentPositionInSteps(0);
    stepperY.setupMoveInSteps(lngth * 500);
    char buf[11];
    sprintf(buf, "%u of %u", JOB_COUNT, quantity);
    display(MD_Menu::DISP_L1, buf);
    while (!stepperY.motionComplete()) {
      stepperY.processMovement();
      if ((digitalRead(STOP_BUTTON_PIN) == LOW) && (stopFlag == false)) {
        stepperY.setupStop();
        stopFlag = true;
        display(MD_Menu::DISP_L1, "Emergency Stop");
        JOB_STAGE = 0;
      }
    }
    if (JOB_STAGE == 1) {
      stepperZ.setCurrentPositionInSteps(0);
      stepperZ.setupMoveInSteps(200);
      while (!stepperZ.motionComplete()) {
        stepperZ.processMovement();
        if ((digitalRead(STOP_BUTTON_PIN) == LOW) && (stopFlag == false)) {
          stepperZ.setupStop();
          stopFlag = true;
          display(MD_Menu::DISP_L1, "Emergency Stop");
          JOB_STAGE = 0;
        }
      }
    }
    JOB_COUNT++;
  }
  display(MD_Menu::DISP_L1, "Finished");
  digitalWrite(EN, HIGH); // Disable steppers
  delay(1000);
}

// Standard setup() and loop()
void setup(void)
{
  loadConfig();
  spd = settings.spd;
  accel = settings.accel;
  lngth = settings.lngth;
  quantity = settings.quantity;

  Serial.begin(BAUD_RATE);
  Serial.print("\n[Debug]");

  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
  pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(STOP_BUTTON_PIN, INPUT_PULLUP);

  stepperY.connectToPins(Y_STP, Y_DIR);
  stepperZ.connectToPins(Z_STP, Z_DIR);

  stepperY.setSpeedInStepsPerSecond(spd);
  stepperY.setAccelerationInStepsPerSecondPerSecond(accel);
  stepperZ.setSpeedInStepsPerSecond(spd);
  stepperZ.setAccelerationInStepsPerSecondPerSecond(accel);
  digitalWrite(EN, HIGH);

  pinMode(LED_PIN, OUTPUT);

  display(MD_Menu::DISP_INIT);
  setupNav();

  M.begin();
  M.setMenuWrap(true);
  M.setAutoStart(AUTO_START);
  M.setTimeout(MENU_TIMEOUT);
}

void loop(void)
{
  static bool prevMenuRun = true;

  // Detect if we need to initiate running normal user code
  if (prevMenuRun && !M.isInMenu())
    Serial.print("\n\nSWITCHING TO USER'S NORMAL OPERATION\n");
  prevMenuRun = M.isInMenu();

  // If we are not running and not autostart
  // check if there is a reason to start the menu
  if (!M.isInMenu() && !AUTO_START)
  {
    uint16_t dummy;

    if (navigation(dummy) == MD_Menu::NAV_SEL)
      M.runMenu(true);
  }

  M.runMenu();   // just run the menu code
}

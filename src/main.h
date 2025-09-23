#ifndef MAIN_H
#define MAIN_H

#include <ShiftRegister74HC595.h>
#include <Arduino.h>
#include "Button.h"
#include "Led.h"
#include "CompressorControl.h"
#include "HotTankControl.h"
#include "AgitatorPumpControl.h"
#include "FilterStatusCheck.h"
#include <FlowMeter.h>



const String MODEL = "DAVINCI20PLUS";
const int baudRate = 115200;
const int period = 100;

const double minimumFlowRate = 0.1;

const int chillerBucketPeriod = 1000;
const int ledToggleDuration = 1000;

// agitator
const int agitator_enable_time = 120000;
const int agitator_interval = 1800000;

// yellow button
const int autofillButtonPressTime = 2000;
const int calibrationModeButtonPressTime = 5000;
const unsigned long hotWaterSequenceTimeout = 5000;

// autofill
const int autofill_enable = 3000;
const int autofill_interval = 10000;



#define VERSION 1.6
//#define FAN PC14
// #define COMPRESSOR PA8
#define HOTTANK PA15
#define WATERPUMP PA9
// #define AGITATOR PA10

//Chiller bucket leveler
#define LevelSwitchUp PB4
#define LevelSwitchDown PC4

#define CO2_V PC13
#define COLD_V 15
#define HOT_V 14
#define LEVEL_V 13
#define MAIN_V 12
#define SPK_V 11
#define NOZZLE_V 10
#define NO_V 9
#define ALK_V 8

#define YELLOWBUTTON PC5
#define WHITEBUTTON PB0
#define BLUEBUTTON PB1
#define GREENBUTTON PB2
#define REDBUTTON PB10
#define FILTERBUTTON PB11
#define CHILDSAFETYSWITCH PB12

#define OUTPUT3 PB9
#define OUTPUT4 PB8
#define OUTPUT5 PB7
#define OUTPUT6 PB6
#define OUTPUT7 PB5

//LEDs
#define REDLED 5
#define GREENLED 6
#define BLUELED 7

//Button LEDs
#define REDBUTTONLED 4
#define GREENBUTTONLED 3
#define BLUEBUTTONLED 2
#define WHITEBUTTONLED 1
#define YELLOWBUTTONLED 0

const FlowSensorProperties FHKCS_1mm_0deg = {0.4, 39.7, {1.0, 0.995, 0.9925, 0.992, 0.9925, 0.994, 0.995, 0.9975, 1.00125, 1.006}};

enum WaterType
{
  sparkling,
  cold,
  alkaline,
  hot,
  none
};

enum SystemMode
{
  manual,
  autofill,
  calibrate,
  configuration,
  chillerBucketRefill,
  hotTankConfig,
  chillerBucketConfig,
  thirtysecs,
  safety,
  hotWaterDispense
};

struct AutofillVolume
{
  float spkVolume = 250;
  float alkVolume = 250;
  float coldVolume = 250;
};

struct DispenseStats
{
  float totalDispenseVolume = 0;
  float lastDispenseVolume = 0;
  String lastDispenseTimeString = "";
  int lastDispenseTime = 0;
  unsigned long lastDispenseDuration = 0;
  int pourCount = 0;
  float lastDispenseTemperature = 0;
};

// struct NTC
// {
//   float cold_ntc;
//   float hot_ntc;
//   float ntc3;
// };

// struct TempConfig
// {
//   float ChillerBucketMax;
//   float ChillerBucketMin;
//   int HotTankMax;
//   int HotTankMin;
// };

// setup
void startSerial();
void createButtons();
void initializePins();
void initializeFlowMeter();
void setDefaultTemp();
void getCurrentTempConfig();
void checkChillerBucket();
void systemCheck();
void processHotPreset();

// loop
void readTemperature();
// void checkFilterStatus();
// void HotTankControl();
// void CompressorControl();
// void AgitatorPumpControl();

void readSerial();
void checkChildSafetySwitch();
void run();

void updateButtons();
void flashButtonLed(Button& button, int duration);
void flashLed(Led& led, int duration);
void handleButtonStates();
void safetyModeCheck();
// void flashButtonLed(Button &button, int duration);

void meterISR();
void sendStopDispenseStats(WaterType waterType);

// serial commmands
void handleSerialCommand(String incomingSerialCommand);
void handleIotCommand(String arg);
void handleTempCommand(String arg);
void handleSetChillerBucketMaxCommand(String arg);
void handleSetChillerBucketMinCommand(String arg);
void handleSetHotTankMaxCommand(String arg);
void handleSetHotTankMinCommand(String arg);
void handleSetChillerBucketTempCommand(String arg);
void handleSetHotTankTempCommand(String arg);
void handleFlowMeterCommand(String arg);
void handleAutoSparkCommand(String arg);
void handleAutoColdCommand(String arg);
void handleAutoAlkCommand(String arg);
void handleAutoHotCommand(String arg);

void handleRemoteAutoSparkCommand(void);
void handleRemoteAutoColdCommand(void);
void handleRemoteAutoAlkCommand(void);

void handleSetSparklingWaterAutofillVolume(String arg);
void handleSetColdWaterAutofillVolume(String arg);
void handleSetAlkalineWaterAutofillVolume(String arg);

void handleTurnOnSparklingWaterCommand(String arg);
void handleTurnOnColdWaterCommand(String arg);
void handleTurnOnAlkalineWaterCommand(String arg);
void handleTurnOnHotWaterCommand(String arg);

void handleSetDateTimeCommand(String arg);
void handleSetEpochTimeCommand(String arg);
void handleEnableHotTankCommand(String arg);
void handleFactoryResetCommand(String arg);
void handleGetDispenseStatsCommand(String arg);
void handleGetMachineStatusCommand();
void handleResetFilterCommand();
void handleEnableWaterPumpCommand(String arg);
void handleEnableCompressorCommand(String arg);
void handleEnableWaterPumpControlCommand(String arg);
void handleEnableCompressorControlCommand(String arg);
void handleBlackTeaCommand(String arg);
void handleCupNoodleCommand(String arg);



// temp
void cold_default();
void coldest();



// system modes
void manualMode();
void autofillMode();
void calibrateMode();
void configurationMode();
void chillerBucketRefillMode();

// manual mode
void handleGreenButtonStateChanged();
void handleWhiteButtonStateChanged();
void handleBlueButtonStateChanged();
void handleRedButtonStateChanged();
void handleYellowButtonStateChanged();
void handleFilterButtonStateChanged();

// void turnOnAgitatorPump(bool);
// void turnOnHotTank(bool);
void turnOnCompressor(bool);
void turnOnFan(bool);
void printNtcTemperatures();
void checkLastHotWaterDispenseDateTime();
void addButtonPinToArray(int pin);

void updateFlowRate();
void stopAndSaveCalibration();
bool areAnyOtherButtonsPressed(Button button);
// void serial2Remote(void);
// void serialCommand(String);
// void iotSending(void);
// void tempRead(void);

// void Manual_Mode(void);
// void Autofill_Mode(void);
// void Calibrate_Mode(void);
// void Config_Mode(void);

// void autospk(int);

// //void NozzleLED(void);
// void flash_buttonLED(String buttonLED, int speed);
// void ChildSafetySwitch(void);
// void Hottank_Control(void);
// void Compressor_Control(void);
// void Agitator_Control(void);
// void turn_on_waterpump(void);
// void turn_off_waterpump(void);
// void turn_on_compressor(void);
// void turn_off_compressor(void);
// void turn_on_fan(void);
// void turn_off_fan(void);
// void turn_on_hottank(void);
// void turn_off_hottank(void);
// void turn_on_agitator(void);
// void turn_off_agitator(void);
// void Enable_Agitator(void);
void turnOnSparklingWater(bool);
void turnOnColdWater(bool);
void turnOnHotWater(bool);
void turnOnAlkalineWater(bool);
void autofillSparklingWater(int);
void autofillColdWater(int);
void autofillAlkalineWater(int);
void autofillHotWater(int);
void stopAutofill(WaterType, unsigned long);

void turnOnMainValve(bool);
void turnOnColdValve(bool);
void turnOnSparklingValve(bool);
void turnOnCo2Valve(bool);
void turnOnWaterPump(bool);
void turnOnAlkalineValve(bool);
void turnOnHotValve(bool);
void turnOnNozzleValve(bool);
void turnOnLevelValve(bool isEnabled);

void turnOnAllLeds(bool);
void turnOnBlueButtonLed(bool);
void turnOnGreenButtonLed(bool);
void turnOnWhiteButtonLed(bool);
void turnOnYellowButtonLed(bool);
void turnOnRedButtonLed(bool);

void turnOnGreenLed(bool);

bool hasEnoughTimeElapsed(int,unsigned long);
void resetFilter();
void cancelAutofill();
void stopDispense();
void enterSafetyMode();
void stopAllPouring();
void flashAllButtonLeds(int duration);
void sendLogMessage(String msg);
#endif

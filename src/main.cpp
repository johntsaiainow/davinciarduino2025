#include <main.h>
#include <EEPROM.h>
#include <DateTime.h>
#include <temp_read.h>
#include <ArduinoJson.h>

// --- Preset command prototypes ---
void handleBlackTeaCommand(String arg);
void handleCupNoodleCommand(String arg);
void processHotPreset();
void handleEnableHotWaterSafetyCommand(String arg);


//HardwareSerial Serial2(PA3, PA2);
ShiftRegister74HC595<uint8_t(2)> sr(PB13, PB15, PB14);

TempConfig currentTempConfig;
AutofillVolume autofillVolume;
FlowMeter *flowMeter;
NTC ntc;

double firmwareVersion = 2.1;

unsigned long currentTime;
unsigned long currentVolume;
unsigned long totalDispenseVolumeSinceStart;

double currentFlowrate = 0;

unsigned long currentDispenseStartTime = 0;

WaterType waterType = none;
SystemMode systemMode = manual;

bool isIotEnabled = true;

bool hotPresetActive = false;
// Hot preset state machine
enum HotPresetState { HotPresetIdle, HotPresetHeating, HotPresetDispensing };
volatile HotPresetState hotPresetState = HotPresetIdle;
int presetTargetTempC = 0;
int presetVolumeMl    = 0;

int savedHotTankMax = 0;
int savedHotTankMin = 0;
bool isChildSafetySwitchEnabled = false;
// bool isHotTankEnabled = false;
bool isAgitatorPumpEnabled = false;
bool isCompressorEnabled = false;
bool isWaterPumpControlEnabled = true;
bool isFilterOk = false;
bool isDispensing = false;
bool cancelDispense = false;
bool isChillerBucketRefillPaused = false;
bool isFactoryReset = false;
bool isFloatSensorOk = true;
bool isChillerBucketCheckEnabled = true;
bool hasSentChillerBucketLowMessage = false;
bool isCompressorControlEnabled = true; 
bool isWaterPumpEnabled = false;
bool isHotWaterSafetySequenceEnabled = true;

// === NEW: auto-dispense marker ===
bool isAutoDispensing = false;

// === NEW: remote-command hold latches ===
bool holdSparkling = false;
bool holdCold      = false;
bool holdAlkaline  = false;

bool isWaterRefilled = false;

bool isMainValveOn = false;
bool isLevelValveOn = false;
bool isSparklingValveOn = false;
bool isAlkalineValveOn = false;
bool isColdValveOn = false;
bool isHotValveOn = false;
bool isCo2ValveOn = false;
bool isNozzleValveOn = false;
bool isFloatSensorLow = false;

const int eeAddress = 0;
unsigned long previousTime = 0;
unsigned long lastLedFlashTime = 0;

unsigned long agitatorpump_start_time;
unsigned long agitatorpump_stop_time;
unsigned long autofillStartTime = 0;
unsigned long calibrationModeStartTime = 0;
unsigned long hotWaterDispenseSequenceStartTime = 0;

unsigned long dispenseStartTime = 0;
unsigned long chillerBucketRefillStartTime = 0;
unsigned long waterRefillTime = 0;
unsigned long dispenseStopTime = 0;
unsigned long sparklingWaterStartStopTime = 0;
unsigned long alkalineWaterStartStopTime = 0;
unsigned long coldWaterStartStopTime = 0;
unsigned long enterSafetyModeTime = 0;
unsigned long thirtySecStartStopTime = 0;

unsigned long lastReadTempTime = 5000;

int totalDispenseVolumeSinceFilterReset = 0;
int totalDispenseVolumeSinceFilterResetAddress = 0;

int lastHotWaterDispenseTime = 0;
int lastHotWaterDispenseTimeAddress = 0;

int lastFilterResetTime = 0;
int lastFilterResetTimeAddress = 0;

int usedEepromSize = 0;

bool isSerialInJsonMode = false;
int isSerialInJsonModeAddress = 0;

// autofill
unsigned long autoFillStopVolume = 0; // (kept; also cleared where needed)
int compressorEnableCount = 0;

Button blueButton(BLUEBUTTON, BLUEBUTTONLED, sr);
Button redButton(REDBUTTON, REDBUTTONLED, sr);
Button yellowButton(YELLOWBUTTON, YELLOWBUTTONLED, sr);
Button greenButton(GREENBUTTON, GREENBUTTONLED, sr);
Button whiteButton(WHITEBUTTON, WHITEBUTTONLED, sr);
Button filterButton(FILTERBUTTON, -1, sr);

int previouslyPressedButtons[5]; 

Led redLed(REDLED, sr);
Led greenLed(GREENLED, sr);
Led blueLed(BLUELED, sr);

CompressorControl compressorControl;
HotTankControl hotTankControl;
AgitatorPumpControl agitatorPumpControl;
FilterStatusCheck filterStatusCheck;

DispenseStats coldWaterDispenseStats;
DispenseStats alkalineWaterDispenseStats;
DispenseStats sparklingWaterDispenseStats;
DispenseStats hotWaterDispenseStats;

StaticJsonDocument<2048> json;

void setup()
{
  initializePins();
  turnOnAllLeds(false);
  rtcCheck();
  lastHotWaterDispenseTimeAddress = eeAddress + sizeof(currentTempConfig);  
  totalDispenseVolumeSinceFilterResetAddress = lastHotWaterDispenseTimeAddress + sizeof(lastHotWaterDispenseTime);
  lastFilterResetTimeAddress = totalDispenseVolumeSinceFilterResetAddress + sizeof(totalDispenseVolumeSinceFilterReset);
  isSerialInJsonModeAddress = lastFilterResetTimeAddress + sizeof(lastFilterResetTime);
  usedEepromSize = isSerialInJsonModeAddress + sizeof(isSerialInJsonMode);
/*
  if (redButton.startPressTimeString=="")
  {
    redButton.startPressTimeString = getDateTime();
  }
*/
  EEPROM.get(isSerialInJsonModeAddress, isSerialInJsonMode);
  startSerial();
  EEPROM.get(totalDispenseVolumeSinceFilterResetAddress, totalDispenseVolumeSinceFilterReset);
  EEPROM.get(lastFilterResetTimeAddress, lastFilterResetTime);

  if (lastFilterResetTime <= 0)
  {
    handleFactoryResetCommand("");
  }
  // createButtons();
  initializeFlowMeter();
  // setDefaultTemp();
  getCurrentTempConfig();
  readTemperature();
  systemCheck();
  delay(4000);
  turnOnAllLeds(true);
  //EEPROM.put(lastFilterResetTimeAddress, -1);
}

void loop()
{
  readTemperature();
  // checkFilterStatus();
  currentTime = millis();
  run();
  sendLogMessage("Loop: systemMode: " + String(systemMode) + ", isDispensing: " + String(isDispensing));
  
  processHotPreset();
  checkChildSafetySwitch();
  checkChillerBucket();
  agitatorPumpControl.run();
  readSerial();
  safetyModeCheck();
  checkLastHotWaterDispenseDateTime();
  updateFlowRate();

  // EEPROM.get(lastFilterResetTimeAddress, lastFilterResetTime);
  //sendLogMessage(lastFilterResetTime);
  filterStatusCheck.run(totalDispenseVolumeSinceFilterReset, lastFilterResetTime);
  // sendLogMessage(totalDispenseVolumeSinceFilterReset);

  if (millis() > 60000 && isChillerBucketCheckEnabled == true && systemMode != chillerBucketRefill)
  {
    isChillerBucketCheckEnabled = false;
  }

  if (millis() - 5000 > hotWaterDispenseSequenceStartTime && !isDispensing && systemMode == hotWaterDispense)
  {
    memset(previouslyPressedButtons, 0, 5*sizeof(*previouslyPressedButtons));
    turnOnAllLeds(true);
    systemMode = manual;
  }

  if (systemMode == hotWaterDispense && !isDispensing)
  {
    if (previouslyPressedButtons[0] == REDBUTTON)
    {
      yellowButton.led.flash(500);
    }
    else if (previouslyPressedButtons[0] == YELLOWBUTTON)
    {
      redButton.led.flash(500);
    }
  }

  if (hotTankControl.isHotTankEnabled)
  {
    hotTankControl.run(ntc, currentTempConfig);
  }

  if (!filterStatusCheck.isFilterOk)
  {
    greenLed.flash(1000);
  }

  if (systemMode != chillerBucketRefill && isCompressorControlEnabled){
      compressorControl.run(ntc, currentTempConfig);
  }

  if (systemMode == configuration)
  {
    redButton.led.flash(1000);
    greenButton.led.flash(1000);
    blueButton.led.flash(1000);
    if (isHotWaterSafetySequenceEnabled)
    {
      yellowButton.led.turnOn(true);
    }
    else
    {
      yellowButton.led.flash(1000);
    }
  }
  if (systemMode == hotTankConfig || systemMode == chillerBucketConfig)
  {
    redButton.led.flash(1000);
    greenButton.led.flash(1000);
    blueButton.led.flash(1000);
    whiteButton.led.flash(1000);
  }

  if (systemMode == chillerBucketRefill)
  {
    if (compressorControl.isCompressorEnabled)
    {
      compressorControl.turnOn(false);
    }
    if (agitatorPumpControl.isAgitatorPumpEnabled)
    {
      agitatorPumpControl.turnOnAgitatorPump(false);
    }
    if (!isDispensing && !blueButton.isPressed && !whiteButton.isPressed)
    {
      whiteButton.led.flash(500);
      blueButton.led.flash(500);
    }

    // wait for blue and white button 3 seconds
    if (blueButton.startPressTime != 0 && whiteButton.startPressTime != 0 && blueButton.isPressed && whiteButton.isPressed)
    {
      if (currentTime - 3000 > blueButton.startPressTime && currentTime - 3000 > whiteButton.startPressTime && !isDispensing)
      {
        turnOnAllLeds(false);
        greenButton.led.lastLedFlashTime = millis();
        blueButton.led.lastLedFlashTime = millis();
        whiteButton.led.lastLedFlashTime = millis();
        yellowButton.led.lastLedFlashTime = millis();
        redButton.led.lastLedFlashTime = millis();

        sendLogMessage("start filling chiller bucket");
        // start filling chiller bucket
        chillerBucketRefillStartTime = millis();
        turnOnMainValve(true);
        turnOnLevelValve(true);
      } else if (currentTime - 3000 < blueButton.startPressTime && currentTime - 3000 < whiteButton.startPressTime)
      {
        whiteButton.led.turnOn(true);
        blueButton.led.turnOn(true);
      }
    }
  }

  // === UPDATED: enforce stop at target volume whenever auto is active ===
  if ((isAutoDispensing || systemMode == autofill) && isDispensing)
  {
    yellowButton.led.flash(500);
    stopAutofill(waterType, autoFillStopVolume);
    if (waterType == sparkling) { greenButton.led.flash(500); }
    if (waterType == alkaline)  { whiteButton.led.flash(500); }
    if (waterType == cold)      { blueButton.led.flash(500);  }
  }

  if (systemMode == autofill && !isDispensing)
  {
    yellowButton.led.flash(500);
    if (currentTime - 5000 > autofillStartTime)
    {
      sendLogMessage("Autofill mode timed out... returning to manual mode");
      turnOnAllLeds(true);
      systemMode = manual;
    }
  }

  if (systemMode == calibrate && !isDispensing)
  {
    yellowButton.led.turnOn(false);
    greenButton.led.flash(500);
    blueButton.led.flash(500);
    whiteButton.led.flash(500);

    if (currentTime - 5000 > autofillStartTime)
    {
      sendLogMessage("Calibration mode timed out... returning to manual mode");
      turnOnAllLeds(true);
      systemMode = manual;
    }
  }

  if (isDispensing)
  {
    blueLed.turnOn(true);
  }
  else 
  {
    if (millis() - 3000 > dispenseStopTime && blueLed.isOn)
    {
      blueLed.turnOn(false);
    }
  }

//  sr.updateRegisters();
}

void startSerial()
{
  Serial2.begin(115200);
  SerialUSB.begin(115200);
  sendLogMessage("serial start");
  sendLogMessage("DRINKSTATION DaVinci 3.2 DEVELOPMENT KIT");
}

void initializePins()
{
  //pinMode(FAN, OUTPUT);
  //pinMode(COMPRESSOR, OUTPUT);
  // pinMode(HOTTANK, OUTPUT);
  pinMode(WATERPUMP, OUTPUT);
  // pinMode(AGITATOR, OUTPUT);
  pinMode(CO2_V, OUTPUT);
  pinMode(OUTPUT3, OUTPUT);
  pinMode(OUTPUT4, OUTPUT);
  pinMode(OUTPUT5, OUTPUT);
  pinMode(OUTPUT6, OUTPUT);
  pinMode(OUTPUT7, OUTPUT);
}

void setDefaultTemp()
{
  handleTempCommand("default");
  handleTempCommand("write");
}

void getCurrentTempConfig()
{
  EEPROM.get(eeAddress, currentTempConfig);
}

void initializeFlowMeter()
{
  flowMeter = new FlowMeter(digitalPinToInterrupt(PC0), FHKCS_1mm_0deg, meterISR, RISING);
}

void meterISR()
{
  // let our flow meter count the pulses
  flowMeter->count();
}

void systemCheck()
{
  json.clear();
  json["topic"] = "System Check";
  json["lastFilterResetTime"] = lastFilterResetTime;
  json["machineStartTime"] = getDateTime();
  json["chillerBucketMaxTemp"] = currentTempConfig.ChillerBucketMax;
  json["chillerBucketMinTemp"] = currentTempConfig.ChillerBucketMin;
  json["hottankMaxTemp"] = currentTempConfig.HotTankMax;
  json["hottankMinTemp"] = currentTempConfig.HotTankMin;
  json["coldNtc"] = ntc.cold_ntc;
  json["hotNtc"] = ntc.hot_ntc;
  json["ntc3"] = ntc.ntc3;
  json["lifetimeDispenseVolume"] = totalDispenseVolumeSinceFilterReset;
  serializeJson(json, Serial2);
  Serial2.print('\n');

  if (ntc.cold_ntc == -100)
  {
    sendLogMessage("Cold NTC does not exist");
  }
  if (ntc.hot_ntc == -100)
  {
    sendLogMessage("Hot NTC does not exist");
  }
  if (ntc.ntc3 == -100)
  {
    sendLogMessage("NTC3 does not exist");
  }
}

void readTemperature()
{
  if (millis() - 5000 > lastReadTempTime)
  {
    unsigned int ad_buf1[10],ad_buf2[10],ad_buf3[10],ad_buf4[10],ad_buf5[10];
    unsigned int temp1,temp2,temp3,temp4,temp5;
    static unsigned int ICE_buf[10];
    unsigned char i;
    float kk;

    for (i = 0; i < 10; i++)
    {
      ad_buf1[i] = ADC_RD(COLD_NTC_PIN);
      ad_buf2[i] = ADC_RD(HOT_NTC_PIN);
      ad_buf3[i] = ADC_RD(NTC3_PIN);
      ad_buf4[i] = ADC_RD(ICE_PIN);
      ad_buf5[i] = ADC_RD(HOT_LEVEL_PIN);
    }

    temp1=maxminaverage(ad_buf1,10,6);
    temp2=maxminaverage(ad_buf2,10,6);
    temp3=maxminaverage(ad_buf3,10,6);
    temp4=maxminaverage(ad_buf4,10,6);
    temp5=maxminaverage(ad_buf5,10,6);

    kk = temp_change_cold(temp1);
    ntc.cold_ntc = (kk / 100);
    kk = temp_change_hot(temp2);
    ntc.hot_ntc = (kk / 100);
    kk = temp_change_cold(temp3);
    ntc.ntc3 = (kk / 100);

    lastReadTempTime = millis();
  }
}

void run()
{
  updateButtons();
  handleButtonStates();

  if (systemMode == thirtysecs&& millis() - 200 > thirtySecStartStopTime)
  {
      turnOnCo2Valve(false);
      turnOnWaterPump(false);
      if (greenButton.isReleased&&whiteButton.isReleased&&blueButton.isReleased&&redButton.isReleased&&yellowButton.isReleased)
      {
        systemMode = manual;
        turnOnAllLeds(true);
      }
  }
  else if( systemMode == safety&& millis() - 200 > enterSafetyModeTime)
  {
    turnOnCo2Valve(false);
    turnOnWaterPump(false);
  }
  else if (waterType == sparkling && millis() - 200 > sparklingWaterStartStopTime)
  {
    if (greenButton.isPressed || systemMode == autofill || systemMode == calibrate || isAutoDispensing || holdSparkling)
    {
        turnOnCo2Valve(true);
        turnOnWaterPump(true);
    }
    else
    {
      turnOnSparklingValve(false);
      turnOnNozzleValve(false);
      turnOnMainValve(false);
      if (systemMode != autofill && systemMode != calibrate)
      {
        waterType = none;
      }
    }
  }
  else if (waterType == alkaline && millis() - 200 > alkalineWaterStartStopTime)
  {
    if (whiteButton.isPressed || systemMode == autofill || systemMode == calibrate || isAutoDispensing || holdAlkaline)
    {
      turnOnWaterPump(true);
    }
    else
    {
      turnOnAlkalineValve(false);
      turnOnMainValve(false); 
      if (systemMode != autofill && systemMode != calibrate)
      {
        waterType = none;
      }
    }
  }
  else if (waterType == cold && millis() - 200 > coldWaterStartStopTime)
  {
    if (blueButton.isPressed || systemMode == autofill || systemMode == calibrate || isAutoDispensing || holdCold)
    {
      turnOnWaterPump(true);
    }
    else
    {
      turnOnColdValve(false);
      turnOnMainValve(false); 
      if (systemMode != autofill && systemMode != calibrate)
      {
        waterType = none;
      }
    }
  }
}

bool hasEnoughTimeElapsed(int duration, unsigned long startTime)
{
  if (startTime == 0)
  {
    return false;
  }
  if (currentTime - duration > startTime)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void updateButtons()
{
  blueButton.update();
  greenButton.update();
  redButton.update();
  yellowButton.update();
  whiteButton.update();
  filterButton.update();
}

void handleButtonStates()
{
  handleRedButtonStateChanged();
  handleWhiteButtonStateChanged();
  handleGreenButtonStateChanged();
  handleBlueButtonStateChanged();
  handleYellowButtonStateChanged();
  handleFilterButtonStateChanged();
}

void readSerial()
{
  if (Serial2.available() > 0)
  {
    const String command = Serial2.readStringUntil('\n');
    handleSerialCommand(command);
  }

   if (SerialUSB.available() > 0)
  {
    const String command = SerialUSB.readStringUntil('\n');
    handleSerialCommand(command);
  }
}

void handleSerialCommand(String incomingCommandString)
{
  incomingCommandString.trim();

  const String commandString = incomingCommandString.substring(0, incomingCommandString.indexOf(" "));
  const String arg = incomingCommandString.substring(incomingCommandString.indexOf(" ") + 1);

  if (commandString.equalsIgnoreCase("iot"))
    handleIotCommand(arg);
  else if (commandString.equalsIgnoreCase("temp"))
    handleTempCommand(arg);
  else if (commandString.equalsIgnoreCase("setchillerbucketmax"))
    handleSetChillerBucketMaxCommand(arg);
  else if (commandString.equalsIgnoreCase("setchillerbucketmin"))
    handleSetChillerBucketMinCommand(arg);
  else if (commandString.equalsIgnoreCase("sethottankmax"))
    handleSetHotTankMaxCommand(arg);
  else if (commandString.equalsIgnoreCase("sethottankmin"))
    handleSetHotTankMinCommand(arg);
  else if (commandString.equalsIgnoreCase("setchillerbuckettemp"))
    handleSetChillerBucketTempCommand(arg);
  else if (commandString.equalsIgnoreCase("sethottanktemp"))
    handleSetHotTankTempCommand(arg);
  else if (commandString.equalsIgnoreCase("flowmeter"))
    handleFlowMeterCommand(arg);
  else if (commandString.equalsIgnoreCase("autospk"))
    handleAutoSparkCommand(arg);
  else if (commandString.equalsIgnoreCase("autocold"))
    handleAutoColdCommand(arg);
  else if (commandString.equalsIgnoreCase("autoalk"))
    handleAutoAlkCommand(arg);
  else if (commandString.equalsIgnoreCase("autohot"))
    handleAutoHotCommand(arg);
  else if (commandString.equalsIgnoreCase("remoteautocold"))
    handleRemoteAutoColdCommand();
  else if (commandString.equalsIgnoreCase("remoteautoalk"))
    handleRemoteAutoAlkCommand();
  else if (commandString.equalsIgnoreCase("remoteautospk"))
    handleRemoteAutoSparkCommand();
  else if (commandString.equalsIgnoreCase("setsparklingwaterautofillvolume"))
    handleSetSparklingWaterAutofillVolume(arg);
  else if (commandString.equalsIgnoreCase("setcoldwaterautofillvolume"))
    handleSetColdWaterAutofillVolume(arg);
  else if (commandString.equalsIgnoreCase("setalkalinewaterautofillvolume"))
    handleSetAlkalineWaterAutofillVolume(arg);
  else if (commandString.equalsIgnoreCase("setdatetime"))
    handleSetDateTimeCommand(arg);
  else if (commandString.equalsIgnoreCase("setepochtime"))
    handleSetEpochTimeCommand(arg);
  else if (commandString.equalsIgnoreCase("enablehottank"))
    handleEnableHotTankCommand(arg);
  else if (commandString.equalsIgnoreCase("getdispensestats"))
    handleGetDispenseStatsCommand(arg);
  else if (commandString.equalsIgnoreCase("factoryreset"))
    handleFactoryResetCommand(arg);
  else if (commandString.equalsIgnoreCase("getmachinestatus"))
    handleGetMachineStatusCommand();
  else if (commandString.equalsIgnoreCase("dispensesparklingwater"))
    handleTurnOnSparklingWaterCommand(arg);
  else if (commandString.equalsIgnoreCase("dispensecoldwater"))
    handleTurnOnColdWaterCommand(arg);
  else if (commandString.equalsIgnoreCase("dispensealkalinewater"))
    handleTurnOnAlkalineWaterCommand(arg);
  else if (commandString.equalsIgnoreCase("dispensehotwater"))
    handleTurnOnHotWaterCommand(arg);
  else if (commandString.equalsIgnoreCase("resetfilter"))
    handleResetFilterCommand();
  else if (commandString.equalsIgnoreCase("enablewaterpump"))
    handleEnableWaterPumpCommand(arg);
  else if (commandString.equalsIgnoreCase("enablecompressor"))
    handleEnableCompressorCommand(arg);
  else if (commandString.equalsIgnoreCase("enablecompressorcontrol"))
    handleEnableCompressorControlCommand(arg);
  else if (commandString.equalsIgnoreCase("enablewaterpumpcontrol"))
    handleEnableWaterPumpControlCommand(arg);
  else if (commandString.equalsIgnoreCase("enableHotWaterSafety"))
    handleEnableHotWaterSafetyCommand(arg);

  else if (commandString.equalsIgnoreCase("blacktea"))
    handleBlackTeaCommand(arg);
  else if (commandString.equalsIgnoreCase("cupnoodle"))
    handleCupNoodleCommand(arg);
  else
    sendLogMessage("Unrecognized command: " + incomingCommandString);
}

void handleIotCommand(String arg)
{
  if (arg.equalsIgnoreCase("on"))
  {
    isIotEnabled = true;
  }
  else if (arg.equalsIgnoreCase("off"))
  {
    isIotEnabled = false;
  }
}

void handleEnableWaterPumpCommand(String arg)
{
  if (arg.equalsIgnoreCase("true"))
  {
    turnOnWaterPump(true);
  }
  else if (arg.equalsIgnoreCase("false"))
  {
    turnOnWaterPump(false);
  }
}

void handleEnableWaterPumpControlCommand(String arg)
{
  if (arg.equalsIgnoreCase("true"))
  {
    isWaterPumpControlEnabled = true;
  }
  else if (arg.equalsIgnoreCase("false"))
  {
    isWaterPumpControlEnabled = false;
  }
}

void handleEnableHotWaterSafetyCommand(String arg)
{
  if (arg.equalsIgnoreCase("true"))
  {
    isHotWaterSafetySequenceEnabled = true;
    sendLogMessage("Hot water safety sequence enabled");
  }
  else if (arg.equalsIgnoreCase("false"))
  {
    isHotWaterSafetySequenceEnabled = false;
    sendLogMessage("Hot water safety sequence disabled");
  }
}

void handleEnableCompressorCommand(String arg)
{
  if (arg.equalsIgnoreCase("true"))
  {
    compressorControl.turnOn(true);
  }
  else if (arg.equalsIgnoreCase("false"))
  {
    compressorControl.turnOn(false);
  }
}

void handleEnableCompressorControlCommand(String arg)
{
  if (arg.equalsIgnoreCase("true"))
  {
    isCompressorControlEnabled = true;
  }
  else if (arg.equalsIgnoreCase("false"))
  {
    isCompressorControlEnabled = false;
  }
}

void handleResetFilterCommand()
{
  resetFilter();
  EEPROM.put(totalDispenseVolumeSinceFilterResetAddress, 0);
  EEPROM.put(lastFilterResetTimeAddress, getEpochTime());
  totalDispenseVolumeSinceFilterReset = 0;
  lastFilterResetTime = getEpochTime();
  currentVolume = 0;
}

void handleGetDispenseStatsCommand(String arg)
{
  json.clear();
  if (arg == "cold")
  {
    json["lastDispenseTime"] = coldWaterDispenseStats.lastDispenseTimeString;
    json["lastDispenseDuration"] = coldWaterDispenseStats.lastDispenseDuration;
    json["lastDispenseVolume"] = coldWaterDispenseStats.lastDispenseVolume;
    json["pourCount"] = coldWaterDispenseStats.pourCount;
    json["totalDispenseVolume"] = coldWaterDispenseStats.totalDispenseVolume;
  }
  else if (arg == "sparkling")
  {
    json["lastDispenseTime"] = sparklingWaterDispenseStats.lastDispenseTimeString;
    json["lastDispenseDuration"] = sparklingWaterDispenseStats.lastDispenseDuration;
    json["lastDispenseVolume"] = sparklingWaterDispenseStats.lastDispenseVolume;
    json["pourCount"] = sparklingWaterDispenseStats.pourCount;
    json["totalDispenseVolume"] = sparklingWaterDispenseStats.totalDispenseVolume;
  }
  else if (arg == "alkaline")
  {
    json["lastDispenseTime"] = alkalineWaterDispenseStats.lastDispenseTimeString;
    json["lastDispenseDuration"] = alkalineWaterDispenseStats.lastDispenseDuration;
    json["lastDispenseVolume"] = alkalineWaterDispenseStats.lastDispenseVolume;
    json["pourCount"] = alkalineWaterDispenseStats.pourCount;
    json["totalDispenseVolume"] = alkalineWaterDispenseStats.totalDispenseVolume;
  }
  else if (arg == "hot")
  {
    json["lastDispenseTime"] = hotWaterDispenseStats.lastDispenseTimeString;
    json["lastDispenseDuration"] = hotWaterDispenseStats.lastDispenseDuration;
    json["lastDispenseVolume"] = hotWaterDispenseStats.lastDispenseVolume;
    json["pourCount"] = hotWaterDispenseStats.pourCount;
    json["totalDispenseVolume"] = hotWaterDispenseStats.totalDispenseVolume;
  }
  serializeJson(json, Serial2);
  Serial2.print('\n');
}

void handleTempCommand(String arg)
{
  json.clear();
  if (arg.equalsIgnoreCase("status"))
  {
    json["chillerBucketMax"] = currentTempConfig.ChillerBucketMax;
    json["chillerBucketMin"] = currentTempConfig.ChillerBucketMin;
    json["hotTankMax"] = currentTempConfig.HotTankMax;
    json["hotTankMin"] = currentTempConfig.HotTankMin;
    serializeJson(json, Serial2);
    Serial2.print('\n');
  }
  else if (arg.equalsIgnoreCase("write"))
  {
    EEPROM.put(eeAddress, currentTempConfig);
  }
  else if (arg.equalsIgnoreCase("default"))
  {
    currentTempConfig.ChillerBucketMax = 0.4;
    currentTempConfig.ChillerBucketMin = -1.8;
    currentTempConfig.HotTankMax = 88;
    currentTempConfig.HotTankMin = 85;

    json["chillerBucketMax"] = currentTempConfig.ChillerBucketMax;
    json["chillerBucketMin"] = currentTempConfig.ChillerBucketMin;
    json["hotTankMax"] = currentTempConfig.HotTankMax;
    json["hotTankMin"] = currentTempConfig.HotTankMin;
    serializeJson(json, Serial2);
    Serial2.print('\n');
  }
  else if (arg.equalsIgnoreCase("cold_default"))
  {
    currentTempConfig.ChillerBucketMax = 0.4;
    currentTempConfig.ChillerBucketMin = -1.8;
  }
  else if (arg.equalsIgnoreCase("coldest"))
  {
    currentTempConfig.ChillerBucketMax = 0.3;
    currentTempConfig.ChillerBucketMin = -2.1;
  }
  else if (arg.equalsIgnoreCase("lesscold"))
  {
  }
  else if (arg.equalsIgnoreCase("cold_saving"))
  {
    currentTempConfig.ChillerBucketMax = 1;
    currentTempConfig.ChillerBucketMin = -0.4;
  }
  else if (arg.equalsIgnoreCase("hot_default"))
  {
    currentTempConfig.HotTankMax = 88;
    currentTempConfig.HotTankMin = 85;
  }
  else if (arg.equalsIgnoreCase("hottest"))
  {
    currentTempConfig.HotTankMax = 91;
    currentTempConfig.HotTankMin = 88;
  }
  else if (arg.equalsIgnoreCase("lesshot"))
  {
    currentTempConfig.HotTankMax = 85;
    currentTempConfig.HotTankMin = 82;
  }
  else if (arg.equalsIgnoreCase("hot_saving"))
  {
    currentTempConfig.HotTankMax = 83;
    currentTempConfig.HotTankMin = 78;
  }
}

void handleSetChillerBucketMaxCommand(String arg)
{
  currentTempConfig.ChillerBucketMax = arg.toFloat();
  sendLogMessage("Set Chiller Bucket Max. to " + arg + " C");
}

void handleSetChillerBucketMinCommand(String arg)
{
  currentTempConfig.ChillerBucketMin = arg.toFloat();
  sendLogMessage("Set Chiller Bucket Min. to " + arg + " C");
}

void handleSetHotTankMaxCommand(String arg)
{
  currentTempConfig.HotTankMax = arg.toFloat();
  sendLogMessage("Set Hottank Max. to " + arg + " C");
}

void handleSetHotTankMinCommand(String arg)
{
  currentTempConfig.HotTankMin = arg.toFloat();
  sendLogMessage("Set Hottank Min. to " + arg + " C");
}

void handleSetChillerBucketTempCommand(String arg)
{
   sendLogMessage("setting chiller bucket temperature to " + arg);
  if (arg.equalsIgnoreCase("default"))
  {
    currentTempConfig.ChillerBucketMax = 0.4;
    currentTempConfig.ChillerBucketMin = -1.8;
  }
  else if (arg.equalsIgnoreCase("lesscold"))
  {
    currentTempConfig.ChillerBucketMax = 0.5;
    currentTempConfig.ChillerBucketMin = -1.0;
  }
  else if (arg.equalsIgnoreCase("coldest"))
  {
    currentTempConfig.ChillerBucketMax = 0.3;
    currentTempConfig.ChillerBucketMin = -2.1;
  }
  else if (arg.equalsIgnoreCase("saving"))
  {
    currentTempConfig.ChillerBucketMax = 1;
    currentTempConfig.ChillerBucketMin = -0.4;
  }
}

void handleSetHotTankTempCommand(String arg)
{
  sendLogMessage("setting hot tank temperature to " + arg);
  if (arg.equalsIgnoreCase("default"))
  {
    currentTempConfig.HotTankMax = 88;
    currentTempConfig.HotTankMin = 85;
  }
  else if (arg.equalsIgnoreCase("lesshot"))
  {
    currentTempConfig.HotTankMax = 85;
    currentTempConfig.HotTankMin = 82;
  }
  else if (arg.equalsIgnoreCase("hottest"))
  {
    currentTempConfig.HotTankMax = 91;
    currentTempConfig.HotTankMin = 88;
  }
  else if (arg.equalsIgnoreCase("saving"))
  {
    currentTempConfig.HotTankMax = 83;
    currentTempConfig.HotTankMin = 78;
  }
}

void handleFlowMeterCommand(String arg)
{
  if (arg.equalsIgnoreCase("status"))
  {
    json.clear();
    json["totalDuration"] = flowMeter->getTotalDuration() / 1000;
    json["totalFlowrate"] = flowMeter->getTotalFlowrate();
    json["totalVolume"] = flowMeter->getTotalVolume() * 10;
    serializeJson(json, Serial2);
    Serial2.print('\n');
  }
}

void handleTurnOnSparklingWaterCommand(String arg)
{
  if (arg.equalsIgnoreCase("true")) { 
    holdSparkling = true;
    turnOnSparklingWater(true);
  } else if (arg.equalsIgnoreCase("false")) {
    holdSparkling = false;
    turnOnSparklingWater(false);
  }
}

void handleTurnOnColdWaterCommand(String arg)
{
  if (arg.equalsIgnoreCase("true")) { 
    holdCold = true;
    turnOnColdWater(true);
  } else if (arg.equalsIgnoreCase("false")) {
    holdCold = false;
    turnOnColdWater(false);
  }
}

void handleTurnOnHotWaterCommand(String arg)
{
  if (arg.equalsIgnoreCase("true"))
  {
    turnOnHotWater(true);
  }
  else if (arg.equalsIgnoreCase("false"))
  {
    turnOnHotWater(false);
  }
}

void handleTurnOnAlkalineWaterCommand(String arg)
{
  if (arg.equalsIgnoreCase("true")) { 
    holdAlkaline = true;
    turnOnAlkalineWater(true);
  } else if (arg.equalsIgnoreCase("false")) {
    holdAlkaline = false;
    turnOnAlkalineWater(false);
  }
}

void handleAutoSparkCommand(String arg)
{
  systemMode = hotWaterDispense;
  autofillSparklingWater(arg.toInt());
}

void handleAutoColdCommand(String arg)
{
  systemMode = hotWaterDispense;
  autofillColdWater(arg.toInt());
}

void handleAutoAlkCommand(String arg)
{
  systemMode = hotWaterDispense;
  autofillAlkalineWater(arg.toInt());
}

void handleAutoHotCommand(String arg)
{
  systemMode = hotWaterDispense;
  autofillHotWater(arg.toInt());
}


// --- Preset convenience commands ---
// blacktea: ~90°C, default 280 mL unless overridden by arg
void handleBlackTeaCommand(String arg)
{
  savedHotTankMax = currentTempConfig.HotTankMax;
  savedHotTankMin = currentTempConfig.HotTankMin;

  arg.trim();
  int vol = arg.length() ? arg.toInt() : 0;
  if (vol <= 0) vol = 280;
  if (vol > 1500) vol = 1500;

  hotPresetActive   = true;
  presetTargetTempC = 90;
  presetVolumeMl    = vol;
  hotPresetState    = HotPresetHeating;

  sendLogMessage("Preset blacktea → target " + String(presetTargetTempC) + "°C, volume " + String(presetVolumeMl) + " mL");
}

// cupnoodle: near-boiling, default 300 mL unless overridden by arg
void handleCupNoodleCommand(String arg)
{
  savedHotTankMax = currentTempConfig.HotTankMax;
  savedHotTankMin = currentTempConfig.HotTankMin;

  arg.trim();
  int vol = arg.length() ? arg.toInt() : 0;
  if (vol <= 0) vol = 300;
  if (vol > 1500) vol = 1500;

  hotPresetActive   = true;
  presetTargetTempC = 96;
  presetVolumeMl    = vol;
  hotPresetState    = HotPresetHeating;

  sendLogMessage("Preset cupnoodle → target " + String(presetTargetTempC) + "°C, volume " + String(presetVolumeMl) + " mL");
}

void handleRemoteAutoSparkCommand(void)
{
  systemMode = hotWaterDispense;
  autofillSparklingWater(autofillVolume.spkVolume);
}

void handleRemoteAutoColdCommand(void)
{
  systemMode = hotWaterDispense;
  autofillSparklingWater(autofillVolume.coldVolume);
}

void handleRemoteAutoAlkCommand(void)
{
  systemMode = hotWaterDispense;
  autofillSparklingWater(autofillVolume.alkVolume);
}


void handleSetSparklingWaterAutofillVolume(String arg)
{
  autofillVolume.spkVolume = arg.toFloat();
}

void handleSetColdWaterAutofillVolume(String arg)
{
  autofillVolume.coldVolume = arg.toFloat();
}

void handleSetAlkalineWaterAutofillVolume(String arg)
{
  autofillVolume.alkVolume = arg.toFloat();
}

void handleSetDateTimeCommand(String arg)
{
  char * strtokIndx;
  char argAsCharArray[32];
  
  arg.toCharArray(argAsCharArray, 32);

  strtokIndx = strtok(argAsCharArray,",");
  int hour = atoi(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  int min = atoi(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  int sec = atoi(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  int mday = atoi(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  int mon = atoi(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  int year = atoi(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  int wday = atoi(strtokIndx);

  setDateTime(hour,min,sec,mday,mon,year,wday);
}

void handleSetEpochTimeCommand(String arg)
{
  setEpochTime(arg.toInt());
}

void handleFactoryResetCommand(String arg)
{
  sendLogMessage("FACTORY RESET");
  // wipe EEPROM
  for (int i = 0 ; i < usedEepromSize ; i++) {
    EEPROM.write(i, 0);
  }
  // set defaults
  sendLogMessage("SETTING DEFAULTS");
  EEPROM.put(lastFilterResetTimeAddress, getEpochTime());
  EEPROM.put(lastHotWaterDispenseTimeAddress, getEpochTime());
  EEPROM.put(totalDispenseVolumeSinceFilterResetAddress, 0);
  EEPROM.put(isSerialInJsonModeAddress, true);
  setDefaultTemp();
  turnOnAllLeds(true);
  NVIC_SystemReset();
}

void handleEnableHotTankCommand(String arg)
{
  if (arg == "true")
  {
    hotTankControl.enableHotTank(true);
    sendLogMessage("Enabled hot tank");
  }
  else if (arg == "false")
  {
    hotTankControl.enableHotTank(false);
    sendLogMessage("disabled hot tank");
  }
  else 
  {
    sendLogMessage("Unrecognized arg: " + arg);
  }
}

void handleGetMachineStatusCommand()
{
  json.clear();
  json["totalDispenseVolumeSinceFilterReset"] = totalDispenseVolumeSinceFilterReset;
  
  json["totalDispenseVolumeSinceStart"] = totalDispenseVolumeSinceStart;
  json["daysSinceLastFilterReset"] = (getEpochTime() - lastFilterResetTime) / 86400;

  json["coldWaterTotalDispenseVolume"] = coldWaterDispenseStats.totalDispenseVolume;
  json["coldWaterLastDispenseVolume"] = coldWaterDispenseStats.lastDispenseVolume;
  json["coldWaterLastDispenseDuration"] = coldWaterDispenseStats.lastDispenseDuration;
  json["coldWaterLastDispenseTemperature"] = coldWaterDispenseStats.lastDispenseTemperature;
  json["coldWaterPourCount"] = coldWaterDispenseStats.pourCount;

  json["ambientWaterTotalDispenseVolume"] = alkalineWaterDispenseStats.totalDispenseVolume;
  json["ambientWaterLastDispenseVolume"] = alkalineWaterDispenseStats.lastDispenseVolume;
  json["ambientLastDispenseDuration"] = alkalineWaterDispenseStats.lastDispenseDuration;
  json["ambientWaterLastDispenseTemperature"] = alkalineWaterDispenseStats.lastDispenseTemperature;
  json["ambientWaterPourCount"] = alkalineWaterDispenseStats.pourCount;

  json["sparklingWaterTotalDispenseVolume"] = sparklingWaterDispenseStats.totalDispenseVolume;
  json["sparklingWaterLastDispenseVolume"] = sparklingWaterDispenseStats.lastDispenseVolume;
  json["sparklingLastDispenseDuration"] = sparklingWaterDispenseStats.lastDispenseDuration;
  json["sparklingWaterLastDispenseTemperature"] = sparklingWaterDispenseStats.lastDispenseTemperature;
  json["sparklingWaterPourCount"] = sparklingWaterDispenseStats.pourCount;

  json["hotWaterTotalDispenseVolume"] = hotWaterDispenseStats.totalDispenseVolume;
  json["hotWaterLastDispenseVolume"] = hotWaterDispenseStats.lastDispenseVolume;
  json["hotWaterLastDispenseDuration"] = hotWaterDispenseStats.lastDispenseDuration;
  json["hotWaterLastDispenseTemperature"] = hotWaterDispenseStats.lastDispenseTemperature;
  json["hotWaterPourCount"] = hotWaterDispenseStats.pourCount;

  json["chillerBucketCurrentTemp"] = ntc.cold_ntc;
  json["chillerBucketMaxTemp"] = currentTempConfig.ChillerBucketMax;
  json["chillerBucketMinTemp"] = currentTempConfig.ChillerBucketMin;
  json["hotTankCurrentTemp"] = ntc.hot_ntc;
  json["hotTankMaxTemp"] = currentTempConfig.HotTankMax;
  json["hotTankMinTemp"] = currentTempConfig.HotTankMin;

  json["coldWaterAutofillVolume"] = autofillVolume.coldVolume;
  json["sparklingWaterAutofillVolume"] = autofillVolume.spkVolume;
  json["ambientWaterAutofillVolume"] = autofillVolume.alkVolume;

  json["isCompressorEnabled"] = compressorControl.isCompressorEnabled;
  json["isFanEnabled"] = compressorControl.isCompressorEnabled;
  json["isAgitatorPumpEnabled"] = agitatorPumpControl.isAgitatorPumpEnabled;
  json["isHotTankEnabled"] = hotTankControl.isHotTankEnabled;
  json["isHotTankOn"] = hotTankControl.isHotTankOn;
  json["isWaterPumpOn"] = isWaterPumpEnabled;
  json["isWaterPumpEnabled"] = isWaterPumpControlEnabled;

  json["isGreenButtonPressed"] = greenButton.isPressed;
  json["isWhiteButtonPressed"] = whiteButton.isPressed;
  json["isBlueButtonPressed"] = blueButton.isPressed;
  json["isRedButtonPressed"] = redButton.isPressed;
  json["isYellowButtonPressed"] = yellowButton.isPressed;
  json["isResetButtonPressed"] = filterButton.isPressed;

  json["isGreenButtonLedOn"] = greenButton.led.isOn;
  json["isWhiteButtonLedOn"] = whiteButton.led.isOn;
  json["isBlueButtonLedOn"] = blueButton.led.isOn;
  json["isRedButtonLedOn"] = redButton.led.isOn;
  json["isYellowButtonLedOn"] = yellowButton.led.isOn;

  json["greenButtonLastPressTime"] = greenButton.startPressTimeString;
  json["whiteButtonLastPressTime"] = whiteButton.startPressTimeString;
  json["blueButtonLastPressTime"] = blueButton.startPressTimeString;
  json["redButtonLastPressTime"] = redButton.startPressTimeString;
  json["yellowButtonLastPressTime"] = yellowButton.startPressTimeString;
  json["resetButtonLastPressTime"] = filterButton.startPressTimeString;

  json["isGreenLedOn"] = greenLed.isOn;
  json["isRedLedOn"] = redLed.isOn;
  json["isBlueLedOn"] = blueLed.isOn;

  json["isChildSafetySwitchEnabled"] = isChildSafetySwitchEnabled;

  json["isMainValveOn"] = isMainValveOn;
  json["isAlkalineValveOn"] = isAlkalineValveOn;
  json["isCo2ValveOn"] = isCo2ValveOn;
  json["isColdValveOn"] = isColdValveOn;
  json["isSparklingValveOn"] = isSparklingValveOn;
  json["isLevelValveOn"] = isLevelValveOn;
  json["isNozzleValveOn"] = isNozzleValveOn;
  json["isHotValveOn"] = isHotValveOn;
  json["waterType"] = waterType;
  json["currentFlowRate"] = currentFlowrate;

  json["isFloatSensorLow"] = isFloatSensorLow;

  json["currentSystemTime"] = getDateTime();

  json["model"] = "Davinci 2.0";
  json["version"] = firmwareVersion;

  switch (systemMode)
  {
  case manual:
    json["systemMode"] = "manual"; break;
  case autofill:
    json["systemMode"] = "autofill"; break;
  case calibrate:
    json["systemMode"] = "calibrate"; break;
  case configuration:
    json["systemMode"] = "configuration"; break;
  case chillerBucketConfig:
    json["systemMode"] = "chillerBucketConfig"; break;
  case hotTankConfig:
    json["systemMode"] = "hotTankConfig"; break;
  case chillerBucketRefill:
    json["systemMode"] = "chillerBucketRefill"; break;
  case safety:
    json["systemMode"] = "safety"; break;
  case hotWaterDispense:
    json["systemMode"] = "hotWaterDispense"; break;  
  }
  
  serializeJson(json, Serial2);
  Serial2.print('\n');
  serializeJson(json,SerialUSB);
  SerialUSB.print('\n');
}

void checkChildSafetySwitch()
{
  if (systemMode != safety)
  {
    if (digitalRead(CHILDSAFETYSWITCH))
    {
      sr.set(REDLED, false);
      isChildSafetySwitchEnabled = false;
    }
    else
    {
      sr.set(REDLED, true);
      isChildSafetySwitchEnabled = true;
    }
  }
}

void updateFlowRate()
{
  if (millis() - previousTime >= period)
  {
    flowMeter->tick(period);
    currentFlowrate = flowMeter->getCurrentFlowrate();
    previousTime = millis();
  }
}

void sendStopDispenseStats(WaterType waterType)
{
  totalDispenseVolumeSinceStart = flowMeter->getTotalVolume() * 10000;
  totalDispenseVolumeSinceFilterReset = totalDispenseVolumeSinceFilterReset + (totalDispenseVolumeSinceStart - currentVolume);
  EEPROM.put(totalDispenseVolumeSinceFilterResetAddress, totalDispenseVolumeSinceFilterReset);

  dispenseStopTime = millis();

  json.clear();
  switch (waterType)
  {
  case cold:
    coldWaterDispenseStats.lastDispenseDuration = millis() - currentDispenseStartTime;
    coldWaterDispenseStats.lastDispenseTimeString = getDateTime();
    coldWaterDispenseStats.lastDispenseTime = getEpochTime();
    coldWaterDispenseStats.lastDispenseVolume = flowMeter->getTotalVolume() * 10000 - currentVolume;
    coldWaterDispenseStats.totalDispenseVolume = flowMeter->getTotalVolume() * 10000;
    coldWaterDispenseStats.pourCount = coldWaterDispenseStats.pourCount + 1;
    coldWaterDispenseStats.lastDispenseTemperature = ntc.cold_ntc;
    json["lastDispenseDuration"] = coldWaterDispenseStats.lastDispenseDuration;
    json["lastDispenseTimeString"] = coldWaterDispenseStats.lastDispenseTimeString;
    json["lastDispenseTime"] = coldWaterDispenseStats.lastDispenseTime;
    json["lastDispenseVolume"] = coldWaterDispenseStats.lastDispenseVolume;
    json["lastDispenseTemperature"] = coldWaterDispenseStats.lastDispenseTemperature;
    json["totalDispenseVolume"] = coldWaterDispenseStats.totalDispenseVolume;
    json["pourCount"] = coldWaterDispenseStats.pourCount;
    break;
  case sparkling:
    sparklingWaterDispenseStats.lastDispenseDuration = millis() - currentDispenseStartTime;
    sparklingWaterDispenseStats.lastDispenseTimeString = getDateTime();
    sparklingWaterDispenseStats.lastDispenseTime = getEpochTime();
    sparklingWaterDispenseStats.lastDispenseVolume = flowMeter->getTotalVolume() * 10000 - currentVolume;
    sparklingWaterDispenseStats.totalDispenseVolume = flowMeter->getTotalVolume() * 10000;
    sparklingWaterDispenseStats.pourCount = sparklingWaterDispenseStats.pourCount + 1;
    sparklingWaterDispenseStats.lastDispenseTemperature = ntc.cold_ntc;
    json["lastDispenseDuration"] = sparklingWaterDispenseStats.lastDispenseDuration;
    json["lastDispenseTimeString"] = sparklingWaterDispenseStats.lastDispenseTimeString;
    json["lastDispenseTime"] = sparklingWaterDispenseStats.lastDispenseTime;
    json["lastDispenseVolume"] = sparklingWaterDispenseStats.lastDispenseVolume;
    json["lastDispenseTemperature"] = sparklingWaterDispenseStats.lastDispenseTemperature;
    json["totalDispenseVolume"] = sparklingWaterDispenseStats.totalDispenseVolume;
    json["pourCount"] = sparklingWaterDispenseStats.pourCount;
    break;
  case alkaline:
    alkalineWaterDispenseStats.lastDispenseDuration = millis() - currentDispenseStartTime;
    alkalineWaterDispenseStats.lastDispenseTimeString = getDateTime();
    alkalineWaterDispenseStats.lastDispenseTime = getEpochTime();
    alkalineWaterDispenseStats.lastDispenseVolume = flowMeter->getTotalVolume() * 10000 - currentVolume;
    alkalineWaterDispenseStats.totalDispenseVolume = flowMeter->getTotalVolume() * 10000;
    alkalineWaterDispenseStats.pourCount = alkalineWaterDispenseStats.pourCount + 1;
    alkalineWaterDispenseStats.lastDispenseTemperature = ntc.ntc3;
    json["lastDispenseDuration"] = alkalineWaterDispenseStats.lastDispenseDuration;
    json["lastDispenseTimeString"] = alkalineWaterDispenseStats.lastDispenseTimeString;
    json["lastDispenseTime"] = alkalineWaterDispenseStats.lastDispenseTime;
    json["lastDispenseVolume"] = alkalineWaterDispenseStats.lastDispenseVolume;
    json["lastDispenseTemperature"] = alkalineWaterDispenseStats.lastDispenseTemperature;
    json["totalDispenseVolume"] = alkalineWaterDispenseStats.totalDispenseVolume;
    json["pourCount"] = alkalineWaterDispenseStats.pourCount;
    break;
  case hot:
    hotWaterDispenseStats.lastDispenseDuration = millis() - currentDispenseStartTime;
    hotWaterDispenseStats.lastDispenseTimeString = getDateTime();
    hotWaterDispenseStats.lastDispenseTime = getEpochTime();
    hotWaterDispenseStats.lastDispenseVolume = flowMeter->getTotalVolume() * 10000 - currentVolume;
    hotWaterDispenseStats.totalDispenseVolume = flowMeter->getTotalVolume() * 10000;
    hotWaterDispenseStats.pourCount = hotWaterDispenseStats.pourCount + 1;
    hotWaterDispenseStats.lastDispenseTemperature = ntc.hot_ntc;
    json["lastDispenseDuration"] = hotWaterDispenseStats.lastDispenseDuration;
    json["lastDispenseTimeString"] = hotWaterDispenseStats.lastDispenseTimeString;
    json["lastDispenseTime"] = hotWaterDispenseStats.lastDispenseTime;
    json["lastDispenseVolume"] = hotWaterDispenseStats.lastDispenseVolume;
    json["lastDispenseTemperature"] = hotWaterDispenseStats.lastDispenseTemperature;
    json["totalDispenseVolume"] = hotWaterDispenseStats.totalDispenseVolume;
    json["pourCount"] = hotWaterDispenseStats.pourCount;
    break;
  default:
    break;
  }

  serializeJson(json, Serial2);
  Serial2.print('\n');

  if (systemMode != calibrate)
  {
    currentVolume = flowMeter->getTotalVolume() * 10000;
  }
}


void handleGreenButtonStateChanged()
{
  if ((systemMode == safety || systemMode == chillerBucketRefill) || (systemMode == manual && !isDispensing && (whiteButton.isPressed || blueButton.isPressed || redButton.isPressed || yellowButton.isPressed)))
  {
    return;
  }
  if (greenButton.isPressed && greenButton.hasStateChanged)
  {
    sendLogMessage(" Green button pressed at " + getDateTime());
    greenButton.startPressTime = millis();
    greenButton.startPressTimeString = getDateTime();
    addButtonPinToArray(greenButton.pin);

    if (systemMode == manual)
    {
      turnOnAllLeds(false);
      greenButton.led.turnOn(true);
      turnOnSparklingWater(true);
    }
    // === UPDATED: also allow cancel while auto is active ===
    else if (systemMode == autofill || isAutoDispensing)
    {
      if (isDispensing)
      {
        cancelDispense = true;
      }
      else
      {
        turnOnAllLeds(false);
        yellowButton.led.lastLedFlashTime = millis();
        greenButton.led.lastLedFlashTime = millis();
        autofillSparklingWater(autofillVolume.spkVolume);
      }
    }
    else if (systemMode == configuration)
    {
      resetFilter();
      EEPROM.put(totalDispenseVolumeSinceFilterResetAddress, 0);
      EEPROM.put(lastFilterResetTimeAddress, getEpochTime());
      currentVolume = 0;
      greenLed.turnOn(true);
      turnOnAllLeds(true);
    }
    else if (systemMode == calibrate)
    {
      if (isDispensing)
      {
        if (waterType != sparkling)
        {
          stopDispense();
          turnOnAllLeds(true);
          systemMode = manual;
        }
        else 
        {
          turnOnSparklingWater(false);
          sendStopDispenseStats(sparkling);
          autofillVolume.spkVolume = flowMeter->getTotalVolume() * 10000 - currentVolume;
          currentVolume = flowMeter->getTotalVolume() * 10000;
          sendLogMessage("autofill sparkling volume set to: " + String(autofillVolume.spkVolume));
          turnOnAllLeds(true);
          systemMode = manual;
        }
      }
      else
      {
        turnOnAllLeds(false);
        yellowButton.led.lastLedFlashTime = millis();
        greenButton.led.lastLedFlashTime = millis();
        turnOnSparklingWater(true);
      }
    }
    else if (systemMode == hotTankConfig)
    {
      handleSetHotTankTempCommand("default");
      turnOnAllLeds(true);
    }
    else if (systemMode == chillerBucketConfig)
    {
      handleSetChillerBucketTempCommand("default");
      turnOnAllLeds(true);
    }
    else if (systemMode == hotWaterDispense)
    {
      turnOnAllLeds(true);
    }
  }
  else if (greenButton.isPressed)
  {
    if (systemMode == manual)
    {
      greenButton.led.flash(1000);
    }
  }
  else if (greenButton.isReleased && greenButton.hasStateChanged)
  {
    switch (systemMode)
    {
    case manual:
      turnOnSparklingWater(false);
      turnOnAllLeds(true);
      sendStopDispenseStats(sparkling);
      break;
    case calibrate:
      break;
    case configuration:
      systemMode = manual;
      break;
    case hotTankConfig:
      systemMode = manual;
      break;
    case chillerBucketConfig:
      systemMode = manual;
      break;
    case hotWaterDispense:
      systemMode = manual;
    default:
      break;
    }
  }
  greenButton.hasStateChanged = false;
}

void resetFilter()
{
  sendLogMessage("RESETTING FILTER");
  flowMeter->reset(); // buggy?
  flowMeter->setTotalVolume(0);
  flowMeter->setTotalDuration(0);
}

void handleBlueButtonStateChanged()
{
  if ((systemMode == safety) || (systemMode == manual && !isDispensing &&(greenButton.isPressed || whiteButton.isPressed || redButton.isPressed || yellowButton.isPressed)))
  {
    return;
  }
  if (blueButton.isPressed && blueButton.hasStateChanged)
  {
    sendLogMessage("blue button pressed at " + getDateTime());
    blueButton.startPressTime = millis();
    blueButton.startPressTimeString = getDateTime();
    addButtonPinToArray(blueButton.pin);

    if (systemMode == manual)
    {
      turnOnAllLeds(false);
      blueButton.led.turnOn(true);
      turnOnColdWater(true);
    }
    // === UPDATED: also allow cancel while auto is active ===
    else if (systemMode == autofill || isAutoDispensing)
    {
      if (isDispensing)
      {
        cancelDispense = true;
      }
      else
      {
        turnOnAllLeds(false);
        yellowButton.led.lastLedFlashTime = millis();
        blueButton.led.lastLedFlashTime = millis();
        autofillColdWater(autofillVolume.coldVolume);
      }
    }
    else if (systemMode == calibrate)
    {
      if (isDispensing)
      {
        if (waterType != cold)
        {
          stopDispense();
          turnOnAllLeds(true);
          systemMode = manual;
        }
        else 
        {
          turnOnColdWater(false);
          autofillVolume.coldVolume = flowMeter->getTotalVolume() * 10000 - currentVolume;
          currentVolume = flowMeter->getTotalVolume() * 10000;
          sendLogMessage("autofill cold volume set to: " + String(autofillVolume.coldVolume));
          turnOnAllLeds(true);
          systemMode = manual;
        }
      }
      else
      {
        turnOnAllLeds(false);
        yellowButton.led.lastLedFlashTime = millis();
        blueButton.led.lastLedFlashTime = millis();
        turnOnColdWater(true);
      }
    }
    else if (systemMode == configuration)
    {
      sendLogMessage("Entering chiller bucket config");
      turnOnAllLeds(false);
      redButton.led.lastLedFlashTime = millis();
      greenButton.led.lastLedFlashTime = millis();
      whiteButton.led.lastLedFlashTime = millis();
      blueButton.led.lastLedFlashTime = millis();
    }
    else if (systemMode == hotTankConfig)
    {
      handleSetHotTankTempCommand("saving");
      turnOnAllLeds(true);
    }
    else if (systemMode == chillerBucketConfig)
    {
      handleSetChillerBucketTempCommand("coldest");
      turnOnAllLeds(true);
    }
    else if (systemMode == hotWaterDispense)
    {
      turnOnAllLeds(true);
    }
  }
  else if (blueButton.isPressed)
  {
    if (systemMode == manual)
    {
      blueButton.led.flash(1000);
    }
  }
  else if (blueButton.isReleased && blueButton.hasStateChanged)
  {
    if (systemMode == manual)
    {
      turnOnColdWater(false);
      turnOnAllLeds(true);
      sendStopDispenseStats(cold);
    }
    else if (systemMode == calibrate)
    {
    }
    else if (systemMode == configuration)
    {
      systemMode = chillerBucketConfig;
    }
    else if (systemMode == hotTankConfig)
    {
      systemMode = manual;
    }
    else if (systemMode == chillerBucketConfig)
    {
      systemMode = manual;
    }
    else if (systemMode == hotWaterDispense)
    {
      systemMode = manual;
    }
  }
  blueButton.hasStateChanged = false;
}

void handleRedButtonStateChanged()
{
  if ((systemMode == safety || systemMode == chillerBucketRefill || isChildSafetySwitchEnabled) || (systemMode == manual && !isDispensing && (greenButton.isPressed || blueButton.isPressed || whiteButton.isPressed || yellowButton.isPressed)))
  {
    return;
  }

  if (isHotWaterSafetySequenceEnabled)
  {
    // Original logic here
    if (redButton.isPressed && redButton.hasStateChanged)
    {
      sendLogMessage(" Red button pressed at " + getDateTime());
      redButton.startPressTime = millis();
      redButton.startPressTimeString = getDateTime();
      addButtonPinToArray(redButton.pin);
      
      if (systemMode == manual && !isDispensing)
      {
        turnOnAllLeds(false);
        yellowButton.led.turnOn(true); // Yellow LED solid on to indicate next step
        systemMode = hotWaterDispense;
        hotWaterDispenseSequenceStartTime = millis();
        sendLogMessage("Red button pressed. Entered hotWaterDispense mode. Waiting for Yellow.");
      }
      else if (systemMode == calibrate)
      {
        stopDispense();
        turnOnAllLeds(true);
        systemMode = manual;
      }
      else if (systemMode == hotWaterDispense && !isDispensing && previouslyPressedButtons[1] == YELLOWBUTTON && previouslyPressedButtons[2] == REDBUTTON) // This is the third red button press
      {
        if (millis() - hotWaterDispenseSequenceStartTime < hotWaterSequenceTimeout)
        {
          turnOnAllLeds(false); // Clear all LEDs
          redButton.led.turnOn(true); // Red LED solid on for dispense
          turnOnHotWater(true); // Initiate hot water dispense
          // systemMode = manual; // REMOVED: Do not set to manual yet
          memset(previouslyPressedButtons, 0, 5*sizeof(*previouslyPressedButtons)); // Clear sequence
          hotWaterDispenseSequenceStartTime = 0; // Clear sequence timer
          sendLogMessage("Red button pressed. Hot water dispense initiated (Red -> Yellow -> Red sequence). System mode remains hotWaterDispense.");
        }
        else
        {
          // Timeout occurred, reset sequence
          turnOnAllLeds(true); // Return all LEDs to solid on
          systemMode = manual; // Return to manual mode
          memset(previouslyPressedButtons, 0, 5*sizeof(*previouslyPressedButtons)); // Clear sequence
          hotWaterDispenseSequenceStartTime = 0; // Clear sequence timer
          sendLogMessage("Hot water dispense sequence timed out. Returning to manual mode.");
        }
      }
      else if (systemMode == hotWaterDispense && !isDispensing)
      {
        // If red button is pressed again in hotWaterDispense mode, and it's not part of the sequence,
        // we should probably just ignore it or provide feedback.
        // For now, let's just log it and not change LED states or mode.
        sendLogMessage("Red button pressed again in hotWaterDispense mode (not part of sequence).");
      }
      // === UPDATED: allow hot auto cancel too ===
      else if (systemMode == autofill || isAutoDispensing)
      {
        // no volume-set autofill for hot in the button UI, but if auto is active, allow cancel
        if (isDispensing)
        {
          cancelDispense = true;
        }
      }
      else if (systemMode == configuration)
      {
        sendLogMessage("Entering hot tank config");
        turnOnAllLeds(false);
        redButton.led.lastLedFlashTime = millis();
        greenButton.led.lastLedFlashTime = millis();
        whiteButton.led.lastLedFlashTime = millis();
        blueButton.led.lastLedFlashTime = millis();
      }
      else if (systemMode == hotTankConfig)
      {
        handleSetHotTankTempCommand("hottest");
        turnOnAllLeds(true);
      }
      else if (systemMode == chillerBucketConfig)
      {
        handleSetChillerBucketTempCommand("saving");
        turnOnAllLeds(true);
      }
    }
    else if (redButton.isPressed)
    {
      if (systemMode == hotWaterDispense)
      {
        if (!hotTankControl.isHotTankEnabled)
        {
          if (millis() - 15000 > redButton.startPressTime)
          {
            hotTankControl.enableHotTank(true);
          }
          if (millis() - 10000 > redButton.startPressTime)
          {
            redButton.led.flash(500);
          }
          else
          {
            redButton.led.flash(1000);
          }
        }
        else if (!redButton.isPressed)
        {
          redButton.led.flash(1000);
        }
      }
    }
    else if (redButton.isReleased && redButton.hasStateChanged)
    {
      sendLogMessage("Red button released. systemMode: " + String(systemMode) + ", isDispensing: " + String(isDispensing));
      if (systemMode == hotWaterDispense && isDispensing)
      {
        turnOnHotWater(false);
        sendStopDispenseStats(hot);
        turnOnAllLeds(true);
        systemMode = manual; // Set to manual ONLY when dispense stops
        memset(previouslyPressedButtons, 0, 5*sizeof(*previouslyPressedButtons));
        sendLogMessage("Hot water dispense stopped and returned to manual mode.");
      }
      else if (systemMode == hotWaterDispense && !isDispensing && previouslyPressedButtons[1] == YELLOWBUTTON)
      {
        // Sequence incomplete, just clear sequence and return to manual
        turnOnAllLeds(true); // Return all LEDs to solid on
        systemMode = manual;
        memset(previouslyPressedButtons, 0, 5*sizeof(*previouslyPressedButtons));
        sendLogMessage("Red button released. Sequence incomplete. Returning to manual mode.");
      }
      else if (systemMode == hotWaterDispense && !isDispensing && previouslyPressedButtons[1] == REDBUTTON)
      {
        // Sequence incomplete, just clear sequence and return to manual
        turnOnAllLeds(true); // Return all LEDs to solid on
        systemMode = manual;
        memset(previouslyPressedButtons, 0, 5*sizeof(*previouslyPressedButtons));
        sendLogMessage("Red button released. Sequence incomplete. Returning to manual mode.");
      }
      else if (systemMode == configuration)
      {
        systemMode = hotTankConfig;
      }
      else if (systemMode == hotTankConfig)
      {
        systemMode = manual;
      }
      else if (systemMode == chillerBucketConfig)
      {
        systemMode = manual;
      }
    }
  }
  else // isHotWaterSafetySequenceEnabled is false
  {
    if (redButton.isPressed && redButton.hasStateChanged)
    {
        if (systemMode == manual && !isDispensing)
        {
            turnOnAllLeds(false);
            redButton.led.turnOn(true);
            turnOnHotWater(true);
        }
    }
    else if (redButton.isReleased && redButton.hasStateChanged)
    {
        if (systemMode == manual)
        {
            turnOnHotWater(false);
            sendStopDispenseStats(hot);
            turnOnAllLeds(true);
        }
    }
  }
  redButton.hasStateChanged = false;
}

void handleWhiteButtonStateChanged()
{
  if ((systemMode == safety) || (systemMode == manual && !isDispensing && (greenButton.isPressed || blueButton.isPressed || redButton.isPressed || yellowButton.isPressed)))
  {
    return;
  }
  if (whiteButton.isPressed && whiteButton.hasStateChanged)
  {
    sendLogMessage(" White button pressed at " + getDateTime());
    whiteButton.startPressTime = millis();
    whiteButton.startPressTimeString = getDateTime();
    addButtonPinToArray(whiteButton.pin);

    if (systemMode == manual && !isDispensing)
    {
      turnOnAllLeds(false);
      whiteButton.led.turnOn(true);
      turnOnAlkalineWater(true);
    }
    // === UPDATED: also allow cancel while auto is active ===
    else if (systemMode == autofill || isAutoDispensing)
    {
      if (isDispensing)
      {
        cancelDispense = true;
      }
      else
      {
        turnOnAllLeds(false);
        whiteButton.led.lastLedFlashTime = millis();
        yellowButton.led.lastLedFlashTime = millis();
        autofillAlkalineWater(autofillVolume.alkVolume);
      }
    }
    else if (systemMode == calibrate)
    {
      if (isDispensing)
      {
        if (waterType != alkaline)
        {
          stopDispense();
          turnOnAllLeds(true);
          systemMode = manual;
        } 
        else 
        {
          turnOnAlkalineWater(false);
          sendStopDispenseStats(alkaline);
          autofillVolume.alkVolume = flowMeter->getTotalVolume() * 10000 - currentVolume;
          currentVolume = flowMeter->getTotalVolume() * 10000;
          sendLogMessage("autofill alkaline volume set to: " + String(autofillVolume.alkVolume));
          turnOnAllLeds(true);
          systemMode = manual;
        }
      }
      else
      {
        turnOnAllLeds(false);
        whiteButton.led.lastLedFlashTime = millis();
        yellowButton.led.lastLedFlashTime = millis();
        turnOnAlkalineWater(true);
      }
    }
    else if (systemMode == hotTankConfig)
    {
      handleSetHotTankTempCommand("lesshot");
      turnOnAllLeds(true);
    }
    else if (systemMode == chillerBucketConfig)
    {
      handleSetChillerBucketTempCommand("lesscold");
      turnOnAllLeds(true);
    }
    else if (systemMode == configuration)
    {
      // toggle serial mode
      isSerialInJsonMode = !isSerialInJsonMode;
      EEPROM.put(isSerialInJsonModeAddress, isSerialInJsonMode);
      if (isSerialInJsonMode)
      {
        sendLogMessage("Serial Mode Changed to JSON");
      }
      else 
      {
        sendLogMessage("Serial Mode Changed to Command Line"); 
      }
      turnOnAllLeds(true);
      systemMode = manual;
    }
    else if (systemMode == hotWaterDispense)
    {
      turnOnAllLeds(true);
    }
  }
  else if (whiteButton.isPressed)
  {
    if (systemMode == manual)
    {
      if (millis() - ledToggleDuration > whiteButton.startPressTime)
      {
        whiteButton.led.flash(1000);
      }
    }
  }
  else if (whiteButton.isReleased && whiteButton.hasStateChanged)
  {
    if (systemMode == manual)
    {
      turnOnAlkalineWater(false);
      sendStopDispenseStats(alkaline);
      turnOnAllLeds(true);
    }
  }
  whiteButton.hasStateChanged = false;
}

void handleFilterButtonStateChanged()
{
  if (systemMode == safety || systemMode == chillerBucketRefill)
  {
    return;
  }
  if (filterButton.isPressed && filterButton.hasStateChanged)
  {
    sendLogMessage(" Filter button pressed at " + getDateTime());
    addButtonPinToArray(filterButton.pin);
    filterButton.startPressTime = millis();
    filterButton.startPressTimeString = getDateTime();
    whiteButton.led.turnOn(false);
    yellowButton.led.turnOn(false);
    greenButton.led.lastLedFlashTime = millis();
    blueButton.led.lastLedFlashTime = millis();
    redButton.led.lastLedFlashTime = millis();
    if (systemMode == calibrate)
    {
      sendLogMessage("Exiting Calibration Mode via Filter button.");
      turnOnAllLeds(true);
      systemMode = manual;
    }
    systemMode = configuration;
    sendLogMessage("Entering Configuration Mode");
  }
  else if (filterButton.isPressed && !filterButton.hasStateChanged)
  {
    if (millis() > 10000 && millis() - 10000 > filterButton.startPressTime && !isFactoryReset)
    {
      // FACTORY RESET
      isFactoryReset = true;
      turnOnAllLeds(false);
      handleFactoryResetCommand("");
    }
  }
  filterButton.hasStateChanged = false;
}

void handleYellowButtonStateChanged()
{
  sendLogMessage("handleYellowButtonStateChanged called. systemMode: " + String(systemMode) + ", yellowButton.isPressed: " + String(yellowButton.isPressed) + ", yellowButton.hasStateChanged: " + String(yellowButton.hasStateChanged));
  if ((systemMode == safety || systemMode == chillerBucketRefill) || (systemMode == manual && !isDispensing && (greenButton.isPressed || blueButton.isPressed || redButton.isPressed || whiteButton.isPressed)))
  {
    return;
  }
  if (yellowButton.isPressed && yellowButton.hasStateChanged)
  {
    sendLogMessage(" Yellow button pressed at " + getDateTime());
    yellowButton.startPressTime = millis();
    yellowButton.startPressTimeString = getDateTime();
    addButtonPinToArray(yellowButton.pin);
    if (systemMode == manual)
    {
    }
    else if (systemMode == calibrate && isDispensing)
    {
      stopAndSaveCalibration();
      memset(previouslyPressedButtons, 0, 5*sizeof(*previouslyPressedButtons));
    }
    else if (systemMode == calibrate && !isDispensing)
    {
      sendLogMessage("Exiting Calibration Mode via Yellow button.");
      turnOnAllLeds(true);
      systemMode = manual;
    }
    else if (systemMode == autofill)
    {
      stopDispense();
      turnOnAllLeds(true);
      systemMode = manual;
    }
    else if (systemMode == hotWaterDispense && previouslyPressedButtons[1] == REDBUTTON) // Check if RED was pressed before YELLOW
    {
      // This is the second step of the sequence: Red then Yellow
      if (millis() - hotWaterDispenseSequenceStartTime < hotWaterSequenceTimeout)
      {
        turnOnAllLeds(false); // Clear all LEDs
        redButton.led.turnOn(true); // Red LED solid on to indicate next step
        systemMode = hotWaterDispense; // Stay in hotWaterDispense mode
        hotWaterDispenseSequenceStartTime = millis(); // Reset timer for next step (red button press)
        sendLogMessage("Yellow button pressed. Second step of hot water sequence (Red -> Yellow). Waiting for Red.");
      }
      else
      {
        // Timeout occurred, reset sequence
        turnOnAllLeds(true); // Return all LEDs to solid on
        systemMode = manual; // Return to manual mode
        memset(previouslyPressedButtons, 0, 5*sizeof(*previouslyPressedButtons)); // Clear sequence
        hotWaterDispenseSequenceStartTime = 0; // Clear sequence timer
        sendLogMessage("Hot water dispense sequence timed out. Returning to manual mode.");
      }
    }
    else if (systemMode == hotWaterDispense)
    {
      // If yellow button is pressed in hotWaterDispense mode, but not as part of the sequence
      // (i.e., red was not the previously pressed button), then turn off yellow LED and reset timer.
      yellowButton.led.turnOn(false); // Turn yellow LED off
      hotWaterDispenseSequenceStartTime = millis(); // Reset timer
      sendLogMessage("Yellow button pressed in hotWaterDispense mode (not part of sequence).");
    }
    else if (systemMode == configuration)
    {
      isHotWaterSafetySequenceEnabled = !isHotWaterSafetySequenceEnabled;
      if (isHotWaterSafetySequenceEnabled)
      {
        sendLogMessage("Hot water safety sequence enabled");
      }
      else
      {
        sendLogMessage("Hot water safety sequence disabled");
      }
      // Explicitly set all LEDs to solid on when returning to manual mode
      greenButton.led.turnOn(true);
      greenButton.led.lastLedFlashTime = 0;
      whiteButton.led.turnOn(true);
      whiteButton.led.lastLedFlashTime = 0;
      blueButton.led.turnOn(true);
      blueButton.led.lastLedFlashTime = 0;
      redButton.led.turnOn(true);
      redButton.led.lastLedFlashTime = 0;
      yellowButton.led.turnOn(true);
      yellowButton.led.lastLedFlashTime = 0;

      systemMode = manual;
      yellowButton.hasStateChanged = true; // Explicitly set to true to allow release detection
    }
  }
  else if (yellowButton.isPressed)
  {
    const unsigned long pressDuration = millis() - yellowButton.startPressTime;

    if (systemMode == hotWaterDispense)
    {
      return;
    }
    if (pressDuration > calibrationModeButtonPressTime && systemMode != hotWaterDispense && greenButton.led.isOn)
    {
      systemMode = calibrate;
      calibrationModeStartTime = currentTime;
      sendLogMessage("Entered Calibration Mode");
    }
    else if (pressDuration > autofillButtonPressTime && systemMode != calibrate && systemMode != hotWaterDispense)
    {
      systemMode = autofill;
      autofillStartTime = currentTime;
      redButton.led.turnOn(false);
      sendLogMessage("Entered Autofill Mode");
    }
  }
  else if (yellowButton.isReleased && yellowButton.hasStateChanged)
  {
    sendLogMessage("Yellow button released. systemMode: " + String(systemMode));
    if (systemMode == autofill)
    {
      greenButton.led.turnOn(true);
      whiteButton.led.turnOn(true);
      blueButton.led.turnOn(true);

      sendLogMessage("Entering autofill mode");
    }
    else if (systemMode == calibrate && !isDispensing)
    {
      sendLogMessage("Entering calibration mode");
    }
    else if (systemMode == hotWaterDispense && previouslyPressedButtons[1] == YELLOWBUTTON)
    {
      systemMode = manual;
      memset(previouslyPressedButtons, 0, 5*sizeof(*previouslyPressedButtons));
    }
  }
  yellowButton.hasStateChanged = false;
}

void turnOnSparklingWater(bool isEnabled)
{
  if (isEnabled)
  {
    waterType = sparkling;
    turnOnMainValve(true);
    turnOnSparklingValve(true);
    turnOnNozzleValve(true);
    agitatorPumpControl.turnOnAgitatorPump(true);
  } else {
    turnOnWaterPump(false);
    turnOnCo2Valve(false);
  }

  sparklingWaterStartStopTime = millis();
}

void turnOnAlkalineWater(bool isEnabled)
{
  if (isEnabled)
  {
    waterType = alkaline;
    turnOnAlkalineValve(true);
    turnOnMainValve(true);
  } else {
    turnOnWaterPump(false);
  }

  alkalineWaterStartStopTime = millis();  
}

void turnOnColdWater(bool isEnabled)
{
  if (isEnabled)
  {
    waterType = cold;
    turnOnColdValve(true);
    turnOnMainValve(true);
    agitatorPumpControl.turnOnAgitatorPump(true);
  } else {
    turnOnWaterPump(false);
  }
  coldWaterStartStopTime = millis();
}

void turnOnHotWater(bool isEnabled)
{
  sendLogMessage("turnOnHotWater called with isEnabled: " + String(isEnabled));
  isEnabled ? waterType = hot : waterType = none;
  turnOnMainValve(isEnabled);
  turnOnHotValve(isEnabled);

  if (isEnabled)
  {
    const int timeNow = getEpochTime();
    EEPROM.put(lastHotWaterDispenseTimeAddress, timeNow);
  }

  // If a temporary preset was active, restore defaults once hot water stops
  if (!isEnabled && hotPresetActive) {
    currentTempConfig.HotTankMax = savedHotTankMax;
    currentTempConfig.HotTankMin = savedHotTankMin;
    hotPresetActive = false;
    sendLogMessage("Restored hot tank temperature band after preset dispense");
  }
}


// autofill
void autofillSparklingWater(int volume)
{
  autoFillStopVolume = (flowMeter->getTotalVolume()) * 10000 + volume;
  isAutoDispensing = true; // NEW
  sendLogMessage("Start Volume: " + String(flowMeter->getTotalVolume() * 10000));
  sendLogMessage("Stop Volume: " + String(autoFillStopVolume));
  turnOnSparklingWater(true);
}

void autofillColdWater(int volume)
{
  autoFillStopVolume = (flowMeter->getTotalVolume()) * 10000 + volume;
  isAutoDispensing = true; // NEW
  sendLogMessage("Start Volume: " + String(flowMeter->getTotalVolume() * 10000));
  sendLogMessage("Stop Volume: " + String(autoFillStopVolume));
  turnOnColdWater(true);
}

void autofillAlkalineWater(int volume)
{
  autoFillStopVolume = (flowMeter->getTotalVolume()) * 10000 + volume;
  isAutoDispensing = true; // NEW
  sendLogMessage("Start Volume: " + String(flowMeter->getTotalVolume() * 10000));
  sendLogMessage("Stop Volume: " + String(autoFillStopVolume));
  turnOnAlkalineWater(true);
}

void autofillHotWater(int volume)
{
  autoFillStopVolume = (flowMeter->getTotalVolume()) * 10000 + volume;
  isAutoDispensing = true; // NEW
  sendLogMessage("Start Volume: " + String(flowMeter->getTotalVolume() * 10000));
  sendLogMessage("Stop Volume: " + String(autoFillStopVolume));
  turnOnHotWater(true);
}

void stopAutofill(WaterType waterType, unsigned long stopVolume)
{
  double currentVolumeLocal = (flowMeter->getTotalVolume() * 10000);
  if (currentVolumeLocal > stopVolume || cancelDispense == true)
  {
    stopDispense();
    sendStopDispenseStats(waterType);
    cancelDispense = false;
    isAutoDispensing = false;   // NEW
    autoFillStopVolume = 0;     // NEW
    turnOnAllLeds(true);
    systemMode = manual;
    sendLogMessage("Program mode change to Manual");
  }
}

// Valves

void turnOnMainValve(bool isEnabled)
{
  sendLogMessage("turnOnMainValve called with isEnabled: " + String(isEnabled));
  isDispensing = isEnabled;
  dispenseStartTime = currentTime;
  sr.set(MAIN_V, isEnabled);

  isMainValveOn = isEnabled;

  if (isDispensing)
  {
    currentDispenseStartTime = millis();
  }
  sendLogMessage("isDispensing set to: " + String(isDispensing));
}

void turnOnLevelValve(bool isEnabled)
{
  sr.set(LEVEL_V, isEnabled);
  isLevelValveOn = isEnabled;
}

void turnOnColdValve(bool isEnabled)
{
  sr.set(COLD_V, isEnabled);
  isColdValveOn = isEnabled;
}

void turnOnSparklingValve(bool isEnabled)
{
  sr.set(SPK_V, isEnabled);
  isSparklingValveOn = isEnabled;
}

void turnOnHotValve(bool isEnabled)
{
  sr.set(HOT_V, isEnabled);
  isHotValveOn = isEnabled;
}

void turnOnAlkalineValve(bool isEnabled)
{
  sr.set(ALK_V, isEnabled);
  isAlkalineValveOn = isEnabled;
}

void turnOnCo2Valve(bool isEnabled)
{
  digitalWrite(CO2_V, isEnabled);
  isCo2ValveOn = isEnabled;
}

void turnOnNozzleValve(bool isEnabled)
{
  sr.set(NOZZLE_V, isEnabled);
  isNozzleValveOn = isEnabled;
}

// pumps

void turnOnWaterPump(bool isEnabled)
{
  if (isWaterPumpControlEnabled)
  {
    isWaterPumpEnabled = isEnabled;
    digitalWrite(WATERPUMP, isEnabled);
  }
}

void turnOnAllLeds(bool isEnabled)
{
  greenButton.led.turnOn(isEnabled);
  greenButton.led.lastLedFlashTime = 0; // Reset flash time
  whiteButton.led.turnOn(isEnabled);
  whiteButton.led.lastLedFlashTime = 0; // Reset flash time
  blueButton.led.turnOn(isEnabled);
  blueButton.led.lastLedFlashTime = 0; // Reset flash time
  redButton.led.turnOn(isEnabled);
  redButton.led.lastLedFlashTime = 0; // Reset flash time
  yellowButton.led.turnOn(isEnabled);
  yellowButton.led.lastLedFlashTime = 0; // Reset flash time
}

void cancelAutofill()
{
  turnOnSparklingWater(false);
  turnOnAlkalineWater(false);
  turnOnColdWater(false);
  turnOnHotWater(false);
}

void stopDispense()
{
  sendLogMessage("stopDispense called. waterType: " + String(waterType));
  // stop
  if (systemMode == chillerBucketRefill)
  {
    turnOnMainValve(false);
    turnOnLevelValve(false);
    sendStopDispenseStats(none);
  }
  else if (waterType == cold)
  {
    turnOnMainValve(false);
    turnOnColdWater(false);
  }
  else if (waterType == sparkling)
  {
    turnOnMainValve(false);
    turnOnSparklingWater(false);
  }
  else if (waterType == alkaline)
  {
    turnOnMainValve(false);
    turnOnAlkalineValve(false);
    turnOnWaterPump(false);  //temporary fixing bug for Waterpump not stopping for ALK Autofill
  }
  else if (waterType == hot)
  {
    turnOnMainValve(false);
    turnOnHotWater(false);
  }

  // NEW: always clear auto/hold state on any stop
  isAutoDispensing = false;
  autoFillStopVolume = 0;
  holdSparkling = false;
  holdCold      = false;
  holdAlkaline  = false;
}

void safetyModeCheck()
{
  if (systemMode == safety)
  {
    // already in safety mode
    turnOnAllLeds(false);
    greenLed.flash(1000);
    redLed.flash(1000);
  }
  else
  {
    if (isDispensing)
    {
      const double volume = flowMeter->getTotalVolume() * 10000 - currentVolume;

      if (systemMode == chillerBucketRefill && volume > 8000 && isFloatSensorOk)
      {
        sendLogMessage("Float Sensor Failure");
        isFloatSensorOk = false;
      }

      if ((systemMode == calibrate && volume >= 2000))
      {
          turnOnMainValve(false);
          turnOnSparklingValve(false);
          turnOnNozzleValve(false);
          turnOnCo2Valve(false);
          turnOnColdValve(false);
          turnOnAlkalineValve(false);
          turnOnWaterPump(false);
          systemMode = manual;
          turnOnAllLeds(true);
      }

      if ((systemMode != chillerBucketRefill && volume > 2050) || (systemMode == chillerBucketRefill && volume > 11000))
      {
        enterSafetyMode();
        sendStopDispenseStats(waterType);
      }

      if (currentTime > 30000)
      {
        if((currentTime - 30000 > dispenseStartTime)&&systemMode == manual)
        {
          turnOnMainValve(false);
          turnOnSparklingValve(false);
          turnOnNozzleValve(false);
          turnOnCo2Valve(false);
          turnOnColdValve(false);
          turnOnAlkalineValve(false);
          thirtySecStartStopTime = millis();
          systemMode = thirtysecs;
        }
      }
      
      if (currentTime > 8000)
      {
        if (currentTime - 8000 == dispenseStartTime)
        {
          if ((currentFlowrate < minimumFlowRate) && (systemMode != chillerBucketRefill && systemMode != hotWaterDispense))
          {
            sendStopDispenseStats(waterType);
            sendLogMessage("LOW FLOW RATE DETECTED!");
            enterSafetyMode();
          }
        }
      }
      
    }
  }
}

void enterSafetyMode()
{
  // Enter safety mode
  sendLogMessage("ENTERING SAFETY MODE");
  enterSafetyModeTime = millis();
  stopDispense();
  systemMode = safety;
}

void checkChillerBucket()
{
  if (systemMode == safety)
  {
    return;
  }
  const bool isWaterLevelLow = !digitalRead(LevelSwitchUp);
  isFloatSensorLow = isWaterLevelLow;

  if (isChillerBucketCheckEnabled == false )
  {
    if (isWaterLevelLow && hasSentChillerBucketLowMessage == false)
    {
      sendLogMessage("Chiller bucket low!");
      hasSentChillerBucketLowMessage = true;
    }
    else
    {
      return;
    }
  }
  else if (systemMode == chillerBucketRefill && isDispensing)
  {
    flashAllButtonLeds(1000);

    if (millis() - 7000 > chillerBucketRefillStartTime && isWaterLevelLow && !isChillerBucketRefillPaused)
    {
      turnOnLevelValve(false);
      turnOnMainValve(false);
      isDispensing = true; 
      chillerBucketRefillStartTime = millis();
      isChillerBucketRefillPaused = true;
    }
    if (millis() - 3000 > chillerBucketRefillStartTime && isWaterLevelLow && isChillerBucketRefillPaused)
    {
      turnOnLevelValve(true);
      turnOnMainValve(true);
      chillerBucketRefillStartTime = millis();
      isChillerBucketRefillPaused = false;
    }
    if (!isWaterLevelLow)
    {
      if (!isWaterRefilled)
      {
        sendLogMessage("Refilling for another 7 seconds");
        turnOnLevelValve(true);
        turnOnMainValve(true);
        waterRefillTime = currentTime;
        isWaterRefilled = true;
      }
      else if (isWaterRefilled && currentTime - 7000 > waterRefillTime)
      {
        turnOnMainValve(false);
        turnOnLevelValve(false);
        sendStopDispenseStats(none);
        turnOnAllLeds(true);
        sendLogMessage("Chiller bucket refill complete!");
        sendLogMessage("Returning to manual mode");
        systemMode = manual;
        isWaterRefilled = false;
        isChillerBucketCheckEnabled = false;
      }
    }
  }

  else if (isWaterLevelLow && systemMode != chillerBucketRefill)
  {
    // enter chiller bucket refill mode
    turnOnAllLeds(false);
    blueButton.led.lastLedFlashTime = millis();
    whiteButton.led.lastLedFlashTime = millis();
    sendLogMessage("Chiller Bucket water level low!");
    sendLogMessage("Entering chiller bucket refill mode");
    systemMode = chillerBucketRefill;
  }
}

void flashAllButtonLeds(int duration)
{
  greenButton.led.flash(duration);
  blueButton.led.flash(duration);
  whiteButton.led.flash(duration);
  yellowButton.led.flash(duration);
  redButton.led.flash(duration);
}

void printNtcTemperatures()
{
  sendLogMessage("Cold NTC: " + String(ntc.cold_ntc));
  sendLogMessage("Hot NTC: " + String(ntc.hot_ntc));
  sendLogMessage("NTC 3: " + String(ntc.ntc3));

  if (ntc.cold_ntc == -100)
  {
    sendLogMessage("Cold NTC does not exist");
  }
  if (ntc.hot_ntc == -100)
  {
    sendLogMessage("Hot NTC does not exist");
  }
  if (ntc.ntc3 == -100)
  {
    sendLogMessage("NTC3 does not exist");
  }
}

void checkLastHotWaterDispenseDateTime()
{
  const int address = eeAddress + sizeof(currentTempConfig);
  EEPROM.get(address, lastHotWaterDispenseTime);
  if (getEpochTime() - 2592000 > lastHotWaterDispenseTime)
  {
    redButton.led.flash(1000);
  }
  else if (getEpochTime() - 2678400 > lastHotWaterDispenseTime)
  {
    if (redButton.led.isOn)
    {
      redButton.led.turnOn(false);
    }
    if (hotTankControl.isHotTankOn)
    {
      hotTankControl.enableHotTank(false);
    }
  }

  if (getEpochTime() - 120 > lastHotWaterDispenseTime)
  {
    // at least two min since last hot water dispense, do nothing
  }
  else
  {
    // within 2min of last hot water dispense
    hotTankControl.runPostDispense(ntc, currentTempConfig);
  }
}


void addButtonPinToArray(int pin)
{
  byte arrayLen = sizeof(previouslyPressedButtons) / sizeof(previouslyPressedButtons[0]);
  for(byte i=arrayLen - 1; i>0; i--)
  {
    previouslyPressedButtons[i] = previouslyPressedButtons[i-1];
  }
  previouslyPressedButtons[0] = pin;
}

void sendLogMessage(String msg)
{
  if (isSerialInJsonMode)
  {
    json.clear();
    json["info"] = msg;
    serializeJson(json, Serial2);
    Serial2.print('\n');
  }
  else
  {
    Serial2.println(msg);
  }
}

void stopAndSaveCalibration()
{
  switch (previouslyPressedButtons[1])
  {
    case GREENBUTTON:
      turnOnSparklingWater(false);
      sendStopDispenseStats(sparkling);
      autofillVolume.spkVolume = flowMeter->getTotalVolume() * 10000 - currentVolume;
      currentVolume = flowMeter->getTotalVolume() * 10000;
      sendLogMessage("autofill sparkling volume set to: " + String(autofillVolume.spkVolume));
      break;
    case BLUEBUTTON:
      turnOnColdWater(false);
      sendStopDispenseStats(cold);
      autofillVolume.coldVolume = flowMeter->getTotalVolume() * 10000 - currentVolume;
      currentVolume = flowMeter->getTotalVolume() * 10000;
      sendLogMessage("autofill cold volume set to: " + String(autofillVolume.coldVolume));
      break;
    case WHITEBUTTON:
      turnOnAlkalineWater(false);
      sendStopDispenseStats(cold);
      autofillVolume.alkVolume = flowMeter->getTotalVolume() * 10000 - currentVolume;
      currentVolume = flowMeter->getTotalVolume() * 10000;
      sendLogMessage("autofill alkaline volume set to: " + String(autofillVolume.alkVolume));
      break;

    default:
      break;
  }
  turnOnAllLeds(true);
}

bool areAnyOtherButtonsPressed(Button button)
{
  switch (button.pin)
  {
  case GREENBUTTON:
    return whiteButton.isPressed || blueButton.isPressed || redButton.isPressed || yellowButton.isPressed;
  case BLUEBUTTON:
    return greenButton.isPressed || whiteButton.isPressed || redButton.isPressed || yellowButton.isPressed;
  case WHITEBUTTON:
    return greenButton.isPressed || blueButton.isPressed || redButton.isPressed || yellowButton.isPressed;
  case REDBUTTON:
    return greenButton.isPressed || whiteButton.isPressed || blueButton.isPressed || yellowButton.isPressed;
  case YELLOWBUTTON:
    return greenButton.isPressed || whiteButton.isPressed || blueButton.isPressed || redButton.isPressed;
  }
  return false;
}


// ===================== Hot Preset Controller =====================
// Non-blocking: heat to target → dispense volume → release control.
void processHotPreset()
{
  if (!hotPresetActive) { hotPresetState = HotPresetIdle; return; }

  switch (hotPresetState)
  {
    case HotPresetHeating:
      hotTankControl.enableHotTank(true);

      if (ntc.hot_ntc >= presetTargetTempC) {
        systemMode = hotWaterDispense;
        sendLogMessage("Preset: using hotWaterDispense mode to bypass low-flow guard.");
        if (presetVolumeMl <= 0) presetVolumeMl = (presetTargetTempC < 93 ? 200 : 300);
        autofillHotWater(presetVolumeMl);   // sets isAutoDispensing + stop volume
        hotPresetState = HotPresetDispensing;
        sendLogMessage("Preset: reached " + String(ntc.hot_ntc) + "°C → dispensing "
                       + String(presetVolumeMl) + " mL");
      }
      break;

    case HotPresetDispensing:
      // Enforce volume stop while we’re in preset dispense
      stopAutofill(hot, autoFillStopVolume);

      if (!isDispensing) {
        hotTankControl.enableHotTank(false);
        hotPresetActive = false;
        hotPresetState  = HotPresetIdle;
        sendLogMessage("Preset: complete → normal hot-tank control restored.");
      }
      break;

    case HotPresetIdle:
    default:
      break;
  }
}

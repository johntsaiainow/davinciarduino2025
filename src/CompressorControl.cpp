#include "CompressorControl.h"

CompressorControl::CompressorControl()
{
  //Serial2.println("ctor.CompressorControl");
  pinMode(FAN, OUTPUT);
  pinMode(COMPRESSOR, OUTPUT);
}

void CompressorControl::run(NTC ntc, TempConfig currentTempConfig)
{
  if (ntc.cold_ntc > currentTempConfig.ChillerBucketMax)
  {
    if (hasCompressorHitUpperLimit == false)
    {
      hasCompressorHitUpperLimit = true;
      compressorCrossUpperLimitTime = millis();
    }
    // turn on compressor 1 min after crossing upper limit
    if (millis() > compressorCrossUpperLimitTime + 60000 && isCompressorEnabled == false)
    {
      // Serial2.println("Turning on compressor");
      isCompressorEnabled = true;
      turnOnCompressor(true);
      turnOnFan(true);
    }
  }
  else if (ntc.cold_ntc < currentTempConfig.ChillerBucketMin)
  {
    if (hasCompressorHitLowerLimit == false)
    {
      hasCompressorHitLowerLimit = true;
      compressorCrossLowerLimitTime = millis();
    }
     //turn off compressor 1 min after crossing lower limit
    if (millis() > compressorCrossLowerLimitTime + 60000 && isCompressorEnabled == true)
    {
      // Serial2.println("Turning off compressor");
    if (isCompressorEnabled == true)
    {
      isCompressorEnabled = false;
      turnOnCompressor(false);
      turnOnFan(false);
    }
    }
  }
  else
  {
    if (hasCompressorHitUpperLimit)
    {
      hasCompressorHitUpperLimit = false;
    }
    if (hasCompressorHitLowerLimit)
    {
      hasCompressorHitLowerLimit = false;
    }
  }
}

void CompressorControl::turnOnCompressor(bool isEnabled)
{
  digitalWrite(COMPRESSOR, isEnabled);
}

void CompressorControl::turnOnFan(bool isEnabled)
{
  digitalWrite(FAN, isEnabled);
}

void CompressorControl::turnOn(bool isEnabled)
{
  isCompressorEnabled = isEnabled;
  turnOnCompressor(isEnabled);
  turnOnFan(isEnabled);

  if (!isEnabled)
  {
    hasCompressorHitLowerLimit = false;
    hasCompressorHitUpperLimit = false;
  }
}
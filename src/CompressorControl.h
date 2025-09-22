#ifndef COMPRESSORCONTROL_H
#define COMPRESSORCONTROL_H

#define FAN PC14
#define COMPRESSOR PA8

#include "AppTypes.h"
#include <Arduino.h>

class CompressorControl
{

    bool hasCompressorHitUpperLimit = false;
    bool hasCompressorHitLowerLimit = false;
    unsigned long compressorCrossUpperLimitTime = 0;
    unsigned long compressorCrossLowerLimitTime = 0;
    void turnOnCompressor(bool isEnabled);
    void turnOnFan(bool isEnabled);

public:
    bool isCompressorEnabled = false;
    CompressorControl();
    void run(NTC ntc, TempConfig currentTempConfig);
    void turnOn(bool isEnabled);
};

#endif
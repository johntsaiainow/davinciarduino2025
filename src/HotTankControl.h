#ifndef HOTTANKCONTROL_H
#define HOTTANKCONTROL_H

#include "AppTypes.h"
#include <Arduino.h>

#define HOTTANK PA15

class HotTankControl
{
    public:
    bool isHotTankOn = false;
    bool isHotTankEnabled = true;
    HotTankControl();
    void turnOnHotTank(bool isEnabled);
    void enableHotTank(bool isEnabled);
    void run(NTC ntc, TempConfig currentTempConfig);
    void runPostDispense(NTC ntc, TempConfig currentTempConfig);
};

#endif
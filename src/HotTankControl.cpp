#include "HotTankControl.h"

HotTankControl::HotTankControl()
{
    //Serial2.println("ctor.HotTankControl");
    pinMode(HOTTANK, OUTPUT);
}

void HotTankControl::run(NTC ntc, TempConfig currentTempConfig)
{
    if (ntc.hot_ntc > currentTempConfig.HotTankMax)
    {
        isHotTankOn = false;
        turnOnHotTank(false);
    }

    if (ntc.hot_ntc < currentTempConfig.HotTankMin)
    {
        isHotTankOn = true;
        turnOnHotTank(true);
    }
}

void HotTankControl::runPostDispense(NTC ntc, TempConfig currentTempConfig)
{
    if (ntc.hot_ntc > currentTempConfig.HotTankMax && ntc.hot_ntc < currentTempConfig.HotTankMax + 3)
    {
        isHotTankOn = false;
        turnOnHotTank(false);
    }

    if (ntc.hot_ntc < currentTempConfig.HotTankMax)
    {
        isHotTankOn = true;
        turnOnHotTank(true);
    }
}

void HotTankControl::turnOnHotTank(bool isEnabled)
{
    isHotTankOn = isEnabled;
    digitalWrite(HOTTANK, isEnabled);
}

void HotTankControl::enableHotTank(bool isEnabled)
{
    isHotTankEnabled = isEnabled;
    turnOnHotTank(isEnabled);
}
#include "FilterStatusCheck.h"

FilterStatusCheck::FilterStatusCheck()
{
}

void FilterStatusCheck::run(int totalDispenseVolumeSinceFilterReset, int lastFilterResetTime)
{
    if (totalDispenseVolumeSinceFilterReset > filterReplaceVolume || getEpochTime() - 15552000 > lastFilterResetTime)
    {
        isFilterOk = false;
        // turnOnGreenLed(false);
        // filter LED is flashing
    }
    else
    {
        isFilterOk = true;
        // turnOnGreenLed(true);
    }
}
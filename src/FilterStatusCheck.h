#ifndef FILTERSTATUSCHECK_H
#define FILTERSTATUSCHECK_H

#include "AppTypes.h"
#include <DateTime.h>

class FilterStatusCheck
{
    const int filterReplaceVolume = 5000000; // in mL

    public:
        bool isFilterOk = false;
        FilterStatusCheck();
        void run(int totalDispenseVolumeSinceFilterReset, int lastFilterResetTime);
};

#endif
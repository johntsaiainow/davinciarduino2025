#ifndef AGITATORPUMPCONTROL_H
#define AGITATORPUMPCONTROL_H

#define AGITATOR PA10

#include "AppTypes.h"
#include <Arduino.h>

class AgitatorPumpControl
{
    unsigned long agitatorpump_start_time = 0;
    unsigned long agitatorpump_stop_time = 0;

public:
    bool isAgitatorPumpEnabled = false;
    AgitatorPumpControl();
    void run();
    void turnOnAgitatorPump(bool isEnabled);
};

#endif
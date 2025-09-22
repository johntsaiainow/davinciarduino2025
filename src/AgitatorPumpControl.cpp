#include "AgitatorPumpControl.h"

AgitatorPumpControl::AgitatorPumpControl()
{
    pinMode(AGITATOR, OUTPUT);
}

void AgitatorPumpControl::run()
{
    if (isAgitatorPumpEnabled == false)
    {
        // turn on agitator if it hasnt been on in 30 min
        if (millis() > agitatorpump_stop_time + 1800000)
        {
            turnOnAgitatorPump(true);
        }
    }
    else if (isAgitatorPumpEnabled == true)
    {
        // turn off agitator after its been on for 2min
        if (millis() > agitatorpump_start_time + 120000)
        {
            turnOnAgitatorPump(false);
        }
    }
}

void AgitatorPumpControl::turnOnAgitatorPump(bool isEnabled)
{
    digitalWrite(AGITATOR, isEnabled);
    isAgitatorPumpEnabled = isEnabled;

    if (isEnabled)
    {
        agitatorpump_start_time = millis();
    }
    else
    {
        agitatorpump_stop_time = millis();
    }
}
#include "Led.h"
#include "AppTypes.h"

Led::Led(int pin, ShiftRegister74HC595<uint8_t(2)>& sr)
: sr(sr)
{
    this->pin = pin;
}

void Led::flash(int duration)
{
    if (millis() - duration > lastLedFlashTime)
    {
        sr.set(pin, !isOn);
        this->isOn = !isOn;
        this->lastLedFlashTime = millis();
    }
}

void Led::turnOn(bool isEnabled)
{
    sr.set(pin, !isEnabled);
    this->isOn = isEnabled;
}
#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <ShiftRegister74HC595.h>

class Led
{
    ShiftRegister74HC595<uint8_t(2)>& sr;
public:
    
    bool isOn = true;
    int pin;
    unsigned long lastLedFlashTime;
    Led(int pin, ShiftRegister74HC595<uint8_t(2)>& sr);
    void flash(int duration);
    void turnOn(bool isEnabled);
};

#endif
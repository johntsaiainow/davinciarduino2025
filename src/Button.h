#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "Led.h"

class Button
{
    ShiftRegister74HC595<uint8_t(2)>& sr;
public:
    int pin;
    bool isPressed;
    bool isReleased;
    bool hasStateChanged;
    unsigned long startPressTime;
    String startPressTimeString = "";
    Led led;
    Button(int pin, int ledPin, ShiftRegister74HC595<uint8_t(2)>& sr);
    void update();
};

#endif

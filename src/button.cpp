#include "Button.h"

Button::Button(int pin, int ledPin, ShiftRegister74HC595<uint8_t(2)> &sr)
    : sr(sr), led(ledPin, sr)
{
    pinMode(pin, INPUT);
    this->pin = pin;
}

void Button::update()
{
    const bool newValue = !digitalRead(pin);

    if (isPressed != newValue)
    {
        hasStateChanged = true;
        if (!isPressed && newValue)
        {
            isPressed = true;
            isReleased = false;
        }
        if (isPressed && !newValue)
        {
            isPressed = false;
            isReleased = true;
        }
    }
}

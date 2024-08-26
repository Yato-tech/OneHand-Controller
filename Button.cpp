#include "Button.h"
#include <Arduino.h>

Button::Button(int pin) : _pin(pin) {}

void Button::begin() {
    pinMode(_pin, INPUT_PULLUP);
}

bool Button::isPressed() {
    return digitalRead(_pin) == LOW;
}

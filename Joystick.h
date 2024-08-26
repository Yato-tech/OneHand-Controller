#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>

class Joystick {
public:
    Joystick(int vrxPin, int vryPin);
    void begin();
    void update();
    int getX();
    int getY();
    bool isPressed();

private:
    int applyCalibration(int value, int centerMin, int centerMax, int deadzoneMin, int deadzoneMax);

    int vrxPin;
    int vryPin;
    int swPin;

    int vrxValue;
    int vryValue;

    const int numberOfSamples = 5;
    const int delayBetweenSamples = 4;
    const int DEADZONE_MIN = 0;  // Adjusted deadzone values
    const int DEADZONE_MAX = 4095;
    const int CENTER_MIN = 2200;
    const int CENTER_MAX = 2299;
};

#endif

#include "Joystick.h"

Joystick::Joystick(int vrxPin, int vryPin)
    : vrxPin(vrxPin), vryPin(vryPin) {
}

void Joystick::begin() {
    pinMode(vrxPin, INPUT);
    pinMode(vryPin, INPUT);
}

void Joystick::update() {
    int vrxValues[numberOfSamples];
    int vryValues[numberOfSamples];
    vrxValue = 0;
    vryValue = 0;

    // Read multiple samples for noise reduction
    for (int i = 0; i < numberOfSamples; i++) {
        vrxValues[i] = analogRead(vrxPin);
        vryValues[i] = analogRead(vryPin);
        vrxValue += vrxValues[i];
        vryValue += vryValues[i];
        delay(delayBetweenSamples);
    }

    // Calculate the average
    vrxValue = vrxValue / numberOfSamples;
    vryValue = vryValue / numberOfSamples;

    // Apply calibration logic
    vrxValue = applyCalibration(vrxValue, CENTER_MIN, CENTER_MAX, DEADZONE_MIN, DEADZONE_MAX);
    vryValue = applyCalibration(vryValue, CENTER_MIN, CENTER_MAX, DEADZONE_MIN, DEADZONE_MAX);
}

int Joystick::getX() {
    return vrxValue;
}

int Joystick::getY() {
    return vryValue;
}

bool Joystick::isPressed() {
    // Implement button press logic here
    return false;
}

int Joystick::applyCalibration(int value, int centerMin, int centerMax, int deadzoneMin, int deadzoneMax) {
    if (value < deadzoneMin || value > deadzoneMax || 
        (value >= centerMin && value <= centerMax)) {
        return 14000; // Centered or dead zone, set to 14000
    } else {
        return map(value, deadzoneMin, deadzoneMax, 0, 28000);
    }
}

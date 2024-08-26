#include "Joystick.h"
#include "IMU.h"
#include "Button.h"
#include "BLEManager.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <BleGamepad.h>
#include <utility/imumaths.h>

// Change pins for your use
const int VRX_PIN = 13;
const int VRY_PIN = 27;
const int SW_PIN = 34;
const int BUTTON_A_PIN = 26;
const int BUTTON_B_PIN = 25;
const int BUTTON_X_PIN = 33;
const int BUTTON_Y_PIN = 32;

// Instances of classes
Joystick joystick(VRX_PIN, VRY_PIN);
IMU myimu;
Button buttonA(BUTTON_A_PIN);
Button buttonB(BUTTON_B_PIN);
Button buttonX(BUTTON_X_PIN);
Button buttonY(BUTTON_Y_PIN);
BLEManager bleManager;

void setup() {
    Serial.begin(115200);
    joystick.begin();
    myimu.begin();
    buttonA.begin();
    buttonB.begin();
    buttonX.begin();
    buttonY.begin();
    bleManager.begin();
}

void loop() {
    if (bleManager.isConnected()) {
        joystick.update();
        myimu.update();

        // Set joystick axes
        bleManager.setAxes(joystick.getX(), joystick.getY(), joystick.isPressed(), myimu.getPitch(), myimu.getYaw(), myimu.getRoll());

        // Check buttons
        if (buttonA.isPressed()) {
            bleManager.pressButton(BUTTON_1);
        } else {
            bleManager.releaseButton(BUTTON_1);
        }

        if (buttonB.isPressed()) {
            bleManager.pressButton(BUTTON_2);
        } else {
            bleManager.releaseButton(BUTTON_2);
        }

        if (buttonX.isPressed()) {
            bleManager.pressButton(BUTTON_4);
        } else {
            bleManager.releaseButton(BUTTON_4);
        }

        if (buttonY.isPressed()) {
            bleManager.pressButton(BUTTON_5);
        } else {
            bleManager.releaseButton(BUTTON_5);
        }

        delay(5);
    }
}

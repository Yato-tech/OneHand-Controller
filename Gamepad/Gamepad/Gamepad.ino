#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <BleGamepad.h>
#include <utility/imumaths.h>

// Define pin numbers for joystick axes and switch
const int VRX_PIN = 13; // Analog pin for X-axis
const int VRY_PIN = 27; // Analog pin for Y-axis
const int SW_PIN = 34;  // Digital pin for joystick switch

// Define pin numbers for additional buttons
const int BUTTON_A_PIN = 26; // Digital pin for button A
const int BUTTON_B_PIN = 25; // Digital pin for button B
const int BUTTON_X_PIN = 33; // Digital pin for button X
const int BUTTON_Y_PIN = 32; // Digital pin for button Y

// IMU setup
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

BleGamepad bleGamepad;

// Sampling parameters
const int numberOfSamples = 5;        // Number of samples to take (to smooth the values)
const int delayBetweenSamples = 4;    // Delay in milliseconds between samples
const int delayBetweenHIDReports = 5; // Additional delay in milliseconds between HID reports

// Dead zone and center zone thresholds
const int DEADZONE_MIN = 100;   // Anything below this is a dead zone
const int DEADZONE_MAX = 4095;  // Anything above this is a dead zone
const int CENTER_MIN = 2200;    // Lower bound of center zone
const int CENTER_MAX = 2299;    // Upper bound of center zone

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

    // Initialize joystick pins
    pinMode(VRX_PIN, INPUT);
    pinMode(VRY_PIN, INPUT);
    pinMode(SW_PIN, INPUT_PULLUP);

    // Initialize button pins
    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    pinMode(BUTTON_B_PIN, INPUT_PULLUP);
    pinMode(BUTTON_X_PIN, INPUT_PULLUP);
    pinMode(BUTTON_Y_PIN, INPUT_PULLUP);

    // Initialize BLE gamepad
    bleGamepad.begin();

    // Initialize IMU
    if (!bno.begin()) {
        Serial.println("Failed to initialize BNO055! Check your wiring.");
        while (1);
    }
    bno.setExtCrystalUse(true);
}

void loop() {
    if (bleGamepad.isConnected()) {
        int vrxValues[numberOfSamples]; // Array to store VRX readings
        int vryValues[numberOfSamples]; // Array to store VRY readings
        int vrxValue = 0;               // Variable to store calculated VRX reading average
        int vryValue = 0;               // Variable to store calculated VRY reading average

        // Populate readings for VRX and VRY
        for (int i = 0; i < numberOfSamples; i++) {
            vrxValues[i] = analogRead(VRX_PIN);
            vryValues[i] = analogRead(VRY_PIN);
            vrxValue += vrxValues[i];
            vryValue += vryValues[i];
            delay(delayBetweenSamples);
        }
        Serial.print("X: ");
        Serial.print(vrxValue);
        Serial.print(" Y: ");
        Serial.println(vryValue);

        // Calculate the averages
        vrxValue = vrxValue / numberOfSamples;
        vryValue = vryValue / numberOfSamples;

        // Apply dead zones
        if (vrxValue < DEADZONE_MIN || vrxValue > DEADZONE_MAX || 
            (vrxValue >= CENTER_MIN && vrxValue <= CENTER_MAX)) {
            vrxValue = 0; // Centered or dead zone, set to 0
        } else {
            // Map analog reading from 0 ~ 4095 to -32767 ~ 32767 for use as axis readings
            vrxValue = map(vrxValue, DEADZONE_MIN, DEADZONE_MAX, -32767, 32767);
        }

        if (vryValue < DEADZONE_MIN || vryValue > DEADZONE_MAX || 
            (vryValue >= CENTER_MIN && vryValue <= CENTER_MAX)) {
            vryValue = 0; // Centered or dead zone, set to 0
        } else {
            // Map analog reading from 0 ~ 4095 to -32767 ~ 32767 for use as axis readings
            vryValue = map(vryValue, DEADZONE_MIN, DEADZONE_MAX, -32767, 32767);
        }

        // Get IMU data
        imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
        int16_t imuX = map(euler.x(), -180, 180, -32767, 32767); // Map to gamepad axis range
        int16_t imuY = map(euler.y(), -180, 180, -32767, 32767);
        int16_t imuZ = map(euler.z(), -180, 180, -32767, 32767);

        // Set axes values for joystick and IMU data
        bleGamepad.setAxes(vrxValue, vryValue, imuX, imuY, imuZ, 0, 0, 0);

        // Read digital value from joystick switch
        bool swState = digitalRead(SW_PIN) == LOW; // Assuming the switch is active low

        // Set button state for joystick switch
        if (swState) {
            bleGamepad.press(BUTTON_3); // Use any button you prefer
        } else {
            bleGamepad.release(BUTTON_3);
        }

        // Read digital values from buttons
        bool buttonAState = digitalRead(BUTTON_A_PIN) == LOW; // Assuming the button is active low
        bool buttonBState = digitalRead(BUTTON_B_PIN) == LOW; // Assuming the button is active low
        bool buttonXState = digitalRead(BUTTON_X_PIN) == LOW; // Assuming the button is active low
        bool buttonYState = digitalRead(BUTTON_Y_PIN) == LOW; // Assuming the button is active low

        // Set button states for A, B, X, Y buttons
        if (buttonAState) {
            bleGamepad.press(BUTTON_1); // A button
        } else {
            bleGamepad.release(BUTTON_1);
        }

        if (buttonBState) {
            bleGamepad.press(BUTTON_2); // B button
        } else {
            bleGamepad.release(BUTTON_2);
        }

        if (buttonXState) {
            bleGamepad.press(BUTTON_4); // X button
        } else {
            bleGamepad.release(BUTTON_4);
        }

        if (buttonYState) {
            bleGamepad.press(BUTTON_5); // Y button
        } else {
            bleGamepad.release(BUTTON_5);
        }

        delay(delayBetweenHIDReports);
    }
}

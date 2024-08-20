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
const int DEADZONE_MIN = 0;   // Anything below this is a dead zone
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

        // Calculate the averages (noise reduction)
        vrxValue = vrxValue / numberOfSamples;
        vryValue = vryValue / numberOfSamples;

        // Apply dead zones for joystick axes
        if (vrxValue < DEADZONE_MIN || vrxValue > DEADZONE_MAX || 
            (vrxValue >= CENTER_MIN && vrxValue <= CENTER_MAX)) {
            vrxValue = 14000; // Centered or dead zone, set to 14000
        } else {
            // Map analog reading from 110 ~ 4095 to 0 ~ 28000 for use as axis readings
            vrxValue = map(vrxValue, DEADZONE_MIN, DEADZONE_MAX, 0, 28000);
        }

        if (vryValue < DEADZONE_MIN || vryValue > DEADZONE_MAX || 
            (vryValue >= CENTER_MIN && vryValue <= CENTER_MAX)) {
            vryValue = 14000;
        } else {
            vryValue = map(vryValue, DEADZONE_MIN, DEADZONE_MAX, 0, 28000);
        }

        // Get IMU data
        uint8_t system, gyro, accel, mg = 0;
        bno.getCalibration(&system, &gyro, &accel, &mg);
        imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
        int16_t imuX = static_cast<int16_t>(euler.x()); // Yaw, X axis for joystick
        int16_t imuY = static_cast<int16_t>(euler.y()); // Roll, Y axis for joystick
        int16_t imuZ = static_cast<int16_t>(euler.z()); // Pitch

        // Shift and wrap the Yaw (imuX) by 180 degrees
        imuX = imuX + 180;
        if (imuX >= 360) {
            imuX -= 360; // Ensure it wraps around within 0-360 degrees
        }

        // Apply custom dead zones and center zones for yaw (X-axis)
        if (imuX >= 175 && imuX <= 185) {
            imuX = 180; // Center zone, map to the midpoint for more precise control
        }

        // Map the IMU values to 0-28000 range
        // Use a narrower input range for increased sensitivity
        imuX = map(imuX, 140, 220, 0, 28000); // Map 90-270 degrees to 0-28000
        imuY = map(imuY, -45, 45, 0, 28000); // Example: Narrower range for Roll (Y-axis)
        imuZ = map(imuZ, -90, 90, 0, 28000); // Example: Narrower range for Pitch (Z-axis)

        Serial.print(" X: ");
        Serial.print(imuX);
        Serial.print(" Y: ");
        Serial.print(imuY);
        Serial.print(" Z: ");
        Serial.print(imuZ);
        Serial.print(",");
        Serial.print(accel);
        Serial.print(",");
        Serial.print(gyro);
        Serial.print(",");
        Serial.print(mg);
        Serial.print(",");
        Serial.println(system);

        // Set axes values for joystick and IMU data
        bleGamepad.setAxes(vrxValue, vryValue, SW_PIN, imuZ, imuX, imuY, 0, 0); // Left Thumb X, Left Thumb Y, Left Trigger, Right Trigger Y, Left Thumb X, Right Thumb Y, Slider 1, Slider 2

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

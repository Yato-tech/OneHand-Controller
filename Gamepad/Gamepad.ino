#include <Arduino.h>
#include <BleGamepad.h>

// Define pin numbers
const int VRX_PIN = 13; // Analog pin for X-axis
const int VRY_PIN = 27; // Analog pin for Y-axis
const int SW_PIN = 14;  // Digital pin for switch

BleGamepad bleGamepad;

const int numberOfSamples = 5;        // Number of samples to take (to smooth the values)
const int delayBetweenSamples = 4;    // Delay in milliseconds between samples
const int delayBetweenHIDReports = 5; // Additional delay in milliseconds between HID reports

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

    pinMode(VRX_PIN, INPUT);
    pinMode(VRY_PIN, INPUT);
    pinMode(SW_PIN, INPUT_PULLUP);

    bleGamepad.begin();
    // The default bleGamepad.begin() above enables 16 buttons, all axes, one hat, and no simulation controls or special buttons
}

void loop()
{
    if (bleGamepad.isConnected())
    {
        int vrxValues[numberOfSamples]; // Array to store VRX readings
        int vryValues[numberOfSamples]; // Array to store VRY readings
        int vrxValue = 0; // Variable to store calculated VRX reading average
        int vryValue = 0; // Variable to store calculated VRY reading average

        // Populate readings for VRX and VRY
        for (int i = 0; i < numberOfSamples; i++)
        {
            vrxValues[i] = analogRead(VRX_PIN);
            vryValues[i] = analogRead(VRY_PIN);
            vrxValue += vrxValues[i];
            vryValue += vryValues[i];
            delay(delayBetweenSamples);
        }

        // Calculate the averages
        vrxValue = vrxValue / numberOfSamples;
        vryValue = vryValue / numberOfSamples;

        // Map analog reading from 0 ~ 4095 to -32767 ~ 32767 for use as axis readings
        int xAxis = map(vrxValue, 0, 4095, -32767, 32767);
        int yAxis = map(vryValue, 0, 4095, -32767, 32767);

        // Set axes values
        bleGamepad.setAxes(xAxis, yAxis, 0, 0, 0, 0, 0, 0);

        // Read digital value from switch
        bool swState = digitalRead(SW_PIN) == LOW; // Assuming the switch is active low

        // Set button state
        if (swState)
        {
            bleGamepad.press(BUTTON_1); // Use any button you prefer
        }
        else
        {
            bleGamepad.release(BUTTON_1);
        }

        delay(delayBetweenHIDReports);
    }
}

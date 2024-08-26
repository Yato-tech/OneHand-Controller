#include "IMU.h"
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

// Constructor
IMU::IMU() : bno(Adafruit_BNO055(55)) {
    imuX = 0;
    imuY = 0;
    imuZ = 0;
}

// Initialize IMU
void IMU::begin() {
    if (!bno.begin()) {
        Serial.println("Failed to initialize BNO055! Check your wiring.");
        while (1);
    }
    bno.setExtCrystalUse(true); // Enable external crystal for better accuracy
}

// Update IMU data and process it for joystick emulation
void IMU::update() {
    // Get IMU data
    uint8_t system, gyro, accel, mg = 0;
    bno.getCalibration(&system, &gyro, &accel, &mg);

    // Get Euler angles (Yaw, Roll, Pitch)
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    imuX = static_cast<int16_t>(euler.x()); // Yaw, X-axis for joystick
    imuY = static_cast<int16_t>(euler.y()); // Roll, Y-axis for joystick
    imuZ = static_cast<int16_t>(euler.z()); // Pitch, Z-axis for trigger

    // Shift and wrap the Yaw (imuX) by 180 degrees
    imuX += 180;
    if (imuX >= 360) {
        imuX -= 360; // Wrap around within 0-360 degrees
    }

    // Apply custom dead zones and center zones for Yaw (X-axis)
    if (imuX >= 175 && imuX <= 185) {
        imuX = 180; // Center zone, set to midpoint for more precise control
    }

    // Apply custom dead zones and center zones for Roll (Y-axis)
    if (imuY >= -5 && imuY <= 5) {
        imuY = 0; // Center zone
    } else if (imuY <= -45) {
        imuY = -45; // Clamp to -45
    } else if (imuY >= 45) {
        imuY = 45; // Clamp to 45
    }

    // Map the IMU values to 0-28000 range for joystick emulation
    imuX = map(imuX, 140, 220, 0, 28000);  // Map 140-220 degrees to 0-28000
    imuY = map(imuY, -45, 45, 0, 28000);   // Map -45 to 45 degrees to 0-28000
    imuZ = map(imuZ, -90, 90, 0, 28000);   // Map -90 to 90 degrees to 0-28000
}

// Get Yaw value
int16_t IMU::getYaw() {
    return imuX;
}

// Get Roll value
int16_t IMU::getRoll() {
    return imuY;
}

// Get Pitch value
int16_t IMU::getPitch() {
    return imuZ;
}

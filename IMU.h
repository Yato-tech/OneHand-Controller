#ifndef IMU_H
#define IMU_H

#include <Adafruit_BNO055.h>

class IMU {
public:
    IMU();
    void begin();
    void update();
    int16_t getYaw();
    int16_t getRoll();
    int16_t getPitch();

private:
    Adafruit_BNO055 bno;

    // Declare the member variables for IMU data
    int16_t imuX; // Yaw
    int16_t imuY; // Roll
    int16_t imuZ; // Pitch
};

#endif

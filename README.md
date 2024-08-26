# One-Handed Controller with IMU and Joystick

This project implements a one-handed controller using an ESP32, a joystick, buttons, and an IMU (Inertial Measurement Unit). The controller is designed to work as a Bluetooth gamepad, using the ESP32 BLE capabilities.

## Components

- **ESP32-WROOM-DA Module**
- **Adafruit BNO055 (IMU sensor)**
- **Analog Joystick Module**
- **Tactile Push Buttons**
- **BLE Gamepad Library**

## Features

- **Analog Joystick**: Used for left thumb X and Y axis inputs.
- **IMU (BNO055)**: Provides Yaw, Pitch, and Roll data to simulate joystick inputs.
- **Buttons**: Four push buttons are used to simulate gamepad button inputs.
- **Bluetooth Connectivity**: The controller is recognized as a Bluetooth gamepad, which can be used with various devices like PCs, smartphones, etc.

## Wiring

- **Joystick**: 
  - VRX (X-axis) -> GPIO 13
  - VRY (Y-axis) -> GPIO 27
  - SW (Button) -> GPIO 34
- **Buttons**: 
  - Button A -> GPIO 26
  - Button B -> GPIO 25
  - Button X -> GPIO 33
  - Button Y -> GPIO 32
- **IMU (BNO055)**: I2C communication with ESP32
  - SDA -> GPIO 21
  - SCL -> GPIO 22

## Installation

1. Install the Arduino IDE.
2. Install the ESP32 core for Arduino from the Boards Manager.
3. Install the required libraries:
   - `Adafruit_BNO055`
   - `Adafruit_Sensor`
   - `imumaths`
   - `ESP32-BLE-Gamepad`
4. Clone this repository or download the ZIP file and extract it.

```bash
git clone https://github.com/yourusername/onehanded-controller.git
```

5. Open the MyProject.ino file in the Arduino IDE.
6. Select your board as ESP32-WROOM-DA Module in the Arduino IDE.
7. Compile and upload the code to your ESP32.

## Usage
Power on the ESP32. The device will start in Bluetooth mode and will be discoverable as a gamepad.
Connect to the gamepad from your PC, smartphone, or any compatible device.
Use the joystick to control the X and Y axes.
The IMU sensor will control additional axes (Yaw, Pitch, and Roll).
The buttons can be used to simulate gamepad buttons.

## Code Structure
MyProject.ino: The main entry point of the project. Initializes all components and handles the main loop.
Joystick.h/cpp: Handles the joystick inputs and converts them into gamepad axis values.
IMU.h/cpp: Manages the IMU sensor data (Yaw, Pitch, Roll) and converts it into usable gamepad axis values.
Button.h/cpp: Handles button inputs.
BLEManager.h/cpp: Manages Bluetooth communication and sets up the BLE Gamepad.

## Troubleshooting
IMU Not Detected: Ensure that the BNO055 IMU is connected correctly via I2C. Check the wiring for any loose connections.
Bluetooth Connection Issues: Make sure that the ESP32 is properly configured for BLE. If you're having trouble pairing, try resetting the device or clearing the Bluetooth cache on your phone/PC.
Joystick/Buttons Not Responding: Verify that all GPIO pins are correctly assigned and wired. Ensure that the ESP32 is receiving analog inputs from the joystick.
Contributing
Feel free to open an issue or submit a pull request for any improvements or bug fixes.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

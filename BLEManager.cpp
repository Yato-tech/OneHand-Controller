#include "BLEManager.h"

BLEManager::BLEManager() {}

void BLEManager::begin() {
    bleGamepad.begin();
}

bool BLEManager::isConnected() {
    return bleGamepad.isConnected();
}

void BLEManager::setAxes(int x, int y, int z, int rz, int rx, int ry) {
    bleGamepad.setAxes(x, y, z, rz, rx, ry, 0, 0);  // Left Thumb X, Left Thumb Y, Left Trigger, Right Trigger Y, Left Thumb X, Right Thumb Y, Slider 1, Slider 2
}

void BLEManager::pressButton(int button) {
    bleGamepad.press(button);
}

void BLEManager::releaseButton(int button) {
    bleGamepad.release(button);
}

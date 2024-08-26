#ifndef BLEMANAGER_H
#define BLEMANAGER_H

#include <BleGamepad.h>

class BLEManager {
public:
    BLEManager();
    void begin();
    bool isConnected();
    void setAxes(int x, int y, int z, int rz, int rx, int ry);
    void pressButton(int button);
    void releaseButton(int button);

private:
    BleGamepad bleGamepad;
};

#endif

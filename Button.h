#ifndef BUTTON_H
#define BUTTON_H

class Button {
public:
    Button(int pin);
    void begin();
    bool isPressed();

private:
    int _pin;
};

#endif

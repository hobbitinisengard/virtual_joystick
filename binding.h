#ifndef BINDING_H
#define BINDING_H
#include <malloc.h>
#include <vjdevice.h>
// represents key binding to vjoy controller button/axis movement

struct Binding
{
    uint8_t device_id;
    SignalType type;
    // mapped keyboard key
    int key;
    // value to be mapped. For buttons, button1: value = 0, for axis, axisX: 65535 is maximum right joystick movement
    //uint16_t value;
    Binding(uint8_t ID, SignalType TYPE, int KEY)
        : device_id(ID), type(TYPE), key(KEY)
    {};
};

#endif // BINDING_H

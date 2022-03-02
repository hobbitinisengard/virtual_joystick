#ifndef VJDEVICE_H
#define VJDEVICE_H
#include <QtGlobal>
#include <vjoyinterface.h>
#include <cstdint>
class vJDevice
{
private:
    JOYSTICK_POSITION_V2 iReport;
public:
    uint8_t device_id;

    vJDevice();
    vJDevice(int id);
    void Feed_data(JOYSTICK_POSITION_V2 &iReport);
};

#endif // VJDEVICE_H

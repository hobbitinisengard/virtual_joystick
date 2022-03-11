#ifndef VJDEVICE_H
#define VJDEVICE_H
#include <QtGlobal>
#include <QtCore>
#include <QtGui>
#include <QEvent>
#include <vjoyinterface.h>
#include <cstdint>
#include <Structs.h>
#include <exception>

class vJDevice
{
private:
    //JOYSTICK_POSITION_V2 iReport;
    void Feed_data();
    void DecodePOVMacro(const int macro, int &value, int &npov) const;
public:
    uint8_t id;

    vJDevice();
    vJDevice(int id);
    static void Send_data(const Binding *binding, bool Pressed_notReleased);
};

#endif // VJDEVICE_H

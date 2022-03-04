#ifndef VJDEVICE_H
#define VJDEVICE_H
#include <QtGlobal>
#include <QtCore>
#include <QtGui>
#include <QEvent>
#include <vjoyinterface.h>
#include <cstdint>
#include <Structs.h>


class vJDevice
{
private:
    //JOYSTICK_POSITION_V2 iReport;
    void Feed_data();
public:
    uint8_t id;

    vJDevice();
    vJDevice(int id);
    void Send_data(const Binding *binding, bool Pressed_notReleased) const;
};

#endif // VJDEVICE_H

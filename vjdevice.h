#ifndef VJDEVICE_H
#define VJDEVICE_H
#include <QtGlobal>
#include <QtCore>
#include <QtGui>
#include <QEvent>
#include <vjoyinterface.h>
#include <cstdint>

enum class SignalType{BUTTON, AXIS, DISCRETE_POV, CONTINUOUS_POV };
//enum class BUTTONAXIS {
//    lButtons,
//    wThrottle,
//    wRudder,
//    wAileron,
//    wAxisX,
//    wAxisY,
//    wAxisZ,
//    wAxisXRot,
//    wAxisYRot,
//    wAxisZRot,
//    wSlider,
//    wDial,
//    wWheel,
//    wAxisVX,
//    wAxisVY,
//    wAxisVZ,
//    wAxisVBRX,
//    wAxisVBRY,
//    wAxisVBRZ,
//    bHats,
//    bHatsEx1,
//    bHatsEx2,
//    bHatsEx3,
//    lButtonsEx1,
//    luttonsEx2,
//    lButtonsEx3
//};
class vJDevice
{
private:
    JOYSTICK_POSITION_V2 iReport;
    void Feed_data();
public:
    uint8_t id;

    vJDevice();
    vJDevice(int id);
    void Send_data(SignalType type, bool Pressed_notReleased, uint16_t value);
};

#endif // VJDEVICE_H

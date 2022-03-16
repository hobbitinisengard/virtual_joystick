#ifndef VJOYCONNECTOR_H
#define VJOYCONNECTOR_H
#include <malloc.h>
#include "vjoyinterface.h"
#include "infobox.h"
#include <QMessageBox>
#include <QtCore>
#include <vjdevice.h>
class VJoyConnector
{
public:
    std::vector<vJDevice*> devices {new vJDevice()};
    vJDevice* current_device = devices[0];
    QString latest_link_to_driver =  "https://github.com/njz3/vJoy/releases/tag/v2.2.1.1";
    VJoyConnector();
    ~VJoyConnector();
    void Create_new_Device(uint8_t vjoy_id);
};

#endif // VJOYCONNECTOR_H

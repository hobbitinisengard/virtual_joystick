#include <VJoyConnector.h>
VJoyConnector::VJoyConnector()
{
    WORD VerDll, VerDrv;
    if (!DriverMatch(&VerDll, &VerDrv))
    {
        Infobox infoBox("DLL and driver versions do not match. Remove vjoy driver you currently have and"
                " install newer one here:<a href='" + latest_link_to_driver + "'>Link</a>");
        qCritical("Failed\r\nvJoy Driver (version %04x) does not match\
            vJoyInterface DLL (version %04x)\n", VerDrv ,VerDll);
        throw VJD_STAT_MISS_Exception();
    }

    if(!vJoyEnabled() || GetVJDStatus(1) == VJD_STAT_MISS || GetVJDStatus(1) == VJD_STAT_UNKN)
    {
        Infobox infoBox("Device 1 is not active. Enable it in 'Configure VJoy' program.");
    }
    ResetAll();
}
VJoyConnector::~VJoyConnector()
{
    for(const auto device : devices)
        RelinquishVJD(device->id);
}
void VJoyConnector::Create_new_Device(uint8_t vjoy_id)
{
    if(vjoy_id < 1 || vjoy_id > 16){
        throw std::out_of_range("id should be in range [1;16]");
    }
    for(auto &dev : devices)
        if(dev->id == vjoy_id)
            throw std::out_of_range("Object with this id already exists");
    qDebug() << "Created device " << vjoy_id;
    devices.push_back(new vJDevice(vjoy_id));
    current_device = devices.back();
}
//ResetVJD(UINT rID);			// Reset all controls to predefined values in the specified VDJ
//ResetAll(void);				// Reset all controls to predefined values in all VDJ
//ResetButtons(UINT rID);		// Reset all buttons (To 0) in the specified VDJ
//ResetPovs(UINT rID);		// Reset all POV Switches (To -1) in the specified VDJ
//SetAxis(LONG Value, UINT rID, UINT Axis);		// Write Value to a given axis defined in the specified VDJ
// value 1-32768
// AXIS | MACRO | VALUE
//X HID_USAGE_X 0x30 (-x, 0, +x)
//Y HID_USAGE_Y 0x31
//Z HID_USAGE_Z 0x32
//Rx HID_USAGE_RX 0x33 (0, +x)
//Ry HID_USAGE_RY 0x34
//Rz HID_USAGE_RZ 0x35
//Slider0 HID_USAGE_SL0 0x36
//Slider1 HID_USAGE_SL1 0x37
//Wheel HID_USAGE_WHL 0x38
//do not use ->POV HID_USAGE_POV 0x39
//SetBtn(BOOL Value, UINT rID, UCHAR nBtn);		// Write Value to a given button defined in the specified VDJ
//SetDiscPov(int Value, UINT rID, UCHAR nPov);	// Write Value to a given descrete POV defined in the specified VDJ
//nPov values:
//0 North (or Forwards)
//1 East (or Right)
//2 South (or backwards)
//3 West (or left)
//-1 Neutral (Nothing pressed)
//SetContPov(DWORD Value, UINT rID, UCHAR nPov);	// Write Value to a given continuous POV defined in the specified VDJ
// nPov: 1-4 (which POV to use)
/* Value: -1 (neutral); 0-35999 represents its position in 1/100 degree units, where 0 signifies North (or forwards),
9000 signifies East (or right), 18000 signifies South (or backwards), 27000 signifies West (or left) and so
forth.
*/


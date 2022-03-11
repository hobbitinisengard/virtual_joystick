#include "vjdevice.h"
vJDevice::vJDevice()
    : vJDevice(1)
{};

vJDevice::vJDevice(int DEVICE_ID)
{
    if(!(DEVICE_ID >=1 && DEVICE_ID<=16))
    {
        throw std::out_of_range("device_id must be in scope of [1;16]");
    }
    id = DEVICE_ID;
    VjdStat status = GetVJDStatus(id);
    switch (status)
    {
    case VJD_STAT_OWN:
        qInfo("vJoy device %d is already owned by this feeder\n", id);
        break;
    case VJD_STAT_FREE:
        qInfo("vJoy device %d is free\n", id);
        break;
    case VJD_STAT_BUSY:
        qWarning("vJoy device %d is already owned by another feeder\nCannot continue\n", id);
        throw new VJD_STAT_BUSY_Exception();
        //throw "This device is already owned by another feeder";
    case VJD_STAT_MISS:
        qWarning("vJoy device %d is not installed or disabled\nCannot continue\n", id);
        throw new VJD_STAT_MISS_Exception();
    default:
        qWarning("vJoy device %d general error\nCannot continue\n", id);
        //Create_OK_MessageBox("vJoy device general error. Cannot continue.");
        throw new std::exception();
    };

    // Acquire the vJoy device
    if (!AcquireVJD(id))
    {
        qWarning("Failed to acquire vJoy device number %d.\n", id);
        throw VJD_STAT_UNKN;//"Failed to acquire this vJDevice";
    }

    // Start FFB
    #if 0
        BOOL Ffbstarted = FfbStart(DevID);
        if (!Ffbstarted)
        {
            _tprintf("Failed to start FFB on vJoy device number %d.\n", DevID);
            int dummy = getchar();
            stat = -3;
            goto Exit;
        }
        else
            _tprintf("Started FFB on vJoy device number %d - OK\n", DevID);
    #endif // 1
    // Register Generic callback function
    // At this point you instruct the Receptor which callback function to call with every FFB packet it receives
    // It is the role of the designer to register the right FFB callback function
    //FfbRegisterGenCB(FfbFunction1, NULL);
    // FFB Note:
    // All FFB activity is performed in a separate thread created when registered the callback function
}

//void vJDevice::Feed_data()
//{
//    iReport.bDevice = this->id;
//    iReport.lButtons = 10;
//    // Send position data to vJoy device
//    PVOID pPositionMessage = (PVOID)(&iReport);

//    if (!UpdateVJD(this->id, pPositionMessage))
//    {
//        qWarning("Feeding vJoy device number %d failed - try to enable device then press enter\n", id);
//        Sleep(500);
//        //getchar();
//        AcquireVJD(id);
//    }
//    Sleep(2);
//}
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
void vJDevice::Send_data(const Binding *binding, const bool Pressed_notReleased)
{
    bool succeeded = false;
    int value = -1;
    int npov = 1;
    qDebug() << Pressed_notReleased;
    switch(binding->type)
    {
    case SignalType::BUTTON:
        succeeded = SetBtn(Pressed_notReleased, binding->device_id, binding->macro);
        break;
    case SignalType::AXIS:
        if(binding->direction == Direction::POSITIVE)
            succeeded = SetAxis(Pressed_notReleased ? 32767 : 16383, binding->device_id, binding->macro);
        else
            succeeded = SetAxis(Pressed_notReleased ? 0 : 16383, binding->device_id, binding->macro);
        break;
    case SignalType::DISCRETE_POV:
        if(Pressed_notReleased){
            value = (uint8_t)binding->macro; // <0; 3>
            npov = (uint8_t)(binding->macro >> 8); // <1;4>
        }
        succeeded = SetDiscPov(value, binding->device_id, npov);
        break;
    case SignalType::CONTINUOUS_POV:
        succeeded = SetContPov(65534, binding->device_id, binding->macro);
        break;
    }
    if(!succeeded){
        qWarning("Couldn't send signal to vjoystick");
    }
}

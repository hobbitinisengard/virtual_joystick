#include "vjdevice.h"
vJDevice::vJDevice()
    : vJDevice(1)
{};


vJDevice::vJDevice(int DEVICE_ID)
{
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
        //throw Create_OK_MessageBox("This device is already owned by another feeder");
        throw "This device is already owned by another feeder";
    case VJD_STAT_MISS:
        qWarning("vJoy device %d is not installed or disabled\nCannot continue\n", id);
        //Create_OK_MessageBox("This vJoy device is not installed or disabled. Stopped.");
        throw "This vJoy device is not installed or disabled. Stopped.";
    default:
        qWarning("vJoy device %d general error\nCannot continue\n", id);
        //Create_OK_MessageBox("vJoy device general error. Cannot continue.");
        throw "vJoy device general error";
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

void vJDevice::Feed_data()
{
    iReport.bDevice = this->id;
    iReport.lButtons = 10;
    // Send position data to vJoy device
    PVOID pPositionMessage = (PVOID)(&iReport);

    if (!UpdateVJD(this->id, pPositionMessage))
    {
        qWarning("Feeding vJoy device number %d failed - try to enable device then press enter\n", id);
        Sleep(500);
        //getchar();
        AcquireVJD(id);
    }
    Sleep(2);
}
//ResetVJD(UINT rID);			// Reset all controls to predefined values in the specified VDJ
//ResetAll(void);				// Reset all controls to predefined values in all VDJ
//ResetButtons(UINT rID);		// Reset all buttons (To 0) in the specified VDJ
//ResetPovs(UINT rID);		// Reset all POV Switches (To -1) in the specified VDJ
//SetAxis(LONG Value, UINT rID, UINT Axis);		// Write Value to a given axis defined in the specified VDJ
//SetBtn(BOOL Value, UINT rID, UCHAR nBtn);		// Write Value to a given button defined in the specified VDJ
//SetDiscPov(int Value, UINT rID, UCHAR nPov);	// Write Value to a given descrete POV defined in the specified VDJ
//SetContPov(DWORD Value, UINT rID, UCHAR nPov);	// Write Value to a given continuous POV defined in the specified VDJ
void vJDevice::Send_data(SignalType type, bool Pressed_notReleased, uint16_t value)
{
    bool succeded = SetBtn(true, 1,1);

    switch(type)
    {
    case SignalType::BUTTON:
        //SetBtn(Pressed_notReleased, id, value);
        break;
    case SignalType::AXIS:
        SetAxis(65534, id, value);
        break;
    case SignalType::DISCRETE_POV:
        break;
    case SignalType::CONTINUOUS_POV:
        break;
    }
//    switch(type){
//    case BUTTONAXIS::lButtons:
//        if(Pressed_notReleased)
//            iReport.lButtons |= 1<<value;
//        else
//            iReport.lButtons &= ~(1UL<<1);
//        break;
//    case BUTTONAXIS::wThrottle:
//        break;
//    case BUTTONAXIS::wRudder:
//        break;
//    case BUTTONAXIS::wAileron:
//        break;
//    case BUTTONAXIS::wAxisX:
//        break;
//    case BUTTONAXIS::wAxisY:
//        break;
//    case BUTTONAXIS::wAxisZ:
//        break;
//    case BUTTONAXIS::wAxisXRot:
//        break;
//    case BUTTONAXIS::wAxisYRot:
//        break;
//    case BUTTONAXIS::wAxisZRot:
//        break;
//    case BUTTONAXIS::wSlider:
//        break;
//    case BUTTONAXIS::wDial:
//        break;
//    case BUTTONAXIS::wWheel:
//        break;
//    case BUTTONAXIS::wAxisVX:
//        break;
//    case BUTTONAXIS::wAxisVY:
//        break;
//    case BUTTONAXIS::wAxisVZ:
//        break;
//    case BUTTONAXIS::wAxisVBRX:
//        break;
//    case BUTTONAXIS::wAxisVBRY:
//        break;
//    case BUTTONAXIS::wAxisVBRZ:
//        break;
//    case BUTTONAXIS::bHats:
//        break;
//    case BUTTONAXIS::bHatsEx1:
//        break;
//    case BUTTONAXIS::bHatsEx2:
//        break;
//    case BUTTONAXIS::bHatsEx3:
//        break;
//    case BUTTONAXIS::lButtonsEx1:
//        break;
//    case BUTTONAXIS::lButtonsEx2:
//        break;
//    case BUTTONAXIS::lButtonsEx3:
//        break;
//    }
    //this->Feed_data();
}

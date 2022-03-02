#include "vjdevice.h"
vJDevice::vJDevice()
    : vJDevice(1)
{};
vJDevice::vJDevice(int DEVICE_ID)
{
    device_id = DEVICE_ID;
    VjdStat status = GetVJDStatus(device_id);

    switch (status)
    {
    case VJD_STAT_OWN:
        qInfo("vJoy device %d is already owned by this feeder\n", device_id);
        break;
    case VJD_STAT_FREE:
        qInfo("vJoy device %d is free\n", device_id);
        break;
    case VJD_STAT_BUSY:
        qWarning("vJoy device %d is already owned by another feeder\nCannot continue\n", device_id);
        //throw Create_OK_MessageBox("This device is already owned by another feeder");
        throw "This device is already owned by another feeder";
    case VJD_STAT_MISS:
        qWarning("vJoy device %d is not installed or disabled\nCannot continue\n", device_id);
        //Create_OK_MessageBox("This vJoy device is not installed or disabled. Stopped.");
        throw "This vJoy device is not installed or disabled. Stopped.";
    default:
        qWarning("vJoy device %d general error\nCannot continue\n", device_id);
        //Create_OK_MessageBox("vJoy device general error. Cannot continue.");
        throw "vJoy device general error";
    };

    // Acquire the vJoy device
    if (!AcquireVJD(device_id))
    {
        qWarning("Failed to acquire vJoy device number %d.\n", device_id);
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

    // Start endless loop
    // The loop injects position data to the vJoy device
    // If it fails it let's the user try again
    //
    // FFB Note:
    // All FFB activity is performed in a separate thread created when registered the callback function
}

void vJDevice::Feed_data(JOYSTICK_POSITION_V2 &iReport)
{
    iReport.bDevice = this->device_id;
    // Send position data to vJoy device
    PVOID pPositionMessage = (PVOID)(&iReport);

    if (!UpdateVJD(this->device_id, pPositionMessage))
    {
        qWarning("Feeding vJoy device number %d failed - try to enable device then press enter\n", device_id);
        Sleep(500);
        //getchar();
        AcquireVJD(device_id);
    }
    Sleep(2);
}

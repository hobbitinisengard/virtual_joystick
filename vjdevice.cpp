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


#pragma once

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_GPS


#include "4_Drivers/GPS_UBX/internal/configs/NeoGPS_cfg.h"
#include "4_Drivers/GPS_UBX/internal/ublox/ubxGPS.h"
#include <Arduino.h>

class MyGPS : public ubloxGPS
{
public:

    MyGPS( Stream *device ) : ubloxGPS( device )
    {
    }

};

// Construct the GPS object and hook it to the appropriate serial device
extern MyGPS gps;

#endif
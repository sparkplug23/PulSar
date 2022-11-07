
#ifndef MUSERCONFIG_SECRET_HEADER
#define MUSERCONFIG_SECRET_HEADER

#include "stdint.h"
#include "2_CoreSystem/mSystemConfig.h"
#include "0_ConfigUser/G0_mFirmwareCustom_Secret.h"

/*********************************************************************************************\
 * This file will override defines in "mBaseConfig.h", and is only regenerated at compile time if the file does not exist.
 * This allows the user to have their own build configuration that wont be changed with future git downloads.
\*********************************************************************************************/

/**
 * Under new TPLink Wifi, devices inside the house will be restricted to only using it
 * Devices outside, or garage, will be allowed to use the garage device. That device, will become the TPlink from the attic, removing BT completely? Though, it likely has better range
 * When new BT fibre comes, put front door smart hub 2 into garage as new AP: "Lucida" ie brightest sky in a constellation
 * */

#ifdef USE_SSIDS_OUTSIDE_HOUSE

// BT box, phasing out
// Dedicated SSID for home controllers
#undef  STA_SSID1
#define STA_SSID1            "HACS2400"                // [Ssid2] Optional alternate AP Wifi SSID
#undef  STA_PASS1
#define STA_PASS1            "af4d8bc9ab"                // [Password2] Optional alternate AP Wifi password

// Garage wifi for devices in there or garden
#undef  STA_SSID2
#define STA_SSID2            "UniFi_AP_1"                // [Ssid2] Optional alternate AP Wifi SSID
#undef  STA_PASS2
#define STA_PASS2            "af4d8bc9ab"                // [Password2] Optional alternate AP Wifi password

// Will remain unpowered, and will only be used if failsafe SSID is needed when primary does not work
#undef  STA_SSID3
#define STA_SSID3            "RecoverySSID"                // [Ssid1] Wifi SSID
#undef  STA_PASS3
#define STA_PASS3            "af4d8bc9ab"                // [Password1] Wifi password

#else // garage or other

// Dedicated SSID for home controllers
#undef  STA_SSID1
#define STA_SSID1            "HACS2400"                // [Ssid2] Optional alternate AP Wifi SSID
#undef  STA_PASS1
#define STA_PASS1            "af4d8bc9ab"                // [Password2] Optional alternate AP Wifi password

// Dedicated SSID for home controllers
#undef  STA_SSID2
#define STA_SSID2            "Doone"                // [Ssid2] Optional alternate AP Wifi SSID
#undef  STA_PASS2
#define STA_PASS2            "71157bjd"                // [Password2] Optional alternate AP Wifi password

// Will remain unpowered, and will only be used if failsafe SSID is needed when primary does not work
#undef  STA_SSID3
#define STA_SSID3            "RecoverySSID"                // [Ssid1] Wifi SSID
#undef  STA_PASS3
#define STA_PASS3            "af4d8bc9ab"                // [Password1] Wifi password

#endif


// MQTT
// #ifndef D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED //TO BE REPLACED WITH FOLLOWING METHOD
// #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED 192,168,1,65
// #endif // D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED

#ifndef MQTT_HOST
#define MQTT_HOST  "192.168.1.65" //Parsing the IP String will be the new method
#endif

// Belfast
#undef LATITUDE
#define LATITUDE               54.50449         // [Latitude] Your location to be used with sunrise and sunset   WHITEHALL CORNER HERE
#undef LONGTITUDE
#define LONGITUDE              -6.30298          // [Longitude] Your location to be used with sunrise and sunset
#undef ALTITUDE_ABOVE_SEALEVEL
#define ALTITUDE_ABOVE_SEALEVEL              26 //metres          // [Longitude] Your location to be used with sunrise and sunset


#define D_FLEXI_O2_PHONE_NUMBER_CTR "07515358597"

#endif // MUSERCONFIG_SECRET_HEADER


